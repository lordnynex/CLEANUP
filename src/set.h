#ifndef NODE_SET_H
#define NODE_SET_H

#include <string>
#include <iostream>
#ifdef __APPLE__
#include <tr1/unordered_set>
#define unordered_set std::tr1::unordered_set
#else
#include <unordered_set>
#define unordered_set std::unordered_set
#endif
#include <node.h>
#include <nan.h>
#include "v8_value_hasher.h"

typedef unordered_set<VersionedPersistent, v8_value_hash, v8_value_equal_to> SetType;

class NodeSet : public Nan::ObjectWrap {
public:
    static void init(v8::Local<v8::Object> target);

    uint32_t StartIterator();
    void StopIterator();
    SetType::const_iterator GetBegin();
    SetType::const_iterator GetEnd();

private:
    NodeSet();
    ~NodeSet();

    SetType _set;
    // each time an iterator starts, the _version gets incremented
    // it is used so that items added after an iterator starts are
    // not visited in the iterator
    uint32_t _version;
    // we keep track of how many running iterators there are so that
    // we can clean up when the last iterator is done
    uint32_t _iterator_count;
    // we store the load factor here before the first iterator starts
    // and then set the load factor to infinity so that rehashes of
    // the set don't happen (from inserts that happen inside an iterator)
    // once the last iterator finishes, the load factor is reset, and
    // a rehash might happen
    float _old_load_factor;

    // new NodeSet() or new NodeSet(buckets)
    static NAN_METHOD(Constructor);

    // set.has(value) : boolean
    static NAN_METHOD(Has);

    // set.add(key, value) : this
    static NAN_METHOD(Add);

    // set.entries() : iterator
    static NAN_METHOD(Entries);

    // set.keys() : iterator
    static NAN_METHOD(Keys);

    // set.values() : iterator
    static NAN_METHOD(Values);

    // set.delete(value) : boolean
    static NAN_METHOD(Delete);

    // set.clear() : undefined
    static NAN_METHOD(Clear);

    // set.size() : number of elements
    static NAN_GETTER(Size);

    // set.rehash(buckets) : undefined
    static NAN_METHOD(Rehash);

    //set.reserve(size) : undefined
    static NAN_METHOD(Reserve);

    //set.forEach(function (key, value) {...}) : undefined
    static NAN_METHOD(ForEach);
};

#endif
