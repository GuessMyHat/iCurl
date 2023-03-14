#include "header.h"

int main() {
	const char BIN[] = "curl";
	int len,op,ret_code;
	char buffer[BUFSIZ];
	char *command; // contiene il comando completo di tutte le opzioni da eseguire

	// Inizializza il vettore con le opzioni
	options_c = 1;
	options_v = (char**) malloc(options_c*sizeof(char*));
	len = strlen(BIN) + 1;
	options_v[0] = (char*) malloc(len*sizeof(char));
	strcpy(options_v[0],BIN);

	// Stampa il titolo e una breve descrizione del programma
	print_title();
	printf("\nWelcome to iCurl (LITE)\n");
	printf("iCurl (Interactive Curl) is a simple tool that allows you to use the well known command line tool curl in a interactive way.\n");
	printf("Curl can be used to send and receive data, manage cookies, download files (...) via many different protocols (e.g. HTTP,FTP,LDAP). Curl is widely used in web programming to automate HTTP requests and interact with web API services.\n");
	printf("iCurl was born to simplify all these operations through the user interaction, especially for who is not very familiar with the command line arguments.\n");
	printf("This is a LITE version: it means that it doesn't support all the operation that curl can natively does, actually it works just for basic HTTP requests.\n\n\n");
	
	puts("Press any keys to start");
	clear_buffer();
	system("clear");

	do {
		puts("Select the HTTP method you want to use:");
		puts("1: GET");
		puts("2: POST");
		puts("3: OPTIONS");
		printf("> ");
		if ((op = menu_scanner(3)) == -1) {
			printf("Please enter a valid option and retry\n\n");
		}
	} while (op == -1);

	switch (op) {
		case(1): // GET
		{
			strcpy(buffer,"-X GET");
			printf("GET method selected\n\n");
			break;
		}
		case(2): // POST
		{
			strcpy(buffer,"-X POST");
			printf("POST method selected\n\n");
			break;
		}
		case(3): // OPTIONS
		{
			strcpy(buffer,"-X OPTIONS");
			printf("OPTIONS method selected\n\n");
			break;
		}
	}
	
	// Update the options vector
	options_c ++;
	options_v = (char**) realloc(options_v,options_c*sizeof(char*));
	if (allocate(buffer,1) == -1) {
		printf("Dynamic allocation error\n");
		puts("Exiting...");
		return -1;
	}

	if (op == 1) { // GET options menu
		do {
			puts("GET features menu:");
			puts("1: Specify GET parameters");
			puts("2: Add a new cookie to the request");
			puts("3: Go to next menu");
			printf("> ");
			op = menu_scanner(3);
			// Loop until you don't skip to next menu
			switch (op) {
				case(1): { // GET parameters
					if ((ret_code = set_GET_data()) == 1) {
						printf("GET parameter successfully set! (%s)\n\n",options_v[ind_GET]);
					}
					op = -1;
					break;
				}
				case(2): { // Cookie
					if ((ret_code = set_cookie()) == 1) {
						printf("Cookie successfully set! (%s)\n\n",options_v[ind_cookie]);
					}
					op = -1;
					break;
				}
				case(3): { // Skip to next menu
					printf("Leaving GET features menu...\n\n");
					break;
				}
				default: {
					printf("Please enter a valid option and retry\n\n");
					op = -1;
				}
			}
			if (ret_code == -1) {
				printf("Exiting...\n");
				return -1;
			}
		} while (op == -1);
	}

	if (op == 2) { // POST options menu
		do {
			puts("POST features menu:");
			puts("1: Specify POST data to send");
			puts("2: Add a new cookie to the request");
			puts("3: Go to next menu");
			printf("> ");
			op = menu_scanner(3);
			// Loop until you don't skip to next menu
			switch (op) {
				case(1): { // POST data
					if ((ret_code = set_POST_data()) == 1) {
						printf("POST parameter successfully set! (%s)\n\n",options_v[ind_POST]);
					}
					op = -1;
					break;
				}
				case(2): { // Cookie string
					if ((ret_code = set_cookie()) == 1) {
						printf("Cookie successfully set! (%s)\n\n",options_v[ind_cookie]);
					}
					op = -1;
					break;
				}
				case(3): { // Skip to next menu
					printf("Leaving POST features menu...\n\n");
					break;
				}
				default: {
					printf("Please enter a valid option and retry\n\n");
					op = -1;
				}
			}
			if (ret_code == -1) {
				printf("Exiting...\n");
				return -1;
			}
		} while (op == -1);
	}

	do { // Additional features menu (all methods compatible)
		puts("Additional features menu:");
		puts("1: Add HTTP header");
		puts("2: Verbose mode");
		puts("3: Save output");
		puts("4: Enter the URL and submit");
		printf("> ");
		op = menu_scanner(4);
		// Loop until you don't skip to next menu
		switch (op) {
			case(1): { // Add HTTP header
				if ((ret_code = set_header()) == 1) {
					printf("Header successfully added! (%s)\n\n",options_v[ind_header]);
				}
				op = -1;
				break;
			}
			case(2): { // Verbose mode
				ret_code = set_verbose();
				if (ret_code == 1) {
					printf("Verbose mode enabled (%s)\n\n",options_v[ind_verbose]);
				}
				else if (ret_code == 0) {
					printf("Verbose mode disabled\n\n");
				}
				op = -1;
				break;
			}
			case(3): { // Output file
				ret_code = set_output_file();
				if (ret_code == 1) {
					printf("Output file successfully set! (%s)\n\n",options_v[ind_output]);
				}
				else if (ret_code == 0) {
					printf("The output won't be saved in a file\n\n");
				}
				op = -1;
				break;
			}
			case(4): { // Skip to URL
				break;
			}
			default: {
				printf("Please enter a valid option and retry\n\n");
				op = -1;
			}
		}
		if (ret_code == -1) {
			printf("Exiting...\n");
			return -1;
		}	
	} while (op == -1);

	// Ask for the URL
	printf("\nEnter the URL\n");
	scanf("%s",buffer);
	clear_buffer();

	// Allocate space for the last option (URL)
	options_c ++;
	options_v = (char**) realloc(options_v,options_c*sizeof(char*));
	if (!options_v) {
		printf("Dynamic allocation error\n");
		return -1;
	}
	if (allocate(buffer,options_c-1) == -1) {
		printf("Dynamic allocation error\n");
		return -1;
	}

	// Assemble the HTTP request
	len = 0;
	for (int i=0; i<options_c; i++) {
		len += strlen(options_v[i]);
	}
	command = (char*) malloc((len+1)*sizeof(char));
	for (int i=0; i<options_c; i++) {
		strcat(command,options_v[i]);
	}
	printf("\nHTTP request created (%s)\n",command);

	printf("Submitting your request...\n\n");
	sleep(1); // ALT the CPU for 1 sec

	puts("########## Server Response ##########");
	system(command);
	printf("\n");
	
	return 0;
}