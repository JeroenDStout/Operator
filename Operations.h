#pragma once
#include "Data.h"
#include <iostream>
#include "Database.h"

struct alignas(sizeof(void*)) operation_namegiver_hello_instr {
    uint32_t            count;
    data_namegiver      **namegiver;
};

inline void operation_namegiver_hello(void *arg, database *)
{
    operation_namegiver_hello_instr &instr = *(operation_namegiver_hello_instr*)arg;

    std::cout << "Saying hello for " << instr.count << " namegivers.\n";

    for (uint32_t i = 0; i < instr.count; i++) {
        instr.namegiver[i]->index = 0;
    }
}

struct alignas(sizeof(void*)) operation_give_name_instr {
    uint32_t            count;
    data_namegiver      **namegiver;
    data_peep_name      **peep_name;
};

inline void operation_give_name(void *arg, database *)
{
    operation_give_name_instr &instr = *(operation_give_name_instr*)arg;
    
    std::cout << "Giving " << instr.count << " names.\n";

    static const char *names[] = { "Bob", "Michael", "Sarah", "Fran", "Steve" };

    for (uint32_t i = 0; i < instr.count; i++) {
        const char *name = names[instr.namegiver[0]->index];
        memcpy(instr.peep_name[i]->name, name, strlen(name) + 1);
        instr.namegiver[0]->index++;
    }
}

struct alignas(sizeof(void*)) operation_shout_name_instr {
    uint32_t            count;
    data_peep_name      **peep_name;
};

inline void operation_shout_name(void *arg, database *)
{
    operation_shout_name_instr &instr = *(operation_shout_name_instr*)arg;
    
    for (uint32_t i = 0; i < instr.count; i++) {
        std::cout << "I am " << instr.peep_name[i]->name << "!\n";
    }
}

struct alignas(sizeof(void*)) operation_food_instr {
    uint32_t            count;
    data_peep_name      **peep_name;
    data_peep_eating    **peep_eating;
};

inline void operation_shout_food(void *arg, database *)
{
    operation_food_instr &instr = *(operation_food_instr*)arg;
    
    for (uint32_t i = 0; i < instr.count; i++) {
        std::cout << "I have food! I am " << instr.peep_name[i]->name << ".\n";
    }
}