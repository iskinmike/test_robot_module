
TARGET = test_module.so

all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o

test_robot_module.o: test_robot_module.cpp
	g++ -std=c++11 -c test_robot_module.cpp

$(TARGET): test_robot_module.o
	g++ -shared -Wall -o $(TARGET) test_robot_module.o










