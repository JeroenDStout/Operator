#pragma once
#include "stdint.h"
#include "Database.h"

enum {
    filter_count_all,
    filter_count_first
};

enum {
    filter_requirement_none,
    filter_requirement_has_datatype
};

struct filter_element {
    uint8_t         count;
    datatype_id     datatype;
};

struct filter {
    std::vector<filter_element>         elements;
};