#!/bin/bash

spawn-fcgi -a 127.0.0.1 -p 55555 -f ./$1
