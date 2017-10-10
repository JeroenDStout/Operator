#pragma once
#include <vector>
#include "Datatype.h"

typedef uint32 count_type;
typedef uint32 datatype_id;
typedef uint32 entity_id;
typedef uint32 data_set_index;

enum {
    entity_id_none      = (entity_id)(-1)
};

struct data_set {
    count_type      count;
    entity_id       *index;
    uint32_t        element_size;
    void            *data;
};

struct filter_element;
class database;

typedef void (*database_operator_ptr)(void *, database *);

class database
{
public:
    static std::vector<data_type_factory>  *ptr_static_types;

    static void print_types();

    entity_id    next_entity_id;
    data_set     *data_sets;

    database();

public:
    void         init();
    entity_id    hard_add_entity(datatype_id *);
    void         hard_add_entry(entity_id, datatype_id);
    void         hard_remove_entry(entity_id, datatype_id);

    void         operator_with_filter(std::vector<filter_element>&, database_operator_ptr);

    data_set     *get_data_set(datatype_id);

    void         print_small_overview();
};