#!/bin/bash

# empty request should send an errror 400


# request
echo -en "\ns"

# excepted
OUTPUT=$1
echo -en "error 400, Bad Request\n" > $OUTPUT