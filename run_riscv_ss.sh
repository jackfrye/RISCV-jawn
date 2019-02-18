#!/bin/bash

bash compile.bash single-cycle

if [ -z $1 ]; then
    echo "No input file included - running default cpu trace"
    ./RVSim configs/RISC_V.cfg out/out cpu_traces/cpu_traces_core_0
else
    if [ -z $2 ]; then
        echo "one arg"
	./RVSim configs/RISC_V.cfg out/out $1
    else
        if [ -z $3 ]; then
            echo "two arg"
	    ./RVSim configs/RISC_V_2.cfg out/out $1 $2
        else
            echo "four arg"
            ./RVSim configs/RISC_V_4.cfg out/out $1 $2 $3 $4
        fi
    fi
fi
