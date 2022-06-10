#!/bin/bash

ERROR_RES="error 400, Bad Request\n"

# request
echo -en "\
GET /test.html?salut#dqwdqwd HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /test.html
VERSION: HTTP/1.1
salut\
" > $OUTPUT