#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <linux/uinput.h>
#include <linux/input.h>

#include "uinputdev.h"

UinputDevice::UinputDevice()
{
}

bool UinputDevice::openDev(const std::string& uinputPath, const std::string& name, uint16_t vendor, uint16_t product)
{
	fd = open(uinputPath.c_str(), O_RDWR);
	if(fd < 0) return false;
	
	struct uinput_setup dev = {0};
	if (name.length() <= UINPUT_MAX_NAME_SIZE) strcpy(dev.name, name.c_str());
	else strcpy(dev.name, name.substr(0, UINPUT_MAX_NAME_SIZE).c_str());
	dev.id.bustype = BUS_VIRTUAL;
	dev.id.vendor = vendor;
	dev.id.product = product;
	
	ioctl(fd, UI_SET_EVBIT, EV_ABS);
	ioctl(fd, UI_SET_ABSBIT, ABS_X);
	ioctl(fd, UI_SET_ABSBIT, ABS_Y);
	ioctl(fd, UI_SET_ABSBIT, ABS_Z);
	
	struct uinput_abs_setup devAbsX = {0};
	devAbsX.code = ABS_X;
	devAbsX.absinfo.minimum = -512;
	devAbsX.absinfo.maximum = 512;
	devAbsX.absinfo.resolution = 1;
	if(ioctl(fd, UI_ABS_SETUP, &devAbsX) < 0) return false;

	struct uinput_abs_setup devAbsY = {0};
	devAbsY.code = ABS_Y;
	devAbsY.absinfo.minimum = -512;
	devAbsY.absinfo.maximum = 512;
	devAbsY.absinfo.resolution = 1;
	if(ioctl(fd, UI_ABS_SETUP, &devAbsY) < 0) return false;
	
	struct uinput_abs_setup devAbsZ = {0};
	devAbsZ.code = ABS_Z;
	devAbsZ.absinfo.minimum = -512;
	devAbsZ.absinfo.maximum = 512;
	devAbsZ.absinfo.resolution = 1;
	if(ioctl(fd, UI_ABS_SETUP, &devAbsZ) < 0) return false;
	
		
	if(ioctl(fd, UI_DEV_SETUP, &dev) < 0) return false;
	if(ioctl(fd, UI_DEV_CREATE) < 0) return false;
	
	return true;
}

bool UinputDevice::sendAbs(uint x, uint y, uint z)
{
	if(fd < 0) return false;
	
	struct input_event ev = {0};
	
	ev.type = EV_ABS;
	ev.code = ABS_X;
	ev.value = x;
	if(write(fd, &ev, sizeof(ev)) != sizeof(ev)) 
		return false;

	ev.code = ABS_Y;
	ev.value = y;
	if(write(fd, &ev, sizeof(ev)) != sizeof(ev)) 
		return false;

	ev.code = ABS_Z;
	ev.value = z;
	if(write(fd, &ev, sizeof(ev)) != sizeof(ev)) 
		return false;

	gettimeofday(&ev.time, NULL);
	ev.type = EV_SYN;
	ev.code = SYN_REPORT;
	ev.value = 0;
	if(write(fd, &ev, sizeof(ev)) != sizeof(ev)) 
		return false;
	
	return true;
}

UinputDevice::~UinputDevice()
{
	if(fd > 0) 
	{
		ioctl(fd, UI_DEV_DESTROY);
		close(fd);
	}
}

