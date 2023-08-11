#include "inc/inc.h"

void format(char letter, int* ap);
bool string_n_compare(char* str1, char* str2, int n);

int open_file(int owner, int file_cluster, int open_mode);
bool close_file(int FIT_index);
bool write_file(int working_directory, int FIT_index, void* buffer, int offset, int count);
bool read_file(int working_directory, int FIT_index, void* buffer, int offset, int count);

void write_file_byte(int write_ds, int offset, int value);
byte read_file_byte(int read_ds, int offset);

int get_file_FIT_index(int file_cluster);
int get_file_cluster(int FIT_index);

void get_dir_entry(int working_directory, int index, dir_entry* pde);
void set_dir_entry(int working_directory, int index, dir_entry de);

void calculate_chs_tuple(int cluster, int* cl, int* dh);
bool load_cluster(int file_cluster, int FIT_index);
bool save_cluster(int file_cluster, int FIT_index);

bool register_FIT(int owner_pid, int start_cluster, int open_mode, int* registered_index);
bool unregister_FIT(int FIT_index);

int find_file_cluster(char name[FILE_MAX_NAME], char ext[FILE_MAX_EXT], int working_directory_cluster);
int navigate_cluster_chain(int start, int count);
int allocate_new_cluster(int start_cluster);

bool get_file_size(int working_directory_FIT_index, int FIT_index, int* filesize);
bool set_file_size(int working_directory_FIT_index, int FIT_index, int filesize);

int open_serial_port(int owner, int port_num, int open_mode);

file_interaction_table_entry FIT[MAX_FIT_ENTRY];

// -----------------------------------------------------------------------
// Initialization
bool io_init(void)
{
	int unused;
	int i;

	for(i = 0;
		i < MAX_FIT_ENTRY * sizeof(file_interaction_table_entry);
		i++)
		((byte*)FIT)[i] = 0;

	if(!register_FIT(0x100, FAT_CLUSTER,
		FIT_FLAGS_READ | FIT_FLAGS_WRITE, &unused)) return false;
	if(!load_cluster(FAT_CLUSTER, unused)) return false;
	if(!register_FIT(0x100, ROOT_CLUSTER,
		FIT_FLAGS_READ | FIT_FLAGS_WRITE, &unused)) return false;
	if(!load_cluster(ROOT_CLUSTER, unused)) return false;

	return true;
}


// -----------------------------------------------------------------------
// Interaction
void printchar(int ch)
{
#asm
	push	bp
	mov	bp, sp
	push	ax
	push	bx

	mov	ah, #0x0e
	mov	al, byte ptr [bp + 4]
	xor	bx, bx
	int	0x10

	pop	bx
	pop	ax
	pop	bp
#endasm
}


void printhex(int num, bool uppercase)
{
	char str[8];
	int i;
	int temp;

	//  0   1   2   3   4   5   6   7
	// '0' 'x' '0' '0' '0' '0'  0   0
	for (i = 0; i < 8; i++) str[i] = '0';
	str[1] = 'x';
	str[6] = 0;	

	for (i = 5; i >= 2; i--)
	{
		temp = (num % 16);
		if (temp < 10) str[i] = temp + '0';
		else str[i] = uppercase ? temp - 10 + 'A' : temp - 10 + 'a';
		num /= 16;

		if (num == 0) break;
	}

	printstring(str);
}


void printnumber(int num)
{
	bool is_negative = num < 0 ? true : false;
	char str[8];
	int i;

	str[7] = 0;

	if(num == 0)
	{
		str[6] = '0';
		i = 6;
	}
	else
	{
		if(is_negative) num *= -1;

		for (i = 6; i >= 0; i--)
		{
			str[i] = (num % 10) + '0';
			num /= 10;
	
			if (num == 0) break;
		}

		if(is_negative) str[--i] = '-';
	}
	printstring(&str[i]);
}


void printstring(char* str, ...)
{
	char* ap = (char*)&str + 2;

	while (*str)
	{
		if (*str != '%')
		{
			printchar(*(str++));
			continue;
		}

		// if the letter is %
		// check for next letter *(++str)
		format(*(++str), (int*)ap);
		ap += 2;
		str++;
	}
}

