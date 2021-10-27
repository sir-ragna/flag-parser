#!/usr/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

cleanup()
{
    make clean >/dev/null
}

trap cleanup EXIT

check_memory_leaks()
{
    TARGET=$1
    shift # remove the first arg($1) from the $@ list
    TMPFILE=/tmp/TARGET-valgrind-out.tmp
    rm $TMPFILE 2>/dev/null
    valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --log-file="${TMPFILE}" --error-exitcode=33 "./$TARGET" $@ >/dev/null
    RETCODE=$?
    if [ "${RETCODE}" -eq "127" ]; then # we have a memory issue
        printf "File not found!!\n"
        printf " ${RED}[FAIL]${NC} ${TARGET} <- Could not run test\n"
        exit 3
    fi
    if [ "${RETCODE}" -eq "33" ]; then # we have a memory issue
        printf " ${RED}[FAIL]${NC} ${TARGET} <- Failed memory test\n"
        cat "${TMPFILE}"
        exit 2
    else
        printf " ${GREEN}[PASS]${NC} ${TARGET} memtest\n"
    fi
}

compile_and_assert()
{
    ALL_ARGS="$@"
    EXPECTED_CODE=$1
    TARGET=$2
    ARGS=("${ALL_ARGS[@]:2}") # alternatively use 'shift'
    # ^ from: https://stackoverflow.com/questions/10569198/take-the-first-command-line-argument-and-pass-the-rest#10569248

    # Compile the target with error!
    clang -std=c89 -Wall -Werror -o "${TARGET}" "${TARGET}.c"
    if [ "$?" -ne "0" ]; then
        printf " ${RED}[FAIL]${NC} ${TARGET} <- Compilation failed\n"
        exit 1
    fi

    # Execute the target, hide the output
    "./${TARGET}" "$@" >/dev/null 2>/dev/null
    ERR="$?"
    rm "${TARGET}" # remove the target

    if [ "$ERR" -eq "$EXPECTED_CODE" ]; then
        printf " ${GREEN}[PASS]${NC} ${TARGET} retcode\n"
    else
        printf " ${RED}[FAIL]${NC} ${TARGET} <- "
        printf "${TARGET} expected ${EXPECTED_CODE} exit code got ${ERR}\n"
        exit 1
    fi
}

compile_and_assert 5 illegal_redefine
compile_and_assert 4 null_define_bool
compile_and_assert 4 null_define_string
compile_and_assert 4 null_define_int
compile_and_assert 1 repeated_parse_flags
compile_and_assert 0 define_rest_collection

make all >/dev/null
check_memory_leaks define_rest_collection test
check_memory_leaks define_rest_collection abc xyz 1234

