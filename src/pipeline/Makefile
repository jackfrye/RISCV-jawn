SOURCE	:= Main.cpp Config.cpp Assembler.cpp Instruction_Memory.cpp Core.cpp Socket.cpp Stages.cpp Control.cpp Imm_gen.cpp Registers.cpp Data_Memory.cpp ALU.cpp
CC	:= g++
FLAGS	:= -O3 -std=c++11 -w
TARGET	:= RVSim

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(FLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm $(TARGET)
