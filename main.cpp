#include <opencv2/opencv.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
//#include <unistd.h>
//#include <sys/time.h>
#include <time.h>
#include <signal.h>
//#include "dxl.hpp"

using namespace std;
using namespace cv;

bool ctrl_c_pressed = false; //ctrl_c 동작여부
void ctrlc_handler(int) { ctrl_c_pressed = true; }  //ctrl_c 누르면 ctrl_c_pressed가 false -> true
bool mode = false;   //다이내믹셀 동작여부

int main() {
	VideoCapture cap("8_lt_cw_100rpm_in.mp4");  //원본 영상
	Mat frame, gray, bin,  //영상
		labels, stats, centroids; //레이블링
	vector<double> lcp_x;  //라인의 중심좌표 저장
	//Dxl mx;  //다이내믹셀
	TickMeter tm;   //실행시간
	double time = 0, k = 0.15; // 실행시간 저장할 변수, 게인
	int  cnt, run = 1, //false;
		min_i = 0, x_i = 0, y = 0,
		lvel = 0, rvel = 0, err; //다이내믹셀
	char c = 0;

	signal(SIGINT, ctrlc_handler);  //시그널 핸들러

	lcp_x.push_back(320);  //초기값

	while (1) {
		/*if (mx.kbhit()) //키보드 입력 체크
		{
			char ch = mx.getch(); //키입력 받기
			if (ch == 'q') break; //q이면 종료
			else if (ch == 's') mode = true; //s면 다이나믹셀 모드 true
		}*/
		if (c == 's') mode = true;
		tm.start();  //실행시간 시작
		cap >> frame;  //영상 불러옴
		if (frame.empty()) { cerr << "frame empty!" << endl; break; }
		//source >> frame;
		gray = frame(Rect(0, 270, 640, 90));                 //ROI선정
		cvtColor(gray, gray, COLOR_RGB2GRAY);                //그레이스케일 변환
		blur(gray, gray, Size(5, 5));                        //노이즈 제거
		gray = gray + (80 - mean(gray)[0]);                  //밝기보정
		threshold(gray, bin, 150, 255, THRESH_BINARY);       //이진화
		cnt = connectedComponentsWithStats(bin, labels, stats, centroids);  //레이블링 함수
		cvtColor(bin, bin, COLOR_GRAY2BGR);                  //컬러 변환

		Point cp(bin.cols / 2, bin.rows / 2); //영상 중심(320, 45)

		//min = abs(cp.x - centroids.at<double>(1, 0));

		for (int i = 1; i < cnt; i++) {
			double* c = centroids.ptr<double>(i);  //라인의 무게중심
			int* s = stats.ptr<int>(i);            //바운딩 박스 정보
			Point cent(c[0], c[1]);                //라인의 중심좌표
			circle(bin, cent, 2, Scalar(255, 0, 0), 2);                               //외부 객체
			rectangle(bin, Rect(s[0], s[1], s[2], s[3]), Scalar(255, 0, 0));       //외부 객체
			if (abs(lcp_x.back() - c[0]) <= 100) {   //영상의 중심좌표에서 가까운 라인 찾아냄
				x_i = i;                            //몇 번째 라인인지 저장
				y = c[1];                           //라인의 무게중심 y값 저장
				//min = abs(lcp_x.back() - c[0]);
				lcp_x.push_back(centroids.at<double>(x_i, 0)); //영상의 중심좌표에서 가까운 라인의 무게중심x
				rectangle(bin, Rect(stats.at<int>(x_i, 0), stats.at<int>(x_i, 1), stats.at<int>(x_i, 2), stats.at<int>(x_i, 3)), Scalar(0, 0, 255)); //진짜 라인
			}
			min_i = lcp_x.back();  //영상의 중심좌표에서 가까운 라인의 무게중심x
			circle(bin, Point(min_i, y), 2, Scalar(0, 0, 255), 2); //진짜 라인
		}
		err = cp.x - lcp_x.back();  //에러 값 저장
		lvel = 100 - k * err;    //좌측속도명령
		rvel = -(100 + k * err); //우측속도명령
		//if(mode) mx.setVelocity(lvel, rvel);  //다이내믹셀 동작
		imshow("frame", frame);  //원본 영상
		imshow("gray", gray);    //그레이 영상
		imshow("bin", bin);      //이진화 영상
		waitKey(30);
		if (ctrl_c_pressed) {
			lvel = 0; rvel = 0;
			break; //Ctrl+c입력시 탈출
		}
		tm.stop(); //실행시간 멈춤
		time = tm.getTimeSec() / tm.getCounter(); //실행시간
		cout << "err: " << err << ", lvel: " << lvel << ", rvel: " << rvel << ", time: " << time << endl; //err,속도, 실행시간 출력
	}
	//mx.close(); // 장치닫기
	return 0;  //리턴값
}
