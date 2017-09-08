#!/bin/bash

cd `dirname $0`;

TEST_BUILD_DIR=./build
TEST_SRC_DIR=./src
LIB_SRC_DIR=../lib

[ -d $TEST_BUILD_DIR ] && rm -rf $TEST_BUILD_DIR

mkdir $TEST_BUILD_DIR

function runTestFile() {
    echo "[run test file] $TEST_SRC_DIR/$1"
    g++ $LIB_SRC_DIR/action.cc $LIB_SRC_DIR/fsm.cc $TEST_SRC_DIR/$1 -I$LIB_SRC_DIR -o $TEST_BUILD_DIR/$1.test
    $TEST_BUILD_DIR/$1.test
}

runTestFile base.cc
