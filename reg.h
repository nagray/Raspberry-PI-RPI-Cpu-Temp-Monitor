#ifndef __REG_H__
#define __REG_H__

#include <stdint.h>

#define MAPPED_REGISTERS_BASE 0x20000000

template<typename T>
static T readMemMappedReg(size_t base, size_t offset) {
    return *reinterpret_cast<volatile T*>(MAPPED_REGISTERS_BASE + base + offset);
}

template<typename T>
static void writeMemMappedReg(size_t base, size_t offset, T data) {
    *reinterpret_cast<T *>(MAPPED_REGISTERS_BASE + base + offset) = data;
}

#endif
