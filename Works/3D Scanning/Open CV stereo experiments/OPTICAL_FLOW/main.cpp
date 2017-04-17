#define _CRT_SECURE_NO_WARNINGS
#include "OpricalFlowAlg.h"
#include "FileIO.h"
#include "Aiming.h"
#include "Calibration.h"

int main(int argc, char * argv[])
{
	setlocale(LC_ALL, "Russian");
	OpticalFlowMatrices of_matr;
	CameraParams cp;
	char command;
	int number_of_cams;
	puts("Проверка камер");
	presets_aiming();

	printf("Нужна ли калибровка? (y//n): ");
	scanf("%c", &command);
	bool calibration_needed = command == 'y';

	printf("Введите число камер (1//2): ");
	scanf("%d", &number_of_cams);

	if (number_of_cams == 1)
	{
		if (calibration_needed)
		{
			//Проводим калибровку и записываем результаты в файл
			int board_width, board_height;
			puts("Введите размеры шахматной доски (число углов)");
			printf("Ширина: "); scanf("%d", &board_width);
			printf("Высота: "); scanf("%d", &board_height);

			Size board_sz(board_width, board_height);
			cp = camera_calibration(board_sz);

			write_camera_calibration_to_file(cp);
		}
		else
		{ //Читаем калибровку из файла
			read_camera_calibration_from_file(cp);
		}

		//Алгоритм оптического потока
		//Делаем фото с камер
		VideoCapture cap(0);

		Mat img0, img1;
		single_camera_aiming(cap);
		cap.read(img0);
		printf("Передвиньте камеру и нажмите любую клавишу");
		single_camera_aiming(cap);
		cap.read(img1);

		//Находим оптический поток и строим облако точек
		Mat point_cloud = single_sfm(img0, img1, cp.CM, cp.D);

		//Сохраняем результаты в файл
		save_3d_to_file("[1]point_cloud.obj", point_cloud, "w");
	}

	else if (number_of_cams == 2)
	{
		if (calibration_needed)
		{
			//Проводим калибровку и записываем результаты в файл
			int board_width, board_height;
			puts("Введите размеры шахматной доски (число углов)");
			printf("Ширина: "); scanf("%d", &board_width);
			printf("Высота: "); scanf("%d", &board_height);

			Size board_sz(board_width, board_height);
			of_matr = stereo_calibration(board_sz);

			write_stereo_calibration_to_file(of_matr);
		}
		else
		{ //Читаем калибровку из файла
			read_stereo_calibration_from_file(of_matr);
		}

		//Алгоритм оптического потока
		//Делаем фото с камер
		VideoCapture cap0(0);
		VideoCapture cap1(1);

		Mat img0, img1;
		aiming(cap0, cap1);
		cap0.read(img0);
		cap1.read(img1);


		//Находим оптический поток и строим облако точек
		Mat point_cloud = stereo_sfm(img0, img1, of_matr);

		//Сохраняем результаты в файл
		save_3d_to_file("[2]point_cloud.obj", point_cloud, "w");
	}


	return 0;
}