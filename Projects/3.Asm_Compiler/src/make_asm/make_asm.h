/*
 * make_asm.h
 *
 *  Created on: 18 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MAKE_ASM_H_
#define MAKE_ASM_H_


typedef int Err_n;

enum
{
    MAKE_OK,
    O_USE_ERR,
    NO_O_ARG,
    R_USE_ERR,
    D_USE_ERR,
    UNDEF_ARG_ERR,
	ASM_FAILED,
	RUN_FAILED,
	DISASM_FAILED
};

const char DEFAULT_BINARY[PATH_MAX] = "source";
const char DEFAULT_DISASM[PATH_MAX] = "source_dsm";

void PrintHelp ();
void ParseKeys (int argc, char** argv, char* dir,
char* target_path, char* binary_path, char* disasm_path,
bool* run_key, bool* disasm_key);
size_t GetFolder (char* path, size_t len);
bool CompareDates (const char* in, const char* out);
bool CheckVersion (const char* exec_path);


#endif /* MAKE_ASM_H_ */
