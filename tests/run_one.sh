#!/bin/bash -e

(cd $1 ; sudo ./server.sh ; ./client.sh) > $1.log