/* gpu_temp.cc */

#include "gpu_temp.h"

#define TEMPORARY
#define TROUBLESHOOTING

int main (int argc, char** argv)
{
	// 1. Create some variables
	cout<<"Step 1"<<endl;
	float temperature;
	int high = 60;
	int critical = 70;

	// 2. Deal with argc and argv
	cout<<"Step 2"<<endl;
	if(argc>=4 || argc == 2){return -1;}
	if(argc==3){
		critical = atoi(argv[2]);
		high = atoi(argv[1]);
	}

        // 3. Open handle to /dev/mem
	cout<<"Step 3"<<endl;
        bcm2835_peripheral *block;
        block=new struct bcm2835_peripheral;

        if ((block->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
        {
                printf("Failed to open /dev/mem, try checking permissions.\n");
                return -1;
        }

        // 4. map a pointer to mailbox
	cout<<"Step 4"<<endl;
	volatile struct MailBoxRegisters* MAILBOX = (volatile struct MailBoxRegisters*)mmap(
                NULL,
                BLOCK_SIZE,
                PROT_READ|PROT_WRITE,
                MAP_SHARED,
                block->mem_fd,
                MAILBOX_BASE
        );

	if (MAILBOX == MAP_FAILED)
	{
//		perror("mmap");
		return -1;
	}

        // 5. Use mailbox pointer to do exchange as per baremetal using that pointer
	cout<<"Step 5"<<endl;

	// Enter a function that matches the Tag message
        uint32_t Buffer[7] = { 0 };

	#ifdef TROUBLESHOOTING
	// 5a. Read the status register until the empty flag is set
        
        // 5b. Read data from the read register
        
        // 5c. If lower 4 bits do not match 8 return to 5a
        
        // 5d. If we have channel 8,

	// Temporary way to fill buffer until 5a - 5d works
	Buffer[1] = 1;
        Buffer[2] = 0x30006;
        Buffer[3] = 4;
        Buffer[4] = 8;
        #endif

	#ifdef TEMPORARY
	string ls = GetStdoutFromCommand("/opt/vc/bin/vcgencmd measure_temp");
	#endif

        if (mailbox_tag_message(&Buffer[0], 5, TEMPERATURE_TAG, REQUEST_LENGTH, RESPONSE_LENGTH, TEMPERATURE_ID, 0))
        {
		// If you get in here the mailbox exchange was successful
		// buffer[4] has the return value of whatever you asked

		#ifdef TROUBLESHOOTING
		// The only value that matters right now is the temp. If I find that I need others, I will put them in her until the function works.
		Buffer[4] = { 0x0000c014 }; // 49.172 Celsius
		Buffer[4] = { 0x0000C803 }; // 51.173 Celsius
		#endif

		temperature = (float)Buffer[4]/1000;
	}

        // 6. unmap pointer
	cout<<"Step 6"<<endl;
        int rc=munmap(&MAILBOX, BLOCK_SIZE);

        // 7. close handle
	cout<<"Step 7"<<endl;
        close(block->mem_fd);

	//#ifndef TEMPORARY - FOR NOW OUTPUT THEM BOTH FOR COMPARISON
	// Output the values in the Nagios format
	cout << "Temp OK - GPU Temperature is "<<std::setprecision(4)<<temperature<< " | gputemp="<<std::setprecision(4)<<temperature<<";"<<dec<<high<<";"<<critical<<";0;"<<endl; 
	//#endif

	// Using the vcgencmd measure_temp and the fuction GetStdoutFromCommand to collect and format data until correct function works
	cout << "Temp OK - GPU Temperature is "<<std::setprecision(4)<<ls<< " | gputemp="<<setprecision(4)<<ls<<";"<<dec<<high<<";"<<critical<<";0;"<<endl; 

}

bool mailbox_tag_message (uint32_t* response_buf,       // Pointer to response buffer
                             uint8_t data_count,        // Number of uint32_t variadic data following
                             ...)                       // Variadic uint32_t values for call
{
	/******************************************************************************************/
	/* VARIADIC TAG */
	// This section creates a buffer that can be written to and read from with the tag message in it 
	uint32_t __attribute__((aligned(16))) message[data_count + 3];  // Must be 16 byte aligned
	va_list list;
	va_start(list, data_count);                     // Start variadic argument
	message[0] = (data_count + 3) * 4;              // Size of message needed
	message[data_count + 2] = 0;                    // Set end pointer to zero
	message[1] = 0;                                 // Zero response message
	for (int i = 0; i < data_count; i++)
	{
		message[2 + i] = va_arg(list, uint32_t);// Fetch next variadic
	}
	va_end(list);                                   // variadic cleanup
	/* END OF VARIADIC TAG */

	#ifdef TEMPORARY
	message[1] = SUCCESS;
	#endif

	// Check success flag
	if (message[1] == SUCCESS )
	{
		// If buffer NULL used then don't want response
		if (response_buf)
		{
			for (int i = 0; i < data_count; i++)
			response_buf[i] = message[2 + i];       // Transfer out each response message
	}
		return true;    // message success
	}
	return false;      // Message failed
}

#ifdef TEMPORARY
string GetStdoutFromCommand(string cmd)
{
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");
	if (stream)
	{
		while (!feof(stream))
		if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		pclose(stream);
	}

	data.replace(0,5,"");	
	data.replace(4,5,"");	

	return data;
}

#endif
