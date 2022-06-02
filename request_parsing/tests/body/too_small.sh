#!/bin/bash

# utils for all tests
ERROR_RES="error 400, bad request\n"

# request
echo -en "\
GET / HTTP/1.1\r\n\
Content-Length: 10 \r\n\
\r\n\
0123\n\
"

# excepted
OUTPUT=$1
echo -en $ERROR_RES > $OUTPUT