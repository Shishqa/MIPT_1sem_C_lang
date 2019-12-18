/*
 * run_disasm.cpp
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

#include "../../libraries/assert_ext.h"
#include "../../libraries/ms_stack/ms_stack.h"
#include "../../libraries/ms_label/ms_label.h"
#include "../config/config.h"
#include "../config/cmd_enum.h"
#include "disasm.h"

void PrintSyntax ();
Err_n ProceedDisassembly (const char* in, const char* out);

const char* DEFAULT_IN_PATH  = "../../source.bin";
const char* DEFAULT_OUT_PATH = "../../source_dsm.asm";

int main (int argc, char** argv)
{
    if (argc < 2)
    {
        PrintSyntax ();
        printf ("press Enter to start with default paths [or type something to exit]:\n>>>");

        int n_chars = 0;

        if (!scanf ("%*[^\n]%n", &n_chars) && n_chars == 0)
        {
        	scanf ("%*c"); // скушать \n

            ProceedDisassembly (DEFAULT_IN_PATH, DEFAULT_OUT_PATH);
        }

        return (DISASM_OK);
    }

    if (argc != 3)
    {
		printf ("error: invalid number of arguments\n\n");
		PrintSyntax ();

		return (NO_ARG_ERR);
    }

    char in_path[PATH_MAX] = {};

    if (strlen(argv[1]) == 2 && !strncmp (argv[1], "--", 2))
    {
		printf ("please, write path of file with binary code:\n>>>");
		scanf ("%s", in_path);
    }
    else
    {
    	strcpy (in_path, argv[1]);
    }

    char out_path[PATH_MAX] = {};

    if (strlen(argv[2]) == 2 && !strncmp (argv[2], "--", 2))
    {
		printf ("please, write path to write the disassembled code:\n>>>");
		scanf ("%s", out_path);
	}
    else
    {
    	strcpy (out_path, argv[2]);
    }

    ProceedDisassembly (in_path, out_path);

    return (DISASM_OK);
}

Err_n ProceedDisassembly (const char* in, const char* out)
{
    FILE* file_in  = fopen (in,  "rb");

    if (file_in == nullptr)
    {
        printf ("error: can't open file %s\n", in);
        return (INPUT_ERR);
    }

    FILE* file_out = fopen (out, "w");

    if (file_out == nullptr)
    {
		printf ("error: can't open/create file %s\n", out);
		return (OUTPUT_ERR);
    }

    Labels labels = {};
    labels.init (LABEL_MAX_CNT, MAX_CALL);

    Err_n check = DISASM_OK;

    for (int i = 1; i <= 2; i++)
    {
    	check = Disassembler (file_in, file_out, &labels);

    	if (check != DISASM_OK)
    	{
    		break;
    	}

    	fseek (file_in, 0, SEEK_SET);
    	fseek (file_out, 0, SEEK_SET);
    }

    fclose (file_in);
    fclose (file_out);
    labels.clear();

    printf ("disassembler returned: %d\n", check);

    return (check);
}

void PrintSyntax ()
{
    printf ("##########################################################################################\n");
    printf ("disasm HELP:\n\n");
    printf ("./disasm {from} {to}\n");
    printf ("keys:\n");
    printf ("\t--\t\tenter path from keyboard\n");
    printf ("\n");
    printf ("default paths:\n");
    printf ("\tin = %s\n", DEFAULT_IN_PATH);
    printf ("\tout = %s\n", DEFAULT_OUT_PATH);
    printf ("##########################################################################################\n");
}




