#!/bin/bash

make

mkdir -p ./tests/res
mkdir -p ./tests/diff

TESTPATH=tests
FILES=`find ${TESTPATH} -name "*.sh"`

if [[ "$1" != "" ]] ; then
	# do only one, in debug mode
	
	bash $1 "./tests/res/`basename "$1"`.excepted" > data.out
	echo "RUNNING TEST: "
	cat data.out
	echo "<==========>"
	printf "\033[1;35m"
	./reqparser < data.out
	printf "\033[0m"
	echo "<==========>"
	
else
	#do all tests
	for TEST in $FILES
	do
		#define
		EXCEPTED="./tests/res/`basename "$TEST"`.excepted"
		OUTPUT="./tests/res/`basename "$TEST"`.output"
		DIFFPATH="./tests/diff/`basename "$TEST"`.diff"

		#run
		bash $TEST $EXCEPTED > data.out
		./reqparser < data.out > $OUTPUT
		
		#compare
		diff $EXCEPTED $OUTPUT > $DIFFPATH
		CONDITION=$(wc -l < $DIFFPATH)

		if [ $CONDITION -eq 0 ]; then
			echo "✅ [" $TEST "] -OK"
		else
			echo "❌ [" $TEST "] -KO"
		fi

	done
fi