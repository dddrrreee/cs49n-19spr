// pi-specific implementations of common routines.
#ifndef __RPI_STDLIB_H__
#define __RPI_STDLIB_H__

// uint32 etc.
#include "rpi.h"

long trunc(double d);

#define aligned(ptr, n)  ((unsigned)ptr % n == 0)
#define aligned4(ptr)  aligned(ptr,4)

#define zero(x) memset(x, 0, sizeof *x)

// type pun a uint32 into a type T (hopefully) without breaking gcc aliasing
// assumptions.
#define u32_to_T(_T, _val) ({                                           \
        AssertNow(sizeof(_T) == 4);                                     \
        AssertNow(sizeof(_val) == 4);                                   \
        union _u { _T v; unsigned u; } u = (union _u){ .u = _val };     \
        u.v;                                                            \
})


// type pun a type T into a uint32 (hopefully) without breaking gcc aliasing
// assumptions.
#define T_to_u32(_T, _val) ({                                           \
        AssertNow(sizeof(_T) == 4);                                     \
        AssertNow(sizeof(_val) == 4);                                   \
        union _u { _T v; unsigned u; } u = (union _u){ .v = _val };     \
        u.u;                                                            \
})


// ugh.  gcc struct packing is dangerous, so byte structs will actually
// be 4 bytes b/c of alignment.    it's up to the user to make sure that
// the byte struct is the first byte of struct!
#define u8_to_T(_T, _val) ({                                           	\
	_T _to;								\
	memset(&_to, 0, sizeof _to); \
	uint8_t _from = _val;						\
	memcpy(&_to, &_from, 1);					\
        _to;                                                           	\
})

#define T_to_u8(_T, _val) ({                                           	\
	uint8_t _to;							\
	_T _from = _val;						\
	memcpy(&_to, &_from, 1);					\
        _to;                                                           	\
})

/*
 * put32_T/get32_T force storing/loading of a 32-bit structure using a 
 * a single 32-bit store instruction.
 * We do it this way b/c it's unclear if storing a 32-bit hw register a 
 * byte at a time gives ok values and since we cannot guarantee compiler 
 * doesn't do this, we force single-store/load behavior by calling asm.
 *
 * Our general device strategy:
 *	- if register is a single field, even < 32, just write the unsigned
 *	directly b/c that will also write 0s.
 *	- if register has multiple subfields and reserved bits, implement
 *	"clear" routines that read the hw value, clear the reserved 
 *	bits and return the result.
 */

// do a put32 of a structure of type(_s)
#define put32_T(_s, _v) put32(&_s, T_to_u32(typeof(_s), _v))

// do a get32 into a structure of type(*_x)
#define get32_T(_x) u32_to_T(typeof(*_x), get32(_x))


// #define check_byte_off(s, field, n) AssertNow(offsetof(s,field) == n)

// check bitfield positions.
#define check_bit_pos(_struct, field, n) do {                           \
        union _u {                                                      \
                _struct s;                                              \
                unsigned u;                                             \
        } x = (union _u) { .s = (_struct){ .field = 1 } };              \
        unsigned exp = 1<<n;                                            \
        if(x.u != exp)                                                  \
                panic("expected %x got %x for %s\n", exp,x.u,#field);   \
        assert(x.u == exp);                                             \
} while(0)

#endif