void printline(char* str, ...)
{
	char* ap = (char*)&str + 2;

	while (*str)
	{
		if (*str != '%')
		{
			printchar(*(str++));
			continue;
		}

		// if the letter is %
		// check for next letter *(++str)
		format(*(++str), (int*)ap);
		ap += 2;
		str++;
	}

	printchar('\r');
	printchar('\n');
}


kobj_io io_open(int cs, int flags, kobj_io working_dir, char* name, char* ext, int open_mode)
{
	int ret;
	char* serial0 = "serial0";
	char* serial1 = "serial1";
	int start_cluster;

	if(string_n_compare(serial0, name, 8))
		return open_serial_port(cs, 0, open_mode);
	if(string_n_compare(serial1, name, 3))
		return open_serial_port(cs, 1, open_mode);

	start_cluster = find_file_cluster(name, ext, working_dir);
	ret = open_file(cs, start_cluster, open_mode);
	asm("mov	ax, word ptr [bp - 6]");
	syscall_return();
}


bool io_close(int cs, int flags, kobj_io koio)
{

}


bool io_write(int cs, int flags, kobj_io koio, void* buffer, int offset, int count)
{

}


bool io_read(int cs, int flags, kobj_io koio, void* buffer, int offset, int count)
{

}


// -----------------------------------------------------------------------
// Internal
void format(char letter, int* ap)
{
	switch (letter)
	{
		case 'c':
			printchar(*ap);
			break;
		case 'd':
			printnumber(*ap);
			break;
		case 's':
			printstring(*ap);
			break;
		case 'X':
			printhex(*ap, true);
			break;
		case 'x':
			printhex(*ap, false);
			break;
		default:
			printchar(' ');
			break;
	}
}


bool string_n_compare(char* str1, char* str2, int n)
{
	int i = 0;
	bool ret = true;

	if(!str1[0] || !str2[0]) return false;

	while(str1[i] && str2[i] && i < n)
	{
		if(str1[i] != str2[i])
		{
			ret = false;
			break;
		}
		i++;
	}

	return ret;
}


int open_file(int owner, int file_cluster, int open_mode)
{
	int FIT_index;
	
	if((FIT_index = get_file_FIT_index(file_cluster))
		!= UNREGISTERED_FIT_ENTRY)
		return FIT_index;
	if(!register_FIT(owner, file_cluster, open_mode, &FIT_index))
		return UNREGISTERED_FIT_ENTRY;
	if(!load_cluster(file_cluster, FIT_index))
		return UNREGISTERED_FIT_ENTRY;
	return FIT_index;
}


bool write_file(int working_directory, int FIT_index, void* buffer, int offset, int count)
{
	int i;
	int j = offset;
	int loaded_cluster = navigate_cluster_chain(
				FIT[FIT_index].start_cluster,
				FIT[FIT_index].file_pointer / 512);
	int new_cluster;
	int filesize;

	if(!(FIT[FIT_index].flags & FIT_FLAGS_WRITE))
		return false;
	if(!get_file_size(working_directory, FIT_index, &filesize))
		return false;
	if(offset > filesize)
		return false;
	if(offset + count > filesize)
	{
		filesize = offset + count;
		if(!set_file_size(working_directory, FIT_index, filesize))
			return false;
	}

	for(i = 0; i < count; i++)
	{
		if(j / 512)
		{
			new_cluster = navigate_cluster_chain(
				FIT[FIT_index].start_cluster,
				FIT[FIT_index].file_pointer / 512);
			if(new_cluster == INVALID_CLUSTER)
			{
				new_cluster = allocate_new_cluster(FIT[FIT_index].start_cluster);
				if(new_cluster == 0) return false;
			}
			if(!save_cluster(loaded_cluster, FIT_index))
				return false;
			if(!load_cluster(new_cluster, FIT_index))
				return false;
			loaded_cluster = new_cluster;
			j = j % 512;
		}

		write_file_byte(0x3000 + FIT_index * 0x20, j, ((byte*)buffer)[i]);
		j++;
	}

	return save_cluster(loaded_cluster, FIT_index);
}


