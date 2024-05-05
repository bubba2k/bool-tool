#include "bitfield.h"

/* Create a bitfield of @size bits. */
Bitfield *bitfield_create(size_t size)
{
    /* Calculate how many ints are needed to store the amount
     * of bits requested. */
    unsigned int num_ints = (size / BITFIELD_INT_SIZE) + 1;

    /* Then allocate that many ints. */
    Bitfield *bitfield = malloc(sizeof(Bitfield) 
                         + (num_ints * sizeof(BITFIELD_INT_TYPE)));

    return bitfield;
}

/* Free a bitfield. */
void bitfield_destroy(Bitfield *bitfield)
{
    free(bitfield);
}

/* Returns the value (bool) of the bit at index @n. */
int bitfield_get_at(Bitfield *bitfield, size_t n)
{
	/* Get the indices. */
	unsigned int_idx = n / BITFIELD_INT_SIZE;
	unsigned bit_idx = n % BITFIELD_INT_SIZE;

	/* Construct the mask and the get the int to be masked 
     * to retrieve the bit of interest. */
	BITFIELD_INT_TYPE mask 	= ((BITFIELD_INT_TYPE) 0x1) << bit_idx;

	return mask & bitfield->ints[int_idx];
}

/* Sets the bit at index @n to @value (which acts as a bool). */
void bitfield_set_at(Bitfield *bitfield, size_t n, int value)
{
	/* Get the indices. */
	unsigned int_idx = n / BITFIELD_INT_SIZE;
	unsigned bit_idx = n % BITFIELD_INT_SIZE;

    BITFIELD_INT_TYPE mask = 0;
	/* Get the mask and modify the int that contains the bit
     * of interest. */
    if(value) { /* Set bit to 1. */
        mask = 0x1 << bit_idx;
        bitfield->ints[int_idx] |= mask;
    }
    else  {     /* Set bit to 0. */
        mask = 0x1 << bit_idx;
        bitfield->ints[int_idx] &= ~mask;
    }
}
