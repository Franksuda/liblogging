#!/bin/bash

set -x

vercmp() {
    if [[ $1 == $2 ]]; then
        return 0
    fi
    local IFS=.
    local i ver1=($1) ver2=($2)
    # fill empty fields in ver1 with zeros
    for ((i = ${#ver1[@]}; i < ${#ver2[@]}; i++)); do
        ver1[i]=0
    done
    for ((i = 0; i < ${#ver1[@]}; i++)); do
        if [[ -z ${ver2[i]} ]]; then
            # fill empty fields in ver2 with zeros
            ver2[i]=0
        fi
        if ((10#${ver1[i]} > 10#${ver2[i]})); then
            return 1
        fi
        if ((10#${ver1[i]} < 10#${ver2[i]})); then
            return 2
        fi
    done
    return 0
}

hash sudo 2>/dev/null || {
    echo >&2 "sudo is required but it's not installed."
    apt update && apt install -y sudo
}

unameOut="$(uname -s)"
arch="x64"
case "${unameOut}" in
Linux*)
    machine=linux

    sudo apt update
    boost_version="$(apt-cache madison libboost-all-dev | grep -oP "\d+(\.\d+)+")"

    sudo apt update && sudo apt install -y \
        git build-essential python3-pip python3-all-dev \
        libicu-dev ca-certificates wget curl openssl libssl-dev zlib1g-dev \
        libcurl4-openssl-dev uuid-dev ccache ninja-build gfortran

    vercmp $boost_version "1.66"

    if [[ $? -ne 2 ]]; then
        sudo apt install -y libboost-all-dev
    else
        sudo apt install -y software-properties-common
        sudo add-apt-repository ppa:mhier/libboost-latest -y
        sudo apt update
        sudo apt install -y libboost1.68-dev
    fi

    ;;
Darwin*)
    machine=osx
    hash sudo 2>/dev/null || {
        echo >&2 "Plesse ensure you have executed 'xcode-select --install' to have build tools ready."
        exit
    }
    brew install boost@1.68 icu4c
    ;;
CYGWIN*) machine=win ;;
MINGW32_NT)
    machine=win
    arch=x86
    ;;
MINGW64_NT)
    machine=win
    ;;
*)
    machine="UNKNOWN:${unameOut}"
    echo "Not supported platform: ${machine}"
    exit -1
    ;;
esac