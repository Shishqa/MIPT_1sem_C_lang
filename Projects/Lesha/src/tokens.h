#ifndef TOKENS                           //flag that tree was defined only once in all project
#define TOKENS

struct for_names {
    char* *names;
    int amount;
    int initial_ammount;

    explicit for_names (int size = 30);
    void append (char* element);

    void print ();

    int search_name (char* name);
    ~for_names ();
};

for_names::for_names (int size) {
    names = new char*[size];
    amount = 0;
}

void for_names::append (char* element) {
    names[amount] = element;
    ++amount;
}

void for_names::print () {
    for (int i = 0; i < amount; ++i) {
        printf ("loc_var: %s\n", names[i]);
    }
}

int for_names::search_name (char *name) {
    for (int i = 0; i < amount; ++i) {
        if (strcmp (names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

for_names::~for_names() {

}

for_names* ids = new for_names;

struct token {
    char* name;
    const int code;
    const char n_args;
    const char length;

    int search_token (char* name);
};

enum token_codes {
    UNKNOWN,
    PROGRAMM,
    BLOCK,
    BAD_GUY,
    INSTRUCTION,
    VARLIST,
    ID,
    VARIABLE,

    DEFINITION,
    CALL,
    RETURN,

    NUMBER,
    ASSIGN,
    OUT,

    IF,
    CONDITION,
    WHILE,
    IS_EQUAL,
    IS_BIGGER,
    IS_LESS,
    NOT_EQUAL,
    PLUS,
    MINUS,                      //......
    MULT,
    DIVIDE,

    INCR,
    DECR,

    OPEN_BR,
    CLOS_BR,
    COND_DIVIDER,

    OPEN_EQ,
    CLOS_EQ,

    ARGUMENTS,
    LAST_ARG,
    ASSIGN_BY_LEN,

    PRINT
};

enum person_states {
    DEAD,                       //states
    DESTROYED,
    DISLOYAL,
    AFRAID,
    FURIOUS,
    ANGRY,
    SAD,
    DISHONEST,
    CONFUSED,
    FORGIVEN,
    LOYAl,
    HAPPY
};

token tokens[] = {
        {"@",       UNKNOWN,    0, 1},
        {"PROGRAM_ROOT",       PROGRAMM,   1, 12},
        {"D",       BLOCK,      2, 1},
        {"B",       BAD_GUY,      2, 1},
        {"O",       INSTRUCTION,2, 1},
        {",", VARLIST,    2, 1},
        {"id",      ID,         1, 2},
        {"var",  VARIABLE,   1, 3},

        {"def", DEFINITION, 1, 8},
        {"CALL",   CALL,       2, 4},
        {"return",  RETURN,     0, 6},

        {"num",  NUMBER,     0, 3},

        {"=",     ASSIGN,     2, 1},
        {"print",    OUT,        1, 5},

        {"if",      IF,         2, 2},
        {"C",CONDITION, 2, 1},

        {"while",   WHILE,      2, 5},

        {"==",      IS_EQUAL,   2, 2},
        {">",  IS_BIGGER,  2, 1},
        {"<",   IS_LESS,    2, 1},
        {"!=",      NOT_EQUAL,  2, 2},

        {"+",       PLUS,        2, 1},
        {"-",       MINUS,       2, 1},
        {"*",       MULT,        2, 1},
        {"/",       DIVIDE,     2, 1},

        {"honor",   INCR,       1, 5},
        {"punish",  DECR,       1, 6},

        {":",       OPEN_BR,    -1, 1},
        {".",       CLOS_BR,    -1, 1},
        {"\"",      COND_DIVIDER,-1,1},

        {"(",       OPEN_EQ,    -1, 1},
        {")",       CLOS_EQ,    -1, 1},
        {"of",      ARGUMENTS,  -1, 2},
        {"and",     LAST_ARG,   -1, 3},
        {"a",       ASSIGN_BY_LEN,-1, 2},

        {"mandate", PRINT,      -1, 7},
};

const token states[] {
        {"0", DEAD, 0, 4},
        {"1", DESTROYED, 0, 9},
        {"2", DISLOYAL, 0, 8},
        {"3", AFRAID, 0, 6},
        {"4", FURIOUS, 0, 7},
        {"5", ANGRY, 0, 5},
        {"6", SAD, 0, 3},
        {"7", DISHONEST, 0, 9},
        {"8", CONFUSED, 0, 8},
        {"9", FORGIVEN, 0, 8},
        {"10", LOYAl, 0, 5},
        {"11", HAPPY, 0, 5},
};

int token::search_token (char *name) {
    int num = (sizeof (tokens) / sizeof (token));

    for (int i = 0; i < num; ++i) {
        if (strncmp (tokens[i].name, name, tokens[i].length) == 0) {
            return i;
        }
    }

    if (strncmp ("OP", name, 2) == 0) {
        return INSTRUCTION;
    }

    return -1;
}


#endif