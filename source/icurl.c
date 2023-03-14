#include "header.h"

void print_title() {
    puts("    _  ______              __");
    puts("   (_)/ ____/ _  __ _____ / /");
    puts("  / // /    / / / // ___// / ");
    puts(" / // /____/ /_/ // /   / /  ");
    puts("/_/ \\_____/\\__,_//_/   /_/   ");
}

void clear_buffer() {
    // Clear the buffer
    while(getchar() != '\n');
}

int ascii_to_int (int ascii) {
    // Return the integer value associated to the ascii code (0-9) and -1 for non integer
    switch (ascii) {
        case(48): {
            return 0;
        }
        case(49): {
            return 1;
        }
        case(50): {
            return 2;
        }
        case(51): {
            return 3;
        }
        case(52): {
            return 4;
        }
        case(53): {
            return 5;
        }
        case(54): {
            return 6;
        }
        case(55): {
            return 7;
        }
        case(56): {
            return 8;
        }
        case(57): {
            return 9;
        }
        default: {
            return -1;
        }
    }
}

int ask_confirm() {
    // Present a yes/no menu and return the choice (1=Yes/2=No)
    char ans;
    int ascii;

    printf("> ");
    scanf(" %c",&ans);
    ascii = tolower(ans);
    clear_buffer();
    if (ascii == 121) { // y
        ans = 1;
    }
    else if (ascii == 110) { // n
        ans = 2;
    }
    else {
        printf("Please enter a valid option and retry\n");
        ans = -1;
    }
    return ans;
}

int menu_scanner (int menu_options) {
    /* Function used to get and parse the menu input, based on it's available options
    On success it returns the op value while -1 it's returned in case of errors or invalid options*/
    char choice;
    int op;

    scanf(" %c",&choice);
    clear_buffer();
    op = ascii_to_int(choice);
    if (op == 0 || op == -1) {
        return -1;
    }
    switch (menu_options) {
        default: {
            if (op > menu_options) {
                return -1;
            }
        }
    }
    return op;
}

int parameter_scanner(char *parameter) {
    /* Function that set a parameter for GET and POST data
    Returns a positive value if the parameter has been set, 0 if not and -1 for errors*/
    int check,i,ans;
    char aux[BUFSIZ];

    if (!fgets(aux,BUFSIZ,stdin)) {
        return -1;
    }
    check = 0;
    // Parser for GET and POST data
    for (i=0; aux[i]!='\0'; i++) {
        if (aux[i] == '=') {
            if (aux[i+1]!='\0' && (i-1)>=0) {
                check = 1;
            }
        }
    }
    if (check != 1) { // Not in the "name=value" format
        puts("WARNING: unknown data type entered (for default raw data the syntax is the following: \"name\"=\"value\")");
        puts("You may consider to add a new header specifying the data type (e.g. \"Content-Type: application/json\" for json data)");
        do {
            puts("Do you want to continue anyway? (y/n)");
            ans = ask_confirm();
        } while (ans == -1);
        if (ans == 1) { // y
            check = 2;
        }
        else { // n
            check = 0;
        }
    }

    aux[i-1] = '\0'; // Replace '\n' with '\0'
    strcpy(parameter,aux);
    return check;
}

int cookie_scanner(char *cookie) {
    /* Function that set a cookie from string or filename
    Returns 1 if the cookie has been set, 0 for invalid format and -1 for errors*/
    int check;
    char aux[BUFSIZ];
    FILE *file;

    check = scanf("%s",aux);
    clear_buffer();
    if (check == EOF) {
        return -1;
    }
    check = 0;
    // Parser cookie data
    for (int i=0; aux[i]!='\0'; i++) {
        if (aux[i] == '=') {
            if (aux[i+1]!='\0' && (i-1)>=0) {
                check = 1;
                strcpy(cookie,aux);
            }
        }
    }
    if (!check) { // Checks for an existing file
        if (file = fopen(aux,"r")) {
            fclose(file);
            strcpy(cookie,aux);
            check = 1;
        }
    }
    return check;
}

int header_scanner(char *header) {
    /* Function that set a new header
    Returns 1 if the header has been set, 0 for invalid format and -1 for errors*/
    int check,i;
    char aux[BUFSIZ];

    if (!fgets(aux,BUFSIZ,stdin)) {
        return -1;
    }
    check = 0;
    // Parser for header data
    for (i=0; aux[i]!='\0'; i++) {
        if (aux[i] == ':') {
            if (aux[i+1]!='\0' && (i-1)>=0 && aux[i-1]!=' ') {
                check = 1;
            }
        }
    }
    aux[i-1] = '\0'; // Replace '\n' with '\0'
    strcpy(header,aux);
    return check;
}

