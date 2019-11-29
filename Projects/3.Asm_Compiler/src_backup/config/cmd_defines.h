/*
 * cmd_defines.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#include "macros.h"

/*
 * DEF_CMD (OPCODE, NAME, TYPE, LEN_OF_NAME, N_ARGS, FROM_STACK,
 * {
 *     DSL code;
 * })
 */


DEF_CMD (0, END, SYS, 3, 0, 0,
{
    RET (CPU_OK)
})


//1
DEF_OP (1, ADD, +, 3)


//2
DEF_OP (2, SUB, -, 3)


//3
DEF_OP (3, MUL, *, 3)


//4
DEF_OP (4, DIV, /, 3)


DEF_OP (5, MOD, %, 3)


//5
DEF_CMD (6, DUMP, SYS, 4, 0, 0,
{
    DUMP ()
    MCUR (1)
})


//6
DEF_CMD (7, POP, SYS, 3, 1, 1,
{
	MCUR (1)

	GET_ARG ()

	if (!INT_P && !REG_P)
	{
		POP (T_REG(0))
		MCUR (SOF_ARG)
		break;
	}

	if (MEM_P && REG_P)
	{
		POP (ram[reg[(int)arg.reg] + arg.val])
	}
	else if (MEM_P)
	{
		POP (ram[arg.val])
	}
	else if (REG_P)
	{
		POP (reg[(int)arg.reg])
	}
	else
	{
		RET (WRONG_ARG_TYPE_ERR)
	}

	MCUR (SOF_ARG)
})


//7
DEF_CMD (8, IN, SYS, 2, 0, 0,
{
	MCUR (1)

    if (scanf ("%d", &T_REG(0)) == 0)
    {
    	printf ("error: IN couldn't get number (%lu)\n", curr_line);
    	RET (IN_ERR)
    }

    PUSH (T_REG(0))
})


//8
DEF_CMD (9, OUT, SYS, 3, 0, 1,
{
	MCUR (1)

    POP (T_REG(0))

    printf ("%d", T_REG(0));
})


DEF_CMD (10, PUSH, SYS,  4, 1, 0,
{
	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	PUSH (T_REG(0))

	MCUR (SOF_ARG);
})


DEF_CMD (11, MOV, SYS, 3, 2, 0,
{
	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	MCUR (SOF_ARG)

	GET_ARG ()

	if (MEM_P && REG_P)
	{
		ram[reg[(int)arg.reg] + arg.val] = T_REG(0);
	}
	else if (MEM_P)
	{
		ram[arg.val] = T_REG(0);
	}
	else if (REG_P)
	{
		reg[(int)arg.reg] = T_REG(0);
	}
	else
	{
		RET (WRONG_ARG_TYPE_ERR)
	}

	MCUR (SOF_ARG)
})



DEF_CMD (12, OUTC, STD, 4, 1, 0,
{
	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	printf ("%c", T_REG(0));

	MCUR (SOF_ARG)
})


DEF_CMD (13, POW, STD, 3, 0, 2,
{
	MCUR (1)

	POP (T_REG(1))
	POP (T_REG(0))

	if (T_REG(1) < 0)
	{
		RET (POW_NEGATIVE_ERR)
	}

    PUSH (BinaryPow (T_REG(0), T_REG(1)))
})


DEF_CMD (14, JMP, JUMP, 3, 1, 0,
{
	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	SCUR (T_REG(0))
})


DEF_JMP (15, JA, >, 2)


DEF_JMP (16, JAE, >=, 3)


DEF_JMP (17, JB, <, 2)


DEF_JMP (18, JBE, <=, 3)


DEF_JMP (19, JE, ==, 2)


DEF_JMP (20, JNE, !=, 3)


DEF_CMD (21, CALL, JUMP, 4, 1, 0,
{
	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	MCUR (SOF_ARG)

	PUSH_CALL (curr - code_begin)

	SCUR (T_REG(0))
})


DEF_CMD (22, RET, JUMP, 3, 0, 0,
{
    POP_CALL (T_REG(0))
    SCUR (T_REG(0))
})


DEF_CMD (23, SQRT, FLOAT, 4, 1, 1,
{
	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	MCUR (SOF_ARG)

	POP (T_REG(1))

	if (T_REG(1) < 0)
	{
		RET (SQRT_NEGATIVE_ERR)
	}

	T_REG(0) = BinaryPow (10, T_REG(0));

	double tmp_val = T_REG(1);
	tmp_val /= T_REG(0);

	tmp_val = sqrt (tmp_val);

	T_REG(1) = tmp_val * T_REG(0);

	PUSH (T_REG(1))
})


DEF_CMD (24, OUTF, FLOAT, 4, 1, 1,
{
	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	MCUR (SOF_ARG)

	POP (T_REG(1))

	T_REG(0) = BinaryPow (10, T_REG(0));

	double tmp_val = T_REG(1);
	tmp_val /= T_REG(0);

    printf ("%lf", tmp_val);
})


DEF_CMD (25, INIT_SCR, GRAPH, 8, 0, 0,
{
	initscr ();
	cbreak ();
	noecho ();
	nodelay (stdscr, TRUE);
	start_color();
	init_pair (1, COLOR_WHITE, COLOR_WHITE);
	init_pair (2, COLOR_BLACK, COLOR_BLACK);
	MCUR (1)
})


DEF_CMD (26, END_WIN, GRAPH, 7, 0, 0,
{
	endwin ();
	MCUR (1)
})


DEF_CMD (27, CLR, GRAPH, 3, 0, 0,
{
	clear ();
	MCUR (1)
})


DEF_CMD (28, REF, GRAPH, 3, 0, 0,
{
	clear();

	//int r = 0;
	//int g = 0;
	//int b = 0;

	for (int i = 0; i < VRAM_SIZE; i++)
	{
		attron(COLOR_PAIR(ram[RAM_SIZE + i]));
		mvwaddch (stdscr, i / COLS, i % COLS, 32);
		attroff(COLOR_PAIR(ram[RAM_SIZE + i]));
	}

	refresh ();

	MCUR (1)
})


DEF_CMD (29, GETCH, GRAPH, 5, 0, 0,
{
	char ch = 0;

	if ((ch = getch ()) == ERR)
	{
		PUSH (-1)
	}
	else
	{
		PUSH (ch)
	}

	MCUR (1)
})


DEF_CMD (30, ADDCH, GRAPH, 5, 1, 2,
{
	POP (T_REG(2))
	POP (T_REG(1))

	MCUR (1)

	GET_ARG ()

	GET_VAL (T_REG(0))

	MCUR (SOF_ARG)

	mvwaddch (stdscr, T_REG(2), T_REG(1), (char)(T_REG(0)));
	wmove (stdscr, 0, 0);
})


DEF_CMD (31, HEIGHT, GRAPH, 6, 0, 0,
{
	PUSH (LINES)
	MCUR (1)
})


DEF_CMD (32, WIDTH, GRAPH, 5, 0, 0,
{
	PUSH (COLS)
	MCUR (1)
})


DEF_CMD (33, V_RAM, GRAPH, 5, 0, 0,
{
	PUSH (RAM_SIZE)
	MCUR(1)
})


DEF_CMD (34, DUMP_RAM, SYS, 8, 0, 0,
{
	printf ("dump:\n");
	for (int i = 0; i < 50; i++)
	{
		printf ("%d\n", ram[RAM_SIZE + i]);
	}
	MCUR (1)
})





