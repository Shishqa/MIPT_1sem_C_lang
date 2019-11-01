/*
 * disasm.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef DISASM_H_
#define DISASM_H_


typedef int Err_n;

enum
{
    DISASM_OK         = 0,
    WRONG_CMD_ERR     = 1,
    NO_ARG_ERR        = 2,
    INPUT_ERR         = 3,
    OUTPUT_ERR        = 4
};

Err_n Disassembler (FILE* in, FILE* out, Labels* labels);
char* PrintArgs (FILE* out, char* curr, int n_args, Labels* labels, int type);
int GetLabel (Labels* labels, int curr);

#endif /* DISASM_H_ */
