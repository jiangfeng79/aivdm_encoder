/* bits.c - bitfield extraction code
 * 
 * This file is Copyright (c)2010 by the GPSD project
 * BSD terms apply: see the file COPYING in the distribution root for details.
 *
 * Bitfield extraction functions.  In each, start is a bit index (not
 * a byte index) and width is a bit width.  The width bounded above by
 * the bit width of a long long, which is 64 bits in all standard data
 * models for 32- and 64-bit processors.
 *
 * The sbits() function assumes twos-complement arithmetic.
 */
#include <assert.h>
#include <string.h>

#include "bits.h"
#ifdef DEBUG
#include <stdio.h>
#include "gpsd.h"
#endif /* DEBUG */

#define BITS_PER_BYTE	8

unsigned long long ubits(char buf[], unsigned int start, unsigned int width)
/* extract a (zero-origin) bitfield from the buffer as an unsigned big-endian long long */
{
	unsigned long long fld = 0;
	unsigned int i;
	unsigned end;

	/*@i1@*/ assert(width <= sizeof(long long) * BITS_PER_BYTE);
	for (i = start / BITS_PER_BYTE;
			i < (start + width + BITS_PER_BYTE - 1) / BITS_PER_BYTE; i++) {
		fld <<= BITS_PER_BYTE;
		fld |= (unsigned char)buf[i];
	}

	end = (start + width) % BITS_PER_BYTE;
	if (end != 0) {
		fld >>= (BITS_PER_BYTE - end);
	}

	/*@ -shiftimplementation @*/
	fld &= ~(-1LL << width);
	/*@ +shiftimplementation @*/

	return fld;
}

signed long long sbits(char buf[], unsigned int start, unsigned int width)
/* extract a bitfield from the buffer as a signed big-endian long */
{
	unsigned long long fld = ubits(buf, start, width);

	if (fld & (1 << (width - 1))) {
		/*@ -shiftimplementation @*/
		fld |= (-1LL << (width - 1));
		/*@ +shiftimplementation @*/
	}
	return (signed long long)fld;
}

// clear subdue signed bits
void setbyte(char * dest, int off, char * src){
	char tmp = *dest;
	unsigned char clearChar = 0xFF >> (off%BITS_PER_BYTE);
	tmp |= clearChar;	
	*dest |= *src; 
	*dest &= tmp;
}
int get6bitcode(char c)
{
	int rt=-1;
	const char sixchr[64] =
		"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^- !\"#$%&`()*+,-./0123456789:;<=>?";
	for(rt=0;rt<64;rt++)
	{
		if(sixchr[rt] == c)
			break;
	}
	return rt;

}

void put6bitschars(char *buf, unsigned int start, unsigned int length, char * str)
{
	int i;
	int l_len = strlen(str);
	if(l_len>length)
	{
		//		printf("put6bitschars(): buffer over flow!\n");
		return;
	}
	for(i=0;i<l_len;++i)	
		putbits(buf, start+i*6, 6, (long long)get6bitcode(str[i]));	
	for(i=l_len;i<length;++i)
		putbits(buf, start+i*6, 6, (long long)get6bitcode(' '));
	// or
	//putbits(buf, start+i*6, 6, (long long)get6bitcode('@'));
}

void putbits(char *buf, unsigned int start, unsigned int width, long long value)
/* extract a (zero-origin) bitfield from the buffer as an unsigned big-endian long long */
{
	int offset;
	char * cPtr;
	int startByte;
	int i;
	int startI;

	offset= BITS_PER_BYTE - 1 - (start + width - 1)%BITS_PER_BYTE ;

	value = value << offset;

	cPtr = (char *) &value;

	startByte = start/BITS_PER_BYTE;

	startI = (width+offset-1)/BITS_PER_BYTE;

	//big endian, so put lsb first with ptr
	for(i=startI;i>=0;i--)
	{
		if(i!=0)
			buf[startByte+i] = *(cPtr++);
		else
			setbyte(&buf[startByte], start, cPtr);
	}	
}
