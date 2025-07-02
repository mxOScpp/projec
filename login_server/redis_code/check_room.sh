#!/bin/bash
docker ps  -f ancestor=game:v1 |grep game_start |grep Up |awk -F '->' '{print $1}'|awk -F : '{print $NF}'|grep "$1" >/dev/null 2>&1
