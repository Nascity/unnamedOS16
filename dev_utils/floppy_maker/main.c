#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

void read_and_write(FILE* fp_read, char* filename);
void read_and_write_bootsector(FILE* fp_bootsector);
void write_to_FAT(int cluster_ptr, int value);
void write_to_floppy(int sector_count, void* buffer);
void error_handler(char* msg);

byte sector[512];
word FAT[256];
size_t cluster_ptr;
FILE* fp_floppy;


int main(int argc, char* argv[])
{
	int i;
	FILE* fp_read;

	if(argc == 1) error_handler("this program needs arguments.");

	cluster_ptr = 3;
	FAT[3] = END_OF_CLUSTER;

	fp_floppy = fopen(FLOPPY_LOCATION, "w");
	if(!fp_floppy) error_handler("cannot open floppy.img.");

	for(i = 1; i < argc; i++)
	{
		fp_read = fopen(argv[i], "r");
		printf("-----------------------\r\n");
		if(!fp_read) error_handler("file not found.");
		printf("%-15s\t%s.\r\n", FILE_OPEN, argv[i]);

		if(i == 1) read_and_write_bootsector(fp_read);
		else read_and_write(fp_read, argv[i]);

		fclose(fp_read);
		printf("%-15s\t%s\r\n", FILE_CLOSE, argv[i]);
	}

	printf("-----------------------\r\n");
	write_to_floppy(2, FAT);
	printf("-----------------------\r\n");

	fclose(fp_floppy);
	return 0;
}


void read_and_write(FILE* fp_read, char* filename)
{
	size_t count;

	while((count = fread(sector, 1, 512, fp_read)) == 512)
	{
		printf("%-15s\t%ld bytes read from %s\r\n", FILE_READ, count, filename);
		
		cluster_ptr++;
		write_to_FAT(cluster_ptr, cluster_ptr + 1);
		printf("%-15s\t%#.4lx written at FAT index %ld.\r\n", FAT_RECORD, cluster_ptr + 1, cluster_ptr);

		write_to_floppy(cluster_ptr, sector);
	 	memset(sector, 0, 512);
	}

	if(count > 0) 
	{
		printf("%-15s\t%ld bytes read from %s.\r\n", FILE_READ, count, filename);
		printf("%-15s\tEnd of %s reached.\r\n", FILE_READ, filename);

		cluster_ptr++;
		write_to_FAT(cluster_ptr, END_OF_CLUSTER);
		printf("%-15s\t%#.4x written at FAT index %ld.\r\n", FAT_RECORD, END_OF_CLUSTER, cluster_ptr);

		write_to_floppy(cluster_ptr, sector);
	}
	else
	{
		printf("%-15s\tEnd of %s reached.\r\n", FILE_READ, filename);
		write_to_FAT(cluster_ptr, END_OF_CLUSTER);
		printf("%-15s\toverwritten FAT index %ld with %#.4x.\r\n", FAT_RECORD, cluster_ptr, END_OF_CLUSTER);
	}
}


void read_and_write_bootsector(FILE* fp_bootsector)
{
	size_t count = fread(sector, 1, 512, fp_bootsector);

	printf("%-15s\t%ld bytes read from bootloader.bin\r\n", FILE_READ, count);
	write_to_floppy(1, sector);
}


void write_to_FAT(int cluster_ptr, int value)
{
	char* ptr = (char*)(&FAT[cluster_ptr]);

	*ptr = (value >> 8) & 0xFF;
	*(ptr + 1) = value & 0xFF;
}


void write_to_floppy(int start_sector, void* buffer)
{
	size_t count;

	fseek(fp_floppy, (start_sector - 1) * 512, SEEK_SET);
	count = fwrite(buffer, 1, 512, fp_floppy);
	if(count != 512) error_handler("failed to write data to floppy.img");
	printf("%-15s\t%ld bytes written to floppy.img at sector %d.\r\n", FLOPPY_WRITE, count, start_sector);
}


void error_handler(char* msg)
{
	printf("%-15s\t%s\r\n", ERROR, msg);
	exit(-1);
}
