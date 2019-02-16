#!/bin/bash

bash compile.bash single-cycle

./RVSim configs/RISC_V.cfg out/out cpu_traces/task_0
