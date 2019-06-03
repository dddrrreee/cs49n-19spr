// need to pass context around since we can have multiple neopixel strings.
struct neo_handle;
typedef struct neo_handle *neo_t;

neo_t neopix_init(uint8_t in_pin, uint32_t in_npixel);

// addressable write.  last write wins.  assert fail if out of bounds.
void neopix_write(neo_t h, uint32_t pos, uint8_t r, uint8_t g, uint8_t b);
void neopix_flush_writes(neo_t h);

// support stuff.
void neopix_fast_clear(neo_t h, int upto);
void neopix_clear(neo_t h);

void neopixel_check_timings(neo_t h);

// they probably shouldn't be using these -- are the raw versions.
void neopix_sendpixel(neo_t h, uint8_t r, uint8_t g, uint8_t b);

