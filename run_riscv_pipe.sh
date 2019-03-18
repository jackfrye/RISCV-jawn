#!/bin/bash

bash compile.bash pipeline

if [ -z $1 ]; then
    echo "No input file included - running default cpu trace"
    ./RVSim configs/RISC_V.cfg out/out cpu_traces/test_forwarding_ex_mem
else
    ./RVSim configs/RISC_V.cfg out/out $1
fi
