#pragma once
#include <stdint.h>
#include <string>

class UinputDevice
{
private:
	int fd = -1;
	
public:
	UinputDevice();
	~UinputDevice();
	bool openDev(const std::string& uinputPath, const std::string& name, uint16_t vendor, uint16_t product);
	bool sendAbs(uint x, uint y, uint z);
};
