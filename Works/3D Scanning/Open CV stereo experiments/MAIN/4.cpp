//StereoBM + StereoSGBM + reprojectImageto3D 
//Титов Алексей Константинович
//Построение карты глубины и облака точек по двум изображениям
//
//TODO: 
// 1 - Заменить картинки потоками с камер
// 2 - Получить матрицу Q не из воздуха
// 3 - Разобрать настройки алгоритмов (возможно сделать граф интерйес для настройки)
// 4 - Рефакторинг кода
// 
//NOTE:
// 1 - Point_cloud получаемая через SGBM ужасна. Это означает, что нам позарез нужен способ высчитывать матрицу Q
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <clocale>
#include <iostream>  
#include <Windows.h>
#include <opencv2\core\mat.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>



//Ptr<StereoSGBM> sgbm;
int minDisparity = 30;
int numDisparities = 80;
int disp12MaxDiff = 10;
int uniquenessRatio = 10;
int speckleWindowSize = 100;
int speckleRange = 32;
int p1 = 1600;
int p2 = 2200;

using namespace cv;
using namespace std;


struct CallbackData
{
	Mat* left;
	Mat* right;
};

double _Q[] = { 1., 0., 0., -3.2554532241821289e+002,
				0., 1., 0., -2.4126087760925293e+002,
				0., 0., 0., 4.2440051858596559e+002,
				0.,0., -2.9937622423123672e-001, 0. };

//Прототипы функций
void preset_sbm_alghoritm(Ptr<StereoBM> sbm);
void preset_sgbm_alghoritm(Ptr<StereoSGBM> sgbm);
void saveXYZ(const char* filename, const Mat& mat);
void print_matrix_MAT(const Mat & mat);
void callback(int wtf, void* data);

void calib3D();
void find_depth_map_and_point_cloud();
void depth_map_in_real_time();
void single_calibration();
int aiming(VideoCapture & cap1, VideoCapture & cap2);
void get_images_from_camera();
void displayMap(Mat& leftGrey, Mat& rightGrey);
void calculatePointCloud(cv::Ptr<cv::StereoSGBM> sgbm, Mat & imgU1, Mat & imgU2);

struct Corrected_Images
{
	Mat img_1;
	Mat img_2;
};

bool stereo_calibration(Corrected_Images & correct_pair);
Mat StereoBM_alg(const Mat & left_img, const Mat & right_img, Mat & normalized_depth_map);
Mat StereoSGBM_alg(const Mat & left_img, const Mat & right_img, Mat & normalized_depth_map);


int main(int argc, char* argv[])
{
	find_depth_map_and_point_cloud();
	////get_images_from_camera();
	//Corrected_Images correct_pair;
	//bool success = stereo_calibration(correct_pair);
	//if (success)
	//{
	//	puts("Калибровка успешно завершена!");
	//	imshow("test1", correct_pair.img_1);
	//	imshow("test2", correct_pair.img_2);
	//	waitKey(0);
	//}

	//else
	//{
	//	puts("Калибровка завершена с ошибкой");
	//}

	//system("pause");
	return 0;
}

void get_images_from_camera()
{
	VideoCapture cap0(0);
	VideoCapture cap1(1);
	VideoCapture cap2(2);
	Mat left_image, right_image, center_image;

	while (true)
	{
		cap0 >> left_image;
		cap1 >> right_image;
		cap2 >> center_image;
		imshow("0", left_image);
		imshow("1", right_image);
		imshow("2", center_image);
		if (waitKey(1) != -1)break;
	}
	destroyAllWindows();
	//aiming(cap0, cap1);
	//cap0 >> left_image;
	//cap1 >> right_image;
	imwrite("left_out.png", left_image);
	imwrite("right_out.png", right_image);
}


int aiming(VideoCapture & cap1, VideoCapture & cap2)
{
	Mat temp_img;
	int key_code;
	while (true)
	{
		cap1 >> temp_img;
		resize(temp_img, temp_img, Size(400, 400));
		imshow("Aiming1", temp_img);
		moveWindow("Aiming1", 0, 0);
		cap2 >> temp_img;
		resize(temp_img, temp_img, Size(400, 400));
		imshow("Aiming2", temp_img);
		moveWindow("Aiming2", 400, 0);
		if ((key_code = waitKey(1)) != -1)
			break;
	}

	destroyWindow("Aiming1");
	destroyWindow("Aiming2");

	return key_code;
}

