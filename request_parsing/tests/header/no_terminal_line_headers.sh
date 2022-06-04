#!/bin/bash

# no terminal line, with headers

# utils for all tests
ERROR_RES="error 400, bad request\n"

# request
echo -en "\
GET / HTTP/1.1\r\n\
Random-header1:      salut      \r\n\
Random-header2:                 salut                  \r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /
VERSION: HTTP/1.1
==>[Content-Length]: [0]
==>[Random-header1]: [salut]
==>[Random-header2]: [salut]
<==={START BODY}===>
<==={END BODY}===>
" > $OUTPUT