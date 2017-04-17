#pragma once
#include "Libraries.h"

bool calibrate_aiming(VideoCapture & cam1, VideoCapture & cam2, Size board_sz);
void aiming(VideoCapture & cam1, VideoCapture & cam2);
int show_images(const Mat & img1, const Mat & img2, const char * window_name, bool wait_key_press);
void showOpticalFlow(const Mat & flow, const Mat & left_photo);
bool single_camera_aiming(VideoCapture & cap);
void presets_aiming(void);

//Функция, помогающая сделать корректные снимки шахматной доски
//Она показывает видео с камер и завершается по нажатию клавиши
//IN:
//VideoCapture & cam1: ссылка на объект первой камеры
//VideoCapture & cam2: ссылка на объект второй камеры
//Size board_sz: объект с размером шахматной доски (число углов)
//OUT:
//bool: Был ли нажат Enter
//Для завершения серии снимков
bool calibrate_aiming(VideoCapture & cam1, VideoCapture & cam2, Size board_sz)
{
	const char window_name[] = "CALIBRATE_AIMING";
	Mat img1, img2;
	int exit_key;

	bool found_corners1 = false;
	bool found_corners2 = false;
	Mat corners1, corners2,
		temp_corners1, temp_corners2,
		gray1, gray2,
		temp_grey1, temp_grey2;

	while (true)
	{
		//Делаем снимок с камеры и переводим его в черно - белый формат
		cam1.read(img1);
		cam2.read(img2);
		cvtColor(img1, gray1, CV_BGR2GRAY);
		cvtColor(img2, gray2, CV_BGR2GRAY);

		temp_grey1 = gray1.clone(); //Без углов шахматной доски
		temp_grey2 = gray2.clone();

		//Ищем углы
		found_corners1 = findChessboardCorners(gray1, board_sz, corners1, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		found_corners2 = findChessboardCorners(gray2, board_sz, corners2, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (found_corners1 && found_corners2)
		{
			drawChessboardCorners(gray1, board_sz, corners1, found_corners1);
			drawChessboardCorners(gray2, board_sz, corners2, found_corners2);
		}


		//Отображаем один из вариантов
		exit_key = show_images(found_corners1 ? gray1 : temp_grey1, found_corners2 ? gray2 : temp_grey2, window_name, false);

		//Выход из цикла при нажатии клавиши
		//enter для завершения серии снимков
		if (exit_key != -1) break;
	}

	bool is_final_frame = exit_key == 13;
	//if (is_final_frame)
	cv::destroyWindow(window_name);

	return is_final_frame;
}

//Функция, помогающая сделать корректные снимки шахматной доски
//Она показывает видео с камер и завершается по нажатию клавиши
//IN:
//VideoCapture & cam: ссылка на объект камеры
//Size board_sz: объект с размером шахматной доски (число углов)
//OUT:
//bool: Был ли нажат Enter
//Для завершения серии снимков
void aiming(VideoCapture & cam1, VideoCapture & cam2)
{
	Mat img1, gray1;
	Mat img2, gray2;

	const char window_name[] = "AIMING";

	while (true)
	{
		//Делаем снимок с камер и переводим его в черно - белый формат
		cam1.read(img1);
		cam2.read(img2);
		cvtColor(img1, gray1, CV_BGR2GRAY);
		cvtColor(img2, gray2, CV_BGR2GRAY);

		if (show_images(gray1, gray2, window_name, false) != -1) break;
	}

	cv::destroyWindow(window_name);
}


//Отображает 2 изображения в 1 окне
//IN:
//Mat img1: первое изображение
//Mat img2: второе изображение

//OUT: 
//int pressed_button_code :код нажатой пользователем клавишы
int show_images(const Mat & img1, const Mat & img2, const char * window_name, bool wait_key_press)
{
	
	Mat appended(img1.rows, img1.cols + img2.cols, img1.type());
	img1.copyTo(Mat(appended, Rect(0, 0, img1.cols, img1.rows)));
	img2.copyTo(Mat(appended, Rect(img1.cols, 0, img2.cols, img2.rows)));
	resize(appended, appended, Size(appended.cols / 2, appended.rows / 2));

	imshow(window_name, appended);
	int time_awaiting;
	time_awaiting = wait_key_press ? 0 : 1;
	return waitKey(time_awaiting);
}

void showOpticalFlow(const Mat & flow, const Mat & left_photo)
{
	Mat canvas = left_photo.clone();
	for (int y = 0; y < flow.rows; y += 6)
	for (int x = 0; x < flow.cols; x += 6)
	{
		auto flowPoint = flow.at<Point2f>(y, x);
		float x_flow = flowPoint.x;
		float y_flow = flowPoint.y;
		line(canvas, Point2f(x, y), Point2f(x + x_flow, y + y_flow), Scalar(0, 0, 0));
	}

	imshow("Optical flow", canvas);
	waitKey(0);
}

bool single_camera_aiming(VideoCapture & cap)
{
	Mat img, gray;

	const char window_name[] = "AIMING";
	int key;
	while (true)
	{
		//Делаем снимок с камер и переводим его в черно - белый формат
		cap.read(img);
		cvtColor(img, gray, CV_BGR2GRAY);
		imshow(window_name, gray);
		key = waitKey(1);
		if (key != -1) break;
	}

	cv::destroyWindow(window_name);

	return key == 13;
}


void presets_aiming(void)
{
	VideoCapture cam1(0), cam2(1);
	Mat img1, gray1;
	Mat img2, gray2;
	int key;
	const char window_name_first[]  = "1";
	const char window_name_second[] = "2";

	while (true)
	{
		//Делаем снимок с камер и переводим его в черно - белый формат
		cam1.read(img1);
		cam2.read(img2);
		cvtColor(img1, gray1, CV_BGR2GRAY);
		cvtColor(img2, gray2, CV_BGR2GRAY);

		resize(gray1, gray1, Size(0, 0), 0.5, 0.5);
		resize(gray2, gray2, Size(0, 0), 0.5, 0.5);

		imshow(window_name_first, gray1);
		imshow(window_name_second, gray2);
		if (waitKey(1) != -1) break;
	}

	cv::destroyWindow(window_name_first);
	cv::destroyWindow(window_name_second);
}