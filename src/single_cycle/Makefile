SOURCE	:= Main.cpp Config.cpp Assembler.cpp Instruction_Memory.cpp Core.cpp Socket.cpp Registers.cpp Control.cpp ALU.cpp Data_Memory.cpp Imm_gen.cpp
CC	:= g++
FLAGS	:= -O3 -std=c++11 -w
TARGET	:= RVSim

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(FLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm $(TARGET)
