#!/bin/bash
set -e

SCRIPT_DIR="$(dirname $(realpath ${BASH_SOURCE[0]}))"

${SCRIPT_DIR}/file-name.sh
${SCRIPT_DIR}/coding-style.sh -a
#${SCRIPT_DIR}/build.sh -ua
#${SCRIPT_DIR}/build.sh -ut
#${SCRIPT_DIR}/build.sh -uc

# installation test
# distribution test
