#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

class GetCPUTemp
{
public:
    GetCPUTemp();
    float getCurrentTemperature();
};

//double GetCPUTemp::getCurrentTemperature()
float GetCPUTemp::getCurrentTemperature()
{
    string val;
    string preparedTemp;
    float temperature;

    ifstream temperatureFile ("/sys/class/thermal/thermal_zone0/temp");

    if (temperatureFile.fail()){
	// cout for testing only. return value of -1 should prompt error
	// message.
	cout << "Failed to open temperatureFile!\n";
        return -1;
    }

    // The temperature is stored in 5 digits.  The first two are degrees
    // in C. The rest are decimal precision.
	temperatureFile >> val;
	temperatureFile.close();
	preparedTemp = val.insert(2, 1, '.');
	temperature = std::stod(preparedTemp);
	return temperature;
}

GetCPUTemp::GetCPUTemp() { }

int main (void)
{
	float temp;
	float temp2;
	float val1;
	float val2;
	float val3;

	GetCPUTemp Check=GetCPUTemp();

	temp=Check.getCurrentTemperature();
	cout << "Temp OK - CPU Temperature is "<<std::setprecision(4)<<temp<<" | cputemp="<<std::setprecision(4)<<temp<<";60;70"<<endl;
}
