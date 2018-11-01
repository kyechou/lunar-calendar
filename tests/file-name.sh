#!/bin/bash

EXITCODE=0
SCRIPT_DIR="$(dirname $(realpath ${BASH_SOURCE[0]}))"
FILES=$(basename -a $(find ${SCRIPT_DIR}/../src ${SCRIPT_DIR} -type f) | sort | uniq -d)

for FILE in $FILES; do
	echo >&2
	echo "[!] multiple files have the same name '$FILE'." >&2
	echo '    Please run the following command to get the files:' >&2
	echo "        find ${SCRIPT_DIR}/../src ${SCRIPT_DIR} -type f -name $FILE" >&2
	EXITCODE=1
done

exit $EXITCODE
