#pragma once
#include <functional>

#define DECLARE_DATA_TYPE(struct, name) \
    extern data_type_factory_specific<struct> type_##name;

#define DEFINE_DATA_TYPE(struct, name) \
    data_type_factory_specific<struct> type_##name(#name);

class data_type_factory {
public:
    const char      *name;
    uint32          size;
    uint32          hash;
};

void database_add_static_type(const data_type_factory*);

template<typename data_type>
class data_type_factory_specific : public data_type_factory {
public:
    data_type_factory_specific(const char *_name) {
        this->name  = _name;
        this->size  = sizeof(data_type);
        std::hash<const char*> createHash;
        this->hash  = (uint32)createHash(_name);
        database_add_static_type(this);
    }
};