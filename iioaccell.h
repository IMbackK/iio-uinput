#pragma once
#include <string>
#include <fstream>

class Accelerometer
{
public:
	
	struct Frame
	{
		double x;
		double y;
		double z;
	};
	
private:
	std::fstream fileX;
	std::fstream fileY;
	std::fstream fileZ;
	double xScale;
	double yScale;
	double zScale;
	
	std::string deviceDir;
	int rate_ = -1;
	
	double readFile(const std::string& fileName, bool& status);
	double readFile(std::fstream& file, bool& status);
	
public:
	
	bool openDevice(const std::string& device);
	int rate();
	Frame getFrame();
	~Accelerometer();
};
