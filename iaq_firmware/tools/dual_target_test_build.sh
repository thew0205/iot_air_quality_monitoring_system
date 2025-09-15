#!/bin/bash
set -e
# ========================================================================
# test.sh
# ------------------------------------------------------------------------
# A script to build and run Sunspec PAI tests for either host or target.
# Usage:
#   ./test.sh <TEST_TYPE> [SUNSPEC_PAI_ROOT_DIR]
#   TEST_TYPE: 'host' or 'target'
#   SUNSPEC_PAI_ROOT_DIR: optional, root directory of Sunspec PAI project
# ========================================================================

# ------------------------------------------------------------------------
# Input arguments
# ------------------------------------------------------------------------
TEST_TYPE=$1                   # First argument: which test type to build/run
IOT_FIRMWARE_ROOT_DIR=$2        # Second argument: root directory of the project

# ------------------------------------------------------------------------
# Derived directories
# ------------------------------------------------------------------------
SOURCE_DIR="$IOT_FIRMWARE_ROOT_DIR/tests/${TEST_TYPE}"      # Path to the source directory for the selected test type
BUILD_DIR="$IOT_FIRMWARE_ROOT_DIR/build/tests/${TEST_TYPE}" # Path to the build directory for the selected test type
# PICOTOOL_PATH=${HOME}/.pico-sdk/picotool/2.2.0/picotool/picotool
# ------------------------------------------------------------------------
# Color definitions for terminal output
# ------------------------------------------------------------------------
RED='\033[0;31m'        # Red text (used for errors)
GREEN='\033[0;32m'      # Green text (success messages)
YELLOW='\033[1;33m'     # Yellow text (warnings)
BLUE='\033[0;34m'       # Blue text
MAGENTA='\033[0;35m'    # Magenta text
NC='\033[0m'            # No Color / reset
BOLD='\033[1m'
BG_BLUE='\033[44m'
BOLD_RED='\033[1;31m'
BOLD_GREEN='\033[1;32m'
# ------------------------------------------------------------------------
# Check if TEST_TYPE is provided
# ------------------------------------------------------------------------
if [[ -z $TEST_TYPE ]]; then
    echo -e "${RED}Error: choose a test type (host or target)!!!${NC}"
    exit 1
fi

# ------------------------------------------------------------------------
# Check if SUNSPEC_PAI_ROOT_DIR is provided, otherwise default to current directory
# ------------------------------------------------------------------------
if [[ -z $IOT_FIRMWARE_ROOT_DIR ]]; then
    echo -e "${MAGENTA}SUNSPEC_PAI_ROOT_DIR not specified. Assuming current working directory.${NC}"
    IOT_FIRMWARE_ROOT_DIR=$(pwd)
fi

# ------------------------------------------------------------------------
# Determine which tests to enable based on TEST_TYPE
# ------------------------------------------------------------------------
if [[ ${TEST_TYPE,,} == "target" ]]; then
    # Target test selected
    echo -e "${GREEN}Building TARGET tests${NC}"
    HOST_TEST=OFF
    TARGET_TEST=ON
else
    # Host test selected
    echo -e "${GREEN}Building HOST tests${NC}"
    HOST_TEST=ON
    TARGET_TEST=OFF
fi

# Flag to indicate Sunspec PAI test build
SUNSPEC_TEST=ON

# ------------------------------------------------------------------------
# Run CMake configuration and build
# ------------------------------------------------------------------------
cmake -S$SOURCE_DIR \
      -B$BUILD_DIR \
      -DIOT_FIRMWARE_ROOT_DIR=$IOT_FIRMWARE_ROOT_DIR \
      -DTESTS=OFF \
      -DHOST_TEST=$HOST_TEST \
      -DTARGET_TEST=$TARGET_TEST \
      -DSUNSPEC_PAI_TEST=$SUNSPEC_TEST
cmake --build $BUILD_DIR -j$(nproc)

if [[ $? -ne 0 ]]; then
    echo -e "${BOLD_RED} ${TEST_TYPE} Build failed!!! :(${NC}"
    exit 1
fi