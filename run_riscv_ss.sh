#!/bin/bash

bash compile.bash single-cycle

if [ -z $1 ]; then
    echo "No input file included - running default cpu trace"
    ./RVSim configs/RISC_V.cfg out/out cpu_traces/cpu_traces_core_0
else
    ./RVSim configs/RISC_V.cfg out/out $1
fi
