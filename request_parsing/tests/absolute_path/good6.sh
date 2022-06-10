#!/bin/bash

ERROR_RES="error 501, Not Implemented\n"

# request
echo -en "\
GET /1/2/3/../../.. HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /1/2/3/../../..
VERSION: HTTP/1.1
" > $OUTPUT