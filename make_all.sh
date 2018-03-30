#!/bin/bash

function setup(){
	cd "$1"
	. configure.sh
	cd ..
}

function mbuild(){
	cd $1
	echo "pwd: $(pwd)"
	echo "PKG_CONFIG_PATH = $PKG_CONFIG_PATH"
	make $2
	cd ..
}

export PKG_CONFIG_PATH="$(pwd):"
#export LD_LIBRARY_PATH="$(pwd)/credentials:$(pwd)/linkedlist:$(pwd)/objstring:$(pwd)/objcurl:$(pwd)/terminput:$LD_LIBRARY_PATH"
setup credentials
setup linkedlist
setup objstring
setup objcurl
setup terminput
export PKG_CONFIG_PATH

mbuild credentials "install static"
mbuild credentials dynamic

mbuild linkedlist "install static"
mbuild linkedlist dynamic

mbuild objstring "install static"
mbuild objstring dynamic

mbuild objcurl "install static"
mbuild objcurl dynamic

mbuild terminput "install static"
mbuild terminput dynamic

make all
