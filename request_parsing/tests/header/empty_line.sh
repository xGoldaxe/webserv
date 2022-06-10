#!/bin/bash

# empty request should send an errror 400
ERROR_RES="error 400, Bad Request\n"

# request
echo -en "\
GET / HTTP/1.1\r\n\
Random-header1: salut\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /
VERSION: HTTP/1.1
==>[Random-header1]: [salut]
" > $OUTPUT