#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <fcntl.h>
#include "gpu_temp.h"

using namespace std;

class GetGPUTemp
{
public:
    GetGPUTemp();
//    float getCurrentTemperature(struct bcm2835_peripheral*);
    float getCurrentTemperature(void);
};

float GetGPUTemp::getCurrentTemperature()
//float GetGPUTemp::getCurrentTemperature(struct bcm2835_peripheral *p)
//int GetGPUTemp::map_peripheral(struct bcm2835_peripheral *p)
{
	float temperature;
//	// Map periphreal space
//	// Open /dev/mem
//	if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
//	{
//		printf("Failed to open /dev/mem, try checking permissions.\n");
//		return -1;
//	}
//
//	// Call to mmap function
//	p->map = mmap(
//		NULL,
//		BLOCK_SIZE,
//		PROT_READ|PROT_WRITE,
//		MAP_SHARED,
//		p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
//		p->addr_p       // Address in physical map that we want this memory block to expose
//	);
//
//	// If we can't map return error
//	if (p->map == MAP_FAILED)
//	{
//		perror("mmap");
//		return -1;
//	}
//
//	// Otherwise we have a pointer to the mapped space
//	p->addr = (volatile unsigned int *)p->map;
//
//	//Poke at the map to get the temperature
//	
//	// Unmap the memory space
//	munmap(p->map, BLOCK_SIZE);
//	close(p->mem_fd);
//
	temperature=34.50;
	return temperature;
}

GetGPUTemp::GetGPUTemp() { }

int main (int argc, char** argv)
{
	float temp;
	int high = 60;
	int critical = 70;
	if(argc>=4 || argc == 2){return -1;}
	if(argc==3){
		critical = atoi(argv[2]);
		high = atoi(argv[1]);
	}
	GetGPUTemp Check=GetGPUTemp(); // GetGPUTemp constructor
	temp=Check.getCurrentTemperature(); // Get the GPU temp
//	temp=Check.getCurrentTemperature(gpio); // Get the GPU temp
	cout << "Temp OK - GPU Temperature is "<<std::setprecision(4)<<temp<<" | gputemp="<<std::setprecision(4)<<temp<<";"<<high<<";"<<critical<<endl; }
