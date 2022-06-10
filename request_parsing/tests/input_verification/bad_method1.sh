#!/bin/bash

ERROR_RES="error 501, Not Implemented\n"

# request
echo -en "\
ADD / HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en $ERROR_RES > $OUTPUT