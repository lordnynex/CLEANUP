#include "set.h"
#include "iterator.h"
#include <iostream>

using namespace v8;

void NodeSet::init(Local<Object> target) {
    Nan::HandleScope scope;

    Local<FunctionTemplate> constructor = Nan::New<FunctionTemplate>(Constructor);

    constructor->SetClassName(Nan::New("NodeSet").ToLocalChecked());
    constructor->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(constructor, "add", Add);
    Nan::SetPrototypeMethod(constructor, "has", Has);
    Nan::SetPrototypeMethod(constructor, "entries", Entries);
    Nan::SetPrototypeMethod(constructor, "keys", Keys);
    Nan::SetPrototypeMethod(constructor, "values", Values);
    Nan::SetPrototypeMethod(constructor, "delete", Delete);
    Nan::SetPrototypeMethod(constructor, "clear", Clear);
    Nan::SetPrototypeMethod(constructor, "forEach", ForEach);
    Nan::SetPrototypeMethod(constructor, "rehash", Rehash);
    Nan::SetPrototypeMethod(constructor, "reserve", Reserve);
    Nan::SetAccessor(constructor->InstanceTemplate(), Nan::New("size").ToLocalChecked(), Size);

    target->Set(Nan::New("NodeSet").ToLocalChecked(), constructor->GetFunction());

    SingleNodeIterator::init(target);
}

NodeSet::NodeSet() {
    this->_version = 0;
    this->_iterator_count = 0;
}

NodeSet::~NodeSet() {
    for(SetType::const_iterator itr = this->_set.begin(); itr != this->_set.end(); ) {
        itr = this->_set.erase(itr);
    }
}

uint32_t NodeSet::StartIterator() {
    uint32_t version = this->_version;
    this->_version++;
    if (this->_iterator_count == 0) {
        // if this is the first iterator, set the max load facto to infinity
        // so that a rehash doesn't happen while iterating
        this->_old_load_factor = this->_set.max_load_factor();
        this->_set.max_load_factor(std::numeric_limits<float>::infinity());
    }
    this->_iterator_count++;

    // return the latest version that should be valid for this iterator
    return version;
}

void NodeSet::StopIterator() {
    this->_iterator_count--;
    if (this->_iterator_count != 0) {
        return;
    }
    // that was the last iterator running, so now go through the whole set
    // and actually delete anything marked for deletion
    for(SetType::const_iterator itr = this->_set.begin(); itr != this->_set.end(); ) {
        if (itr->IsDeleted()) {
            itr = this->_set.erase(itr);
        } else {
            itr++;
        }
    }
    // since it was the last iterator, reset the max load factor back
    // to what it was before the first iterator, this might cause a
    // rehash to happen
    this->_set.max_load_factor(this->_old_load_factor);
}

SetType::const_iterator NodeSet::GetBegin() {
    return this->_set.begin();
}

SetType::const_iterator NodeSet::GetEnd() {
    return this->_set.end();
}


NAN_METHOD(NodeSet::Constructor) {
    Nan::HandleScope scope;
    NodeSet *obj = new NodeSet();

    Local<Array> arr;
    uint32_t i;
    Local<String> add = Nan::New("add").ToLocalChecked();
    Local<String> next = Nan::New("next").ToLocalChecked();
    Local<String> done = Nan::New("done").ToLocalChecked();
    Local<String> value = Nan::New("value").ToLocalChecked();
    Local<Object> iter;
    Local<Value> func_info[1];
    Local<Function> adder;
    Local<Function> next_func;

    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());

    if(info.Length() == 0) {
        return;
    }

    if (!info.This()->Has(add) || !Nan::Get(info.This(), add).ToLocalChecked()->IsFunction()) {
        Nan::ThrowTypeError("Invalid add method");
        return;
    }

    adder = Nan::Get(info.This(), add).ToLocalChecked().As<Function>();
    if (info[0]->IsArray()) {
        arr = info[0].As<Array>();
        for (i = 0; i < arr->Length(); i += 1) {
            func_info[0] = Nan::Get(arr, i).ToLocalChecked();
            adder->Call(info.This(), 1, func_info);
        }
    } else if (info[0]->IsObject()) {
        iter = Nan::To<Object>(info[0]).ToLocalChecked();
        if (iter->Has(next) && iter->Get(next)->IsFunction() && iter->Has(value) && iter->Has(done)) {
            next_func = Nan::Get(iter, next).ToLocalChecked().As<Function>();
            // a value iterator
            while(!Nan::Get(iter, done).ToLocalChecked()->BooleanValue()) {
                func_info[0] = Nan::Get(iter, value).ToLocalChecked();
                adder->Call(info.This(), 1, func_info);
                next_func->Call(iter, 0, 0);
            }
        }
    }

    return;
}

