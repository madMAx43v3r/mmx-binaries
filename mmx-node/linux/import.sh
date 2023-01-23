#!/bin/bash

rm -r $2
mkdir $2

cp -r $1/build/dist/* $2/
cp -r overlay/generic/* $2/
cp -r overlay/$2/* $2/

