#!/bin/bash

function setup(){
	cd "$1"
	. configure.sh
	cd ..

}

export PKG_CONFIG_PATH="$(pwd):"

setup credentials
setup linkedlist
setup objcurl
setup objstring
setup terminput

export PKG_CONFIG_PATH
