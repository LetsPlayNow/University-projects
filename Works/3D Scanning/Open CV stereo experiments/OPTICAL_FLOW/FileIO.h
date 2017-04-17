#pragma once
#include "Libraries.h"
#include "Calibration.h"

void save_3d_to_file(const char * filename, const Mat & mat, const char * mode, int scale);
void read_stereo_calibration_from_file(OpticalFlowMatrices & of_matr);
void write_stereo_calibration_to_file(OpticalFlowMatrices & of_matr);
void read_camera_calibration_from_file(CameraParams & cp);
void write_camera_calibration_to_file(CameraParams & cp);

//Сохраняет облако точек в файл
//IN
//const char * filename: Имя файла для сохранения
//const Mat &:  Сохраняемая матрица (облако точек)
void save_3d_to_file(const char * filename, const Mat & mat, const char * mode, int scale = 1)
{
	FILE * file = fopen(filename, mode);
	for (int i = 0; i < mat.cols; i++)
	{
		float x = mat.at<float>(0, i) * scale;
		float y = mat.at<float>(1, i) * scale;
		float z = mat.at<float>(2, i) * scale;

		fprintf(file, "v %.5f %.5f %.5f\n", x, y, z);
	}
	fclose(file);
}


//ЗаписатьOpticalFlowMatrices в файл
//OUT: 
//OpticalFlowMatrices: структура, содержащая параметры калибровки
void read_stereo_calibration_from_file(OpticalFlowMatrices & of_matr)
{
	puts("[LOG]READ_CALIB_DATA_FROM_FILE = START");
	FileStorage fs1("stereo.yml", FileStorage::READ);
	fs1["CM1"] >> of_matr.CM1;
	fs1["CM2"] >> of_matr.CM2;
	fs1["D1"] >> of_matr.D1;
	fs1["D2"] >> of_matr.D2;
	fs1["P1"] >> of_matr.P1;
	fs1["P2"] >> of_matr.P2;
	fs1.release();
	puts("[LOG]READ_CALIB_DATA_FROM_FILE = END");
}

//Записать OpticalFlowMatrices в файл
//IN: 
//OpticalFlowMatrices: структура, содержащая параметры калибровки
void write_stereo_calibration_to_file(OpticalFlowMatrices & of_matr)
{
	puts("[LOG]WRITE_CALIB_DATA_TO_FILE = START");
	FileStorage fs1("stereo.yml", FileStorage::WRITE);
	fs1 << "CM1" << of_matr.CM1;
	fs1 << "CM2" << of_matr.CM2;
	fs1 << "D1" << of_matr.D1;
	fs1 << "D2" << of_matr.D2;
	fs1 << "P1" << of_matr.P1;
	fs1 << "P2" << of_matr.P2;
	fs1.release();
	puts("[LOG]WRITE_CALIB_DATA_TO_FILE = END");
}

void read_camera_calibration_from_file(CameraParams & cp)
{
	puts("[LOG]READ_CALIB_DATA_FROM_FILE = START");
	FileStorage fs1("single.yml", FileStorage::READ);
	fs1["CM"] >> cp.CM;
	fs1["D"] >> cp.D;
	fs1.release();
	puts("[LOG]READ_CALIB_DATA_FROM_FILE = END");
}

//Записать матрицы CM и D в файл
//IN: 
//Camera_Params: структура для CM и D
void write_camera_calibration_to_file(CameraParams & cp)
{
	puts("[LOG]WRITE_CALIB_DATA_TO_FILE = START");
	FileStorage fs1("single.yml", FileStorage::WRITE);
	fs1 << "CM" << cp.CM;
	fs1 << "D" << cp.D;
	fs1.release();
	puts("[LOG]WRITE_CALIB_DATA_TO_FILE = END");
}