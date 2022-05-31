#!/bin/bash

make

mkdir -p ./tests/res
mkdir -p ./tests/diff

TEMPFILE=./tests/data.out

TESTPATH=tests
FILES=`find ${TESTPATH} -name "*.sh"`

if [[ "$1" != "" ]] ; then
	# do only one, in debug mode
	
	bash "./tests/$1" "./tests/res/`basename "$1"`.excepted" > $TEMPFILE
	echo "RUNNING TEST: "
	cat $TEMPFILE
	echo "<==========>"
	printf "\033[1;35m"
	./reqparser < $TEMPFILE
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
		bash $TEST $EXCEPTED > $TEMPFILE
		./reqparser < $TEMPFILE > $OUTPUT
		
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