#!/bin/bash

ERROR_RES="error 400, Bad Request\n"

# request
echo -en "\
GET /test.html?weqweqwe?dqwdqwd HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /test.html
VERSION: HTTP/1.1
weqweqwe?dqwdqwd\
" > $OUTPUT