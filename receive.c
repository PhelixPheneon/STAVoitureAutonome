#include "receive.h"
#include "traj.h"

void * receive(void *arg) {
  
  printf("Thread \"recv_depuis_serveur\" lancé\n\n");
  
  struct PARAMS *params = (struct PARAMS *)arg;
  int sd = params->sd;
  char *ptr;
  int code;
  int x, y;
  char *x_coor, *y_coor;
  struct Point point;
  int bytesReceived;
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
        x_coor = strtok(NULL, ":");
        x = atoi(x_coor);
        y_coor = strtok(NULL, ":");
        y = atoi(y_coor);

        point.x = x;
        point.y = y;
        printf("Consigne reçue :\nx=%d\ny=%d\n", x, y);
        //Point chemin[154];
        params->chemin = (struct Point *)malloc(sizeof(struct Point) * 154);
        for(int i=0; i<params->nb_points; i++) {
            params->chemin[i].x = 0;
            params->chemin[i].y = 0;
        }
        Trajectory(params, point);

      } else {

        char *message = strtok(NULL, ":");
        printf("\nMessage reçu du serveur : %s\n", message);
      }
    }
  }
}