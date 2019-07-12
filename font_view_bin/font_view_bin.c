// Просмотр бинарных шрифтов
//
//
// Версия v0.1 ----------------------------------------------------------------
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "get_opt.h"
#include "git_commit.h"
#include "version.h"

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int read_file(FILE * fi, uint8_t * buf, size_t *size);

const char PRINT_HELP[] = {
" Run & Parameters:\n\r"
" font_view_bin -i <file-input-bin>\n\r"
"--------------------------------------------------------------------------------\n\r"
};

const char PRINT_TIRE[] = {"================================================================================\n\r"};
const char PRINT_PROG_NAME[] = {" Viewer for BIN-FONT (c) Sviridov Georgy 2019, www.lab85.ru sgot@inbox.ru\n\r"};

#define FILE_RW_BLOCK_SIZE (1024)
#define FILE_SIZE_MAX      (1*1024*1024*1024)  // максимальный размер файла который можем обрабатывать

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	size_t file_size_in = 0;
    size_t result = 0;
	size_t i, j;

	struct stat stat_buf;
	FILE * fi = 0;
	int res = 0;
	uint8_t *mem = NULL;   // ukazatel na buf-mem dla READ_WRITE SPI mem.

	param_opt_st param_opt;        // clean structure
	param_opt.i_file_name = NULL;


	printf( PRINT_TIRE );
    printf( PRINT_PROG_NAME );
	printf( " Ver %d.%d.%d\n\r", SOFT_VER_H, SOFT_VER_L, SOFT_VER_Y );
    printf( " GIT = %s\n\r", git_commit_str );
	printf( PRINT_TIRE );

	if (argc == 1){
	    printf( PRINT_HELP );
		printf("Maximum input file size: %d bytes\n\r", FILE_SIZE_MAX);
		return 0;
	}

	// razbor perametrov
	res = get_opt(argc, argv, &param_opt);
	if (res == -1){
		printf("\n\rERROR: input parameters.\n\r");
		return 1;
	}

	{// file load -------------------------------------------------------------
		printf(PRINT_TIRE);

		if (stat(param_opt.i_file_name, &stat_buf) == -1){
			printf("ERROR: fstat return error !\n\r");
			return 1;
		}
		file_size_in = stat_buf.st_size;

    	// proverka diapazona offset+size <= density
		if (file_size_in > FILE_SIZE_MAX ){
			printf("ERROR: file is BIG !!! file_size( %lld ) > chip_size(+offset %lld )\n\r", (long long)stat_buf.st_size, (long long)(FILE_SIZE_MAX));
			return 1;
		}

    	mem = (uint8_t*)malloc( file_size_in );
	    if (mem == NULL){
		    printf("ERROR: malloc( size[  %d  ])\n\r", file_size_in);
		    return 1;
	    }

		printf("File size = %lld bytes.\n\r", (long long)stat_buf.st_size);

		if (file_size_in == 0){
			printf("ERROR: file is empty.\n\r");
			return 1;
		}

		printf("Read file(%s) : ", param_opt.i_file_name);
        // read data from file
		
		fi = fopen(param_opt.i_file_name, "rb");
		if ( fi == NULL ){
			printf("ERROR: open file.\n\r");
			return 1;
		}

		read_file(fi, mem, &result);

        fclose(fi);

		if (result != file_size_in){
    		printf("ERROR: file size = %lld, read size = %lld\n", file_size_in, result);
	    	return 1;
	    }

		//-------------------------------------------------------------------------

		printf("- OK\n\r");
	}// file load -------------------------------------------------------------


	// ========================================================================================
	printf("\n");
	for (i=0; i<file_size_in; i++){
		for(j=0; j<8; j++){
			printf("%c", *(mem + i) & (0x80>>j) ? '#' : ' ');
		}
        printf("\n");
	}

    printf("\n");

	free(mem);

	return 0;
}

//=============================================================================
// Read data from file to buff
//=============================================================================
int read_file(FILE * fi, uint8_t * buf, size_t *size)
{
	size_t pos    = 0;
	size_t result = 0;
	size_t r_size = FILE_RW_BLOCK_SIZE;

	pos = 0;
	while( (result = fread( &buf[pos], 1, r_size, fi)) > 0){
		pos = pos + result;
    }

	*size = pos;

	return 0;
}