bool read_file(int working_directory, int FIT_index, void* buffer, int offset, int count)
{
	int i;
	int j = offset;
	int loaded_cluster = navigate_cluster_chain(
				FIT[FIT_index].start_cluster,
				FIT[FIT_index].file_pointer / 512);
	int new_cluster;
	int filesize;

	if(!(FIT[FIT_index].flags & FIT_FLAGS_READ))
		return false;
	if(!get_file_size(working_directory, FIT_index, &filesize))
		return false;
	if(offset + count > filesize)
	{
		count -= offset + count - filesize;
		if(count <= 0) return false;
	}

	for(i = 0; i < count; i++)
	{
		if(j / 512)
		{
			new_cluster = navigate_cluster_chain(
				FIT[FIT_index].start_cluster,
				FIT[FIT_index].file_pointer / 512);
			if(new_cluster == INVALID_CLUSTER)
				return false;
			if(!load_cluster(new_cluster, FIT_index))
				return false;
			loaded_cluster = new_cluster;
			j = j % 512;
		}

		((byte*)buffer)[i] =
			read_file_byte(0x3000 + FIT_index * 0x20, j);
		j++;
	}

	return true;
}


void write_file_byte(int write_ds, int offset, int value)
{
	int ds = write_ds;
	int off = offset;
	int val = value;
#asm
	mov	ax, ds
	push	ax
	
	mov	ax, word ptr [bp - 6]
	mov	bx, word ptr [bp - 8]
	mov	cx, word ptr [bp - 10]
	mov	ds, ax
	mov	byte ptr [bx], cl

	pop	ax
	mov	ds, ax
#endasm
}


byte read_file_byte(int read_ds, int offset)
{
	int ds = read_ds;
	int off = offset;
#asm
	mov	ax, ds
	push	ax
	
	mov	ax, word ptr [bp - 6]
	mov	bx, word ptr [bp - 8]
	mov	ds, ax
	mov	di, word ptr [bx]

	pop	ax
	mov	ds, ax

	mov	ax, di
#endasm
}


int get_file_FIT_index(int file_cluster)
{
	int i;

	for(i = 0; i < MAX_FIT_ENTRY; i++)
		if((FIT[i].flags & FIT_FLAGS_USED_ENTRY)
			&& FIT[i].start_cluster == file_cluster)
			return i;

	return UNREGISTERED_FIT_ENTRY;
}


int get_file_cluster(int FIT_index)
{
	return FIT[FIT_index].start_cluster;
}


void get_dir_entry(int working_directory, int index, dir_entry* pde)
{
	int i;

	for(i = 0; i < sizeof(dir_entry); i++)
		((byte*)pde)[i] = read_file_byte(
			0x3000 + working_directory * 0x20,
			index * sizeof(dir_entry) + i);
}


void set_dir_entry(int working_directory, int index, dir_entry de)
{
	int i;

	for(i = 0; i < sizeof(dir_entry); i++)
		write_file_byte(
			0x3000 + working_directory * 0x20,
			index * sizeof(dir_entry) + i,
			((byte*)&de)[i]);
}


void calculate_chs_tuple(int cluster, int* cl, int* dh)
{
	*dh = cluster / SECTORS_PER_HEAD;
	*cl = cluster % SECTORS_PER_HEAD;
}


bool load_cluster(int file_cluster, int FIT_index)
{
	int bx = FIT_index * 0x200;
	int cl;
	int dh;
	int ret;

	calculate_chs_tuple(file_cluster, &cl, &dh);
#asm
	mov	ax, #0x3000
	mov	es, ax

	mov	ax, #0x0201
	mov	bx, word ptr [bp - 6]
	xor	ch, ch
	mov	cl, byte ptr [bp - 8]
	xor	dl, dl
	mov	dh, byte ptr [bp - 10]
	int	0x13

	jc	load_cluster_carry	; carry == 1 when error
	xor	ax, ax
	inc	ax
	jmp	load_cluster_end
load_cluster_carry:
	xor	ax, ax
load_cluster_end:
	mov	word ptr [bp - 12], ax
#endasm
	return ret;
}


