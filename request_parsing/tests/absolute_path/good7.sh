#!/bin/bash

ERROR_RES="error 501, Not Implemented\n"

# request
echo -en "\
GET /1/2/3/../../../dqw HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /1/2/3/../../../dqw
VERSION: HTTP/1.1
" > $OUTPUT