#!/bin/bash

TESTPATH=tests
FILES=`find $TESTPATH/ -name "*.sh"`

INTERNAL="./internal"

RESPATH_INTERNAL="$INTERNAL/res"
DIFFPATH_INTERNAL="$INTERNAL/diff"

mkdir -p $INTERNAL
mkdir -p $RESPATH_INTERNAL
mkdir -p $DIFFPATH_INTERNAL

rsync --quiet -av -f"+ */" -f"- *" "$TESTPATH/" "$RESPATH_INTERNAL"
rsync --quiet -av -f"+ */" -f"- *" "$TESTPATH/" "$DIFFPATH_INTERNAL"

TEMPFILE=./internal/data.out

# bad design, but fast and work
if [[ "$1" == "clean" ]] ; then
	echo "Cleaning residues ..."
	rm -rf $INTERNAL
	make fclean
	echo "Done"
	exit 0
fi

# update binary if required
make re

if [[ "$1" != "" ]] ; then
	# do only one, in debug mode
	
	bash "$TESTPATH/$1" "$RESPATH_INTERNAL/$1.excepted" > $TEMPFILE
	echo "RUNNING TEST: "
	cat $TEMPFILE
	echo "<==========>"
	printf "\033[1;35m"
	#dirty
	if [[ "$2" != "--yes=valgrind" ]] ; then
		./reqparser < $TEMPFILE
	else
		valgrind ./reqparser < $TEMPFILE
	fi
	printf "\033[0m"
	echo "<==========>"
	
else
	#do all tests
	for TEST in $FILES
	do
		TEST="${TEST##*tests/}"

		#define
		EXCEPTED="$RESPATH_INTERNAL/$TEST.excepted"
		OUTPUT="$RESPATH_INTERNAL/$TEST.output"
		DIFFPATH="$DIFFPATH_INTERNAL/$TEST.diff"

		#run
		bash $TESTPATH/$TEST $EXCEPTED > $TEMPFILE
		#dirty
		if [[ "$1" != "--yes=valgrind" ]] ; then
			./reqparser < $TEMPFILE > $OUTPUT
		else
			valgrind ./reqparser < $TEMPFILE > $OUTPUT
		fi
		
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