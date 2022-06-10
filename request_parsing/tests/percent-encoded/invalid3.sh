#!/bin/bash

ERROR_RES="error 400, Bad Request\n"

# request
echo -en "\
GET /test%AA HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en $ERROR_RES > $OUTPUT