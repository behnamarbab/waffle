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
export TEST_DIR=${ROOT_DIR}/tests/test6

if [ -d "/tmp/waffle_tests/test6"  ]; then
    rm -rf /tmp/waffle_tests/test6
fi

mkdir -p /tmp/waffle_tests/test6

cp -r ${TEST_DIR} /tmp/waffle_tests
export TMP_TEST_DIR=/tmp/waffle_tests/test6

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
${AFL_PATH}/memlock-stack-clang++ -g -O0 -fsanitize=address example.cpp -o example6Waffle
if [ -d "${TMP_TEST_DIR}/in"  ]; then
    rm -rf in
fi
mkdir in
echo a > in/testcase
i=0
for ((i=1; i<100; i++))
do
    if ! [ -d "${TMP_TEST_DIR}/out_Waffle$i" ]; then
        break
    fi
done
echo "Execute the fuzzing..."
# export ASAN_OPTIONS=detect_odr_violation=0:allocator_may_return_null=1:abort_on_error=1:symbolize=0:detect_leaks=0
${AFL_PATH}/memlock-stack-fuzz -i ${TMP_TEST_DIR}/in -o ${TMP_TEST_DIR}/out_Waffle$i -m none -d -t 5000 -- ${TMP_TEST_DIR}/example6Waffle @@ || true
# echo "Execute the afl-showmap..."
# ss=$(echo 0 | ${AFL_PATH}/afl-showmap -m none -q -o .example6Waffle ${TEST_DIR}/example6Waffle) || true
# echo "-------- $ss"
echo
