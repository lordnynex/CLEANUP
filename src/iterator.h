#ifndef NODE_ITERATOR_H
#define NODE_ITERATOR_H

#include <string>
#include <iostream>
#include <node.h>
#include <nan.h>
#include "set.h"

class SingleNodeIterator : public node::ObjectWrap {
public:
    static void init(v8::Local<v8::Object> target);
    static v8::Local<v8::Object> New(int type, NodeSet *obj);

    const static int KEY_TYPE = 1;
    const static int VALUE_TYPE = 1 << 1;

private:
    static Nan::Persistent<v8::FunctionTemplate> k_constructor;
    static Nan::Persistent<v8::FunctionTemplate> v_constructor;
    static Nan::Persistent<v8::FunctionTemplate> kv_constructor;

    SingleNodeIterator(NodeSet *set_obj);
    ~SingleNodeIterator();

    uint32_t _version;
    SetType::const_iterator _iter;
    SetType::const_iterator _end;
    NodeSet *_set_obj;

    // iterator.done : boolean
    static NAN_GETTER(GetDone);

    // iterator.value : boolean
    static NAN_GETTER(GetValue);

    // iterator.next() : undefined
    static NAN_METHOD(Next);
};

#endif
