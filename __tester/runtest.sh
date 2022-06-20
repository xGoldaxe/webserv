for var in "$@"
do
    arrModular=(${var//=/ })

    if [ "$var" == "--re" ] ; then
        make re
    elif [ "$var" == "--debug" ] ; then
        DEBUG="y"
    else
        MODE=$var
    fi
done

make

if [[ "$MODE" == "demo" ]] ; then
	./tester "suite=tests/demo.cpp" "suite=demo/empty" "suite=demo/bad"
elif [[ "$MODE" == "header" ]] ; then
	./tester "suite=tests/content-length.cpp" "suite=tests/header_chunked.cpp"
elif [[ "$MODE" == "multipart" ]] ; then
	./tester "suite=tests/multipart.cpp"
elif [[ "$MODE" == "chunk" ]] ; then
	./tester "suite=chunk.cpp"
elif [[ "$MODE" == "length" ]] ; then
	./tester "suite=tests/content-length.cpp"
elif [[ "$MODE" == "utils" ]] ; then
	./tester "suite=tests/utils.cpp"
else
	./tester
fi

if [[ "$DEBUG" != "y" ]] ; then
	make clean
fi