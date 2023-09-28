#include "inc/inc.h"

void format(char letter, int* ap);
bool string_n_compare(char* str1, char* str2, int n);
int string_n_size_compare(char* str1, char* str2, int n);

int open_file(int owner, int working_dir, int file_cluster, int open_mode);
bool write_file(int working_directory, int FIT_index, void* buffer, int offset, int count);
bool read_file(int working_directory, int FIT_index, void* buffer, int offset, int count);

bool create_directory(int working_dir, word file_cluster, word parent);
bool delete_directory(int working_dir, word file_cluster);

void write_file_byte(int write_ds, int offset, int value);
byte read_file_byte(int read_ds, int offset);

int can_create_file(int working_dir);
void sort_dir_entry(int working_dir);

int get_file_FIT_index(int file_cluster);
int get_file_cluster(int FIT_index);

void get_dir_entry(int working_directory, int index, dir_entry* pde);
void set_dir_entry(int working_directory, int index, dir_entry* pde);

void calculate_chs_tuple(int cluster, int* cl, int* dh);
bool load_cluster(int file_cluster, int FIT_index);
bool save_cluster(int file_cluster, int FIT_index);

bool register_FIT(int owner_pid, int start_cluster, int open_mode, int* registered_index);
bool unregister_FIT(int FIT_index);

int find_file_cluster(char name[FILE_MAX_NAME], char ext[FILE_MAX_EXT], int working_directory_cluster, dir_entry* pde);
int navigate_cluster_chain(int start, int count);
int allocate_new_cluster(int start_cluster);

bool get_file_size(int working_directory_FIT_index, int FIT_index, int* filesize);
bool set_file_size(int working_directory_FIT_index, int FIT_index, int filesize);

// pte_creation and pte_opened are optional parameters. Must be set to NULL if not used.
void get_file_time(int working_dir, int file_cluster, time_entry* pte_creation, time_entry* pte_opened);
// If NULL is passed as a pte_opened, the time is set to current time.
bool set_file_time(int working_dir, int file_cluster, time_entry* pte_creation, time_entry* pte_opened);

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


void printhex(unsigned int num, bool uppercase)
{
	if(num >= 16)
		printhex(num / 16, uppercase);
	if(uppercase)
		printchar("0123456789ABCDEF"[num % 16]);
	else
		printchar("0123456789abcdef"[num % 16]);
}


void printnumber(int num)
{
	if(num < 0)
	{
		printchar('-');
		num *= -1;
	}
	if(num == 0xFFFF)
	{
		printstring("-32768");
		return;
	}
	if(num >= 10)
		printnumber(num / 10);
	printchar(num % 10 + '0');
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
	int start_cluster;

	start_cluster = find_file_cluster(name, ext, working_dir, NULL);
	ret = open_file(cs, working_dir, start_cluster, open_mode);
	set_file_time(working_dir, start_cluster, NULL, NULL);

	asm("mov	ax, word ptr [bp - 6]");
	syscall_return();
}


bool io_close(int cs, int flags, kobj_io koio)
{
	int ret = unregister_FIT(koio);

	asm("mov	ax, word ptr [bp - 6]");
	syscall_return();
}


bool io_write(int cs, int flags, kobj_io working_dir, kobj_io koio, void* buffer, int offset, int count)
{
	int ret;

	ret = write_file(working_dir, koio, buffer, offset, count);

	asm("mov	ax, word ptr [bp - 6]");
	syscall_return();
}


bool io_read(int cs, int flags, kobj_io working_dir, kobj_io koio, void* buffer, int offset, int count)
{
	int ret;

	ret = read_file(working_dir, koio, buffer, offset, count);

	asm("mov	ax, word ptr [bp - 6]");
	syscall_return();
}


