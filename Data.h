#pragma once

#include "Datatype.h"

struct data_peep_name {
    char         name[32];
};

struct data_peep_location {
    uint8_t      location_x, location_y;
};

struct data_peep_eating {
    uint8_t      eating_type;
};

struct data_namegiver {
    uint8_t      index;
};

DECLARE_DATA_TYPE(data_peep_name,       peep_name);
DECLARE_DATA_TYPE(data_peep_location,   peep_location);
DECLARE_DATA_TYPE(data_peep_eating,     peep_eating);
DECLARE_DATA_TYPE(data_namegiver,       namegiver);