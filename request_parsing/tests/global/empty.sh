#!/bin/bash

# empty request should send an errror 400

# utils for all tests
ERROR_RES="error 400, bad request\n"

# request
echo -en ""

# excepted
OUTPUT=$1
echo -en $ERROR_RES > $OUTPUT