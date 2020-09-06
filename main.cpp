#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <signal.h>
#include "uinputdev.h"
#include "iioaccell.h"
#include "argpopt.h"

bool stop = false;

void sigTerm(int dummy) 
{
    stop = true;
}

int main(int argc, char** argv)
{
	UinputDevice dev;
	if(!dev.openDev("/dev/uinput", "AccelerometerJoystick", 0x46d, 0xc214))
	{
		std::cerr<<"Failed to open /dev/uinput: ";
		perror(NULL);
		return -1;
	}
	
	std::string accellPath;
	
	Config config;
	argp_parse(&argp, argc, argv, 0, 0, &config);
	
	if(config.device.empty())
	{
		config.device = Accelerometer::findAccellerometer();
		if(config.device.empty())
		{
			std::cout<<"No accelerometer specifyed and none found\n";
			return 0;
		}
	}
	
	Accelerometer accel;
	
	int startingRate = accel.getRate();
	
	if(!accel.openDevice(config.device))
	{
		std::cerr<<"failed to open iio device "<<config.device<<std::endl;
		return -1;
	}
	
	signal(SIGINT, sigTerm);
	signal(SIGTERM, sigTerm);
	signal(SIGHUP, sigTerm);
	
	if(config.rate > 0)
		accel.setRate(config.rate);
	
	while(!stop)
	{
		Accelerometer::Frame frame = accel.getFrame();
		frame.scale(255/9.81);
		if(abs(frame.x) > 512)
			frame.x = 0;
		if(abs(frame.y) > 512)
			frame.y = 0;
		if(abs(frame.z) > 512)
			frame.z = 0;
		dev.sendAbs(frame.x,frame.y,frame.z);
	}
	
	accel.setRate(startingRate);
	
	return 0;
}
