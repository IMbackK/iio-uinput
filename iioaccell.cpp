#include <filesystem>
#include <iostream>

#include "iioaccell.h"

bool Accelerometer::openDevice(const std::string& device)
{
	deviceDir = device;
	
	std::fstream statusFile;
	
	bool status = false;
	
	fileX.open(deviceDir + "/in_accel_x_raw");
	if(!fileX.is_open())
	{
		std::cerr<<"in_accel_x_raw is required but not availble\n";
		return false;
	}
	
	fileY.open(deviceDir + "/in_accel_y_raw");
	if(!fileX.is_open())
	{
		std::cerr<<"in_accel_y_raw is required but not availble\n";
		return false;
	}
	
	fileZ.open(deviceDir + "/in_accel_z_raw");
	if(!fileZ.is_open())
	{
		std::cerr<<"in_accel_z_raw is required but not availble\n";
		return false;
	}
	
	xScale = readFile(deviceDir + "/in_accel_x_scale", status);
	if(!status) 
	{
		xScale = 1;
		std::cerr<<"Device dosent have in_accel_x_scale assuming 1\n";
	}
	
	yScale = readFile(deviceDir + "/in_accel_y_scale", status);
	if(!status) 
	{
		yScale = 1;
		std::cerr<<"Device dosent have in_accel_y_scale assuming 1\n";
	}
	
	zScale = readFile(deviceDir + "/in_accel_z_scale", status);
	if(!status) 
	{
		std::cerr<<"Device dosent have in_accel_z_scale assuming 1\n";
		zScale = 1;
	}
	return true;
}

Accelerometer::Frame Accelerometer::getFrame()
{
	Frame frame;
	bool status;
	frame.x = readFile(fileX, status)*xScale;
	frame.y = readFile(fileY, status)*yScale;
	frame.z = readFile(fileZ, status)*zScale;
	return frame;
}

double Accelerometer::readFile(const std::string& fileName, bool& status)
{
	std::fstream file;
	file.open(fileName);
	if(!file.is_open())
	{
		status = false;
		return 0;
	}
	double result = readFile(file, status);
	file.close();
	return result;
}

double Accelerometer::readFile(std::fstream& file, bool& status)
{
	double result = 0;
	file.seekg(std::ios_base::beg);
	file >> result;
	status = !file.rdstate();
	return result;
}

Accelerometer::~Accelerometer()
{
	fileX.close();
	fileY.close();
	fileZ.close();
}


