/* Copyright (c) 2016 Mike Lubinets (aka mersinvald)
 * See LICENSE
 *
 * Originaly written by Mike Lubinets 03.24.2016 */

#ifndef DYNAMICCALLER_H
#define DYNAMICCALLER_H

#include <functional>
#include <map>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <assert.h>

/// Using namespace msvd as for part of author's own class library.
#ifdef MSVD_LIB
namespace msvd {
#endif

using std::map;
using std::string;
using std::stringstream;
using std::function;
using std::runtime_error;

template<typename, class>
class DynamicCaller;

template<typename id_type, class Ret, class... Args>
class DynamicCaller<id_type, Ret(Args...)> {
public:
    typedef function<Ret(Args...)>  func_type;
    typedef map<id_type, func_type> func_map;

    /* @brief: Register new function by identifier */
    void PutFunction(const func_type& function, const id_type& id);

    /* @brief: Get std::function object by id
     * throws runtime_error exception of lookup failure */
    const func_type& GetFunction(const id_type& id);

    /* @brief: Call function and get return value
     * throws runtime_error exception of lookup failure */
    Ret CallFunction(const id_type& id, Args&...);
private:
    enum StorageOpcode {
        GET,
        PUT
    };

    const func_type& _Storage(StorageOpcode op, const func_type* func, const id_type& id);
    const func_type& _StorageGet(func_map& fmap, const id_type& id);
    void             _StoragePut(func_map& fmap, const func_type* func, const id_type& id);
};

template<typename id_type, class Ret, class... Args>
void
DynamicCaller<id_type, Ret(Args...)>::

PutFunction(const func_type& function, const id_type& id) {
    _Storage(PUT, &function, id);
}

template<typename id_type, class Ret, class... Args>
const function<Ret(Args...)>&
DynamicCaller<id_type, Ret(Args...)>::

GetFunction(const id_type& id) {
    return _Storage(GET, nullptr, id);
}

template<typename id_type, class Ret, class... Args>
Ret
DynamicCaller<id_type, Ret(Args...)>::

CallFunction(const id_type& id, Args&... args) {
    auto func = _Storage(GET, nullptr, id);
    return func(args...);
}

template<typename id_type, class Ret, class... Args>
const function<Ret(Args...)>&
DynamicCaller<id_type, Ret(Args...)>::

_Storage(StorageOpcode op, const func_type* func, const id_type& id) {
    static func_map fmap;

    if(op == GET) {
        return _StorageGet(fmap, id);
    } else {
        assert(func != nullptr);
        _StoragePut(fmap, func, id);
    }

    /// Returning null reference
    return *reinterpret_cast<func_type*>(NULL);
}

template<typename id_type, class Ret, class... Args>
const function<Ret(Args...)>&
DynamicCaller<id_type, Ret(Args...)>::

_StorageGet(func_map& fmap, const id_type& id) {
    auto it = fmap.find(id);
    if(it == fmap.end()) {
        stringstream ss;
        ss << "Lookup failed: no function " << id;
        throw runtime_error(ss.str());
    }
    return it->second;
}

template<typename id_type, class Ret, class... Args>
void
DynamicCaller<id_type, Ret(Args...)>::

_StoragePut(func_map& fmap, const func_type* func, const id_type& id) {
    fmap[id] = *func;
}

#ifdef MSVD_LIB
}
#endif

#endif // DYNAMICCALLER_H
