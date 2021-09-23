
#define MAX_PRINT 5
#define FALSE 0
#define TRUE 1
#define MAX_STR_LEN 120
#define UNKNOWN_ENDIAN  0000

//FILE *open_file(char *filename, char*extension, char *access);

float compare(float value, float liminf, float limsup);

float evaluate_event(int mode, float wtin, float ein, float minweight, float maxweight, float minenergy, float maxenergy);

//int check_byte_order(void);

void print_help();
