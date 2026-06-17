#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

size_t LEN = 1000;


struct IP {
	uint8_t oct[4];
};

int main(void) {

	FILE *f = fopen("sample.txt", "r");
	if (!f) return -1;

	// Initialize buffer to read a single line from the file

	char *ln = (char*) malloc(sizeof(char)*LEN);
	ssize_t status = 0;

	while ((status = getline(&ln, &LEN, f)) != -1) {

		// Copy string since tokenization is destructive to the original string
	
		size_t ln_len = strlen(ln);

		char *ln_cpy = (char*) malloc(ln_len + 1);
		strncpy(ln_cpy, ln, ln_len);
		ln_cpy[ln_len] = '\0';

		// tokenize the copied string

		char *save;
		const char delim[] = ",";
		char *token = strtok_r(ln_cpy, delim, &save);

		while (token != NULL) {
			
			// At this stage individual IPs are extractible, need to implement range extraction

			if(strchr(token, '-') != NULL) {
				
				// RANGE FOUND!!! Begin extraction

				printf("Range found: %s\n", token); 

				// Copy token since tokenization is destructive

				size_t token_len = strlen(token);
				char *token_cpy = (char*) malloc(token_len+1);
				strncpy(token_cpy, token, token_len);
				token_cpy[token_len] = '\0';

				printf("TOKEN CPY %s\n", token_cpy);


				// WORKS UPTILL HERE

				char *ip_delim = "-";
				char *save_tok;
				char *ip_initials = strtok_r(token_cpy, ip_delim, &save_tok);

				while(ip_initials != NULL) {
					puts("RANGE BROKEN DOWN");
					printf("IP_STARTS: %s", ip_initials);
					ip_initials = strtok_r(NULL, ip_delim, &save_tok);
				}

			} else {

				puts(token);
			}

			token = strtok_r(NULL, delim, &save);

		}


	}


	return 0;
}
