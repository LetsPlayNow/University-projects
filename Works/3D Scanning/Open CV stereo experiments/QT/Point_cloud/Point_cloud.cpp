#include "Point_cloud.h"


Point_cloud::Point_cloud(const Mat other_depth_map)
{
	depth_map = other_depth_map;
	//Матрицу Q нужно расчитать, а не брать наобум
	Q = (Mat_<double>(4, 4) <<  1., 0., 0., -3.25545,
								0., 1., 0., -2.4126,
								0., 0., 0., 4.2440,
								0., 0., -2.993, 0.);
}

Point_cloud::Point_cloud()
{
	Q = (Mat_<double>(4, 4) << 1., 0., 0., -3.25545,
								0., 1., 0., -2.4126,
								0., 0., 0., 4.2440,
								0., 0., -2.993, 0.);
}

Point_cloud::~Point_cloud()
{

}

//Сеттеры
void Point_cloud::set_depth_map(const Mat other_depth_map)
{
	depth_map = other_depth_map;
}

//Расчеты
void Point_cloud::make_point_cloud_from_depth_map()
{
	reprojectImageTo3D(depth_map, //[IN]Карта глубины
					   image3D,   //[OUT]Облако точек
					   Q,         //[IN]Reprojection Matrix
					   true);     //[IN]Использовать ли откинутые точки
}
