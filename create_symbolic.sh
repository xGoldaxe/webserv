mkdir -p .build
for FILE in $1
do
	FN=.build/"$(basename -- $FILE)"
	# if [ ! -f "$FN" ]; then
		ln -f $FILE $FN
	# fi
done