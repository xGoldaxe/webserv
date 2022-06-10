#!/bin/bash

ERROR_RES="error 505, HTTP Version Not Supported\n"

# request
echo -en "\
GET / HTTP/1.2\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en $ERROR_RES > $OUTPUT