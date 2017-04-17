#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <vector>

#include <opencv2\core\mat.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <Windows.h>

using cv::Mat;
using cv::waitKey;
using cv::cvtColor;
using cv::Point2f;
using cv::Point3f;
using cv::SVD;
using cv::Mat_;
using cv::Rect;
using cv::Size;
using cv::imread;
using cv::imshow;
using cv::VideoCapture;
using cv::FileStorage;

using namespace std;
using namespace cv;


struct Camera_Params
{
	Mat CM; //Camera matrix
	Mat D;  //Distorsion vector
};

//Прототипы функций
Mat sfm(Mat& img1, Mat& img2, Mat& cam_matrix, Mat& dist_coeff);
Mat optical_flow_alg(Mat & CM, Mat & D);
bool calibrate_aiming(VideoCapture & cam, Size board_sz);
void aiming(VideoCapture & cam);
void show_images(const Mat & img1, const Mat & img2);
void read_calibration_from_file(Camera_Params & cp);
void write_calibration_to_file(Camera_Params & cp);
void save_4d_to_file(const char * filename, const Mat & mat, const char * mode);
void to_homohenous(Mat & src);
void shift_points_to_center(Mat & mat);
void showOpticalFlow(const Mat & flow, const Mat & left_photo);







//Производит калибровку одной камеры
//IN
//Size size: размер шахматной доски (в количетсве углов)
//OUT
//Mat CM: Camera Matrix
//Mat D: Camera distorsion coefficients
Camera_Params camera_calibration(Size size)
{
	int board_w = size.width;  // number of horizontal corners
	int board_h = size.height; // number of vertical corners

	Size board_sz = size;
	int board_n = board_w*board_h;

	vector<vector<Point3f> > objectPoints;
	vector<vector<Point2f> > imagePoints;
	vector<Point2f> corners;

	//Настроим камеру
	Mat img, gray;
	VideoCapture cap(0);
	int k = 0;
	int success = 0;
	bool found = false;
	

	vector<Point3f> obj;
	for (int j = 0; j<board_n; j++)
	{
		obj.push_back(Point3f(j / board_w, j%board_w, 0.0f));
	}

	bool final_frame = false;
	while (!final_frame)
	{
		final_frame = calibrate_aiming(cap, Size(7, 7));
		cap >> img;
		cvtColor(img, gray, CV_BGR2GRAY);
		found = findChessboardCorners(gray, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (found)
		{
			imagePoints.push_back(corners);
			objectPoints.push_back(obj);
			puts("[OK]Corners stored");
			success++;
		}

		else
		{
			puts("[ERR]Corners don't found");
			continue;
		}


		//Draw Corners on chessboard and show
		cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		drawChessboardCorners(gray, board_sz, corners, found);
		//imshow("corners", gray);
	}

	Mat CM(3, 3, CV_32FC1);
	Mat D;
	vector<Mat> rvecs, tvecs;

	CM.at<float>(0, 0) = 1;
	CM.at<float>(1, 1) = 1;

	puts("[LOG]Calibration Started");
	calibrateCamera(objectPoints, imagePoints, img.size(), CM, D, rvecs, tvecs);
	puts("[LOG]Calibration Finished");

	Mat imgU;
	undistort(img, imgU, CM, D);

	Camera_Params cp = { CM.clone(), D.clone() };
	return cp;
}

//Строит облако точек
//IN
//Mat CM: Матрица камеры
//Mat D:  Коэффициенты дисторсии камеры
//OUT
//Mat OUT: Облако точек
Mat optical_flow_alg(Mat & CM, Mat & D)
{
	VideoCapture cap0(0);
	aiming(cap0);
	Mat img1, img2;
	cap0.read(img1);
	aiming(cap0);
	cap0.read(img2);
	
	//Mat img1 = imread("left.png");
	//Mat img2 = imread("right.png");

	show_images(img1, img2);

	Mat out = sfm(img1, img2, CM, D);


	return out;
}

//Сохраняет облако точек в файл
//IN
//const char * filename: Имя файла для сохранения
//const Mat &:  Сохраняемая матрица (облако точек)
void save_4d_to_file(const char * filename, const Mat & mat, const char * mode)
{
	FILE * file = fopen(filename, mode);
	for (int i = 0; i < mat.cols; i++)
	{
		float x = mat.at<float>(0, i);
		float y = mat.at<float>(1, i);
		float z = mat.at<float>(2, i);

		fprintf(file, "v %.5f %.5f %.5f\n", x, y, z);
	}
	fclose(file);
}


//Записать матрицы CM и D в файл
//OUT: 
//Camera_Params: структура, содержащая CM и D
void read_calibration_from_file(Camera_Params & cp)
{
	puts("[LOG]READ_CALIB_DATA_FROM_FILE = START");
	FileStorage fs1("mycalib.yml", FileStorage::READ);
	fs1["CM"] >> cp.CM;
	fs1["D"]  >> cp.D;
	fs1.release();
	puts("[LOG]READ_CALIB_DATA_FROM_FILE = END");
}

//Записать матрицы CM и D в файл
//IN: 
//Camera_Params: структура для CM и D
void write_calibration_to_file(Camera_Params & cp)
{
	puts("[LOG]WRITE_CALIB_DATA_TO_FILE = START");
	FileStorage fs1("mycalib.yml", FileStorage::WRITE);
	fs1 << "CM" << cp.CM;
	fs1 << "D" << cp.D;
	fs1.release();
	puts("[LOG]WRITE_CALIB_DATA_TO_FILE = END");
}

/**
* Structure from motion from 2 cameras, using farneback optical flow as the 'features'
* No, this doesn't work on more than 2 cams, because that requires bundle adjustment, which
* I'm still searching if there's an OpenCV implementation of it
*/
Mat sfm(Mat& img1, Mat& img2, Mat& cam_matrix, Mat& dist_coeff) 
{
	//img1 = imread("left.png");
	//img2 = imread("right.png");
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
	for (int y = 0; y < img1.rows; y+=6) {
		for (int x = 0; x < img1.cols; x+=6) {
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
	undistortPoints(left_points, left_points, cam_matrix, dist_coeff);
	undistortPoints(right_points, right_points, cam_matrix, dist_coeff);

	/* Try to find essential matrix from the points */
	Mat fundamental = findFundamentalMat(left_points, right_points, cv::FM_RANSAC, 3.0, 0.99);
	Mat essential = cam_matrix.t() * fundamental * cam_matrix;

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

	/*  Triangulate the points to find the 3D homogenous points in the world space
	Note that each column of the 'out' matrix corresponds to the 3d homogenous point
	*/
	Mat out;
	triangulatePoints(P1, P2, left_points, right_points, out);
	to_homohenous(out);

	return out;
}

void to_homohenous(Mat & src)
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

void showOpticalFlow(const Mat & flow, const Mat & left_photo)
{
	Mat canvas = left_photo.clone();
	//Mat canvas(flow.size(), CV_8UC3, Scalar(1, 1, 1));
	for (int y = 0; y < flow.rows; y+=6)
	for (int x = 0; x < flow.cols; x+=6)
	{
		auto flowPoint = flow.at<Point2f>(y, x);
		float x_flow = flowPoint.x;
		float y_flow = flowPoint.y;
		line(canvas, Point2f(x, y), Point2f(x + x_flow, y + y_flow), Scalar(0, 0, 0));
	}	

	imshow("Optical flow", canvas);
	waitKey(0);
}


int main(int argc, const char * argv[]) 
{
	setlocale(LC_ALL, "Russian");
	Size size(7, 7);
	vector<Mat>point_clouds;
	Camera_Params cp;
	//cp = camera_calibration(size);
	//write_calibration_to_file(cp);
	read_calibration_from_file(cp);

	//Набираем данные в цикле
	int key;
	while (true)
	{
		point_clouds.push_back(optical_flow_alg(cp.CM, cp.D));

		key = waitKey(0);
		if (key == 13) break;
	}

	//Собираем все точки в 1 матрицу
	Mat out(point_clouds[0]);
	for (int i = 1; i < point_clouds.size(); i++)
		hconcat(out, point_clouds[i], out);

	shift_points_to_center(out);
	save_4d_to_file("out.obj", out, "w");

	return 0;
}


/*  This is a silly hack to shift the image to the origin coord (0, 0, 0)
by applying K-mean cluster (in this case, 1 cluster), to get the cluster center ...
*/
void shift_points_to_center(Mat & mat)
{
	//Кусок кода ниже нужно перенести в конец цикла, чтобы поработать со всеми точками!
	Mat labels, center;
	kmeans(mat.t(), 1, labels, cv::TermCriteria(CV_TERMCRIT_ITER, 1000, 1e-5), 1, cv::KMEANS_RANDOM_CENTERS, center);

	/*  ... and shift all the points based on the cluster center */
	for (int i = 0; i < mat.cols; i++)
	{
		mat.at<float>(0, i) -= center.at<float>(0, 0);
		mat.at<float>(1, i) -= center.at<float>(0, 1);
		mat.at<float>(2, i) -= center.at<float>(0, 2);
	}
}