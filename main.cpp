#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdlib.h>
#include "uinputdev.h"
#include "iioaccell.h"

#define IIO_DIRECTORY "/sys/bus/iio/devices/"

int main(int argc, char** argv)
{
	UinputDevice dev;
	if(!dev.openDev("/dev/uinput", "AcceleroeterJoystick", 0x15A5, 0x68D6))
		return -1;
	
	if(argc != 2)
	{
		std::cout<<"usage: iio-uinput [IIO_DEVICE]\n";
		return 0;
	}
	
	Accelerometer accel;
	
	if(!accel.openDevice(std::string(IIO_DIRECTORY) + argv[1]))
	{
		std::cerr<<"failed to open iio device "<<IIO_DIRECTORY<<argv[1]<<std::endl;
		return -1;
	}
	
	while(true)
	{
		Accelerometer::Frame frame = accel.getFrame();
		frame.scale(255/9.81);
		std::cout<<"frame "<<frame.x<<' '<<frame.y<<' '<<frame.z<<' '<<'\n';
		if(abs(frame.x) > 512)
			frame.x = 0;
		if(abs(frame.y) > 512)
			frame.y = 0;
		if(abs(frame.z) > 512)
			frame.z = 0;
		dev.sendAbs(frame.x,frame.y,frame.z);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
	return 0;
}
