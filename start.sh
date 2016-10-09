#!/bin/bash

set -euo pipefail

pushd engineSrc/
	make main #make sure data base server is up to date
	./main -s >> log.txt 2>&1 & #run in background and redirect both stdout and stderr to log file
	main_pid=$!
popd

pushd interactiveSrc/
	python Interface.py #start interactive system
popd

kill -kill $main_pid #kill data base server when done