bool save_cluster(int file_cluster, int FIT_index)
{
	int bx = FIT_index * 512;
	int cl;
	int dh;
	int ret;

	calculate_chs_tuple(file_cluster, &cl, &dh);
#asm
	mov	ax, #0x3000
	mov	es, ax

	mov	ax, #0x0301
	mov	bx, word ptr [bp - 6]
	xor	ch, ch
	mov	cl, byte ptr [bp - 8]
	xor	dl, dl
	mov	dh, byte ptr [bp - 10]
	int	0x13

	jc	save_cluster_carry	; carry == 1 when error
	xor	ax, ax
	inc	ax
	jmp	save_cluster_end
save_cluster_carry:
	xor	ax, ax
save_cluster_end:
	mov	word ptr [bp - 12], ax
#endasm
	return ret;
}


bool register_FIT(int owner_pid, int start_cluster, int open_mode, int* registered_index)
{
	file_interaction_table_entry fite;
	bool empty_entry_found = false;
	int i;

	fite.owner_pid = owner_pid;
	fite.start_cluster = start_cluster;
	fite.flags = open_mode | FIT_FLAGS_USED_ENTRY;
	fite.file_pointer = 0;

	for(i = 0; i < MAX_FIT_ENTRY; i++)
	{
		if(!(FIT[i].flags & FIT_FLAGS_USED_ENTRY))
		{
			empty_entry_found = true;
			break;
		}
	}

	if(!empty_entry_found) return false;

	FIT[i] = fite;
	*registered_index = i;
	return true;
}


int find_file_cluster(char name[FILE_MAX_NAME], char ext[FILE_MAX_EXT], int working_directory)
{
	dir_entry de;
	int i;

	for(i = 0; i < 512 / sizeof(de); i++)
	{
		get_dir_entry(working_directory, i, &de);

		if(string_n_compare(de.name, name, FILE_MAX_NAME)
			&& string_n_compare(de.ext, ext, FILE_MAX_EXT))
			return de.start_cluster;
	}

	return INVALID_CLUSTER;
}


int navigate_cluster_chain(int start, int count)
{
	word FAT_entry = 0;
	word current = start;
	int i;

	for(i = 0; i < count; i++)
	{
		if(current == INVALID_CLUSTER) return INVALID_CLUSTER;

		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * current);
		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * current + 1) << 8;
		current = FAT_entry;
		FAT_entry = 0;
	}

	return current;
}


int allocate_new_cluster(int start_cluster)
{
	word max = INVALID_CLUSTER;
	word FAT_entry;
	word current;
	int i;

	for(i = 3; i < 512 / sizeof(word); i++)
	{
		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * i);
		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * i + 1) << 8;
		
		if(FAT_entry == 0xFFFF) max = i;
		if(FAT_entry == 0x0000) break;

		FAT_entry = 0;
	}

	current = start_cluster;

	while(true)
	{
		FAT_entry = 0;
		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * current);
		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * current + 1) << 8;
		if(FAT_entry == EOC || FAT_entry == 0) break;
		current = FAT_entry;
	}

	write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
			2 * current,
			max + 1);
	write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
			2 * current + 1,
			(max + 1) >> 8);
	write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
			2 * (max + 1),
			0xFF);
	write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
			2 * (max + 1) + 1,
			0xFF);

	return max + 1;
}


bool get_file_size(int working_directory_FIT_index, int FIT_index, int* filesize)
{
	dir_entry de;
	int i;

	for(i = 0; i < 512 / sizeof(de); i++)
	{
		get_dir_entry(working_directory_FIT_index, i, &de);

		if(de.start_cluster == FIT[FIT_index].start_cluster)
		{
			*filesize = de.filesize;
			return true;
		}
	}

	return false;
}


bool set_file_size(int working_directory_FIT_index, int FIT_index, int filesize)
{
	dir_entry de;
	int i;

	for(i = 0; i < 512 / sizeof(de); i++)
	{
		get_dir_entry(working_directory_FIT_index, i, &de);

		if(de.start_cluster == FIT[FIT_index].start_cluster)
		{
			de.filesize = filesize;

			set_dir_entry(working_directory_FIT_index, i, de);

			return save_cluster(
					get_file_cluster(working_directory_FIT_index),
					working_directory_FIT_index);
		}
	}

	return false;
}

int open_serial_port(int owner, int port_num, int open_mode)
{

}
