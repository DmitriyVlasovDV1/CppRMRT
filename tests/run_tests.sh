#!/bin/bash

BINARY_DIRECTORY=$1
VECTORS_TEST=vectors-test
MATRICES_TEST=matrices-test

FAIL=0
echo -e "===== \033[1mRunning math module tests\033[0m ====="
{
	echo -e "\n==> \033[1mMatrices\033[0m"
	if ! "${BINARY_DIRECTORY}/${MATRICES_TEST}"; then
		FAIL=1
	fi;

	echo -e "\n==> \033[1mVectors\033[0m"
	if ! "${BINARY_DIRECTORY}/${VECTORS_TEST}"; then
		FAIL=1
	fi;
}

if [[ "$FAIL" == "0" ]]; then
    echo -e "\n===== \033[32;1mALL PASS\033[0m ====="
	exit 0
else
    echo -e "\n===== \033[31;1mSOME FAIL\033[0m ====="
	exit 1
fi

