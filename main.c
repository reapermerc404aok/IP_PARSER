#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

size_t LEN = 1000;
size_t OCT_MAX = 255;


struct IP {
    uint8_t oct[4];
};

void display_ip(struct IP *ip) {
    //printf("%d.%d.%d.%d\n", ip->oct[0], ip->oct[1], ip->oct[2], ip->oct[3]);
    
    // ENDINAN CORRECTNESS

    printf("%d.%d.%d.%d\n", ip->oct[3], ip->oct[2], ip->oct[1], ip->oct[0]);
}

void extractor(struct IP *start, struct IP *end) {

	//Some bit magic would be better, do IP to binary, then increment and print the binary till they equal HEHE
	
	// Extract IP bytes from the memory void

	void *dest = (void*) malloc(sizeof(uint32_t));
	memcpy(dest, start, 4);
	uint32_t *ip1 = (uint32_t*) dest;

	void *dest2 = (void*) malloc(sizeof(uint32_t));
	memcpy(dest2, end, 4);
	uint32_t *ip2 = (uint32_t*) dest2;


	struct IP *tmp = (struct IP*)ip1;
	struct IP *tmp2 = (struct IP*)ip2;

	assert(*ip1 < *ip2);
	printf("Count %d\n", abs(*ip1 - *ip2));
	while(*ip1 < *ip2) {
		display_ip(tmp);
		*ip1 = *ip1 + 1;
	}

	display_ip(tmp);
	display_ip(tmp2);
	


}

void range_incrementer(char *start, char *end) {

	// Start and END IPs are already extracted. For example start will be "10.10.10.10" and end will "10.10.15.20"

	// Extract start IP
	char *oct_delim = ".";
	char *save_tok;
	char *oct4start = strtok_r(start, oct_delim, &save_tok);

	// Initialize IP object with octet values
	
	// STORING IPs in the array in REVERSE for ENDIAN CORRECTNESS
	struct IP *ip_start = (struct IP*) malloc(sizeof(struct IP));
	int i = 3;

	while(oct4start != NULL) {
		ip_start->oct[i--] = atoi(oct4start);
		oct4start = strtok_r(NULL, oct_delim, &save_tok);
	}

	char *save_tok2;
	char *oct4end = strtok_r(end, oct_delim, &save_tok2);

	// STORING IPs in the array in REVERSE for ENDIAN CORRECTNESS
	struct IP *ip_end = (struct IP*) malloc(sizeof(struct IP));
	i = 3;

	while(oct4end != NULL) {
		ip_end->oct[i--] = atoi(oct4end);
		oct4end = strtok_r(NULL, oct_delim, &save_tok2);
	}

	printf("start IP: "); display_ip(ip_start);
	printf("end IP: "); display_ip(ip_end);

	extractor(ip_start, ip_end);

}


int range_handler(char *token_cpy) {

	// Range handler should filter out the start and end of the range, then pass it to an IP incrementer for printing individual IPs.

	char *ip_delim = "-";
	char *save_tok;
	char *ip_start = strtok_r(token_cpy, ip_delim, &save_tok);

	puts("RANGE BROKEN DOWN");
	char *ip_end = strtok_r(NULL, ip_delim, &save_tok);

	range_incrementer(ip_start, ip_end);

	return 0;
}


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

				// Pass token copy to range handler

				int res = range_handler(token_cpy);


			} else {

				puts(token);
			}

			token = strtok_r(NULL, delim, &save);

		}


	}


	return 0;
}