int filename_scanner(char *filename) {
    /* Function used to set a filename
    Returns 0 if the filename has been set, 1 if not and -1 in case of errors*/
    int check,op;
    char aux[BUFSIZ];
    FILE *file;

    check = scanf("%s",filename);
    clear_buffer();
    if (check == EOF) {
        printf("An error is occured while reading your input, try again\n");
        return -1;
    }
    // Check if that file already exists
    if (file = fopen(filename,"r")) {
        fclose(file);
        do {
            printf("There is already an existing file with that filename, do you want to replace it? (y/n)\n");
            op = ask_confirm();
        } while (op == -1);
        if (op == 2) {
            return 1; // Ask again for the filename
        }
    }
    return 0;
}

int set_GET_data() {
    /* Function that set a parameter for GET data
    Returns 1 if the parameter has been set, 0 if not and -1 for errors*/
    char buffer[BUFSIZ],new_option[BUFSIZ];
    int op,len,check;

    do {
        puts("Do you want to add a new GET parameter? (y/n)");
        op = ask_confirm();
    } while (op == -1);
    if (op == 2) {
        return 0;
    }
    do {
        puts("Enter the parameter (name=value)");
        check = parameter_scanner(buffer);
        if (check == -1) {
            puts("An error occured while reading your input\n");
        }
    } while (check < 1);

    if (flag_GET) { // At least one GET parameter already set
        len = strlen(options_v[ind_GET]);
        strcpy(new_option,options_v[ind_GET]);
        strcat(new_option," -d '");
        strcat(new_option,buffer);
        strcat(new_option,"'");
    }
    else { // First GET parameter
        ind_GET = options_c; // Salva l'indice
        strcpy(new_option,"-G -d '");
        strcat(new_option,buffer);
        strcat(new_option,"'");
        // Update the options vector
        options_c ++;
        options_v = (char**) realloc(options_v,options_c*sizeof(char*));
        if (!options_v) {
            printf("Dynamic allocation error\n");
            return -1;
        }
        flag_GET = true;
    }

    if (allocate(new_option,ind_GET) == -1) {
        printf("Dynamic allocation error\n");
        return -1;
    }
    return 1;
}

int set_POST_data() {
    /* Function that set a parameter for GET data
    Returns 1 if the parameter has been set, 0 if not and -1 for errors*/
    char buffer[BUFSIZ],new_option[BUFSIZ];
    int op,len,check;

    do {
        puts("Do you want to add a new POST parameter? (y/n)");
        op = ask_confirm();
    } while (op == -1);
    if (op == 2) {
        return 0;
    }
    do {
        puts("Enter the parameter (name=value)");
        check = parameter_scanner(buffer);
        if (check == -1) {
            puts("An error occured while reading your input\n");
        }
    } while (check < 1);

    if (flag_POST) { // At least one POST parameter already set
        len = strlen(options_v[ind_POST]);
        strcpy(new_option,options_v[ind_POST]);
        strcat(new_option," -d '");
        strcat(new_option,buffer);
        strcat(new_option,"'");
    }
    else { // First POST parameter
        ind_POST = options_c;
        strcpy(new_option,"-d '");
        strcat(new_option,buffer);
        strcat(new_option,"'");
        // Update the options vector
        options_c ++;
        options_v = (char**) realloc(options_v,options_c*sizeof(char*));
        if (!options_v) {
            printf("Dynamic allocation error\n");
            return -1;
        }
        flag_POST = true;
    }

    if (allocate(new_option,ind_POST) == -1) {
        printf("Dynamic allocation error\n");
        return -1;
    }
    return 1;
}

int set_cookie() {
    /* Function that set a cookie
    Returns 1 if the cookie has been set, 0 if not and -1 for errors*/
    char buffer[BUFSIZ],new_option[BUFSIZ];
    int op,len,check;

    do {
        puts("Do you want to add a cookie? (y/n)");
        op = ask_confirm();
    } while (op == -1);
    if (op == 2) {
        return 0;
    }
    do {
        puts("Enter the cookie (name=value/filename)");
        check = cookie_scanner(buffer);
        if (check == 0) {
            puts("Invalid format or filename, check your syntax and try again\n");
        }
        if (check == -1) {
            puts("An error occured while reading your input\n");
        }
    } while (check != 1);
    if (flag_cookie) { // Almost a cookie already set
        len = strlen(options_v[ind_cookie]);
        strncpy(new_option,options_v[ind_cookie],len-1);
        new_option[len-1] = '\0';
        strcat(new_option,";");
        strcat(new_option,buffer);
        strcat(new_option,"\"");
    }
    else { // First cookie
        ind_cookie = options_c;
        strcpy(new_option,"-b \"");
        strcat(new_option,buffer);
        strcat(new_option,"\"");
        // Update the options vector
        options_c ++;
        options_v = (char**) realloc(options_v,options_c*sizeof(char*));
        if (!options_v) {
            printf("Dynamic allocation error\n");
            return -1;
        }
        flag_cookie = true;
    }
    if (allocate(new_option,ind_cookie) == -1) {
        printf("Dynamic allocation error\n");
        return -1;
    }
    return 1;
}

