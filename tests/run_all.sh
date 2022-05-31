#!/bin/bash -e

# Get all tests folder
find . -maxdepth 1 -type d ! -path ./logs ! -path . -exec ./run_one.sh {} \;
