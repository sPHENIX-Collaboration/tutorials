#!/bin/usr/bash

if [ ! -d fileLists ]; then
    mkdir fileLists
fi 

runList=$1

cd fileLists
#You can use the first command once the file catalogue stabalizes
#CreateDstList.pl --cdb 2024p001 --build new --printruns > runList.list
CreateDstList.pl --cdb 2024p001 --build new --list $runList DST_CALO_run2pp
