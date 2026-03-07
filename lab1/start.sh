#!/bin/bash

echo "Starting testing"

gcc main.c tests.c arr_stack.c list_stack.c -o testing

./testing