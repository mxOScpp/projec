#!/bin/bash



DOCKERKEY=$(docker run -d -e MY_ROOM_NO=$1 -P game:v1)
DOCKER_KEY_VALUE=$(echo $DOCKERKEY| cut -c 1-10)

docker ps -n 1 -f ancestor=game:v1 |grep "$DOCKER_KEY_VALUE" |awk -F '->' '{print $1}'|awk -F : '{print $NF}'