void single_calibration()
{
	setlocale(LC_ALL, "Russian");

	int numBoards = 1; // number of different poses
	int board_w = 7;   // number of horizontal corners
	int board_h = 7;   // number of vertical corners

	Size board_sz = Size(board_w, board_h);
	int board_n = board_w*board_h;

	vector<vector<Point3f> > objectPoints;
	vector<vector<Point2f> > imagePoints;
	vector<Point2f> corners;

	Mat img, gray;
	VideoCapture cap(0);
	cap >> img;
	int success = 0;
	int k = 0;
	bool found = false;

	vector<Point3f> obj;
	for (int j = 0; j<board_n; j++)
	{
		obj.push_back(Point3f(j / board_w, j%board_w, 0.0f));
	}

	cap >> img;
	cvtColor(img, gray, CV_BGR2GRAY);
	found = findChessboardCorners(gray, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

	//Если надены признаки шахматной доски
	if (found)
	{
		imagePoints.push_back(corners);
		objectPoints.push_back(obj);
		puts("Углы найдены");
		success++;
	}

	else
	{
		puts("Углы не найдены");
		system("pause");
		return;
	}


	//Рисуем углы на изображении и отображаем их

	cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
	drawChessboardCorners(gray, board_sz, corners, found);
	imshow("corners", gray);
	waitKey(1);

	Mat CM = Mat(3, 3, CV_32FC1);
	Mat D;
	vector<Mat> rvecs, tvecs;

	CM.at<float>(0, 0) = 1;
	CM.at<float>(1, 1) = 1;
	//double calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags = 0, criteria)
	calibrateCamera(objectPoints, imagePoints, img.size(), CM, D, rvecs, tvecs);

	Mat imgU;
	undistort(img, imgU, CM, D);


	FileStorage fs1("mycalib.yml", FileStorage::WRITE);
	fs1 << "CM" << CM;
	fs1 << "D" << D;

	fs1.release();
}

//Функция составлена по туториалу http://www.jayrambhia.com/blog/stereo-calibration/
bool stereo_calibration(Corrected_Images & correct_pair)
{
	setlocale(LC_ALL, "Russian");
	//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
	// == == == == == == == == == == == == == == == =Input= == == == == == == == == == == == == == == == =
	//== == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
	//Описываем доску
	int board_w = 10;   // Число углов по горизонтали
	int board_h = 7;   // Число углов по вертикали
	int board_n = board_w*board_h; //Число углов на фото
	Size board_sz = Size(board_w, board_h); //Размер доски

	//Описываем все остальное
	bool found1, found2 = false; //Найдены ли углы на фото
	vector<Point2f> corners1, corners2; //Массивы углов
	vector<vector<Point2f> > imagePoints1, imagePoints2; //Местоположения найденных точек на соответствующих картинках
	vector<vector<Point3f> > objectPoints; //Реальное местоположение углов в 3D пространстве

	//Настраиваем камеру и фотографии
	Mat img1, img2;
	VideoCapture cap0(0);
	VideoCapture cap1(1); //Создаем поток с камеры ||| У меня (0 - PORTABLE, 1 - STD)
	int key_code = 0;

	while (key_code != 13)
	{

		//Сначала "прицеливаемся" (чтобы понять, попало ли изображение в кадр или нет)
		key_code = aiming(cap0, cap1);

		//Делаем два фото с задержкой в половину секунды
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

			//return false;
		}


		//Рисуем углы на изображениях и отображаем их
		//http://docs.opencv.org/master/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
		cornerSubPix(img1, corners1, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		////http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022
		drawChessboardCorners(img1, board_sz, corners1, found1);
		//http://docs.opencv.org/master/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563
		imshow("img1", img1);
		moveWindow("img1", 0, 0);
		waitKey(0); //Отображать изображение бесконечно

		//http://docs.opencv.org/master/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
		cornerSubPix(img2, corners2, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
		//http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022
		drawChessboardCorners(img2, board_sz, corners2, found2);
		//http://docs.opencv.org/master/d7/dfc/group__highgui.html#ga453d42fe4cb60e5723281a89973ee563
		imshow("img2", img2);
		moveWindow("img2", 300, 0);
		waitKey(0); //Отображать изображение бесконечно
		destroyAllWindows();
	}

	//OUTPUT stereoCalibrate
	Mat CM1(3, 3, CV_64FC1), CM2(3, 3, CV_64FC1); //CameraMatrix указывает как перейти от 3D точек в мировой СК вк 2D точкам в изображении
	Mat D1, D2;
	Mat R, //Матрица поворота между СК первой и второй камер
		T, //Вектор перехода между СК первой и второй камер
		E, //Устанавливает соотношения между точками изображения?????
		F; //Фундаментальная матрица

	//StereoCalibrate
	//http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga246253dcc6de2e0376c599e7d692303a
	stereoCalibrate(objectPoints, imagePoints1, imagePoints2,
	CM1, D1, CM2, D2, img1.size(), R, T, E, F, CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST,
	cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-5));

	//OUTPUT stereoReactify
	Mat R1, R2, //Матрицы поворота для 1 и 2 изображений
		P1, P2, //Матрицы проекции в новых (выпрямленных) координатных системах
		Q;		//Карта неровностей глубины
	//http://docs.opencv.org/master/d9/d0c/group__calib3d.html#ga617b1685d4059c6040827800e72ad2b6
	stereoRectify(CM1, D1, CM2, D2, img1.size(), R, T, R1, R2, P1, P2, Q);

	//Запись результатов в файл
	//http://docs.opencv.org/master/da/d56/classcv_1_1FileStorage.html
	//Подробнее:
	//http://docs.opencv.org/master/dd/d74/tutorial_file_input_output_with_xml_yml.html
	FileStorage fs1("./OUT/mystereocalib.yml", FileStorage::WRITE);
	fs1 << "CM1" << CM1;
	fs1 << "CM2" << CM2;
	fs1 << "D1" << D1;
	fs1 << "D2" << D2;
	fs1 << "R" << R;
	fs1 << "T" << T;
	fs1 << "E" << E;
	fs1 << "F" << F;
	fs1 << "R1" << R1;
	fs1 << "R2" << R2;
	fs1 << "P1" << P1;
	fs1 << "P2" << P2;
	fs1 << "Q" << Q;
	fs1.release();

	//Корректировка изображений
	//Сюда нужно засунуть другие изображения, которые нужно откорректировать
	//Вместо img1 и img2
	Mat map1x, map1y, map2x, map2y;
	Mat imgU1, imgU2;

	//http://docs.opencv.org/master/da/d54/group__imgproc__transform.html#ga7dfb72c9cf9780a347fbe3d1c47e5d5a
	initUndistortRectifyMap(CM1, D1, R1, P1, img1.size(), CV_32FC1, map1x, map1y);
	initUndistortRectifyMap(CM2, D2, R2, P2, img1.size(), CV_32FC1, map2x, map2y);

	//Делаем фотки с той же камеры, по которым будем строить облако точек и карту глубины
	aiming(cap0, cap1);
	cap0 >> img1;
	cvtColor(img1, img1, CV_BGR2GRAY);
	cap1 >> img2;
	cvtColor(img2, img2, CV_BGR2GRAY);
	//http://docs.opencv.org/master/da/d54/group__imgproc__transform.html#gab75ef31ce5cdfb5c44b6da5f3b908ea4
	remap(img1, imgU1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
	remap(img2, imgU2, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

	//Колибровка успешно завершена
	//Возвращаем откорректированные фото
	//correct_pair = { imgU1, imgU2 };

	imshow("Corrected1", imgU1);
	waitKey(1);
	imshow("Corrected2", imgU1);
	waitKey(1);
	//Mat depth_map;
	//StereoBM_alg(imgU1, imgU1, depth_map);
	//imshow("depth_map", depth_map);
	//waitKey(0);

	displayMap(imgU1, imgU1);

	return true;
}





//===========================================================================================
//==============================================================================================
//=============================================OLD=================================================
//=============================================OLD=================================================
//=============================================OLD=================================================
//==============================================================================================
//===========================================================================================
//Карта глубины (без оптимизации по
void depth_map_in_real_time()
{
	Mat left_image, right_image, depth_map;
	Mat left_grey, right_grey;
	Size size(400, 400); //Сжимаем изображение, ибо оно слишком большое
	VideoCapture cap0(0); // open the default camera
	VideoCapture cap1(1); // open the default camera
	for (;;)
	{
		cap0 >> left_image;
		resize(left_image, left_image, size);
		cvtColor(left_image, left_grey, COLOR_BGR2GRAY);
		imshow("left", left_image);

		cap1 >> right_image;
		resize(right_image, right_image, size);
		cvtColor(right_image, right_grey, COLOR_BGR2GRAY);
		imshow("right", right_image);
		if (waitKey(30) >= 0) break;

		StereoBM_alg(left_grey, right_grey, depth_map);
		imshow("depth_map", depth_map);

		Sleep(1000);
	}
}

//Сохранить 3D Mat в файл
//Ограничение Z нужно в случае, если 4-й аргумент reprojectImageTo3D установлен в true (читай описание метода)
static void saveXYZ(const char* filename, const Mat& mat)
{
	const double max_z = 1.0e4;
	FILE* fp = fopen(filename, "wt");
	for (int y = 0; y < mat.rows; y++)
	{
		for (int x = 0; x < mat.cols; x++)
		{
			Vec3f point = mat.at<Vec3f>(y, x);
			if (fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
			fprintf(fp, "v %f %f %f\n", point[0], point[1], point[2]);
		}
	}
	fclose(fp);
}


//DEBUG
//Вывести матрицу Mat в консоль
void print_matrix_MAT(const Mat & mat)
{
	for (int x = 0; x < mat.cols; x++)
	{
		for (int y = 0; y < mat.rows; y++)
		{
			cout << mat.at<double>(x, y) << ' ';
		}
		putchar('\n');
	}
}

//=====================================================================================================
//=================================================BM==================================================
//=====================================================================================================
Mat StereoBM_alg(const Mat & left_img, const Mat & right_img, Mat & normalized_depth_map)
{
	Mat temp;
	//StereoBM - создание карты глубины
	//Настройка____________________________________
	Ptr<StereoBM> sbm = StereoBM::create(16, 9);
	preset_sbm_alghoritm(sbm);
	//Выполнение___________________________________
	sbm->compute(left_img, right_img, temp);
	normalize(temp, normalized_depth_map, 0, 255, CV_MINMAX, CV_8U);
	return temp.clone();
}





//=====================================================================================================
//================================================SGBM=================================================
//=====================================================================================================
Mat StereoSGBM_alg(const Mat & left_img, const Mat & right_img, Mat & normalized_depth_map)
{
	Mat temp;
	//StereoBM - создание карты глубины
	//Настройка____________________________________
	Ptr<StereoSGBM> sgbm = StereoSGBM::create(0, 80, 9);
	preset_sgbm_alghoritm(sgbm);
	//Выполнение___________________________________
	sgbm->compute(left_img, right_img, temp);
	normalize(temp, normalized_depth_map, 0, 255, CV_MINMAX, CV_8U);
	return temp.clone();
}

void preset_sgbm_alghoritm(Ptr<StereoSGBM> sgbm)
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


void preset_sbm_alghoritm(Ptr<StereoBM> sbm)
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


void find_depth_map_and_point_cloud()
{
	//Пути изображений
	char left_filename[] = "./IN/left.png";
	char right_filename[] = "./IN/right.png";

	//Загружаем изображения
	Mat left_img, right_img;
	left_img = imread(left_filename, 0);
	right_img = imread(right_filename, 0);

	//Depth_map
	//=====================================================================
	//OUT
	Mat depth_map_bm, depth_map_sgbm; //Переменные под карты глубин

	StereoBM_alg(left_img, right_img, depth_map_bm);      //StereoBM
	StereoSGBM_alg(left_img, right_img, depth_map_sgbm);  //StereoSGBM


	//3D Point cloud
	//=====================================================================
	//OUT
	Mat image3D_bm, image3D_sgbm; //Переменные под карту точек

	//IN
	//Reprojection 4x4 matrix, can be arbitrary, e.g.the one, computed by cvStereoRectify
	Mat Q = (Mat_<double>(4, 4) << 1., 0., 0., -3.25545,
		0., 1., 0., -2.4126,
		0., 0., 0., 4.2440,
		0., 0., -2.993, 0.);
	print_matrix_MAT(Q);

	//StereoBM
	reprojectImageTo3D(depth_map_bm, //[IN]Карта глубины
		image3D_bm,   //[OUT]Облако точек
		Q,            //[IN]Reprojection Matrix
		true);        //[IN]Использовать ли откинутые точки

	//StereoSGBM
	reprojectImageTo3D(depth_map_sgbm, //[IN]Карта глубины
		image3D_sgbm,   //[OUT]Облако точек
		Q,              //[IN]Reprojection Matrix
		false);         //[IN]Использовать ли откинутые точки

	//Вывод результатов
	imshow("left", left_img);
	imshow("right", right_img);
	imshow("Depth_map_bm", depth_map_bm);
	imshow("Depth_map_sgbm", depth_map_sgbm);
	waitKey(0);

	saveXYZ("bm_point_cloud.obj", image3D_bm);   //StereoBM
	saveXYZ("sgbm_point_cloud.obj", image3D_sgbm); //StereoSGBM

}

void callback(int wtf, void* data)
{
	CallbackData* cData = (CallbackData*)data;
	Ptr<StereoSGBM> sgbm = cv::StereoSGBM::create(0, 80, 9);

	if (numDisparities % 16 != 0)
		numDisparities -= numDisparities % 16;

	sgbm->setMinDisparity(minDisparity - 50);
	sgbm->setNumDisparities(numDisparities);
	sgbm->setDisp12MaxDiff(disp12MaxDiff - 1);
	sgbm->setUniquenessRatio(uniquenessRatio);
	sgbm->setSpeckleWindowSize(speckleWindowSize);
	sgbm->setSpeckleRange(speckleRange);
	sgbm->setMode(cv::StereoSGBM::MODE_SGBM); // : StereoSGBM::MODE_HH
	sgbm->setP1(p1);
	sgbm->setP2(p2);

	calculatePointCloud(sgbm,*(cData->left), *(cData->right));
}

//int minDisparity = 30;
//int numDisparities = 80;
//int disp12MaxDiff = 10;
//int uniquenessRatio = 10;
//int speckleWindowSize = 100;
//int speckleRange = 32;
//int p1 = 1600;
//int p2 = 2200;
void displayMap(Mat& leftGrey, Mat& rightGrey)
{
	namedWindow("depth");
	namedWindow("trackbars");


	CallbackData data;
	data.left = &leftGrey;
	data.right = &rightGrey;

	createTrackbar("Min Disparity", "trackbars", &minDisparity, 100, callback, (void*)&data);
	createTrackbar("Num Disparties", "trackbars", &numDisparities, 100, callback, (void*)&data);
	createTrackbar("Disp12MaxDiff", "trackbars", &disp12MaxDiff, 100, callback, (void*)&data);
	createTrackbar("Uniqueness Ratio", "trackbars", &uniquenessRatio, 100, callback, (void*)&data);
	createTrackbar("Speckle Win Size", "trackbars", &speckleWindowSize, 200, callback, (void*)&data);
	createTrackbar("Speckle Range", "trackbars", &speckleRange, 10, callback, (void*)&data);
	createTrackbar("P1", "trackbars", &p1, 3000, callback, (void*)&data);
	createTrackbar("P2", "trackbars", &p2, 3000, callback, (void*)&data);

	callback(0, (void*)&data);

	waitKey(0);

	destroyWindow("depth");
	destroyWindow("w1");
	destroyWindow("w2");
	destroyWindow("trackbars");
}

void calculatePointCloud(cv::Ptr<cv::StereoSGBM> sgbm, Mat & imgU1, Mat & imgU2)
{
	Mat normalized_depth_map, depth_map;
	sgbm->compute(imgU1, imgU2, depth_map);
	normalize(depth_map, normalized_depth_map, 0, 255, CV_MINMAX, CV_8U);

	imshow("depth_map", normalized_depth_map);
	waitKey(0);
}