/**
Copyright 2013 3DSGuy

This file is part of make_banner.

make_banner is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

make_banner is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with make_banner.  If not, see <http://www.gnu.org/licenses/>.
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bnr.h"

int bnr_main(BNR_CONTEXT bnr)
{
	
	//Calculate padding
	DWORD padding = bnr.cbmd_header.file_size;
	while(padding > 0x10)
		padding -= 0x10;
	padding = 0x10 - padding;
	if(bnr.verbose_bool == TRUE){
		printf("[+] Adding 0x%x bytes of padding to CBMD component\n",padding);
		printf("[+] CWAV file is ");
		if(bnr.cwav_header.endianness != 0xFEFF)
			printf("Big Endian\n");
		else
			printf("Little Endian\n");
	}
		
	
	if(bnr.cwav_header.endianness != 0xFEFF){//Endianness check and recalculation if nesscicary
		u8 tmp[4];
		fseek(bnr.cwav, 0xc, SEEK_SET);
		fread(tmp, 0x4, 1, bnr.cwav);
		bnr.cwav_header.file_size = size_u8toDWORD(tmp);
	}
	
	
	//Calculate BNR filesize
	DWORD bnr_size = bnr.cbmd_header.file_size + padding + bnr.cwav_header.file_size;
	DWORD new_cbmd_size = bnr.cbmd_header.file_size + padding;
	
	if(bnr.verbose_bool == TRUE)
		printf("[+] Input CBMD file size: 0x%x\n[+] Input CWAV file size: 0x%x\n[+] Output BNR file size: 0x%x\n",bnr.cbmd_header.file_size,bnr.cwav_header.file_size,bnr_size); 

	//Fill-out Output file
	fseek(bnr.output, (bnr_size - 0x1) , SEEK_SET);
    	fputc(0x00, bnr.output);
	
	
	//Temporarally Storing Input Files
	u8 *cbmd_buff = malloc(bnr.cbmd_header.file_size);
	u8 *cwav_buff = malloc(bnr.cwav_header.file_size);
	memset(cbmd_buff, 0x0, bnr.cbmd_header.file_size);
	memset(cwav_buff, 0x0, bnr.cwav_header.file_size);
	fseek(bnr.cbmd, 0x0, SEEK_SET);
	fseek(bnr.cwav, 0x0, SEEK_SET);
	if(fread(cbmd_buff, bnr.cbmd_header.file_size, 1, bnr.cbmd) == 0){
		printf("[!] Reading the 'CBMD' file Failed\n");
		return READ_FAIL;
	}
	if(fread(cwav_buff, bnr.cwav_header.file_size, 1, bnr.cwav) == 0){
		printf("[!] Reading the 'CWAV' file Failed\n");
		return READ_FAIL;
	}

	//Writing CBMD to output
	fseek(bnr.output, 0x0, SEEK_SET);
	if(fwrite(cbmd_buff, bnr.cbmd_header.file_size, 1, bnr.output) == 0){
		printf("[!] Failed To Write to Output\n");
		return WRITE_FAIL;
	}
	//Writing New CBMD size(to incl padding)
	fseek(bnr.output, SIZE_OFFSET, SEEK_SET);
	if(fwrite(&new_cbmd_size, sizeof(DWORD), 1, bnr.output) == 0){
		printf("[!] Failed To Write to Output\n");
		return WRITE_FAIL;
	}
	
	//Writing CWAV to output
	fseek(bnr.output, (bnr.cbmd_header.file_size + padding), SEEK_SET);
	if(fwrite(cwav_buff, bnr.cwav_header.file_size, 1, bnr.output) == 0){
		printf("[!] Failed To Write to Output\n");
		return WRITE_FAIL;
	}

	//Freeing malloc buffers
	free(cbmd_buff);
	free(cwav_buff);
	return 0;
}

DWORD size_u8toDWORD(u8 *size)
{
    return (size[3]) | (size[2] << 8) | (size[1] << 16) | (size[0] << 24);
}