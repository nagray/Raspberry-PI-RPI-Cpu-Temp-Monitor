#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class GetCPUTemp
{
public:
    GetCPUTemp();
    double getCurrentTemperature();
};

double GetCPUTemp::getCurrentTemperature()
{
    string val;
    string preparedTemp;
    double temperature;

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
	double temp;
	GetCPUTemp Check=GetCPUTemp();

	temp=Check.getCurrentTemperature();
	cout << "Temp OK - CPU Temperature is "<<temp<<"°C"<<endl;
}
