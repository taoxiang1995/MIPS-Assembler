#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

#define INITIAL_SIZE 5
#define SCALING_FACTOR 2

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_symbol(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable* create_table(int mode) {
    /* YOUR CODE HERE */
    SymbolTable* new_table;
    new_table = (SymbolTable*) malloc(sizeof(SymbolTable));
    new_table->mode = mode;
    //allocate 5 empty Symbol holders
    new_table->tbl = (Symbol*) malloc(INITIAL_SIZE*sizeof(Symbol));
    //new_table->tbl->addr = 0;
    //new_table->tbl->name = "";
    new_table->cap = 5;
    new_table->len = 0;
    if (new_table == NULL || new_table->tbl == NULL) {
        allocation_failed();
       // return NULL;
    }
    return new_table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    /* YOUR CODE HERE */
    int i;
    for (i = 0; i < table->len; i++) {
        free((table->tbl[i]).name);
        (table->tbl[i]).name = NULL;
    }
    free(table->tbl);
    table->tbl=NULL;  

    free(table);
    table = NULL;
}

/* A suggested helper function for copying the contents of a string. */
static char* create_copy_of_str(const char* str) {
    size_t len = strlen(str) + 1;
    char *buf = (char *) malloc(len);
    if (!buf) {
       allocation_failed();
    }
    strncpy(buf, str, len); 
    return buf;
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   ADDR is given as the byte offset from the first instruction. The SymbolTable
   must be able to resize itself as more elements are added. 

   Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
   return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. If memory
   allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0.
 */

//return 1 if is word aligned
//retuen 0 if is not aligned
int is_word_aligned (uint32_t addr) {
    if (addr%4 ==0)
        return 1;
    else
        return 0;
}

//return 1 if the name is already existed
//return 0 if the name is not existed
int if_name_existed (SymbolTable* table, const char* name) {
    int i;
    for (i=0; i<table->len; i++) {
        if (strcmp((table->tbl[i]).name, name)==0)
            return 1;
    }
    return 0;
}

int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    /* YOUR CODE HERE */
    if (is_word_aligned(addr) == 0) {
        addr_alignment_incorrect();
        return -1;
    }

    if (if_name_existed(table, name) == 1 && table->mode == SYMTBL_UNIQUE_NAME) {
        name_already_exists(name);
        return -1;
    }

    //if tbl's capacity is full, then allocate more according to SCALING_FACTOR
    if (table->len>=table->cap) {
        table->tbl = (Symbol*) realloc(table->tbl, sizeof(Symbol)*(table->cap)*SCALING_FACTOR);
        table->cap = table->cap*SCALING_FACTOR;
    }

    //if successfully allocated more space
    if (table->tbl != NULL) {
        table->len++;

        //copy the register name
       
        table->tbl[table->len-1].name =create_copy_of_str(name);
        //strcpy(table->tbl[table->len-1].name, name);
        //copy the address
        table->tbl[table->len-1].addr = addr;
        return 0;
    } else {
        allocation_failed();
    }

    return -1;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    /* YOUR CODE HERE */
    int i;
    for (i = 0; i < table->len; i++) {
        if (strcmp(name, table->tbl[i].name) == 0)
            return table->tbl[i].addr;
    }
    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    /* YOUR CODE HERE */
    int i;
    for (i=0; i<table->len; i++) {
        write_symbol(output, table->tbl[i].addr, table->tbl[i].name);
    }
}
