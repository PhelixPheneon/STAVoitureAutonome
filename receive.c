#include "receive.h"
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

void * receive(void *arg) {
  
    printf("Thread \"recv_depuis_serveur\" lancé\n\n");

    struct PARAMS *params = (struct PARAMS *)arg;
    int sd = params->sd;
    char *ptr;
    int code;
    int bytesReceived;
    struct Point * mission = (Point*)malloc(sizeof(Point));
    char buffer[1024]; // un buff pour stocker les données recues

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
          buffer[bytesReceived] = '\0'; // ajoutez un caractère nul

          ptr = strtok(buffer, ":"); // Analyse du code de requête
          code = atoi(ptr);

          if (code == 105) {
            mission = parse_point(strtok(NULL,":"),strtok(NULL,":")); //apparamment il faut mettre NULL pour qu'il continue avec le meme buffer
            
            //generates the trajectory and stores it in params
            Trajectory(params, *mission);
          } else {

            char *message = strtok(NULL, ":");
            printf("\nMessage reçu du serveur : %s\n", message);
          }
        }
    }
}
