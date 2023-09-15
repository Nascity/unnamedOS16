#include <stdio.h>

#include "inc/main.h"

bool check_bootsector(void);
void display_file_info(dir_entry de);

FILE* fp_floppy;


int main(int argc, char* argv[])
{
	dir_entry de;
	int i;

	fp_floppy = fopen(FLOPPY_LOCATION, "r");	
	if(!fp_floppy) return -1;
	if(!check_bootsector()) return -1;

	printf("\r\nbootsector found.\r\n\r\n");
	fseek(fp_floppy, ROOT_DIR_FILE_POS, SEEK_SET);

	for (i = 0; i < 512 / sizeof(dir_entry); i++)
	{
		fread(&de, 1, sizeof(dir_entry), fp_floppy);
		if(!(de.attrib & DIR_ENTRY_ATTRIB_USED)) continue;
		display_file_info(de);
	}
}


bool check_bootsector(void)
{
	word boot_sig;

	fseek(fp_floppy, 510, SEEK_SET);
	fread(&boot_sig, 1, sizeof(word), fp_floppy);

	return (boot_sig & 0xFFFF) == 0xAA55;
}


void display_file_info(dir_entry de)
{
	int i;
	printf("Created at %d/%d/%d | Last opened at %d/%d/%d | Size: %d | ",
			de.creation_date.year,
			de.creation_date.month,
			de.creation_date.day,
			de.opened_date.year,
			de.opened_date.month,
			de.opened_date.day,
			(de.filesize_high << 16) | de.filesize_low);
	for(i = 0; i < 8; i++) printf("%c", de.name[i]);
	if(de.name[0] != '.' && de.ext[0] != 0) printf(".");
	for(i = 0; i < 3; i++) printf("%c", de.ext[i]);
	printf("\r\n");
}
