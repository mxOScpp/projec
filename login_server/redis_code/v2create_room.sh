#!/bin/bash

docker run --rm -d -P -e MY_ROOM_NO=$1 game:v1 #2>&1 >/dev/null
echo $MY_ROOM_NO
docker ps -n 1 -f ancestor=game:V1.0 | grep game_start | awk -F "->" '{print $1}' | awk -F : '{print $NF}'