bool create_file(int cs, int flags, kobj_io working_dir, char name[FILE_MAX_NAME], char ext[FILE_MAX_EXT], byte attrib)
{
	dir_entry de;
	time_entry te;
	word curret_max_cluster = INVALID_CLUSTER;
	word FAT_entry;
	int i;
	int de_index;

	if((de_index = can_create_file(working_dir)) == -1
		|| find_file_cluster(name, ext, working_dir, NULL) != INVALID_CLUSTER)
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}

	for (i = 0; i < sizeof(de); i++)
		((byte*)&de)[i] = 0;
	for (i = 0; i < FILE_MAX_NAME && name[i]; i++)
		de.name[i] = name[i];
	for (i = 0; i < FILE_MAX_EXT && ext[i]; i++)
		de.ext[i] = ext[i];
	de.attrib = attrib | DIR_ENTRY_ATTRIB_USED;
	de.filesize = 0;
	
	for (i = 3; i < 512 / sizeof(word); i++)
	{
		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * i);
		FAT_entry |= read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20,
					2 * i + 1) << 8;
		
		if(FAT_entry == 0xFFFF) curret_max_cluster = i;
		if(FAT_entry == 0x0000) break;

		FAT_entry = 0;
	}
	write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20, 2 * i, 0xFF);
	write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20, 2 * i + 1, 0xFF);
	save_cluster(FAT_CLUSTER, FIT_FAT_INDEX);
	de.start_cluster = i;

	get_time(&te);
	set_dir_entry(working_dir, de_index, &de);
	set_file_time(working_dir, i, &te, NULL);
	sort_dir_entry(working_dir);
	if(attrib & DIR_ENTRY_ATTRIB_SUBDIR)
	{
		create_directory(working_dir, de.start_cluster, working_dir);
		syscall_return();
	}
	asm("mov	ax, #0x01");
	syscall_return();
}


bool delete_file(int cs, int flags, kobj_io working_dir, char name[FILE_MAX_NAME], char ext[FILE_MAX_EXT])
{
	dir_entry de;
	int de_index;
	int subdir;
	word FAT_entry;
	word current;

	if((current = find_file_cluster(name, ext, working_dir, &de_index)) == INVALID_CLUSTER)
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}
	get_dir_entry(working_dir, de_index, &de);
	if(de.attrib & DIR_ENTRY_ATTRIB_SUBDIR)
	{
		delete_directory(working_dir, de.start_cluster);
		syscall_return();
	}
	while ((FAT_entry =
		(read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20, 2 * current)
		 | (read_file_byte(0x3000 + FIT_FAT_INDEX * 0x20, 2 * current + 1) << 8)))
		!= INVALID_CLUSTER)
	{
		current = FAT_entry;
		write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20, 2 * current, INVALID_CLUSTER & 0xFF);
		write_file_byte(0x3000 + FIT_FAT_INDEX * 0x20, 2 * current + 1, INVALID_CLUSTER & 0xFF);
	}
	de.attrib = !DIR_ENTRY_ATTRIB_USED;
	set_dir_entry(working_dir, de_index, &de);
	asm("mov	ax, #0x01");
	syscall_return();
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

	while(i < n && (str1[i] || str2[i]))
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


int string_n_size_compare(char* str1, char* str2, int n)
{
	int i = 0;

	while(i < n)
	{
		if(str1[i] - str2[i])
			return str1[i] - str2[i];
		i++;
	}
	return 0;
}


int open_file(int owner, int working_dir, int file_cluster, int open_mode)
{
	int FIT_index;
	
	if((FIT_index = get_file_FIT_index(file_cluster))
		!= UNREGISTERED_FIT_ENTRY)
		return FIT_index;
	if(!register_FIT(owner, file_cluster, open_mode, &FIT_index))
		return UNREGISTERED_FIT_ENTRY;
	if(!load_cluster(file_cluster, FIT_index))
	{
		unregister_FIT(FIT_index);
		return UNREGISTERED_FIT_ENTRY;
	}
	return FIT_index;
}


