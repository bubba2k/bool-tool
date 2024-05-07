#ifndef BITFIELD_H_INCLUDED
#define BITFIELD_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

/* A bitfield whose size is static but determined at runtime. */

/* Use array of 32 bit ints as buffer for the bits. */
#define BITFIELD_INT_TYPE uint32_t
#define BITFIELD_INT_SIZE 32u

typedef struct Bitfield
{
    size_t size;
    BITFIELD_INT_TYPE ints[];
} Bitfield;

/* Create a bitfield of @size bits. */
Bitfield *bitfield_create(size_t size);

/* Free a bitfield. */
void bitfield_destroy(Bitfield *bitfield);

/* Basic getters/setters */
/* Returns the value of the bit at index @n. */
int bitfield_get_at(Bitfield *bitfield, size_t n);
/* Sets the bit at index @n to @value (which acts as a bool). */
void bitfield_set_at(Bitfield *bitfield, size_t n, int value);
/* Counts the number of 1s in the bitvector. */
size_t bitfield_sum(Bitfield *bitfield);
/* Print a bitfields value bit for bit. (Little Endian) */
void bitfield_print_le(Bitfield *bitfield);
/* Print a bitfields value bit for bit. (Big Endian) */
void bitfield_print_be(Bitfield *bitfield);

#endif
