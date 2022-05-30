#!/bin/bash -e

$1/server.sh
$1/client.sh > $1.log