NAN_METHOD(NodeSet::Has) {
    Nan::HandleScope scope;

    if (info.Length() < 1 || info[0]->IsUndefined() || info[0]->IsNull()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    NodeSet *obj = Nan::ObjectWrap::Unwrap<NodeSet>(info.This());
    VersionedPersistent persistent(obj->_version, info[0]);

    SetType::const_iterator itr = obj->_set.find(persistent);
    SetType::const_iterator end = obj->_set.end();

    while(itr != end && itr->IsDeleted()) {
        itr++;
    }

    if(itr == end || !info[0]->StrictEquals(itr->GetLocal())) {
        //do nothing and return false
        info.GetReturnValue().Set(Nan::False());
        return;
    }

    info.GetReturnValue().Set(Nan::True());
    return;
}

NAN_METHOD(NodeSet::Add) {
    Nan::HandleScope scope;

    if (info.Length() < 1 || info[0]->IsUndefined() || info[0]->IsNull()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    NodeSet *obj = Nan::ObjectWrap::Unwrap<NodeSet>(info.This());
    VersionedPersistent persistent(obj->_version, info[0]);

    SetType::const_iterator itr = obj->_set.find(persistent);
    SetType::const_iterator end = obj->_set.end();

    while(itr != end && itr->IsDeleted()) {
        itr++;
    }

    if(itr == end || !info[0]->StrictEquals(itr->GetLocal())) {
        obj->_set.insert(persistent);
    }

    //Return this
    info.GetReturnValue().Set(info.This());
    return;
}

NAN_METHOD(NodeSet::Entries) {
    Nan::HandleScope scope;

    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());

    Local<Object> iter = SingleNodeIterator::New(SingleNodeIterator::KEY_TYPE | SingleNodeIterator::VALUE_TYPE, obj);

    info.GetReturnValue().Set(iter);
    return;
}

NAN_METHOD(NodeSet::Keys) {
    Nan::HandleScope scope;

    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());

    Local<Object> iter = SingleNodeIterator::New(SingleNodeIterator::KEY_TYPE, obj);

    info.GetReturnValue().Set(iter);
    return;
}

NAN_METHOD(NodeSet::Values) {
    Nan::HandleScope scope;

    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());

    Local<Object> iter = SingleNodeIterator::New(SingleNodeIterator::VALUE_TYPE, obj);

    info.GetReturnValue().Set(iter);
    return;
}

