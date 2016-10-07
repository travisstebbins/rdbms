#!/bin/bash

set -euo pipefail

pushd engineSrc/
	make main
	./main -s >> log.txt 2>&1 & #run in background and redirect both stdout and stderr to log file
	main_pid=$!
popd

pushd interactiveSrc/
	python Interface.py
popd

kill -kill $main_pid
