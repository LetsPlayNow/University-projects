#pragma once
//Алгоритмы
#include "Depth_map.h"
#include "Point_cloud.h"

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

//Этот класс отвечает за связь окна и алгоритмов
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void connecting();
	void load_images();

private:
	Depth_map dm;
	Point_cloud pc;
	QString left_image_path, right_image_path;
	Mat left_image, right_image;

	Ui::MainWindowClass ui;
};