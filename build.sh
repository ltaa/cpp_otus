#!/bin/bash


DIRS=`git diff --name-only HEAD^ | tail -1`
#DIRS=".asdasd
#asdasd"

CHANGED_DIR=`dirname $DIRS`


echo "$CHANGED_DIR"

if [ "$CHANGED_DIR" != "." ]; then
	cd $CHANGED_DIR
#	echo $CHANGED_DIR
	env CHANGED_DIR=$CHANGED_DIR

fi 
