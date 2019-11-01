/*
 * make_asm.cpp
 *
 *  Created on: 18 окт. 2019 г.
 *      Author: shishqa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "../../libraries/assert_ext.h"
#include "../../libraries/ms_stream/ms_stream.h"
#include "../../libraries/ms_stack/ms_stack.h"
#include "../config/config.h"
#include "../config/cmd_enum.h"
#include "make_asm.h"



 int main (int argc, char** argv)
 {
     if (argc == 1)
     {
		 PrintHelp ();
		 exit (MAKE_OK);
     }

     char current_dir[PATH_MAX] = "../../";
     char target_path[PATH_MAX] = {};
     char binary_path[PATH_MAX] = {};
     char disasm_path[PATH_MAX] = {};

     bool asm_key = false;
     bool run_key = false;
     bool disasm_key = false;

     ParseKeys (argc, argv, current_dir, target_path, binary_path, disasm_path, &run_key, &disasm_key);

     if (CompareDates (target_path, binary_path))
     {
    	 asm_key = true;
     }
     else if (CheckVersion (binary_path))
     {
    	 asm_key = true;
     }

     if (asm_key)
     {
		 char call_asm[PATH_MAX] = "../../asm/Debug/asm ";

		 strcat (call_asm, target_path);
		 strcat (call_asm, " ");
		 strcat (call_asm, binary_path);

		 printf ("%s\n", call_asm);
		 system (call_asm);
     }
     else
     {
    	 printf ("target doesn't require compilation\n");
     }

     if (disasm_key)
     {
		 char call_disasm[PATH_MAX] = "../../disasm/Debug/disasm ";

		 strcat (call_disasm, binary_path);
		 strcat (call_disasm, " ");
		 strcat (call_disasm, disasm_path);

		 printf ("%s\n", call_disasm);
		 system (call_disasm);
     }

     if (run_key)
     {
		 char call_cpu[PATH_MAX] = "../../cpu/Debug/cpu ";

		 strcat (call_cpu, binary_path);

		 //printf ("%s\n", call_cpu);
		 system (call_cpu);
     }

     exit (MAKE_OK);
 }


void PrintHelp ()
{
    printf ("##########################################################################################\n");
    printf ("make_asm HELP:\n\n");
    printf ("./make_asm {target} {keys}\n");
    printf ("keys:\n");
    printf ("\t-o {name}\tuse unique name for executable file (default is \"source\")\n");
    printf ("\t-d {name}\tgenerate disassembled file (default is \"source_dsm\")\n");
    printf ("\t-r\t\trun executable file after compilation\n");
    printf ("##########################################################################################\n");
}


void ParseKeys (int argc, char** argv, char* dir, char* target_path, char* binary_path, char* disasm_path, bool* run_key, bool* disasm_key)
{
    size_t arg_len = strlen (argv[1]);

    bool o_used = false;
    bool d_used = false;
    bool r_used = false;

    strncpy (target_path, dir, strlen (dir));

    size_t dir_len = GetFolder (argv[1], arg_len);

    strncat (dir, argv[1], dir_len);
    strncat (target_path, argv[1], arg_len);

    for (size_t i = 2; i < argc; i++)
    {
		arg_len = strlen (argv[i]);

		if (arg_len == 2 && strncmp (argv[i], "-o", arg_len) == 0)
		{
			if (o_used)
			{
				printf ("Error: key \"-o\" used more than once\n");
				exit (O_USE_ERR);
			}
			if (i == argc - 1)
			{
				printf ("\"-o\" value is not entered\n");
				exit (NO_O_ARG);
			}

			o_used = true;

			i++;

			strcpy  (binary_path, dir);
			strncat (binary_path, argv[i], strlen (argv[i]));
			strncat (binary_path, ".bin", sizeof (".bin"));

			continue;
		}

		if (arg_len == 2 && strncmp (argv[i], "-r", arg_len) == 0)
		{
			if (r_used)
			{
				printf ("Error: key \"-r\" used more than once\n");
				exit (R_USE_ERR);
			}

			r_used = true;

			*run_key = true;

			continue;
		}

		if (arg_len == 2 && strncmp (argv[i], "-d", arg_len) == 0)
		{
			if (d_used)
			{
				printf ("Error: key \"-d\" used more than once\n");
				exit (D_USE_ERR);
			}

			d_used = true;

			*disasm_key = true;

			i++;

			if (i <= argc - 1)
			{
				strcpy  (disasm_path, dir);
				strncat (disasm_path, argv[i], strlen (argv[i]));
				strncat (disasm_path, ".asm", sizeof (".asm"));
			}
			else
			{
				strcpy  (disasm_path, dir);
				strncat (disasm_path, DEFAULT_DISASM, sizeof (DEFAULT_DISASM));
				strncat (disasm_path, ".asm", sizeof (".asm"));
			}

			continue;
		}

		printf ("Error: undefined argument %s\n", argv[i]);

		exit (UNDEF_ARG_ERR);
    }

    if (!o_used)
    {
		strcpy  (binary_path, dir);
		strncat (binary_path, DEFAULT_BINARY, sizeof (DEFAULT_BINARY));
		strncat (binary_path, ".bin", sizeof (".bin"));
    }

    return;
}


size_t GetFolder (char* path, size_t len)
{
    char* dir_end = path;

    for (char* cur = path; cur < path + len; cur++)
    {
		if (*cur == '/')
		{
			dir_end = cur;
		}
    }

    return dir_end - path + 1;
}


bool CompareDates (const char* in, const char* out)
{
    ASSERT (in  != nullptr)
    ASSERT (out != nullptr)

    struct stat target = {};
    struct stat result = {};

    stat (in, &target);

    if (stat (out, &result) != 0)
    {
        return true;
    }

    return (result.st_mtim.tv_sec <= target.st_mtim.tv_sec);
}


bool CheckVersion (const char* exec_path)
{
    ASSERT (exec_path != nullptr)

	Signature file_sign = {};

    bool check_key = false;

    FILE* exec = fopen (exec_path, "rb");

    char* buffer = nullptr;
    Read (&buffer, exec);

    ASSERT (buffer != nullptr)

    if (*(double*)(buffer + sizeof (file_sign.sign)) != file_sign.version)
    {
    	check_key = true;
    }

    free (buffer);
    fclose (exec);

    return check_key;
}

