#!/bin/bash
./game

while [ 1 == 1  ]
do
	sleep 5
	ps aux|grep -w ./game|grep -v grep
	if [ 0 -ne $? ]
	then
		redis-cli -h 192.168.144.183 del "room_no$MY_ROOM_NO"
		exit 0
	fi
done
