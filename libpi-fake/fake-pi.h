#ifndef __FAKE_PI_H__
#define __FAKE_PI_H__

#define __RPI_ASSERT_H__
#include "rpi.h"

#include "demand.h"

#define trace(msg, args...) (fprintf)(stderr, "FAKEPI:%s:" msg, __FUNCTION__, ##args)

void notmain(void);

void fake_gpio_set_bias(int n);
void fake_timer_max_delta(unsigned delta);

#define random bad_random

// not very good random.
unsigned short bad_random(void);

#endif
