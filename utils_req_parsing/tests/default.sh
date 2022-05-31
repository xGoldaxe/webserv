#!/bin/bash

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
" > $OUTPUT