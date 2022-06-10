#!/bin/bash

# no terminal line, only start line
# utils for all tests
ERROR_RES="error 400, Bad Request\n"

# request
echo -en "\
GET / HTTP/1.1 \r\n\
"

# excepted
OUTPUT=$1
echo -en $ERROR_RES > $OUTPUT