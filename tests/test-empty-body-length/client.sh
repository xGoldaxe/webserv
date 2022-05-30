#!/bin/bash -e
rm -rf logs/
mkdir -p logs/

# testBodyLength(test_name, method[GET, POST, DELETE], body_length, body_content)
testBodyLength() {
    ( echo -en "$2 / HTTP/1.1\nHost: 127.0.0.1:8080\nUser-Agent: Tester\nContent-Type: application/x-www-form-urlencoded\nContent-Length: $3\n\n$4\n\n"; sleep 1; echo "quit") | nc  127.0.0.1 8080 | tee logs/$1.logs
    echo ""
}

echo "====================================== Example avec Content-Length valide (157) avec GET"
testBodyLength "valid-get" "GET" "157" "say=Hi&to=Mom"

echo "====================================== Example avec Content-Length valide (157) avec POST"
testBodyLength "valid-post" "POST" "157" "say=Hi&to=Mom"

echo "====================================== Content Length à 1 avec POST"
testBodyLength "content-length-1" "POST" "1" "say=Hi&to=Mom"

echo "====================================== Content Length à 100 avec POST"
testBodyLength "content-length-100" "POST" "100" "say=Hi&to=Mom"
