#!/usr/bin/env bash

test_mbc() {
	printf "[1/4] Generating test files ($(echo $1))... "
	
	# Random file "dummy_raw", raw bytes
	head --quiet --bytes=$1 /dev/urandom 2>/dev/null > dummy_raw
	if [ $? -ne 0 ]; then
		return 1
	fi

	# Random file "dummy_hex", hexadecimal ASCII
	head --quiet --bytes=$1 /dev/urandom 2>/dev/null | hexdump -ve '1/1 "%02x"' 2>/dev/null > dummy_hex
	if [ $? -ne 0 ]; then
		return 1
	fi

	printf "done.\n[2/4] Generating key ($(echo $2))... "

	# Random alphanumeric key including some special characters
	local test_key
	test_key=$(head /dev/urandom | tr -dc 'A-Za-z0-9!#$%&()*+,-./:;<=>?@[\]^_`{|}~' | head --quiet --bytes=$2 2>/dev/null )
	if [ $? -ne 0 ]; then
		return 1
	fi

	printf "done.\n[3/4] Running test... "

	# ENCRYPT/DECRYPT "dummy_raw" into "dummy_raw_out"
	../build/mbc -ek "$test_key" 2>/dev/null < dummy_raw | ../build/mbc -dk "$test_key" 2>/dev/null > dummy_raw_out
	if [ $? -ne 0 ]; then
		return 1
	fi

	# DECRYPT/ENCRYPT "dummy_hex" into "dummy_hex_out"
	../build/mbc -dxk "$test_key" 2>/dev/null < dummy_hex | ../build/mbc -exk "$test_key" 2>/dev/null > dummy_hex_out
	if [ $? -ne 0 ]; then
		return 1
	fi

	printf "done.\n[4/4] Comparing results... "

	# Compare output with original
	cmp --quiet dummy_raw dummy_raw_out && cmp --quiet dummy_hex dummy_hex_out
	local result=$?

	printf "done.\n"

	return $result
}

test_mbc "$@"
test_result=$?
rm dummy_*

if [ "$test_result" = 0 ]; then
	printf "\nTEST PASSED!\n"
	exit 0
else
	printf "\nTEST FAILED!\n"
	exit 1
fi
