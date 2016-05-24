#!/bin/sh

L=$(pwd)/../src/ubuntu-core-launcher

TMP="$(mktemp -d)"
trap "rm -rf $TMP" EXIT

export SNAPPY_LAUNCHER_SECCOMP_PROFILE_DIR="$TMP"
export SNAPPY_LAUNCHER_ENVIRONMENT_DIR="$TMP/environment"
mkdir "$SNAPPY_LAUNCHER_ENVIRONMENT_DIR"

export SNAPPY_LAUNCHER_INSIDE_TESTS="1"


FAIL() {
    printf ": FAIL\n"
    exit 1
}

PASS() {
    printf ": PASS\n"
}
