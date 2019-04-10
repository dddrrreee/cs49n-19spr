#ifndef __RPI_H__
#define __RPI_H__

// these are in start.s
void put32(volatile void *addr, unsigned v);
unsigned get32(volatile void *addr);

#endif
