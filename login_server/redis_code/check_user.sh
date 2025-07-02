#!/bin/bash


if [ "$2" == "" ]
then	
	cat userfile|grep -w "$1" >/dev/null 2>&1
else

	cat userfile|grep -w "$1:$2" >/dev/null 2>&1
fi