bool write_file(int working_directory, int FIT_index, void* buffer, int offset, int count)
{
	int i;
	int j = offset;
	int loaded_cluster;
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
	if(offset / 512 == 0)
	{
		load_cluster(FIT[FIT_index].start_cluster, FIT_index);
		loaded_cluster = FIT[FIT_index].start_cluster;
	}

	for(i = 0; i < count; i++)
	{
		if(j / 512)
		{
			new_cluster = navigate_cluster_chain(
				FIT[FIT_index].start_cluster,
				offset / 512);
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
		offset++;
	}

	return save_cluster(loaded_cluster, FIT_index);
}


bool read_file(int working_directory, int FIT_index, void* buffer, int offset, int count)
{
	int i;
	int j = offset;
	int loaded_cluster;
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
	if(offset / 512 == 0)
	{
		load_cluster(FIT[FIT_index].start_cluster, FIT_index);
		loaded_cluster = FIT[FIT_index].start_cluster;
	}

	for(i = 0; i < count; i++)
	{
		if(j / 512)
		{
			new_cluster = navigate_cluster_chain(
				FIT[FIT_index].start_cluster,
				offset / 512);
			if(new_cluster == INVALID_CLUSTER)
				return false;
			if(!save_cluster(loaded_cluster, FIT_index))
				return false;
			if(!load_cluster(new_cluster, FIT_index))
				return false;
			loaded_cluster = new_cluster;
			j = j % 512;
		}

		((byte*)buffer)[i] =
			read_file_byte(0x3000 + FIT_index * 0x20, j);
		j++;
		offset++;
	}

	return true;
}


bool create_directory(int working_dir, word file_cluster, word parent)
{
	time_entry te;
	dir_entry de;
	int dir_kobj;
	int i;

	dir_kobj = open_file(KERNEL_SEGMENT, working_dir, file_cluster, FILE_OPEN_WRITE | FILE_OPEN_READ);
	if(dir_kobj == UNREGISTERED_FIT_ENTRY)
		return false;
	for (i = 0; i < sizeof(de); i++)
		((byte*)&de)[i] = 0;
	
	de.name[0] = '.';
	de.attrib = DIR_ENTRY_ATTRIB_USED | DIR_ENTRY_ATTRIB_SUBDIR;
	de.filesize = 512;
	de.start_cluster = file_cluster;
	set_dir_entry(dir_kobj, 0, &de);

	get_time(&te);
	set_file_time(dir_kobj, 0, &te, NULL);

	de.name[1] = '.';
	de.start_cluster = parent;
	set_dir_entry(dir_kobj, 1, &de);

	get_time(&te);
	set_file_time(dir_kobj, 1, &te, NULL);

	return unregister_FIT(dir_kobj);
}


bool delete_directory(int working_dir, word file_cluster)
{
	dir_entry de;
	int subdir;
	int i;

	subdir = open_file(KERNEL_SEGMENT, working_dir, file_cluster, FILE_OPEN_WRITE | FILE_OPEN_READ);
	if(subdir == UNREGISTERED_FIT_ENTRY)
		return false;
	for (i = 2; i < 512 / sizeof(de); i++)
	{
		get_dir_entry(subdir, i, &de);
		if(!(de.attrib & DIR_ENTRY_ATTRIB_USED))
			continue;
		if(de.attrib & DIR_ENTRY_ATTRIB_SUBDIR)
			delete_directory(subdir, de.start_cluster);
		delete_file(KERNEL_SEGMENT, 0, subdir, de.name, de.ext);
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
	return;
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
	return;
}


int can_create_file(int working_dir)
{
	dir_entry de;
	int i;

	for(i = 0; i < 512 / sizeof(dir_entry); i++)
	{
		get_dir_entry(working_dir, i, &de);
		if(!(de.attrib & DIR_ENTRY_ATTRIB_USED))
			return i;
	}
	return -1;
}


void sort_dir_entry(int working_dir)
{
	dir_entry de;
	dir_entry de_min;
	int i;
	int j;
	int end;
	int min;

	for (i = 2; i < 512 / sizeof(dir_entry); i++)
	{
		get_dir_entry(working_dir, i, &de);
		if (!(de.attrib & DIR_ENTRY_ATTRIB_USED))
			end = i - 1;
	}
	for (i = 2; i <= end; i++)
	{
		min = i;
		get_dir_entry(working_dir, min, &de_min);
		if (!(de_min.attrib & DIR_ENTRY_ATTRIB_USED))
			continue;
		for (j = i; j <= end; j++)
		{
			get_dir_entry(working_dir, j, &de);
			if (!(de.attrib & DIR_ENTRY_ATTRIB_USED))
				continue;
			if (string_n_size_compare(de_min.name, de.name, FILE_MAX_NAME) > 0)
				min = j;
			if (string_n_size_compare(de_min.name, de.name, FILE_MAX_NAME) == 0
				&& string_n_size_compare(de_min.ext, de.ext, FILE_MAX_EXT) > 0)
				min = j;
		}
		get_dir_entry(working_dir, i, &de);
		get_dir_entry(working_dir, min, &de_min);
		set_dir_entry(working_dir, i, &de_min);
		set_dir_entry(working_dir, min, &de);
	}
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


void set_dir_entry(int working_directory, int index, dir_entry* pde)
{
	int i;

	for(i = 0; i < sizeof(dir_entry); i++)
		write_file_byte(
			0x3000 + working_directory * 0x20,
			index * sizeof(dir_entry) + i,
			((byte*)pde)[i]);

	save_cluster(get_file_cluster(working_directory), working_directory);
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


bool unregister_FIT(int FIT_index)
{
	if(FIT[FIT_index].flags & FIT_FLAGS_USED_ENTRY)
	{
		FIT[FIT_index].flags &= !(FIT_FLAGS_USED_ENTRY);
		return true;
	}
	else return false;
}


int find_file_cluster(char name[FILE_MAX_NAME], char ext[FILE_MAX_EXT], int working_directory, int* pde_index)
{
	dir_entry de;
	int i;

	for(i = 0; i < 512 / sizeof(de); i++)
	{
		get_dir_entry(working_directory, i, &de);

		if(string_n_compare(de.name, name, FILE_MAX_NAME)
			&& string_n_compare(de.ext, ext, FILE_MAX_EXT))
		{
			if(pde_index)
				*pde_index = i;
			return de.start_cluster;
		}
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

			set_dir_entry(working_directory_FIT_index, i, &de);

			return save_cluster(
					get_file_cluster(working_directory_FIT_index),
					working_directory_FIT_index);
		}
	}

	return false;
}


void get_file_time(int working_dir, int file_cluster, time_entry* pte_creation, time_entry* pte_opened)
{
	dir_entry de;
	int i;

	for(i = 0; i < 512 / sizeof(de); i++)
	{
		get_dir_entry(working_dir, i, &de);
		if(file_cluster == de.start_cluster)
		{
			if(pte_creation) *pte_creation = de.creation_date;
			if(pte_opened) *pte_opened = de.opened_date;
			return;
		}
	}
}


bool set_file_time(int working_dir, int file_cluster, time_entry* pte_creation, time_entry* pte_opened)
{
	dir_entry de;
	time_entry te;
	int i;

	for(i = 0; i < 512 / sizeof(de); i++)
	{
		get_dir_entry(working_dir, i, &de);

		if(file_cluster == de.start_cluster)
		{
			if(!get_time(&te)) return false;

			if(pte_creation) de.creation_date = *pte_creation;
			if(pte_opened) de.opened_date = *pte_opened;
			else de.opened_date = te;

			set_dir_entry(working_dir, i, &de);
			return true;
		}
	}

	return false;
}
