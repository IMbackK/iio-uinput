#pragma once
#include <string>
#include <fstream>

#define IIO_DIRECTORY "/sys/bus/iio/devices/"

class Accelerometer
{
public:
	
	class Frame
	{
	public:
		double x;
		double y;
		double z;
		inline void scale(double scale)
		{
			x = x*scale;
			y = y*scale;
			z = z*scale;
		}
	};
	
private:
	std::fstream fileX;
	std::fstream fileY;
	std::fstream fileZ;
	double xScale;
	double yScale;
	double zScale;
	
	std::string deviceDir;
	
	double readFile(const std::string& fileName, bool& status);
	double readFile(std::fstream& file, bool& status);
	
public:
	
	static std::string findAccellerometer();
	bool openDevice(const std::string& device);
	bool setRate(unsigned int rate);
	int getRate();
	Frame getFrame();
	~Accelerometer();
};
