#!/bin/bash -e

# ./run_one.sh (Nom du dossier de test) [nombres de clients à faire tourner en parallèle]
# Exemple:
#  ./run_one.sh test-empty-body-length # Fera tourner le test "test-empty-body-length" avec 1 client
#  ./run_one.sh test-empty-body-length 10 # Fera tourner le test "test-empty-body-length" avec 10 clients simultanés

if [[ -z "$2" ]]
then
    CLIENTS_NUMBER=1;
else
    CLIENTS_NUMBER=$(($2));
fi

if (( $CLIENTS_NUMBER > 1 ))
then
    echo "Launching stress test"

    testsPID=()

    for (( c=0; c <= $CLIENTS_NUMBER-1; c++ ))
    do
        (cd $1 ; ./client.sh &> /dev/null) &
        BACK_PID=$!
        testsPID[${#testsPID[@]}]=$BACK_PID
        echo "launched $c - $BACK_PID"
    done

    echo "Attente que les tests se finissent"
    for t in ${testsPID[@]}; do
        echo "En attente du test avec le ProcessID $t"
        wait $t
    done
else
    echo "Launching single tests with logs."
    (cd $1 ; sudo ./server.sh ; echo "//////////////////// Test avec webserv"; ./client.sh 3000; echo -e "\n//////////////////// Test avec nginx"; ./client.sh) > $1.log
fi
