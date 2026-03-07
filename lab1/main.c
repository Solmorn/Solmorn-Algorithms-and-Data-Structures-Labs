#include <stdlib.h>
#include <stdio.h>

#include "tests.h"


#define TESTS_IN_SERIE 5


int main() {
    double test_arr_stack_time = 0;
    double test_list_stack_time = 0;

    for (int i = 0; i < TESTS_IN_SERIE; i++) {
        int* data_test_1 = CreateData(2500000);

        test_arr_stack_time += TestOne_arr_stack(data_test_1);
        test_list_stack_time += TestOne_list_stack(data_test_1);

        free(data_test_1);
    }


    printf("Test 1 arr  stack time: %lf\n", test_arr_stack_time/TESTS_IN_SERIE);
    printf("Test 1 list stack time: %lf\n\n", test_list_stack_time/TESTS_IN_SERIE);


    for (int i = 0; i < TESTS_IN_SERIE; i++) {
        int* data_test_2 = CreateData(2500000);

        test_arr_stack_time += TestTwo_arr_stack(data_test_2);
        test_list_stack_time += TestTwo_list_stack(data_test_2);

        free(data_test_2);
    }


    printf("Test 2 arr  stack time: %lf\n", test_arr_stack_time/TESTS_IN_SERIE);
    printf("Test 2 list stack time: %lf\n\n", test_list_stack_time/TESTS_IN_SERIE);


    for (int i = 0; i < TESTS_IN_SERIE; i++) {
        int* data_test_3_values = CreateData(2500000);
        int* data_test_3_instructions = CreateData(1000000);

        test_arr_stack_time += TestThree_arr_stack(data_test_3_values, data_test_3_instructions);
        test_list_stack_time += TestThree_list_stack(data_test_3_values, data_test_3_instructions);

        free(data_test_3_values);
        free(data_test_3_instructions);
    }


    printf("Test 3 arr  stack time: %lf\n", test_arr_stack_time/TESTS_IN_SERIE);
    printf("Test 3 list stack time: %lf\n\n", test_list_stack_time/TESTS_IN_SERIE);


    TestFour_both_stack("graph_data.txt");
    
}