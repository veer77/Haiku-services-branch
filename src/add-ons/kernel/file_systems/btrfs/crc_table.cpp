//----------------------------------------------------------------------
//  This software is part of the OpenBeOS distribution and is covered
//  by the OpenBeOS license.
//
//  Copyright (c) 2003 Tyler Dauwalder, tyler@dauwalder.net
//---------------------------------------------------------------------

/*! \file crc_table.cpp

        Standalone program to generate the CRC table used for calculating
        UDF tag id CRC values.
       
        This code based off of crc code in UDF-2.50 specs, as permitted.
        See UDF-2.50 6.5 for more information. 

        Reflected version by Jéme Duval
*/

#include <stdio.h>
#include <sys/types.h>

typedef unsigned int uint32 ;

uint32
reflect32 (uint32 b)
{
	uint32 rw = 0;

	for (int i = 0; i < 32; i++){
		if (b & 1)
			rw |= 1 << (31 - i);
		b >>= 1;
	}
	return rw;
}


int
main(int argc, char *argv[]) {
        uint32 crc, poly;

        if (argc != 2) {
                fprintf(stderr, "USAGE: crc_table <octal polynomial=3667067501 for btrfs>\n");
                return 0;
        }

        sscanf(argv[1], "%lo", &poly);
       
        printf("//! CRC 0%o table, as generated by crc_table.cpp\n", poly);
        printf("static uint32 crc_table[256] = { \n");
        for (int n = 0; n < 256; n++) {
                if (n%8 == 0)
                        printf("    ");
                crc = reflect32(n);
                for (int i = 0; i < 8; i++) {
                        if (crc & 0x80000000)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;
                }
		crc = reflect32(crc);
                printf("0x%08x%s ", crc, (n != 255 ? "," : ""));
                if (n%8 == 7)
                        printf("\n");
        }
        printf("};\n");
        return 0;
}
