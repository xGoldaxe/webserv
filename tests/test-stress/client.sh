#!/bin/bash -e
mkdir -p logs/

# Display section name in the stderr (so it appears in the terminal)
>&2 echo "**************************************"
>&2 echo "* Section stress-test                 "
>&2 echo "**************************************"

port=3000

# testMimeType(test_name, file)
testMimeType() {
    >&2 echo $1 # Display test name in the stderr (so it appears in the terminal)
    echo -en "GET /$2 HTTP/1.1\r\nHost: 127.0.0.1:$port\r\nUser-Agent: Tester\r\nContent-Length: 0\r\n\r\n" | netcat -q 0 -w 60 127.0.0.1 $port | tee logs/$port-$1.logs
    echo ""
}

echo "====================================== Test avec un fichier HTML"
testMimeType "jpeg" "img.jpg"

