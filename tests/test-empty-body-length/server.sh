#!/bin/bash

docker rm --force nginx-test-server || true
docker run --name nginx-test-server \
            -v "$PWD/nginx.conf:/etc/nginx/nginx.conf:ro" \
            -v "$PWD/static:/www/data/" \
            -p 8080:80 \
            -d \
            nginx
