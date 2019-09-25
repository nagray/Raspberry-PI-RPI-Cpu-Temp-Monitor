// test.h

#include <bcm_host.h>
#include <sys/mman.h> //mmap()
#include <unistd.h> //close()
#include <fcntl.h> //open
#include <assert.h> // assert()
#include <iomanip> //printf()
#include <stdarg.h> //argc, argv
#include <iostream>

#define BLOCK_SIZE	0x1000
#define MAIL_EMPTY 	0x40000000
#define SUCCESS         0x80000000
#define TEMPERATURE_ID  0
#define TEMPERATURE_TAG 0x00030006
#define REQUEST_LENGTH 4
#define RESPONSE_LENGTH 8


using namespace std;

struct mailbox_t {
	uint32_t Unused0[2176];	// To shift access to start of registers at 0x880
	uint32_t Read0;	// 0x00         Read data from VC to ARM
	uint32_t Unused1[3];	// 0x04-0x0F
	uint32_t Peek0;		// 0x10
	uint32_t Sender0;	// 0x14
  	uint32_t Status0;	// 0x18         Status of VC to ARM
	uint32_t Config0;	// 0x1C
	uint32_t Write1;	// 0x20         Write data from ARM to VC
	uint32_t Unused2[3];	// 0x24-0x2F
	uint32_t Peek1;		// 0x30
	uint32_t Sender1;	// 0x34
	uint32_t Status1;	// 0x38         Status of ARM to VC
	uint32_t Config1;	// 0x3C
};

mailbox_t* map_mailbox();
uint32_t read_mailbox(mailbox_t mailbox, uint32_t channel);

