#include "main.h"

struct Point * parse_point(char* x, char* y);

typedef struct request_t_data {
        char * request;
        struct PARAMS * params;
        } request_t_data;

void* receive_data(void* arg);

void * send_data(char data[MAX_OCTETS], struct PARAMS params);

void * handle_request(void * arg);

void itoa(int val, char * dest);
