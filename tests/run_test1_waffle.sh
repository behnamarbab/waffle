#!/bin/bash

# For Mac
if [ $(command uname) = "Darwin" ]; then
    if ! [ -x "$(command -v greadlink)"  ]; then
        brew install coreutils
    fi
    BIN_PATH=$(greadlink -f "$0")
    ROOT_DIR=$(dirname $(dirname $BIN_PATH))
# For Linux
else
    BIN_PATH=$(readlink -f "$0")
    ROOT_DIR=$(dirname $(dirname $BIN_PATH))
fi

echo "Make Clean Install!"

export AFL_PATH=${ROOT_DIR}/tool/MemLock
export TEST_DIR=${ROOT_DIR}/tests/test1

if [ -d "/tmp/waffle_tests/test1"  ]; then
    rm -rf /tmp/waffle_tests/test1
fi

mkdir -p /tmp/waffle_tests/test1

cp -r ${TEST_DIR} /tmp/waffle_tests
export TMP_TEST_DIR=/tmp/waffle_tests/test1

echo "----------------"
echo "Making Waffle..."
echo "----------------"
make -C ${AFL_PATH}

echo "--------------"
echo "Making LLVM..."
echo "--------------"
make -C ${AFL_PATH}/llvm_mode

cd ${TMP_TEST_DIR}
ls
# Compile using llvm
${AFL_PATH}/memlock-stack-clang -g -O0 -fsanitize=address example1.c -o example1Waffle
if [ -d "${TEST_DIR}/in"  ]; then
    rm -rf in
fi
mkdir in
echo a > in/testcase
i=0
for ((i=1; i<100; i++))
do
    if ! [ -d "${TEST_DIR}/out_Waffle$i" ]; then
        break
    fi
done
echo "Execute the fuzzing..."
# export ASAN_OPTIONS=detect_odr_violation=0:allocator_may_return_null=1:abort_on_error=1:symbolize=0:detect_leaks=0
${AFL_PATH}/memlock-stack-fuzz -i ${TEST_DIR}/in -o ${TEST_DIR}/out_Waffle$i -m none -d -t 5000 -- ${TEST_DIR}/example1Waffle @@ || true
# echo "Execute the afl-showmap..."
# ss=$(echo 0 | ${AFL_PATH}/afl-showmap -m none -q -o .example1Waffle ${TEST_DIR}/example1Waffle) || true
echo "-------- $ss"
echo
