#include "main.h"

struct Point * parse_point(char* x, char* y);

typedef struct request_t_data {
        char * request;
        struct PARAMS * params;
        } request_t_data;

void* receive(void* arg);
