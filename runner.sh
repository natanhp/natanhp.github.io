#! /bin/zsh

rm -rf bin
cmake . --fresh
make clean
make
cd bin 
 ./natanhpid.wt --docroot . --http-address 0.0.0.0 --http-port 8080
