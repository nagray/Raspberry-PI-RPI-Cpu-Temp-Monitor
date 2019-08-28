#include <fstream>
#include <string>
#include <iostream>
#include "reg.h"
#include "postman.h"

#ifndef uint32
#define uint32 uint32_t
#endif

#ifndef byte
#define byte int8_t
#endif

#define READ_OFFSET 0x00
#define POLL_OFFSET 0x10
#define SENDER_OFFSET 0x14
#define STATUS_OFFSET 0x18
#define CONFIG_OFFSET 0x1c
#define WRITE_OFFSET 0x20

// This bit is set in the status register if there is no space to write into the mailbox
#define MAIL_FULL 0x80000000

// This bit is set in the status register if there is nothing to read from the mailbox
#define MAIL_EMPTY 0x40000000

// Base address for the mailbox register
#define MAIL_BASE 0xB880

using namespace std;

class GetGPUTemp
{
public:
	uint32 ReadMailbox(byte channel);
};

uint32 GetGPUTemp::ReadMailbox(byte channel)
{
	channel &= 0xf;
	uint32 data;
	readMemMappedReg<uint32>(MAIL_BASE, READ_OFFSET);

	// Loop until we receive something from the requested channel
//	for (;;)
//	{
//		while (ReadMemMappedReg<uint32>(MAIL_BASE, STATUS_OFFSET) & MAIL_EMPTY) != 0)
//		{
//			// Wait for data
//		}
//		// Read the data
//		data = readMemMappedReg<uint32>(MAIL_BASE, READ_OFFSET);
//		byte readChannel = data & 0xF;
		data >>= 4;
//		// Return it straight away if it's for the requested channel
//		if (readChannel == channel)
		return data;
//	}
}


int main (void)
{
	byte data;
	uint32 temp;
	GetGPUTemp Temp=GetGPUTemp();
	temp=Temp.ReadMailbox(data);
//	cout << "data: "<<data<<endl;
//	cout << "temp: "<<temp<<endl;
}

