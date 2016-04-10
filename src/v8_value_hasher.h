#ifndef V8_VALUE_HASHER_H
#define V8_VALUE_HASHER_H

#include <string>
#include <iostream>
#include <node.h>
#ifdef __APPLE__
#include <tr1/unordered_set>
#define hash std::tr1::hash
#else
#include <unordered_set>
#define hash std::hash
#endif
#include <nan.h>

class VersionedPersistent {
public:
    VersionedPersistent(uint32_t version, v8::Local<v8::Value> handle) : _version(version), _is_deleted(false) {
        _persistent.Reset(handle);
    }

    VersionedPersistent(const VersionedPersistent &copy) {
        Nan::HandleScope scope;
        _is_deleted = copy._is_deleted;
        _version = copy._version;
        _persistent.Reset(copy.GetLocal());
    }

    ~VersionedPersistent() {
        this->Delete();
    }

    void Delete() const {
        if (_is_deleted) {
            return;
        }

        _is_deleted = true;
        _persistent.Reset();
        //delete this->_persistent;
        //this->_persistent = NULL;
    }
    bool IsDeleted() const {
        return _is_deleted;
    }
    bool IsValid(uint32_t version) const {
        return !_is_deleted && (_version <= version);
    }
    v8::Local<v8::Value> GetLocal() const {
        return v8::Local<v8::Value>::New(v8::Isolate::GetCurrent(), this->_persistent);
    }

private:
    mutable uint32_t _version;
    mutable bool _is_deleted;
    mutable Nan::Persistent<v8::Value> _persistent;
};


struct v8_value_hash
{
    size_t operator()(VersionedPersistent k) const {
        Nan::HandleScope scope;
        v8::Local<v8::Value> key = k.GetLocal();

        std::string s;
        if (key->IsString() || key->IsBoolean() || key->IsNumber()) {
            return hash<std::string>()(*Nan::Utf8String(key));
        }
        return hash<int>()(Nan::To<v8::Object>(key).ToLocalChecked()->GetIdentityHash());
    }
};

struct v8_value_equal_to
{
    bool operator()(VersionedPersistent pa, VersionedPersistent pb) const {
        Nan::HandleScope scope;

        /*
        if (pa == pb) {
            return true;
        }
        */

        if (pa.IsDeleted() || pb.IsDeleted()) {
            return false;
        }

        v8::Local<v8::Value> a = pa.GetLocal();
        v8::Local<v8::Value> b = pb.GetLocal();

        if (a->StrictEquals(b)) {          /* same as JS === */
            return true;
        }

        return Nan::To<v8::Object>(a).ToLocalChecked()->GetIdentityHash() == Nan::To<v8::Object>(b).ToLocalChecked()->GetIdentityHash();
    }
};

#endif
