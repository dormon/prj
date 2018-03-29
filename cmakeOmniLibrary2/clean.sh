#!/bin/sh

#clean libs
rm -rf install/
cd libs
rm -rf build/
cd ..

#clean apps
cd apps
rm -rf build/
cd ..
