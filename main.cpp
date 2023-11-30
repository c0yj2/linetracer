#수정수정수정수정수정

//시뮬레이션 영상
//1_lt_ccw_50rpm_out
//2_lt_ccw_50rpm_in
//3_lt_cw_50rpm_out
//4_lt_cw_50rpm_in
//5_lt_cw_100rpm_out
//6_lt_ccw_100rpm_out
//7_lt_ccw_100rpm_in
//8_lt_cw_100rpm_in

/*
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat frame(360, 640, CV_8UC1), gray, bin,
		labels, stats, centroids;
	vector<double> x;
	double x_min = 0, x_max = 0;
	int cent_x = bin.cols / 2, cent_y = bin.rows / 2, cnt, k = 0.15,
		lvel = 0, rvel = 0, err;

	
	VideoCapture cap("6_lt_ccw_100rpm_out.mp4");

	while (true) {
		cap >> frame;
		gray = frame(Rect(0, 270, 640, 90));                 //ROI선정
		cvtColor(gray, gray, COLOR_RGB2GRAY);                //그레이스케일 변환
		blur(gray, gray, Size(5, 5));                        //노이즈 제거
		gray = gray + (110 - mean(gray)[0]);                 //밝기보정
		threshold(gray, bin, 180, 255, THRESH_BINARY);       //이진화

		cnt = connectedComponentsWithStats(bin, labels, stats, centroids);

		cvtColor(bin, bin, COLOR_GRAY2BGR);

		for (int i = 1; i < cnt; i++) {
			double* p = centroids.ptr<double>(i);            //라인의 무게중심
			circle(bin, Point(cent_x, cent_y), 2, Scalar(0, 0, 255), -1);

			
			if (35 < p[1] && p[1] < 85) {
				err = bin.cols / 2 - p[0];
				x.push_back(p[0]);
				minMaxLoc(x, &x_min, &x_max);
				cent_x = p[0]; cent_y = p[1];
				rectangle(bin, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(0, 0, 255), 2);
				putText(bin, to_string(p[1]), Point(p[0], p[1]), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0));
				//cout << stats.at<int>(i, 4) << endl;
				cout << err << endl; // "  " << x_min << "  " << x_max << endl;
			}
			else {
				cent_x = p[0]; cent_y = p[1];
				rectangle(bin, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(255, 0, 0), 2);
				putText(bin, to_string(stats.at<int>(i, 4)), Point(p[0], p[1]), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0));
			}
			for (int j = 0; j < x.size(); j++) {
				//err = bin.cols / 2 - x[j];
				/*if (!(0 < p[0] && p[0] < 640)) {
					if (0 < x[j] && x[j] < 320) circle(bin, Point(x_min, 85), 2, Scalar(0, 255, 0), -1);
					else if (0 < x[j] && x[j] < 640) circle(bin, Point(x_max, 85), 2, Scalar(0, 255, 0), -1);
				//}
			}
			
		}
		//for (int k = 0; k < x.size(); k++) cout << x[k] << endl;
		lvel = 100 - k * err;
		rvel = -(100 + k * err);
		//cout << "err: " << err << ", lvel: " << lvel << ", rvel: " << rvel << endl;
		imshow("frame", frame);
		imshow("gray", gray);
		imshow("line", bin);
		if(waitKey(33) == 27) break;
	}
	return 0;
} */


#include <opencv2/opencv.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
//#include <unistd.h>
//#include <sys/time.h>
//#include <signal.h>
//#include "dxl.hpp"

using namespace std;
using namespace cv;

bool ctrl_c_pressed = false;
void ctrlc_handler(int) { ctrl_c_pressed = true; }


