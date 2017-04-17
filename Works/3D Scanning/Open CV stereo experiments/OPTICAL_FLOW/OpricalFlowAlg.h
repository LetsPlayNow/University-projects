#pragma once
#include "Libraries.h"
#include "Calibration.h"

void from_homohenous(Mat & src);
Mat stereo_sfm(Mat& img1, Mat& img2, OpticalFlowMatrices & of_matr);
Mat single_sfm(Mat& img1, Mat& img2, Mat & camera_matrix, Mat distortion_coeff);

/**
* Structure from motion from 2 cameras, using farneback optical flow as the 'features'
* No, this doesn't work on more than 2 cams, because that requires bundle adjustment, which
* I'm still searching if there's an OpenCV implementation of it
*/
Mat stereo_sfm(Mat& img1, Mat& img2, OpticalFlowMatrices & of_matr)
{
	Mat gray1, gray2;
	cvtColor(img1, gray1, CV_BGR2GRAY);
	cvtColor(img2, gray2, CV_BGR2GRAY);

	Mat flow_mat;
	//Считает оптический поток
	//http://docs.opencv.org/master/dc/d6b/group__video__track.html#ga5d10ebbd59fe09c5f650289ec0ece5af
	calcOpticalFlowFarneback(gray1, gray2, flow_mat, 0.5, 3, 12, 3, 5, 1.2, 0);
	showOpticalFlow(flow_mat, gray1); //Отрисовать оптический поток

	//выбираем точки
	vector<Point2f> left_points, right_points;
	for (int y = 0; y < img1.rows; y += 3) {
		for (int x = 0; x < img1.cols; x += 3) {
			//Поток - расстояние между точками на левом и правом изображениях
			Point2f flow = flow_mat.at<Point2f>(y, x);

			//Если оптический поток слишком мал, пренебрегаем им
			if (fabs(flow.x) < 0.1 && fabs(flow.y) < 0.1)
				continue;

			left_points.push_back(Point2f(x, y));
			right_points.push_back(Point2f(x + flow.x, y + flow.y));
		}
	}

	//Исправляем изображения в зависимости от параметров камеры (insintric params & dist coefficients)
	undistortPoints(left_points,  left_points,  of_matr.CM1, of_matr.D1, of_matr.R1, of_matr.P1); 
	undistortPoints(right_points, right_points, of_matr.CM2, of_matr.D2, of_matr.R2, of_matr.P2);

	Mat point_cloud;
	triangulatePoints(of_matr.P1, of_matr.P2, left_points, right_points, point_cloud);
	from_homohenous(point_cloud);

	return point_cloud;
}

//Для одной камеры
Mat single_sfm(Mat& img1, Mat& img2, Mat & camera_matrix, Mat distortion_coeff)
{
	Mat img1U, img2U;
	undistort(img1, img1U, camera_matrix, distortion_coeff); //Это можно кое где применить
	undistort(img2, img2U, camera_matrix, distortion_coeff); //Это можно кое где применить

	Mat gray1, gray2;
	cvtColor(img1U, gray1, CV_BGR2GRAY);
	cvtColor(img2U, gray2, CV_BGR2GRAY);

	

	Mat flow_mat;
	//Считает оптический поток
	//http://docs.opencv.org/master/dc/d6b/group__video__track.html#ga5d10ebbd59fe09c5f650289ec0ece5af
	calcOpticalFlowFarneback(gray1, gray2, flow_mat, 0.5, 3, 12, 3, 5, 1.2, 0);
	showOpticalFlow(flow_mat, gray1); //Отрисовать оптический поток

	//выбираем точки
	vector<Point2f> left_points, right_points;
	for (int y = 0; y < img1U.rows; y += 6) {
		for (int x = 0; x < img1U.cols; x += 6) {
			//Поток - расстояние между точками на левом и правом изображениях
			Point2f flow = flow_mat.at<Point2f>(y, x);

			//Если оптический поток слишком мал, пренебрегаем им
			if (fabs(flow.x) < 0.1 && fabs(flow.y) < 0.1)
				continue;

			left_points.push_back(Point2f(x, y));
			right_points.push_back(Point2f(x + flow.x, y + flow.y));
		}
	}

	//Исправляем изображения в зависимости от параметров камеры (insintric params & dist coefficients)
	undistortPoints(left_points, left_points, camera_matrix, distortion_coeff);
	undistortPoints(right_points, right_points, camera_matrix, distortion_coeff);

	/* Try to find essential matrix from the points */
	Mat fundamental = findFundamentalMat(left_points, right_points, cv::FM_RANSAC, 3.0, 0.99);
	Mat essential = camera_matrix.t() * fundamental * camera_matrix;

	/* Find the projection matrix between those two images */
	SVD svd(essential);
	static const Mat W = (Mat_<double>(3, 3) <<
		0, -1, 0,
		1, 0, 0,
		0, 0, 1);

	static const Mat W_inv = W.inv();

	Mat_<double> R1 = svd.u * W * svd.vt;
	Mat_<double> T1 = svd.u.col(2);

	Mat_<double> R2 = svd.u * W_inv * svd.vt;
	Mat_<double> T2 = -svd.u.col(2);

	static const Mat P1 = Mat::eye(3, 4, CV_64FC1);
	Mat P2 = (Mat_<double>(3, 4) <<
		R1(0, 0), R1(0, 1), R1(0, 2), T1(0),
		R1(1, 0), R1(1, 1), R1(1, 2), T1(1),
		R1(2, 0), R1(2, 1), R1(2, 2), T1(2));

	Mat point_cloud;
	triangulatePoints(P1, P2, left_points, right_points, point_cloud); //of_matr.P2
	from_homohenous(point_cloud);

	return point_cloud;
}




//Переводим из общих координат
void from_homohenous(Mat & src)
{
	/* Since it's homogenous (x, y, z, w) coord, divide by w to get (x, y, z, 1) */
	//Делим x, y, z на w
	for (int i = 0; i < src.cols; i++)
	{
		float w = src.at<float>(3, i);
		float x = src.at<float>(0, i);
		float y = src.at<float>(1, i);
		float z = src.at<float>(2, i);

		if (w == 0) x = y = z = 0;

		src.at<float>(0, i) = x / w;
		src.at<float>(1, i) = y / w;
		src.at<float>(2, i) = z / w;

	}

	//Удаляем четвертый столбец из матрицы
	src = src.rowRange(0, src.rows - 1);
}