NAN_METHOD(NodeSet::Delete) {
    Nan::HandleScope scope;

    if (info.Length() < 1 || info[0]->IsUndefined() || info[0]->IsNull()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    NodeSet *obj = Nan::ObjectWrap::Unwrap<NodeSet>(info.This());
    VersionedPersistent persistent(obj->_version, info[0]);
    bool using_iterator = (obj->_iterator_count != 0);
    bool ret;

    if (using_iterator) {
        obj->StartIterator();
    }

    SetType::const_iterator itr = obj->_set.find(persistent);
    SetType::const_iterator end = obj->_set.end();

    while(itr != end && itr->IsDeleted()) {
        itr++;
    }

    ret = (itr != end && info[0]->StrictEquals(itr->GetLocal()));

    if (using_iterator) {
        if (ret) {
            itr->Delete();
        }
        obj->StopIterator();
    } else {
        if (ret) {
            obj->_set.erase(itr);
        }
    }

    if (ret) {
        info.GetReturnValue().Set(Nan::True());
    } else {
        info.GetReturnValue().Set(Nan::False());
    }
    return;
}

NAN_METHOD(NodeSet::Clear) {
    Nan::HandleScope scope;

    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());
    bool using_iterator = (obj->_iterator_count != 0);

    if (using_iterator) {
        obj->StartIterator();
    }

    for(SetType::const_iterator itr = obj->_set.begin(); itr != obj->_set.end(); ) {
        if (using_iterator) {
            itr->Delete();
            itr++;
        } else {
            itr = obj->_set.erase(itr);
        }
    }

    if (using_iterator) {
        obj->StopIterator();
    }

    info.GetReturnValue().Set(Nan::Undefined());
    return;
}

NAN_GETTER(NodeSet::Size) {
    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());
    uint32_t size = 0;
    if (obj->_iterator_count == 0) {
        size = obj->_set.size();
        info.GetReturnValue().Set(Nan::New<Integer>(size));
        return;
    }

    SetType::const_iterator itr = obj->_set.begin();
    SetType::const_iterator end = obj->_set.end();
    for (; itr != end; itr++) {
        if (itr->IsValid(obj->_version)) {
            size += 1;
        }
    }

    info.GetReturnValue().Set(Nan::New<Integer>(size));
    return;
}

NAN_METHOD(NodeSet::Rehash) {
    Nan::HandleScope scope;

    if (info.Length() < 1 || !info[0]->IsInt32()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());

    if (obj->_iterator_count != 0) {
        // iterators are currently happening, the rehash
        // will happen automatically when the count reaches
        // zero
        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    size_t buckets = info[0]->Int32Value();

    obj->_set.rehash(buckets);

    info.GetReturnValue().Set(Nan::Undefined());
    return;
}

NAN_METHOD(NodeSet::Reserve) {
    Nan::HandleScope scope;

    if (info.Length() < 1 || !info[0]->IsInt32()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());

    if (obj->_iterator_count != 0) {
        // iterators are currently happening, the rehash
        // will happen automatically when the count reaches
        // zero
        info.GetReturnValue().Set(Nan::Undefined());
        return;
    }

    size_t elements = info[0]->Int32Value();

    obj->_set.rehash(elements);

    info.GetReturnValue().Set(Nan::Undefined());
    return;
}

NAN_METHOD(NodeSet::ForEach) {
    Nan::HandleScope scope;

    NodeSet *obj = ObjectWrap::Unwrap<NodeSet>(info.This());

    if (info.Length() < 1 || !info[0]->IsFunction()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }
    Local<Function> cb = info[0].As<v8::Function>();

    Local<Object> ctx;
    if (info.Length() > 1 && info[1]->IsObject()) {
        ctx = info[1]->ToObject();
    } else {
        ctx = Nan::GetCurrentContext()->Global();
    }

    const unsigned argc = 3;
    Local<Value> argv[argc];
    argv[2] = info.This();

    uint32_t version = obj->StartIterator();
    SetType::const_iterator itr = obj->_set.begin();
    SetType::const_iterator end = obj->_set.end();

    while (itr != end) {
        if (itr->IsValid(version)) {
            argv[0] = itr->GetLocal();
            argv[1] = argv[0];
            cb->Call(ctx, argc, argv);
        }
        itr++;
    }
    obj->StopIterator();

    info.GetReturnValue().Set(Nan::Undefined());
    return;
}


extern "C" void
init (Local<Object> target) {
    Nan::HandleScope scope;

    NodeSet::init(target);
}

NODE_MODULE(set, init);
