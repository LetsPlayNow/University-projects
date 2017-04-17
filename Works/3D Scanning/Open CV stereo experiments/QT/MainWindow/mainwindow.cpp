#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	dm = Depth_map();
	pc = Point_cloud();
	load_images();
	ui.setupUi(this);
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::load_images()
{
	//right_image_path.toLocal8Bit().data()
	left_image = imread("D:\\Projects\\VS_2013\\open_cv_test\\OpenCV_ceemple_test\\IN\\left.png", 0);
	right_image = imread("D:\\Projects\\VS_2013\\open_cv_test\\OpenCV_ceemple_test\\IN\\right.png", 0);

	imshow("left_image", left_image);
	imshow("right_image", right_image);
}