int main() {
	//Dxl mx;
	//struct timeval start, end1;
	Mat frame, gray, bin,
		labels, stats, centroids;
	vector<double> x, y;
	double x_min = 100, x_max = 0, time = 0, k = 0.15;
	int cent_x = 320, cent_y = 45, cnt, x_i = 1,
		lvel = 0, rvel = 0, err;
	bool run = 1; // false;

	//signal(SIGINT, ctrlc_handler);

	VideoCapture cap("7_lt_ccw_100rpm_in.mp4");

	/*string src ="nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)640, height=(int)360, format=(string)NV12 ! nvvidconv flip-method=0 ! video/x-raw, width=(int)640, height=(int)360, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
	VideoCapture source(src, CAP_GSTREAMER);
	if (!source.isOpened()){ cout << "Camera error" << endl; return -1; }

	string dst1 ="appsrc ! videoconvert ! video/x-raw, format=BGRx ! nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! h264parse ! rtph264pay pt=96 ! udpsink host=192.168.0.69 port=9999 sync= false";
	VideoWriter writer1(dst1, 0, (double)30, Size(640, 360), true);
	if (!writer1.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}*/

	x.push_back(cent_x); y.push_back(cent_y);

	while (true) {

		/*gettimeofday(&start, NULL);

		if (mx.kbhit()) //키보드입력 체크
		{
			char c = mx.getch(); //키입력 받기
			if (c == 's') run = true;

		} */
		

		if (run) {
			cap >> frame;
			if (frame.empty()) { cerr << "frame empty!" << endl; break; }
			//source >> frame;
			gray = frame(Rect(0, 270, 640, 90));                 //ROI선정
			cvtColor(gray, gray, COLOR_RGB2GRAY);                //그레이스케일 변환
			blur(gray, gray, Size(5, 5));                        //노이즈 제거
			gray = gray + (110 - mean(gray)[0]);                 //밝기보정
			threshold(gray, bin, 180, 255, THRESH_BINARY);       //이진화
			cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
			cvtColor(bin, bin, COLOR_GRAY2BGR);

			x_min = abs(x.back() - centroids.at<double>(1, 0));
			
			cout << x_min << "  " << x.back() << "  " << centroids.at<double>(1, 0) << endl;
			
			for (int i = 1; i < cnt; i++) {
				double* c = centroids.ptr<double>(i);            //라인의 무게중심
				int* s = stats.ptr<int>(i);
				Point cent(cent_x, cent_y);                      //영상의 중심좌표(320, 45)
				if (100 < s[4]) {
					circle(bin, Point(c[0], c[1]), 2, Scalar(255, 0, 0), 2);
					rectangle(bin, Rect(s[0], s[1], s[2], s[3]), Scalar(255, 0, 0), 2);
					putText(bin, to_string(s[4]), Point(c[0], c[1]), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0));
				}

				if (abs(x.back() - cent.x) <= x_min) {  //
					x_i = i;
					x_min = abs(x.back() - c[0]);
					cout << i << " 2: " << x_min << endl;
				}
				
				/*
				if (40 < p[1] && 200 < p[0] && p[0] < 400) {
					x.push_back(p[0]);
					color = Scalar(0, 0, 255);
					err = bin.cols / 2 - p[0];
					minMaxLoc(x, &x_min, &x_max);

					cent_x = p[0]; cent_y = p[1];

					putText(bin, to_string(p[1]), Point(p[0], p[1]), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0));
					cout << stats.at<int>(i, 4) << endl;
					cout << err << endl; // "  " << x_min << "  " << x_max << endl;
				}
				else {
					cent_x = p[0]; cent_y = p[1];
					color = Scalar(255, 128, 0);
					putText(bin, to_string(stats.at<int>(i, 4)), Point(p[0], p[1]), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0));
				}*/

				/*if (!(0 < p[0] && p[0] < 640)) {
					if (0 < x[j] && x[j] < 320) circle(bin, Point(x_min, 85), 2, Scalar(0, 255, 0), -1);
					else if (0 < x[j] && x[j] < 640) circle(bin, Point(x_max, 85), 2, Scalar(0, 255, 0), -1);
				}*/
			}
			x.push_back(centroids.at<double>(x_i, 0));

			circle(bin, Point(centroids.at<double>(x_i, 0), centroids.at<double>(x_i, 1)), 2, Scalar(255, 0, 0), 2);
			rectangle(bin, Rect(stats.at<int>(x_i, 0), stats.at<int>(x_i, 1), stats.at<int>(x_i, 2), stats.at<int>(x_i, 3)), Scalar(0, 0, 255), 2);
			imshow("frame", frame);
			imshow("gray", gray);
			imshow("line", bin);
			waitKey(30);
			//writer1 << frame;
			//writer1 << gray;
			//writer1 << bin;

			//for (int k = 0; k < x.size(); k++) cout << x[k] << endl;
			//err = bin.cols / 2 - x.min;
			//lvel = 100 - k * err;
			//rvel = -(100 + k * err);
			//mx.setVelocity(vel1, vel2);
			if (ctrl_c_pressed) break; //Ctrl+c입력시 탈출
			//usleep(20 * 1000);
			//gettimeofday(&end1, NULL);
			//time1 = end1.tv_sec - start.tv_sec + (end1.tv_usec - start.tv_usec) / 1000000.0;
			//cout << "err: " << err << ", lvel: " << lvel << ", rvel: " << rvel << endl; // ",time:" << time1 << endl;
		}
	}
	//mx.close(); // 장치닫기
	return 0;
}
