#!/bin/bash
if [ "$#" -eq 1 ]
then
	ITERATIONS=$1
	COUNTER=1
	while [ $COUNTER -le $ITERATIONS ]; do
		echo Round $COUNTER
		./collusion
		let COUNTER=COUNTER+1
		sleep 0.5
	done 
	exit
fi

echo "Usage: "
echo "./run_sim.sh <number of iterations>"
