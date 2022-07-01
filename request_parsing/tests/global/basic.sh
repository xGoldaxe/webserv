#!/bin/bash

# this is the most basic test
# include all future
# should work prefectly

# utils for all tests
ERROR_RES="error 400, bad request\n"

# request
echo -en "\
POST / HTTP/1.1\r\n\
Host: eqw\r\n\
Content-Length: 5\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: POST
PATH: /
VERSION: HTTP/1.1
==>[Content-Length]: [5]
==>[Host]: [eqw]
" > $OUTPUT