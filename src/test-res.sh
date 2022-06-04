#!/usr/bin/bash

gcc -o theater -pthread p3170173-p3170044-p3170110-res.c p3170173-p3170044-p3170110-res.h
./theater 100 1000
read -p "Press enter to exit"
