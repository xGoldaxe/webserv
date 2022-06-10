#!/bin/bash

# empty request should send an errror 400
ERROR_RES="error 400, Bad Request\n"

# request
echo -en "     \r\n"

# excepted
OUTPUT=$1
echo -en $ERROR_RES > $OUTPUT