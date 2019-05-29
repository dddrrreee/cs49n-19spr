// this is super gross.  can't be compiled by iself since it needs inlined
// functions, so we just include the whole thing at the end of the file.

// DO NOT DELETE *inline*.
// you *HAVE* to inline, otherwise its a function pointer call.
// duh.
static inline void 
time_check(const char *fn, neo_t h, void (*fp)(unsigned), unsigned exp) { 
	unsigned pin = h->pin;

	unsigned s = read_cycle_cnt();
	unsigned e = read_cycle_cnt();
	unsigned overhead = e - s;
	assert(overhead==8);

	dev_barrier();
	s = read_cycle_cnt();
		fp(pin);
	e = read_cycle_cnt();
	dev_barrier();
	unsigned t = e - s - 8;

	// +/- 10 cycles seems right?
	// const unsigned tol = 10;
	// wtf, why so high?
	const unsigned tol = 20;
	printk("%s: expected %u cycles, got %u cycles\n", fn, exp, t);
	assert(t <= (exp + tol) && t >= (exp - tol));
}

// fucking gcc bug work around 
static void checkt0l(unsigned);
static void checkt0h(unsigned);
static void checkt1h(unsigned);
static void checkt1l(unsigned);
static void checktreset(unsigned); 

#define get_raw_time() *(volatile unsigned *)0x20003004
static void time_usec(neo_t h) {

#define run(str, f,exp,pin) do {			\
	unsigned b = get_raw_time();		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
	f(pin); f(pin); f(pin); f(pin); f(pin);		\
						\
	unsigned e = get_raw_time();		\
	printk("%s: 100 check: expected %u ns, got %u ns\n", str, exp, (e-b) * 1000/50);	\
  } while(0)

#if 0
	T0H = to_cycles(350),	// Width of a 0 bit in ns
	T1H = to_cycles(900),	// Width of a 1 bit in ns
	T0L = to_cycles(900),	// Width of a 0 bit in ns
	T1L = to_cycles(350), 	// Width of a 1 bit in ns
	FLUSH = to_cycles(50 *1000); // Width of the low gap between bits to cause a frame to latch
#endif


	
	checkt0h(h->pin);
	checkt0l(h->pin);

	checkt1h(h->pin);
	checkt1l(h->pin);
	checktreset(h->pin); 
}

static void checkt0h(unsigned pin) {
	run("t0h", t0h,350,pin);
}

static void checkt0l(unsigned pin) {
	run("t0l", t0l,900,pin);
}

static void checkt1h(unsigned pin) {
	run("t1h", t1h,900,pin);
}

static void checkt1l(unsigned pin) {
	run("t1l", t1l,350,pin);
}
static void checktreset(unsigned pin) {
	run("treset", treset, 50000,pin);
}

void neopix_check_timings(neo_t h) {
	unsigned pin = h->pin;
	unsigned s = read_cycle_cnt();
	unsigned e = read_cycle_cnt();
	unsigned overhead = e - s;
	printk("overhead = %u\n", overhead);

	run("gpio_off_raw", gpio_set_off_raw, 22, pin);
	run("gpio_on_raw", gpio_set_on_raw, 22, pin);

	s = read_cycle_cnt();
		delay_ms(1000);
	e = read_cycle_cnt();
	unsigned t= (e - s) / (1000 * 1000);
	printk("1sec should take about 700M cycle: %u, %u\n", e - s,t);
	// should check a tighter tol (say 50k?)
	assert(t >= MHz-1 && t <= MHz +1);

#if 0
	s = read_cycle_cnt();
		gpio_set_off(pin);
	e = read_cycle_cnt();
	printk("gpio off takes %d\n", e - s);

	s = read_cycle_cnt();
		gpio_set_on(pin);
	e = read_cycle_cnt();
	printk("gpio on takes %d\n", e - s);
#endif

	asm volatile (".align 4");
	time_check("t1h", h, t1h, T1H);
	time_check("t0h", h, t0h, T0H);
	time_check("t1l", h, t1l, T1L);
	time_check("t0l", h, t0l, T0L);
	time_check("treset", h, treset, FLUSH);

	time_usec(h) ;
}
