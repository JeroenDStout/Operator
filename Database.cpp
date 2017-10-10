#include "stdafx.h"
#include "Database.h"
#include "Filter.h"
#include <iostream>
#include <iomanip>

std::vector<data_type_factory> *database::ptr_static_types   = 0;

database::database() {
}

void database::init() {
    this->next_entity_id            = 0;
    this->data_sets                 = new data_set[database::ptr_static_types->size()];
    
    for (int i = 0; i < database::ptr_static_types->size(); i++) {
        this->data_sets[i].count          = 0;
        this->data_sets[i].index          = 0;
        this->data_sets[i].data           = 0;
        this->data_sets[i].element_size   = (*database::ptr_static_types)[i].size;
    }
}

entity_id database::hard_add_entity(datatype_id *data) {
    entity_id   id = this->next_entity_id++;

    while (*data) {
        this->hard_add_entry(id, *data);
        data++;
    }

    return id;
}

void database::hard_add_entry(entity_id entity, datatype_id data) {
    for (int i = 0; i < database::ptr_static_types->size(); i++) {
        data_type_factory &factory = (*database::ptr_static_types)[i];
        if (data != factory.hash)
            continue;

        size_t  indexSize   = factory.size + sizeof(entity_id);

        data_set &set = this->data_sets[i];

        set.count++;
        set.index = (entity_id*)realloc(set.index, sizeof(entity_id) * set.count);
        set.data  = realloc(set.data, factory.size * set.count);

        set.index[set.count-1] = entity;
    }
}

void database::hard_remove_entry(entity_id entity, datatype_id data) {
    for (int i = 0; i < database::ptr_static_types->size(); i++) {
        data_type_factory &factory = (*database::ptr_static_types)[i];
        if (data != factory.hash)
            continue;
        
        data_set &set = this->data_sets[i];

        for (int e = 0; e < set.count; e++) {
            if (set.index[e] != entity)
                continue;

            set.index[e] = entity_id_none;
            break;
        }
    }
}

void database::operator_with_filter(std::vector<filter_element> &elements, database_operator_ptr function) {
    const uint8_t max_element_count = 8;

    void                    *data_as_first[max_element_count];
    char                    instr[sizeof(void**) * max_element_count + sizeof(uint32_t)];
    std::vector<entity_id>  selected;
    std::vector<void*>      data[max_element_count];
    bool                    all_first = true;

    for (int i = 0; i < elements.size(); i++) {
        filter_element &elem = elements[i];
        data_set *set = get_data_set(elem.datatype);
        if (!set || set->count == 0)
            return;

        if (elem.count == filter_count_first) {
            data_as_first[i] = set->data;
            *(intptr_t*)(instr + sizeof(void**) * (1+i)) = (intptr_t)(&data_as_first[i]);
            continue;
        }

        all_first = false;

        for (int q = 0; q < set->count; q++) {
            if (set->index[q] == entity_id_none)
                continue;
            bool found = false;
            for (int v = 0; v < selected.size(); v++) {
                if (selected[v] != set->index[q])
                    continue;
                found = true;
                break;
            }
            if (!found) {
                selected.insert(selected.end(), set->index[q]);
            }
        }
    }

    for (int i = 0; i < elements.size(); i++) {
        filter_element &elem = elements[i];
        data_set *set = get_data_set(elem.datatype);

        if (elem.count == filter_count_first)
            continue;
        
        for (int v = 0; v < selected.size(); v++) {
            bool found = false;

            for (int q = 0; q < set->count; q++) {
                if (selected[v] != set->index[q])
                    continue;
                found = true;
                break;
            }

            if (!found) {
                selected.erase(selected.begin() + v, selected.begin() + v + 1);
                v--;
            }
        }
    }

    for (int i = 0; i < elements.size(); i++) {
        filter_element &elem = elements[i];
        data_set *set = get_data_set(elem.datatype);

        if (elem.count == filter_count_first)
            continue;
        
        void *found = 0;

        for (int v = 0; v < selected.size(); v++) {
            for (int q = 0; q < set->count; q++) {
                if (selected[v] != set->index[q])
                    continue;
                found = (void*)((char*)(set->data) + q * set->element_size);
                break;
            }

            data[i].insert(data[i].end(), found);
        }
        
        intptr_t data_ptr = (intptr_t)(data[i].data());
        *(intptr_t*)(instr + sizeof(void**) * (1+i)) = data_ptr;
    }

    *(uint32_t*)(instr) = all_first? 1 : selected.size();

    if (selected.size() > 0 || all_first) {
        function(instr, this);
    }
}

data_set *database::get_data_set(datatype_id id) {
    std::vector<data_type_factory> &static_types = *database::ptr_static_types;
    for (int i = 0; i < static_types.size(); i++) {
        if (static_types[i].hash == id)
            return this->data_sets + i;
    }
    return 0;
}

void database::print_types() {
    std::vector<data_type_factory> &static_types = *database::ptr_static_types;
    for (int i = 0; i < static_types.size(); i++) {
        std::cout << std::setfill(' ') << std::setw(3) << i << ": ";
        std::cout << std::setfill('0') << std::setw(sizeof(data_type_factory::hash)*2) << std::hex << static_types[i].hash << " ";
        std::cout << std::setfill(' ') << std::setw(15) << static_types[i].name << " : ";
        std::cout << std::setfill(' ') << std::setw(8) << static_types[i].size << "B\n";
    }
}

void database::print_small_overview() {
    std::vector<data_type_factory> &static_types = *database::ptr_static_types;
    for (int i = 0; i < static_types.size(); i++) {
        std::cout << std::setfill(' ') << std::setw(15) << static_types[i].name << " : ";
        std::cout << std::setfill(' ') << std::setw(4) << this->data_sets[i].count << "\n";
    }
}

void database_add_static_type(const data_type_factory *base) {
    if (!database::ptr_static_types) {
        database::ptr_static_types = new std::vector<data_type_factory>();
    }
    database::ptr_static_types->insert(database::ptr_static_types->end(), *base);
}