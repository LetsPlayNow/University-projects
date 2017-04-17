#include "Depth_map.h"


Depth_map::Depth_map(const Mat left_image, const Mat right_image)
{
	left_img = left_image;
	right_img = right_image;
}

Depth_map::Depth_map()
{

}

Depth_map::~Depth_map()
{

}

//Сеттеры
void Depth_map::set_left_img(const Mat left_image)
{
	left_img = left_image;
}

void Depth_map::set_right_image(const Mat right_image)
{
	right_img = right_image;
}

//Алгоритмы расчета карт глубины
void Depth_map::StereoBM_alg()
{
	sbm->compute(left_img, right_img, depth_map);
}

void Depth_map::StereoSGBM_alg()
{
	sgbm->compute(left_img, right_img, depth_map);
}

//Функции настройки этих алгоритмов
void Depth_map::preset_stereoBM_alg()
{
	//Настройка StereoBM (можно поиграться)
	//Подробное описание параметров по адресу
	//http://www.jayrambhia.com/blog/disparity-maps/
	sbm->setBlockSize(9); //Размер блоков (> больше детализация, больше шума)
	sbm->setNumDisparities(48); //Какой-то разрыв (должно делиться на 16) (различимый разрыв)
	sbm->setPreFilterSize(51);
	sbm->setPreFilterCap(50); //Что - то нечетное, влияния не заметил
	sbm->setMinDisparity(-39); //Меньше - разбитое фото, больше - темное (изменения сглаживаются)
	sbm->setTextureThreshold(10); //Порог текстуры (> = появляются черные пятна, < пятен нет, но детализация уменьшается)
	sbm->setUniquenessRatio(15); //Влияет на контуры (> четче)
	sbm->setSpeckleWindowSize(100); //Жесть какая - то
	sbm->setSpeckleRange(32);
	sbm->setDisp12MaxDiff(1);
}

void Depth_map::preset_stereoSGBM_alg()
{
	//Настройка StereoSBGM (можно поиграться)
	//Подробное описание параметров по адресу
	//http://www.emgu.com/wiki/files/3.0.0-alpha/document/html/bb79b616-169d-3cba-7afd-f9921767cb35.htm
	sgbm->setMinDisparity(-20);
	sgbm->setNumDisparities(80);
	sgbm->setDisp12MaxDiff(10);
	sgbm->setUniquenessRatio(10);
	sgbm->setSpeckleWindowSize(100);
	sgbm->setSpeckleRange(32);
	sgbm->setMode(StereoSGBM::MODE_SGBM); // : StereoSGBM::MODE_HH
	sgbm->setP1(1600);
	sgbm->setP2(2200);
}

//Геттеры
Mat Depth_map::get_depth_map()
{
	return depth_map;
}

