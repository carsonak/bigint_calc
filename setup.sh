#!/usr/bin/env sh

if [ "$(id -u)" -ne 0 ]
then sudo "$0" "$@"
    exit
fi

apt-get update
apt-get install gcc make libcriterion-dev
