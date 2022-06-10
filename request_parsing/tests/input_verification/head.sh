#!/bin/bash

ERROR_RES="error 501, Not Implemented\n"

# request
echo -en "\
HEAD / HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: HEAD
PATH: /
VERSION: HTTP/1.1
" > $OUTPUT