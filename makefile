CX = g++
CXFLAGS = -g -Wall
CVFLAGS = `pkg-config opencv4 --cflags --libs`

DXLFLAGS = -I/usr/local/include/dynamixel_sdk -ldxl_x64_cpp

SRCS = main.cpp
TARGET = linetracer

OBJS = main.o dxl.o

$(TARGET) : $(OBJS)
	$(CX) -o $(TARGET) $(OBJS) $(CXFLAGS) $(DXLFLAGS) $(CVFLAGS)

main.o : main.cpp
	$(CX) -c main.cpp $(CXFLAGS) $(DXLFLAGS) $(CVFLAGS)

dxl.o : dxl.hpp dxl.cpp
	$(CX) -c dxl.cpp $(CXFLAGS) $(DXLFLAGS)

.PHONY : all clean
all: $(TARGET)
clean :
	rm -rf $(TARGET) $(OBJS)
