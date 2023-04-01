all: main.o

	$(LD) -b elf64-x86-64 -z muldefs -o $(tmp_output_dir)/shell  $(shell find . -name "*.o") $(ROOT_PATH)/bin/sysroot/usr/lib/libc.a -T shell.lds

	$(OBJCOPY) -I elf64-x86-64 -R ".eh_frame" -R ".comment" -O elf64-x86-64 $(tmp_output_dir)/shell $(output_dir)/shell.elf
main.o: main.c
	$(CC) $(CFLAGS) -c main.c  -o main.o