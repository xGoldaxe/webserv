#!/bin/bash

ERROR_RES="error 501, Not Implemented\n"

# request
echo -en "\
GET /dqwdqwd////qwd/dqw///d/q/w//dq/w/d//qwdqw/dqwdqwdqwdqwd/ HTTP/1.1\r\n\
\r\n\
"

# excepted
OUTPUT=$1
echo -en "\
METHOD: GET
PATH: /dqwdqwd////qwd/dqw///d/q/w//dq/w/d//qwdqw/dqwdqwdqwdqwd/
VERSION: HTTP/1.1
" > $OUTPUT