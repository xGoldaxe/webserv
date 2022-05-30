#!/bin/bash -e

(cd $1 ; ./server.sh ; ./client.sh) > $1.log