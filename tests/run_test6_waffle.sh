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

AFL_PATH=${ROOT_DIR}/tool/MemLock
CLANG_PRE="memlock-stack-"
FUZZ_NAME="wfl"

echo $1

if [ "$1" = "afl" ]; then
    AFL_PATH=${ROOT_DIR}/afl
    CLANG_PRE="afl-"
    FUZZ_NAME="afl"
elif [ "$1" = "mem" ]; then
    AFL_PATH=${ROOT_DIR}/memlock/tool/MemLock
    FUZZ_NAME="mem"
fi

TEST_DIR=${ROOT_DIR}/tests/test6

if [ "$2" == "m" ]; then
    if [ -d "/tmp/fuzz_test/test6"  ]; then
        rm -rf /tmp/fuzz_test/test6
    fi

    mkdir -p /tmp/fuzz_test/test6
    cp -r ${TEST_DIR} /tmp/fuzz_test
fi


TMP_TEST_DIR=/tmp/fuzz_test/test6

echo "----------------"
echo "Making Waffle..."
echo "----------------"
make -C ${AFL_PATH} clean
make -C ${AFL_PATH}

echo "--------------"
echo "Making LLVM..."
echo "--------------"
make -C ${AFL_PATH}/llvm_mode clean
make -C ${AFL_PATH}/llvm_mode

cd ${TMP_TEST_DIR}


# Compile using llvm
${AFL_PATH}/${CLANG_PRE}clang++ -g -O0 -fsanitize=address example.cpp -o example6${FUZZ_NAME}

if [ "$2" = "m" ]; then
    if [ -d "${TMP_TEST_DIR}/in"  ]; then
        rm -rf in
    fi
    mkdir in
    echo abcdefghijklm > in/testcase
fi
i=0
# for ((i=1; i<100; i++))
# do
#     if ! [ -d "${TMP_TEST_DIR}/out_$i" ]; then
#         break
#     fi
# done
echo "Execute the fuzzing..."
INPUT_DIR=${TMP_TEST_DIR}/in
OUTPUT_DIR=${TMP_TEST_DIR}/out_$i
FUZZ_COMMAND=${AFL_PATH}/${CLANG_PRE}fuzz
EXEC_PATH=${TMP_TEST_DIR}/example6${FUZZ_NAME}

if [ "$2" = "m" ]; then 
    "----- ${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -t 5000 -M Master-${FUZZ_NAME} -- ${EXEC_PATH} @@"
    ${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -t 5000 -M Master-${FUZZ_NAME} -- ${EXEC_PATH} @@
else
    "===== ${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -t 5000 -S ${FUZZ_NAME} -- ${EXEC_PATH} @@"
    ${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -t 5000 -- ${EXEC_PATH} @@
fi

# else
#     "===== ${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -t 5000 -S ${FUZZ_NAME} -- ${EXEC_PATH} @@"
#     ${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -t 5000 -S ${FUZZ_NAME} -- ${EXEC_PATH} @@
# fi



# export ASAN_OPTIONS=detect_odr_violation=0:allocator_may_return_null=1:abort_on_error=1:symbolize=0:detect_leaks=0
# tmux split-window -h "${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -d -t 5000 -S AFL -- ${EXEC_PATH} @@ "
# tmux split-window -v "${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -d -t 5000 -S MEM -- ${EXEC_PATH} @@ "
# tmux select-pane -R
# tmux split-window -v "${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -d -t 5000 -S WFL -- ${EXEC_PATH} @@ "
# ${FUZZ_COMMAND} -i ${INPUT_DIR} -o ${OUTPUT_DIR} -m none -t 5000 -M Mas -- ${EXEC_PATH} @@

# echo "Execute the afl-showmap..."
# ss=$(echo 0 | ${AFL_PATH}/afl-showmap -m none -q -o .example6Waffle ${TEST_DIR}/example6Waffle) || true
# echo "-------- $ss"
