/* temp_gpu_temp.cc */

#include "gpu_temp.h"

#define TEMPORARY
#define TROUBLESHOOTING

int main (int argc, char** argv)
{

	// 1. Create some variables
        float temperature;
        int high = 60;
        int critical = 70;

        // 2. Deal with argc and argv
        if(argc>=4 || argc == 2){return -1;}
        if(argc==3){
	critical = atoi(argv[2]);
	high = atoi(argv[1]);
        }

	// 3. Open handle to /dev/mem
	bcm2835_peripheral *block;
	block=new struct bcm2835_peripheral;

        if ((block->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
        {
                printf("Failed to open /dev/mem, try checking permissions.\n");
                return -1;
        }

        // 4. map a pointer to mailbox
	volatile void* mmappedData = mmap(
                NULL,
		BLOCK_SIZE,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
                block->mem_fd,
                MAILBOX_BASE);

	if (mmappedData == MAP_FAILED)
	assert(mmappedData != MAP_FAILED);

	cout<<"What I mapped: 0x"<<hex<<MAILBOX_BASE<<endl;
	cout<<"Where I put the map: "<<hex<<&mmappedData<<endl;
	cout<<"Mailbox: 0x"<<hex<<(int)&mmappedData + 0x0880<<endl;
	int* mailbox;
	&mailbox = (int)&mmappedData + (int)0x0880;


	// 5. get GPU Temperature via accessing mailboxes

	#ifdef TROUBLESHOOTING
	//      while(( MAILBOX->Status0 & MAIL_EMPTY != 0)){}


	// 5a. Read the status register until the empty flag is set
	//
	// 5b. Read data from the read register
	//
	// 5c. If lower 4 bits do not match 8 return to 5a
	//
	// 5d. If we have channel 8,

        #endif

	// Enter a function that matches the Tag message
	uint32_t Buffer[8] = { 0 };
	
	// Temporary way to fill buffer until 5a - 5d works
	#ifdef TEMPORARY

	Buffer[1] = 1;
	Buffer[2] = 0x30006;
	Buffer[3] = 4;
	Buffer[4] = 8;

	cout<<endl;
	cout<<"Prior to entry into mailbox_tag_message()"<<endl;
	cout<<"Buffer[0]: 0x"<<hex<<Buffer[0]<<endl;
	cout<<"Buffer[1]: 0x"<<hex<<Buffer[1]<<endl;
	cout<<"Buffer[2]: 0x"<<hex<<Buffer[2]<<endl;
	cout<<"Buffer[3]: 0x"<<hex<<Buffer[3]<<endl;
	cout<<"Buffer[4]: 0x"<<hex<<Buffer[4]<<endl;
	cout<<"Buffer[5]: 0x"<<hex<<Buffer[5]<<endl;
	cout<<"Buffer[6]: 0x"<<hex<<Buffer[6]<<endl;
	cout<<"Buffer[7]: 0x"<<hex<<Buffer[7]<<endl;
	cout<<endl;

        #endif

	if (mailbox_tag_message(Buffer, 5, TEMPERATURE_TAG, REQUEST_LENGTH, RESPONSE_LENGTH, TEMPERATURE_ID, 0))
	//if (mailbox_tag_message(&Buffer[0], 5, TEMPERATURE_TAG, REQUEST_LENGTH, RESPONSE_LENGTH, TEMPERATURE_ID, 0))
	
	Buffer[4] = 0x0000C014; //49.172C
	
	// This is what Buffer has in it after the return of mailbox_tag_message()
	cout<<"After return from mailbox_tag_message()"<<endl;
	cout<<"Buffer[0]: 0x"<<hex<<Buffer[0]<<endl;
	cout<<"Buffer[1]: 0x"<<hex<<Buffer[1]<<endl;
	cout<<"Buffer[2]: 0x"<<hex<<Buffer[2]<<endl;
	cout<<"Buffer[3]: 0x"<<hex<<Buffer[3]<<endl;
	cout<<"Buffer[4]: 0x"<<hex<<Buffer[4]<<endl;
	cout<<"Buffer[5]: 0x"<<hex<<Buffer[5]<<endl;
	cout<<"Buffer[6]: 0x"<<hex<<Buffer[6]<<endl;
	cout<<"Buffer[7]: 0x"<<hex<<Buffer[7]<<endl;
	cout<<endl;

	// Temporary way to fill Buffer until mailbox_tag_message works
	#ifdef TEMPORARY
	Buffer[1] = 1;
	Buffer[2] = 0x30006;
	Buffer[3] = 4;
	Buffer[4] = 8;
        #endif

	// 6. unmap pointer
	int rc=munmap(&mmappedData, BLOCK_SIZE);
	assert(mmappedData != MAP_FAILED);

	// 7. close handle
	close(block->mem_fd);

	//8. Print out data in the format required for NAGIOS

}

bool mailbox_tag_message (uint32_t* response_buf,	// Pointer to response buffer
				uint8_t data_count,	// Number of uint32_t variadic data following
				...)			// Variadic uint32_t values for call
{
        /******************************************************************************************/
        /* VARIADIC TAG */
        // This section creates an 8 deep FIFO buffer (message) of 32 bit words that can be written to and read from (a mailbox).
        uint32_t __attribute__((aligned(16))) message[data_count + 3];  // Must be 16 byte aligned
        va_list list;
        va_start(list, data_count);                     // Start variadic argument
        message[0] = (data_count + 3) * 4;              // Size of message needed
        cout<<"message[0]: "<<message[0]<<endl;
        message[data_count + 2] = 0;                    // Set end pointer to zero
        cout<<"message["<<data_count + 2<<"]: "<<message[data_count + 2]<<endl;
        message[1] = 0;                                 // Zero response message
        cout<<"message[1]: "<<message[1]<<endl;
        for (int i = 0; i < data_count; i++)
        {
                message[2 + i] = va_arg(list, uint32_t);// Fetch next variadic
                cout<<"message["<<2 + i<<"]: 0x"<<hex<<message[2 + i]<<endl;
        }
        va_end(list);                                   // variadic cleanup
        /* END OF VARIADIC TAG */

	// There may be missing code here
        message[1] = SUCCESS;                                 

        // Check success flag
        if (message[1] == SUCCESS )
        {
		cout<<"message[1] is 0x"<<SUCCESS<<endl;
                // If buffer NULL used then don't want response
                if (response_buf)
                {
                        for (int i = 0; i < data_count; i++)
                        response_buf[i] = message[2 + i];       // Transfer out each response message
        }
		cout<<endl;
		cout<<"message success"<<endl;
                return true;    // message success
        }
	cout<<"message failed"<<endl;
	cout<<endl;
        return false;      // Message failed

}
