/* test.cc */

#include "test.h"

#define TESTING

// Map protected registers in memory space.
mailbox_t * map_mailbox()
{
	void *mailbox;
	int fd;
	uint32_t peripheral_base;
	uint32_t mailbox_base;

	//build mailbox base
	peripheral_base = bcm_host_get_peripheral_address();
	mailbox_base = peripheral_base + 0xB000;

	// open file descriptor
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0)
	{
		printf("can't open /dev/mem \n");
		exit(-1);
	}

	// mmap protected space
	mailbox = mmap(
	NULL,
	BLOCK_SIZE,
	PROT_READ | PROT_WRITE,
	MAP_SHARED,
	fd,
	mailbox_base);
	assert(mailbox != MAP_FAILED);

	// Close file descriptor
	close(fd);

	// Return virtual memory space pointer
	return(mailbox_t*)mailbox;
}

uint32_t read_mailbox(mailbox_t* mailbox, uint32_t channel)
{
	uint32_t value;
	while (1)
	{
		while ((mailbox->Status0 & MAIL_EMPTY) != 0){}
                value = mailbox->Read0;
		uint32_t readChannel = value & 0xF;
		value >>= 4;
		if (readChannel == channel)
                        return value;
	}
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

        #ifdef TESTING
        message[1] = SUCCESS;
        message[4] = 0xB810;
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


// main()
int main ()
{

cout<<"Step 1.1"<<endl;
	uint32_t temperature;
	// Map protected registers in memory space.
	mailbox_t* mailbox = map_mailbox();
	int channel = 8;

#ifdef TESTING
	
cout<<"Step 1.2"<<endl;
        mailbox->Status0 = 0xBFFFFF10;
        mailbox->Read0 = 0xB8100008; // Channel 8 and 47120 (0xB810)
	cout << "mailbox->Status0 = 0x"<<hex<<mailbox->Status0<<endl;
        cout << "mailbox->Read0 = 0x"<<hex<<mailbox->Read0<<endl;
	cout << "MAIL_EMPTY = 0x"<<hex<<MAIL_EMPTY<<endl;

#endif

	uint32_t value = read_mailbox(mailbox, channel);
cout<<"Step 1.3"<<endl;
cout<<"value: 0x"<<value<<endl;
	uint32_t Buffer[5] = { 0 };
	Buffer[0]=value;
cout<<"Buffer[0]: 0x"<<Buffer[0]<<endl;
	if (mailbox_tag_message(&Buffer[0], 5, 0x00030006, 4, channel, TEMPERATURE_ID, 0))
	{
		// If you get in here the mailbox exchange was successful
		// buffer[8] has the return value of whatever you aske
		temperature = Buffer[8];
	}

	cout << "Temperature: 0x"<<hex<<temperature<<endl;

}
