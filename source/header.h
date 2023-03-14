#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

void print_title();
void clear_buffer();
int ascii_to_int(int);
int ask_confirm();
int menu_scanner(int);
int parameter_scanner(char*);
int cookie_scanner(char*);
int header_scanner(char*);
int filename_scanner(char*);
int set_GET_data();
int set_POST_data();
int set_cookie();
int set_header();
int set_verbose();
int set_output_file();
int allocate(char*,int);

// Control flag to check already set options
bool flag_GET, flag_POST, flag_cookie, flag_header, flag_verbose, flag_output;
// Options counter and options index
int options_c, ind_GET, ind_cookie, ind_POST, ind_header, ind_verbose, ind_output;
// Options vector
char **options_v;