// Operator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "Database.h"
#include "Data.h"
#include "Filter.h"
#include "Operations.h"

int main()
{
    std::cout << "Types:\n";
    database::print_types();
    std::cout << "\n";

    database data;
    data.init();
    
    datatype_id namegiver[] = { type_namegiver.hash, 0 };
    datatype_id peep0[] = { type_peep_name.hash, type_peep_location.hash, type_peep_eating.hash, 0 };
    datatype_id peep1[] = { type_peep_name.hash, type_peep_location.hash, type_peep_eating.hash, 0 };
    datatype_id peep2[] = { type_peep_name.hash, type_peep_location.hash, 0 };
    datatype_id peep3[] = { type_peep_name.hash, type_peep_location.hash, type_peep_eating.hash, 0 };
    datatype_id peep4[] = { type_peep_name.hash, 0 };
    
    std::cout << "Adding entities\n\n";

    data.hard_add_entity(namegiver);
    datatype_id peep_id0 = data.hard_add_entity(peep0);
    datatype_id peep_id1 = data.hard_add_entity(peep1);
    datatype_id peep_id2 = data.hard_add_entity(peep2);
    datatype_id peep_id3 = data.hard_add_entity(peep3);
    datatype_id peep_id4 = data.hard_add_entity(peep4);

    std::cout << "Overview:\n";
    data.print_small_overview();
    std::cout << "\n";
    
    filter_element  filter_element_get_namegiver;
    filter_element  filter_element_get_peep_name;
    filter_element  filter_element_get_peep_food;

    filter_element_get_namegiver.count          = filter_count_first;
    filter_element_get_namegiver.datatype       = type_namegiver.hash;

    filter_element_get_peep_name.count          = filter_count_all;
    filter_element_get_peep_name.datatype       = type_peep_name.hash;

    filter_element_get_peep_food.count          = filter_count_all;
    filter_element_get_peep_food.datatype       = type_peep_eating.hash;
    
    std::vector<filter_element> filter_get_namegiver = { filter_element_get_namegiver };
    std::vector<filter_element> filter_peep_name_namegiver = { filter_element_get_namegiver, filter_element_get_peep_name };
    std::vector<filter_element> filter_peep_name = { filter_element_get_peep_name };
    std::vector<filter_element> filter_peep_name_food = { filter_element_get_peep_name, filter_element_get_peep_food };
    
    std::cout << "Namegiver hello:\n";
    data.operator_with_filter(filter_get_namegiver, &operation_namegiver_hello);
    std::cout << "\n";
    
    std::cout << "Giving names:\n";
    data.operator_with_filter(filter_peep_name_namegiver, &operation_give_name);
    std::cout << "\n";
    
    std::cout << "Shout name:\n";
    data.operator_with_filter(filter_peep_name, &operation_shout_name);
    std::cout << "\n";
    
    std::cout << "Who has food and a name?\n";
    data.operator_with_filter(filter_peep_name_food, &operation_shout_food);
    std::cout << "\n";
    
    std::cout << "Removing food for peep 0, who now has food and a name?\n";
    data.hard_remove_entry(peep_id0, type_peep_eating.hash);
    data.operator_with_filter(filter_peep_name_food, &operation_shout_food);
    std::cout << "\n";
    
    std::cout << "Removing name for peep 3, who now has food and a name?\n";
    data.hard_remove_entry(peep_id3, type_peep_name.hash);
    data.operator_with_filter(filter_peep_name_food, &operation_shout_food);
    std::cout << "\n";
    
    std::cout << "Shout name:\n";
    data.operator_with_filter(filter_peep_name, &operation_shout_name);
    std::cout << "\n";

    std::cout << "End\n";
    std::getchar();
    return 0;
}

