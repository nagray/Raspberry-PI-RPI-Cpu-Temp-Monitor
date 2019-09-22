/* gpu_temp.h */

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <assert.h>
#include <iomanip>
#include <stdlib.h>
#include <stdarg.h>

//static volatile uint32_t *gpio;

#define BCM2708_PERI_BASE_1       0x20000000    // Peripheral Base for RPi1
#define BCM2708_PERI_BASE_2       0x3F000000    // Peripheral Base for RPi2
#define BCM2708_PERI_BASE_3       0x3F000000    // Peripheral Base for RPi3
#define BCM2708_PERI_BASE_4       0xFE000000    // Peripheral Base for RPi4
#define BCM2708_PERI_BASE BCM2708_PERI_BASE_4   // We are compiling for RPi4
#define BLOCK_SIZE   (0x1000)
#define MAILBOX_BASE 0xFE000000 + 0x200000 + 0xB000

#define GPIO_BASE 0x200000
#define TEMPERATURE_ID  0
#define TEMPERATURE_TAG 0x00030006
#define REQUEST_LENGTH 4
#define RESPONSE_LENGTH 8
#define MAIL_BASE 0xB000
#define MAIL_ADJUST 0x880
#define MAIL_EMPTY      0x40000000              /* Mailbox Status Register: Mailbox Empty */
#define MAIL_FULL       0x80000000              /* Mailbox Status Register: Mailbox Full  */
#define SUCCESS         0x80000000
#define MAILBOX ((volatile __attribute__((aligned(4))) struct MailBoxRegisters*)(uintptr_t)(MAILBOX_BASE))

using namespace std;

bool mailbox_tag_message (uint32_t* response_buf, uint8_t data_count, ...);
string GetStdoutFromCommand(string cmd);
float hex_to_float (uint32_t);

struct bcm2835_peripheral
{
    unsigned long addr_p;
    int mem_fd;
    volatile struct MailBoxRegisters* map;
};

struct MailBoxRegisters
{
        const uint32_t Read0;   // 0x00         Read data from VC to ARM
        uint32_t Unused[3];     // 0x04-0x0F
        uint32_t Peek0;         // 0x10
        uint32_t Sender0;       // 0x14
        uint32_t Status0;       // 0x18         Status of VC to ARM
        uint32_t Config0;       // 0x1C
        uint32_t Write1;        // 0x20         Write data from ARM to VC
        uint32_t Unused2[3];    // 0x24-0x2F
        uint32_t Peek1;         // 0x30
        uint32_t Sender1;       // 0x34
        uint32_t Status1;       // 0x38         Status of ARM to VC
        uint32_t Config1;       // 0x3C
};
