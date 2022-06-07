#!/bin/bash

# no terminal line, only start line
# utils for all tests
ERROR_RES="error 400, bad request\n"

# request
echo -en "\
GET / HTTP/1.1\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /
VERSION: HTTP/1.1
==>[Content-Length]: [0]
<==={START BODY}===>
<==={END BODY}===>
" > $OUTPUT