#!/usr/bin/env sh

# Super user permissions needed
if [ "$(id -u)" -ne 0 ]
then sudo "$0" "$@"
    exit
fi

apt-get update
apt-get install cmake
