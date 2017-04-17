#pragma once
#include "Libraries.h"
#include "Aiming.h"

//Структура с данными, необходимыми для алгоритма оптического потока
struct OpticalFlowMatrices
{
public:
	Mat CM1, CM2;
	Mat D1, D2;
	Mat P1, P2;
	Mat R1, R2;
	OpticalFlowMatrices(){}
	OpticalFlowMatrices(const Mat & CM1_o, const Mat & CM2_o, const Mat & D1_o, const Mat & D2_o, const Mat & P1_o, const Mat & P2_o, const Mat & R1_o, const Mat & R2_o)
	{
		CM1 = CM1_o.clone();
		CM2 = CM2_o.clone();
		D1 = D1_o.clone();
		D2 = D2_o.clone();
		R1 = R1_o.clone();
		R2 = R2_o.clone();
		P1 = P1_o.clone();
		P2 = P2_o.clone();
	}
};

struct CameraParams
{
public:
	Mat CM, D;
	CameraParams(){}
	CameraParams(const Mat & CM_o, const Mat & D_o)
	{
		CM = CM_o.clone();
		D = D_o.clone();
	}
};


OpticalFlowMatrices stereo_calibration(const Size & board_sz);
CameraParams camera_calibration(Size board_sz);


//Функция составлена по туториалу http://www.jayrambhia.com/blog/stereo-calibration/
OpticalFlowMatrices stereo_calibration(const Size & board_sz)
{
	//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
	// == == == == == == == == == == == == == == == =Input= == == == == == == == == == == == == == == == =
	//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
	//Описываем доску
	int board_n = board_sz.width * board_sz.height;
	int board_w = board_sz.width;
	const char window_name[] = "CALIBRATED";

	//Описываем все остальное
	bool found1, found2 = false; //Найдены ли углы на фото
	vector<Point2f> corners1, corners2; //Массивы углов
	vector<vector<Point2f> > imagePoints1, imagePoints2; //Местоположения найденных точек на соответствующих картинках
	vector<vector<Point3f> > objectPoints; //Реальное местоположение углов в 3D пространстве

	//Настраиваем камеру и фотографии
	Mat img1, img2;
	VideoCapture cap0(0);
	VideoCapture cap1(1); //Создаем поток с камеры ||| У меня (0 - PORTABLE, 1 - STD)

	bool is_enter_pressed = false;
	while (!is_enter_pressed)
	{

		//Сначала "прицеливаемся" (чтобы понять, попало ли изображение в кадр или нет)
		is_enter_pressed = calibrate_aiming(cap0, cap1, board_sz);

		//Делаем два фото
		cap0 >> img1;
		cap1 >> img2;

		//Переводим изображение из BGR формата в Grayscale формат
		//http://docs.opencv.org/master/d7/d1b/group__imgproc__misc.html#ga397ae87e1288a81d2363b61574eb8cab
		cvtColor(img1, img1, CV_BGR2GRAY);
		cvtColor(img2, img2, CV_BGR2GRAY);

		vector<Point3f> obj;
		for (int j = 0; j < board_n; j++)
			obj.push_back(Point3f(j / board_w, j%board_w, 0.0f));

		//Ищем углы на фото 
		//http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga93efa9b0aa890de240ca32b11253dd4a
		found1 = findChessboardCorners(img1, board_sz, corners1, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		found2 = findChessboardCorners(img2, board_sz, corners2, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		//Если углы найдены
		if (found1 && found2)
		{
			imagePoints1.push_back(corners1);
			imagePoints2.push_back(corners2);
			objectPoints.push_back(obj);
			printf("Углы найдены!\n");
		}

		//Если углы не найдены, выводим сообщение об этом
		else
		{
			if (!found1)
				puts("На 1 изображении не найдены углы.");
			if (!found2)
				puts("На 2 изображении не найдены углы.");
		}


		//Рисуем углы на изображениях и отображаем их
		cornerSubPix(img1, corners1, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		drawChessboardCorners(img1, board_sz, corners1, found1);

		cornerSubPix(img2, corners2, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		drawChessboardCorners(img2, board_sz, corners2, found2);

		show_images(img1, img2, window_name, true);
		destroyWindow(window_name);
	}
	

	//OUTPUT stereoCalibrate
	Mat CM1(3, 3, CV_64FC1), CM2(3, 3, CV_64FC1); //CameraMatrix указывает как перейти от 3D точек в мировой СК вк 2D точкам в изображении
	Mat D1, D2;
	Mat R, //Матрица поворота между СК первой и второй камер
		T, //Вектор перехода между СК первой и второй камер
		E, //Устанавливает соотношения между точками изображения?????
		F; //Фундаментальная матрица

	//StereoCalibrate
	puts("[LOG]Стерео Калбровка начата");
	//http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga246253dcc6de2e0376c599e7d692303a
	stereoCalibrate(objectPoints, imagePoints1, imagePoints2,
		CM1, D1, CM2, D2, img1.size(), R, T, E, F, CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST,
		cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5));
	puts("[LOG]Стерео Калбровка завершена");

	//OUTPUT stereoReactify
	Mat R1, R2, //Матрицы поворота для 1 и 2 изображений
		P1, P2, //Матрицы проекции в новых (выпрямленных) координатных системах
		Q;		//Карта неровностей глубины
	//http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga617b1685d4059c6040827800e72ad2b6
	stereoRectify(CM1, D1, CM2, D2, img1.size(), R, T, R1, R2, P1, P2, Q);

	return OpticalFlowMatrices(CM1, CM2, D1, D2, P1, P2, R1, R2);
}


CameraParams camera_calibration(Size board_sz)
{
	int board_w = board_sz.width;  // number of horizontal corners
	int board_h = board_sz.height; // number of vertical corners
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
		final_frame = single_camera_aiming(cap);
		cap >> img;
		cvtColor(img, gray, CV_BGR2GRAY);
		found = findChessboardCorners(gray, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if (found)
		{
			imagePoints.push_back(corners);
			objectPoints.push_back(obj);
			puts("[OK]Углы найдены!");
			success++;
		}

		else
		{
			puts("[ERR]Углы не найдены!");
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

	puts("[LOG]Калибровка 1 камеры начата");
	calibrateCamera(objectPoints, imagePoints, img.size(), CM, D, rvecs, tvecs);
	puts("[LOG]Калибровка 1 камеры завершена");

	return CameraParams(CM.clone(), D.clone());
}