#!/bin/bash

# this is the most basic test
# include all future
# should work prefectly

# utils for all tests
ERROR_RES="error 400, bad request\n"

# request
echo -en "\
GET / HTTP/1.1\r\n\
Content-Length: 10 \r\n\
\r\n\
012345678\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /
VERSION: HTTP/1.1
==>[Content-Length]: [10]
<==={START BODY}===>
012345678\n\
<==={END BODY}===>
" > $OUTPUT