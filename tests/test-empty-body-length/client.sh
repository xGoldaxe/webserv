#!/bin/bash -e
mkdir -p logs/

# Display section name in the stderr (so it appears in the terminal)
>&2 echo "**************************************"
>&2 echo "* Section content-length"
>&2 echo "**************************************"

port=$1
if [[ -z "$port" ]]
then
    port=8080
fi

# testBodyLength(test_name, method[GET, POST, DELETE], body_length, body_content)
testBodyLength() {
    >&2 echo $1 # Display test name in the stderr (so it appears in the terminal)
    ( echo -en "$2 / HTTP/1.1\nHost: 127.0.0.1:$port\nUser-Agent: Tester\nContent-Type: application/x-www-form-urlencoded\nContent-Length: $3\n\n$4\n\n") | nc -N 127.0.0.1 $port | tee logs/$port-$1.logs
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
