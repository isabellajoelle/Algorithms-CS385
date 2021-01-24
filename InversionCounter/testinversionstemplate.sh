#!/bin/bash

#*******************************************************************************
# Name        : testinversionstemplate.sh
# Author      : Isabella Cruz
# Version     : 1.0
# Date        : October 26th, 2020
# Description : Test cases for Inversion Counter
# Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
#********************************************************************************

file=inversioncounter.cpp
MAXTIME="8.0"

if [ ! -f "$file" ]; then
    echo -e "Error: File '$file' not found.\nTest failed."
    exit 1
fi

num_right=0
total=0
line="________________________________________________________________________"
compiler=
interpreter=
language=
extension=${file##*.}
if [ "$extension" = "py" ]; then
    if [ ! -z "$PYTHON_PATH" ]; then
        interpreter=$(which python.exe)
    else
        interpreter=$(which python3.2)
    fi
    command="$interpreter $file"
    echo -e "Testing $file\n"
elif [ "$extension" = "java" ]; then
    language="java"
    command="java ${file%.java}"
    echo -n "Compiling $file..."
    javac $file
    echo -e "done\n"
elif [ "$extension" = "c" ] || [ "$extension" = "cpp" ]; then
    language="c"
    command="./${file%.*}"
    echo -n "Compiling $file..."
    results=$(make 2>&1)
    if [ $? -ne 0 ]; then
        echo -e "\n$results"
        exit 1
    fi
    echo -e "done\n"
fi

timeout() {
    time=$1

    # start the command in a subshell to avoid problem with pipes
    # (spawn accepts one command)
    cmd="/bin/bash -c \"$2\""

    { expect -c "set echo \"-noecho\"; set timeout $time; spawn -noecho $cmd; expect timeout { exit 1 } eof { exit 0 }"; } >/dev/null

    if [ $? -eq 1 ]; then
        return 0
    fi
    return 1
}

run_test_args() {
    (( ++total ))
    echo -n "Running test $total..."
    expected=$3
    local ismac=0
    date --version >/dev/null 2>&1
    if [ $? -ne 0 ]; then
       ismac=1
    fi
    local start=0
    if (( ismac )); then
        start=$(python -c 'import time; print time.time()')
    else
        start=$(date +%s.%N)
    fi
(cat << ENDOFTEXT
$2
ENDOFTEXT
) > input.txt
    if timeout $MAXTIME "cat input.txt | $command $1 2>&1 | tr -d '\r' > tmp.txt"; then
        echo "failure [timed out after $MAXTIME seconds]"
    else
        received=$(cat tmp.txt)
        { cat input.txt | $command $1 2>&1 > /dev/null; } 1>/dev/null 2>error.txt
        error=$(cat error.txt)
        if [ ! -z "$error" ]; then
            received="${received}\n$error"
        fi
        local end=$(date +%s.%N)
        if (( ismac )); then
            end=$(python -c 'import time; print time.time()')
        else
            end=$(date +%s.%N)
        fi
        local elapsed=$(echo "scale=3; $end - $start" | bc | awk '{printf "%.3f", $0}') 
        if [ "$expected" != "$received" ]; then
            echo -e "failure\n\nExpected$line\n$expected\n"
            echo -e "Received$line\n$received\n"
        else
            echo "success [$elapsed seconds]"
            (( ++num_right ))
        fi
    fi
    rm -f tmp.txt input.txt
}

# TODO - Make sure your code can handle these cases.
run_test_args "" "x 1 2 3" "Enter sequence of integers, each followed by a space: Error: Non-integer value 'x' received at index 0."
run_test_args "" "1 2 x 3" "Enter sequence of integers, each followed by a space: Error: Non-integer value 'x' received at index 2."
run_test_args "lots of args" "" "Usage: ./inversioncounter [slow]"
run_test_args "average" "" "Error: Unrecognized option 'average'."
run_test_args "" "" "Enter sequence of integers, each followed by a space: Error: Sequence of integers not received."
run_test_args "" "  " "Enter sequence of integers, each followed by a space: Error: Sequence of integers not received."

# TODO - write some tests that use the 'slow' approach. Here is one example.
# You are allowed up to 8 seconds to count inversions on up to 100,000 values.
#given test case (1)
run_test_args "slow" "2 1" "Enter sequence of integers, each followed by a space: Number of inversions: 1"
#if all are the same number (2)
run_test_args "slow" "9 9 9 9 9 9 9 9" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#if the numbers are already sorted (3)
run_test_args "slow" "1 3 5 7 9 11 13 15 17 19" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#worst case, it is in backwards order (4)
run_test_args "slow" "10 9 8 7 6 5 4 3 2 1" "Enter sequence of integers, each followed by a space: Number of inversions: 45"
#random inversions (5) 
run_test_args "slow" "6 2 9 7 10 3" "Enter sequence of integers, each followed by a space: Number of inversions: 6"
#all numbers are the same except 1 (6)
run_test_args "slow" "3 3 6 3 3 3 3 3 3 3 3" "Enter sequence of integers, each followed by a space: Number of inversions: 8"
#worst case with 100 values (7) 
run_test_args "slow" "100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1" "Enter sequence of integers, each followed by a space: Number of inversions: 4950"
#best case with 100 values (8)
run_test_args "slow" "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100" "Enter sequence of integers, each followed by a space: Number of inversions: 0" 
#test if there is only one value (9)
run_test_args "slow" "1" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#best case if there are 100000 values (10)
run_test_args "slow" "$(echo {1..100000})" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#worst case if there are 100000 values (11)
run_test_args "" "$(echo {100000..1..1})" "Enter sequence of integers, each followed by a space: Number of inversions: 4999950000"


# END slow tests

MAXTIME="1.250"

# TODO - write some tests that use the 'fast' approach. Here is one example.
# You are allowed up to 1.25 seconds to count inversions on up to 100,000 values.
run_test_args "" "2 1" "Enter sequence of integers, each followed by a space: Number of inversions: 1"
#if all are the same number (2)
run_test_args "" "9 9 9 9 9 9 9 9" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#if the numbers are already sorted (3)
run_test_args "" "1 3 5 7 9 11 13 15 17 19" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#worst case, it is in backwards order (4)
run_test_args "" "10 9 8 7 6 5 4 3 2 1" "Enter sequence of integers, each followed by a space: Number of inversions: 45"
#random inversions (5) 
run_test_args "" "6 2 9 7 10 3" "Enter sequence of integers, each followed by a space: Number of inversions: 6"
#all numbers are the same except 1 (6)
run_test_args "" "3 3 6 3 3 3 3 3 3 3 3" "Enter sequence of integers, each followed by a space: Number of inversions: 8"
#worst case with 100 values (7) 
run_test_args "" "100 99 98 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 81 80 79 78 77 76 75 74 73 72 71 70 69 68 67 66 65 64 63 62 61 60 59 58 57 56 55 54 53 52 51 50 49 48 47 46 45 44 43 42 41 40 39 38 37 36 35 34 33 32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1" "Enter sequence of integers, each followed by a space: Number of inversions: 4950"
#best case with 100 values (8)
run_test_args "" "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100" "Enter sequence of integers, each followed by a space: Number of inversions: 0" 
#test if there is only one value (9)
run_test_args "" "1" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#best case if there are 100000 values (10)
run_test_args "" "$(echo {1..100000})" "Enter sequence of integers, each followed by a space: Number of inversions: 0"
#worst case if there are 100000 values (11)
run_test_args "" "$(echo {100000..1..1})" "Enter sequence of integers, each followed by a space: Number of inversions: 4999950000"


# END fast tests

echo -e "\nTotal tests run: $total"
echo -e "Number correct : $num_right"
echo -n "Percent correct: "
echo "scale=2; 100 * $num_right / $total" | bc

if [ "$language" = "java" ]; then
    echo -e -n "\nRemoving class files..."
    rm -f *.class
    echo "done"
elif [ "$language" = "c" ]; then
    echo -e -n "\nCleaning project..."
    make clean > /dev/null 2>&1
    echo "done"
fi
