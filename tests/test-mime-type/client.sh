#!/bin/bash -e
mkdir -p logs/

# Display section name in the stderr (so it appears in the terminal)
>&2 echo "**************************************"
>&2 echo "* Section mime-types"
>&2 echo "**************************************"

port=$1
if [[ -z "$port" ]]
then
    port=8080
fi

# testMimeType(test_name, file)
testMimeType() {
    >&2 echo $1 # Display test name in the stderr (so it appears in the terminal)
    nc -N 127.0.0.1 $port < <(echo -en "GET /$2 HTTP/1.1\nHost: 127.0.0.1:$port\nUser-Agent: Tester\n\n") | tee logs/$port-$1.logs
    echo ""
}

echo "====================================== Test avec un fichier HTML"
testMimeType "html" "fichier.html"

echo "====================================== Test avec un fichier JS"
testMimeType "js" "fichier.js"

echo "====================================== Test avec un fichier CSS"
testMimeType "css" "fichier.css"

echo "====================================== Test avec une requete autoindex"
testMimeType "folder" ""
