#pragma once
#ifndef OPENCV
#define OPENCV
#include <opencv2\core\core.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
#endif

class Depth_map
{
public:
	Depth_map(const Mat left_image, const Mat right_image);
	Depth_map();
	~Depth_map();

	//Сеттеры
	void set_left_img(const Mat left_image);
	void set_right_image(const Mat right_image);

	//Алгоритмы расчета карт глубины
	void StereoBM_alg();
	void StereoSGBM_alg();

	//Функции настройки этих алгоритмов
	void preset_stereoBM_alg();
	void preset_stereoSGBM_alg();

	//Геттеры
	Mat get_depth_map();

private:
	//[IN]
	//Левое и правое изображеия соответственно
	Mat left_img;
	Mat right_img;

	//Algoritm objects
	Ptr<StereoBM> sbm;   // = StereoBM::create(16, 9);
	Ptr<StereoSGBM> sgbm;// = StereoSGBM::create(0, 80, 9);

	//[OUT]
	Mat temp_map;  //Необработанные результаты алгоритмов поиска карты глубины
	Mat depth_map; //Обработанные (normalized карты глубин)
};


