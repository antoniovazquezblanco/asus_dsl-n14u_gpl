#!/bin/bash

# Get the current path
path_script=$(readlink -f $0)
path_base=$(dirname $path_script)

# Check if the toolchain is extracted....
if [ ! -d "${path_base}/toolchain_extracted" ]
then
	mkdir -p "${path_base}/toolchain_extracted"
	tar -zxvf ./toolchain/mips-linux-uclibc.tgz -C "${path_base}/toolchain_extracted"
fi

# Set paths
export PATH="${path_base}/toolchain_extracted/mips-linux-uclibc/usr/bin":$PATH
export PATH="${path_base}/toolchain_extracted/mips-linux-uclibc/usr/lib":$PATH
export PATH="${path_base}/toolchain_extracted/mips-linux-uclibc/bin":$PATH
export PATH="${path_base}/toolchain_extracted/mips-linux-uclibc/lib":$PATH
export LD_LIBRARY_PATH="${path_base}/toolchain_extracted/mips-linux-uclibc/usr/lib":$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="${path_base}/toolchain_extracted/mips-linux-uclibc/lib":$LD_LIBRARY_PATH

# Export the profile
export PROFILE=dsl-n14u

