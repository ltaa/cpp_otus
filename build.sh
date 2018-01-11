#!/bin/bash


DIRS=`git diff --name-only HEAD^`
#DIRS="."

CHANGED_DIR=`dirname $DIRS`


#echo "$CHANGED_DIR"

if [ "$CHANGED_DIR" != "." ]; then
	cd $CHANGED_DIR

fi 
