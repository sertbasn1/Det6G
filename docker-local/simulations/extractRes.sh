#!/bin/bash
# parameters to run
#test ids ($1)

ROOTDIR=/simulations
RES_PATH="$ROOTDIR/simResults"

cd "$RES_PATH/test$1/results"  
opp_scavetool x *.sca -o  results.csv
