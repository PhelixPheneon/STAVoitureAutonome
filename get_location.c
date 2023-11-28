
#include "get_location.h"

/////////SHIT I DONT UNDERSTAND//////////
/////////////////////////////////////////
/////////////////////////////////////////
bool terminateProgram=false;
void CtrlHandler(int signum)
{
    terminateProgram=true;
}
// Linux
static sem_t *sem;
struct timespec ts;
void semCallback()
{
    sem_post(sem);
}
/////////SHIT I DONT UNDERSTAND//////////
/////////////////////////////////////////
/////////////////////////////////////////

struct MarvelmindHedge * setupHedge(int argc, char * argv[]) {
    // get port name from command line arguments (if specified)
    const char * ttyFileName;
    if (argc == 4) ttyFileName = argv[3];
    else ttyFileName = DEFAULT_TTY_FILENAME;

    // Init
    struct MarvelmindHedge * hedge = createMarvelmindHedge();
    if (hedge==NULL)
    {
        puts ("Error: Unable to create MarvelmindHedge");
        exit(EXIT_FAILURE);
    }
    hedge->ttyFileName=ttyFileName;
    hedge->verbose=true; // show errors and warnings
    hedge->anyInputPacketCallback= semCallback;
    startMarvelmindHedge(hedge);
  
    sem = sem_open(DATA_INPUT_SEMAPHORE, O_CREAT, 0777, 0);

    /*//sleep (3);
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        printf("clock_gettime error");
        return -1;
    }
    ts.tv_sec += 2;
    sem_timedwait(sem,&ts);
    */
    return hedge;
}

void * get_location (void* arg) {
    struct PARAMS * params = (struct PARAMS*)arg;
    struct MarvelmindHedge * hedge = params->hedge;
    struct PositionValue * mobPos; //position du mobile
    clock_t start;
  
    while(1) {
        start = clock();
        mobPos = (struct PositionValue *)malloc(sizeof(struct PositionValue));
        
        //debug mode without marvelminds
        if (DEBUG != 1) {
            getPositionFromMarvelmindHedge(hedge, mobPos);
        } 

        free(params->pos);
        params->pos = mobPos;
        params->currentPoint.x = mobPos->x;
        params->currentPoint.y = mobPos->y;

        attendre(start, 500.0);
    }
}
