make

if [[ "$1" == "chunk" ]] ; then
	./tester "suite=chunk"
elif [[ "$1" == "content" ]] ; then
	./tester "suite=content-length"
else
	./tester
fi