int set_header() {
    /* Function that set a new http header
    Returns 1 if the header has been set, 0 if not and -1 for errors*/
        // Inizio funzione
    char buffer[BUFSIZ],new_option[BUFSIZ];
    int op,len,check;

    do {
        puts("Do you want to add a new header? (y/n)"); 
        op = ask_confirm();
    } while (op == -1);
    if (op == 2) {
        return 0;
    }
    do {
        puts("Enter the header (name:value)");
        check = header_scanner(buffer);
        if (check == 0) {
            puts("Invalid format, check your syntax and try again\n");
        }
        if (check == -1) {
            puts("An error occured while reading your input\n");
        }
    } while (check != 1);
    if (flag_header) { // At least an header already set
        len = strlen(options_v[ind_header]);
        strcpy(new_option,options_v[ind_header]);
        strcat(new_option," -H \"");
        strcat(new_option,buffer);
        strcat(new_option,"\"");
    }
    else { // First header
        ind_header = options_c;
        strcpy(new_option,"-H \"");
        strcat(new_option,buffer);
        strcat(new_option,"\"");
        // Update option vector
        options_c ++;
        options_v = (char**) realloc(options_v,options_c*sizeof(char*));
        if (!options_v) {
            printf("Dynamic allocation error\n");
            return -1;
        }
        flag_header = true;
    }
    if (allocate(new_option,ind_header) == -1) {
        printf("Dynamic allocation error\n");
        return -1;
    }
    return 1;
}

int set_verbose() {
    /* Function that set the verbose mode
    Returns 1 if the verbose mode has been enabled, 0 if not and -1 for errors*/
    char buffer[BUFSIZ],new_option[BUFSIZ];
    int op,len,check;

    do {
        puts("Do you want to enable verbose mode? (y/n)");  
        op = ask_confirm();
    } while (op == -1);
    if (op == 2) {
        if (flag_verbose) {
            // Remove the -v option
            strcpy(options_v[ind_verbose],"");
            flag_verbose = false;
        }
        return 0;
    }
    if (!flag_verbose) {
        ind_verbose = options_c; // Salva l'indice
        strcpy(new_option,"-v");
        // Update the options vector
        options_c ++;
        options_v = (char**) realloc(options_v,options_c*sizeof(char*));
        if (!options_v) {
            printf("Dynamic allocation error\n");
            return -1;
        }
        if (allocate(new_option,ind_verbose) == -1) {
            printf("Dynamic allocation error\n");
            return -1;
        }
        flag_verbose = true;
    }
    return 1;
}

int set_output_file() {
    /* Function that set an output file
    Returns a positive value if an output file has been set, 0 if not and -1 for errors*/
    char buffer[BUFSIZ],new_option[BUFSIZ];
    int op,len,check;

    do {
        puts("Do you want to save the output in a file? (y/n)");
        op = ask_confirm();
    } while (op == -1);
    if (op == 2) {
        if (flag_output) {
            // Remove the -o option
            strcpy(options_v[ind_output],"");
            flag_output = false;
        }
        return 0;
    }
    if (flag_output) { // Output file already set
        printf("An existing output file is already set (%s)\n",options_v[ind_output]);
        printf("Do you want to change it? (y/n)\n");
        // No
        do {
            op = ask_confirm();
        } while (op == -1);
        if (op == 2) {
            printf("\n");
            return 2;
        }
        // Replace previous output file with the new one
        do {
            printf("Enter the filename\n");
        } while (filename_scanner(buffer) != 0);
        strcpy(new_option," -o \"");
        strcat(new_option,buffer);
        strcat(new_option,"\"");
    }
    else { // Output file not set yet
        do {
            printf("Enter the filename\n");
        } while (filename_scanner(buffer) != 0);
        // Update the options vector
        strcpy(new_option,"-o \"");
        strcat(new_option,buffer);
        strcat(new_option,"\"");
        ind_output = options_c;
        options_c ++;
        options_v = (char**) realloc(options_v,options_c*sizeof(char*));
        if (!options_v) {
            printf("Dynamic allocation error\n");
            return -1;
        }
        flag_output = true;
    }
    if (allocate(new_option,ind_output) == -1) {
        printf("Dynamic allocation error\n");
        return -1;
    }
    return 1;
}

int allocate(char *option, int index) {
    // Dinamically allocate an option
    int len;

    len = strlen(option) + 2; // ' ' e '\0'
    options_v[index] = (char*) realloc(options_v[index],len*sizeof(char));
    if (!options_v[index]) {
        printf("Dynamic allocation error\n");
        return -1;
    }
    strcpy(options_v[index]," ");
    strcat(options_v[index],option);
    return 0;
}
