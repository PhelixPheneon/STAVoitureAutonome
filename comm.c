#include "comm.h"
#include "traj.h"

//receives string of the two final position values of the mission
struct Point * parse_point(char* x, char* y) {
    struct Point * point = (Point*)malloc(sizeof(Point));
    
    int xInt = atoi(x);
    int yInt = atoi(y);

    point->x = xInt;
    point->y = yInt;
    printf("Consigne reçue :\nx=%d\ny=%d\n", xInt, yInt);
    //Point chemin[154];
    
    return point;
}

void * receive_data(void * arg) {

    printf("Thread \"receive_data\" lancé\n\n");

    struct PARAMS *params = (struct PARAMS *)arg;
    int sd = params->sd;
    int bytesReceived;
    char buffer[MAX_OCTETS]; // un buff pour stocker les données recues
    pthread_t tid;
    request_t_data * handle_request_data;

    while (1) {

        bytesReceived = recvfrom(sd, buffer, sizeof(buffer), 0, NULL, NULL);// Recevoir des données du serveur
        //decrypte_client(buffer, strlen(buffer));

        if (bytesReceived < 0) {
          perror("Erreur lors de la réception");
          break;
        } else if (bytesReceived == 0) {
          printf("Connexion au serveur fermée\n");
          break;
        } else {

          //Copie des données dans une nouvelle structure à passer au thread
          handle_request_data = (request_t_data *) malloc(sizeof(request_t_data));
          handle_request_data->params = params;
          handle_request_data->request = (char *)malloc(sizeof(char)*(MAX_OCTETS+1));
          
          strcpy(handle_request_data->request,buffer);
          
          //Création du thread pour gérer la requête
          pthread_create(&tid, NULL, handle_request,(void *)handle_request_data);
        }
    }
    return EXIT_SUCCESS;
}

void * send_data(char data[MAX_OCTETS], struct PARAMS params) {
    int nbcar;
    nbcar=sendto(params.sd, data,MAX_OCTETS+1,0,(const struct sockaddr *) params.server_adr,sizeof(*params.server_adr));
    CHECK_ERROR(nbcar,0,"\nErreur lors de l'émission des données");
    return EXIT_SUCCESS;
}

void * handle_request(void * arg) {
    int code;
    int resp_code = 0;
    char *ptr;
    char resp[MAX_OCTETS+1];
    struct Point * mission = (Point*)malloc(sizeof(Point));

    request_t_data * data = (request_t_data *) arg;
    ptr = strtok(data->request, ":");

    // Analyse du code de requête
    code = atoi(ptr);
    switch (code) {
      case 105: 
        mission = parse_point(strtok(NULL,":"),strtok(NULL,":")); //apparamment il faut mettre NULL pour qu'il continue avec le meme buffer
       //generates the trajectory and stores it in params
        Trajectory(data->params, *mission);
        resp_code = 205;
        itoa(resp_code, resp);
        break;
      default:
        resp_code = 500;
        itoa(resp_code, resp);
        break;
    }
  
  // On envoie la réponse
  send_data(resp, *data->params);
  pthread_exit(NULL);
  return EXIT_SUCCESS;
}

void itoa(int val, char * dest) {
    dest[0] = '\0';
    sprintf(dest, "%d", val);
}