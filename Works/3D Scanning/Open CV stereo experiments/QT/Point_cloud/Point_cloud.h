#pragma once
#ifndef OPENCV
#define OPENCV
#include <opencv2\core\core.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
#endif


//Расчет облака точек по карте глубины
class Point_cloud
{
public:
	Point_cloud(const Mat other_depth_map);
	Point_cloud(); //Нужен в том случае, если карта глубины еще не построена
	~Point_cloud();

	//Сеттеры
	void set_depth_map(const Mat other_depth_map);

	//Расчеты
	void make_point_cloud_from_depth_map();

private:
	//[IN]
	Mat depth_map; //Карта глубины
	Mat Q; //reprojection_matrix

	//[OUT]
	Mat image3D;   //3D матрица (облако точек)


};

