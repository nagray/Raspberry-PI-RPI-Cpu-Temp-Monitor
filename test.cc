/* test.cc */

#include "test.h"

// Map protected registers in memory space.
mailbox_t * get_mailbox()
{
	void *mailbox;
	int fd;

	if ((fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0)
	{
		printf("can't open /dev/mem \n");
		exit(-1);
	}

	mailbox = mmap(
	NULL,
	BLOCK_SIZE,
	PROT_READ | PROT_WRITE,
	MAP_SHARED,
	fd,
	MAILBOX_BASE);

	assert(mmappedData != MAP_FAILED);

	cout<<"got here (3)"<<endl;
	cout<<"What I mapped: 0x"<<hex<<MAILBOX_BASE<<endl;
        cout<<"Where I put the map: "<<hex<<mailbox<<endl;
        mailbox = (char*)mailbox + 0x880;
	cout<<"This is where mailbox should start: "<<mailbox<<endl;

	
	// Close file descriptor
	close(fd);

	// Return virtual memory space pointer
	return(mailbox_t*)mailbox;
}

// main()
int main ()
{
	volatile mailbox_t* mailbox = get_mailbox();
	while (1)
	{
		while ((mailbox->Status0 & MAIL_EMPTY) != 0){}
		volatile uint32_t value = mailbox->Read0;
		exit(0);
	}
	return 0;
}
