#!/bin/bash

cd `dirname $0`;

TEST_BUILD_DIR=./build
TEST_SRC_DIR=./src
LIB_SRC_DIR=../lib

[ -d $TEST_BUILD_DIR ] && rm -rf $TEST_BUILD_DIR

mkdir $TEST_BUILD_DIR

function runTestFile() {
    echo "[build test file] $TEST_SRC_DIR/**.cc"
    g++ -std=c++11 $LIB_SRC_DIR/**.cc $TEST_SRC_DIR/**.cc -I$TEST_SRC_DIR -I$LIB_SRC_DIR -o $TEST_BUILD_DIR/test
    echo "[run binary] $TEST_BUILD_DIR/test"
    $TEST_BUILD_DIR/test
}

runTestFile
