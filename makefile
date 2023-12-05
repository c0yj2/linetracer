CX = g++       //c++ 컴파일러 변수
CXFLAGS = -g -Wall  //디버그 옵션, 모든 워밍 나타냄
CVFLAGS = `pkg-config opencv4 --cflags --libs`  //opencv 헤더파일 경로

DXLFLAGS = -I/usr/local/include/dynamixel_sdk -ldxl_x64_cpp  //다이내믹셀 헤더파일 경로, 라이브러리 경로 

SRCS = main.cpp  //소스파일
TARGET = linetracer //실행파일명

OBJS = main.o dxl.o  //목적파일 목록 변수

//실행파일 만드는 Rule
$(TARGET) : $(OBJS)  //타켓 : 필요항목
	$(CX) -o $(TARGET) $(OBJS) $(CXFLAGS) $(DXLFLAGS) $(CVFLAGS)  // 타겟을 만드는 명령어

//목적파일 만드는 Rule
main.o : main.cpp  //타겟 : 필요항목
	$(CX) -c main.cpp $(CXFLAGS) $(DXLFLAGS) $(CVFLAGS)  //타케을 만드는 명령어

dxl.o : dxl.hpp dxl.cpp
	$(CX) -c dxl.cpp $(CXFLAGS) $(DXLFLAGS)

.PHONY : all clean  //가짜 타겟
all: $(TARGET)  //all이라는 타겟 정의
clean :         //보통 중간 생성 파일 및 실행 파일을 삭제하는 기능을 수행
	rm -rf $(TARGET) $(OBJS) //타겟과 오브젝트 파일 제거
