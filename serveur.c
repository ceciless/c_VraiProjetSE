#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "strhelpers.h"
#include "TP.h"
#include "fun_targa.h"
#define MAXNAME 10
#define MAXTEXT 100

void read_header(int sock, char * username);
void print_msg(char *talker, char * chat);
void send_img(int socket);
void transforme_image(char * argtab[]);


int main(int argc, char * argv[])
{

      int socket_RV, socket_service, socket_talk, socket_RV_talk;
      int pidFils, pidparent;
      
      int port;
      int talkport = 1111; // Fixe
      char nom[30];
      char commandeWrite[80];
      pidparent = getpid();
      struct sockaddr_in adr, adresse;
      socklen_t lgadresse=0;
      if (argc!=2)
      {
        fprintf(stderr,"Usage : %s port-number\n", argv[0]);
        exit(1);
      }

      if (gethostname(nom, 30)==-1) 
      {
        perror("Impossible de recuperer le nom de l'hote");
        exit(1);
      }
      /* ----------------------------------------------------------- 
       * Connexion pour la manipulation distante d'images 
       * ----------------------------------------------------------- */ 
      if ((socket_RV=socket(AF_INET, SOCK_STREAM, 0)) ==-1)
      {
        perror("socket rendez-vous");
        exit(1);
      }

      // Construction de l'adresse
      port = atoi(argv[1]);
      adr.sin_family=AF_INET;
      adr.sin_port=htons(port);
      adr.sin_addr.s_addr = htonl(INADDR_ANY);
      // Liaison
      if (bind(socket_RV, (struct sockaddr *) &adr, sizeof(adr))==-1)
      {
        perror("Impossible d'etablir une liaison");
        exit(1);
      }
      // Ecoute
      if (listen(socket_RV,1)==-1)
      {
        perror("Impossible d'ecouter");
        exit(1);
      }
      socket_service=accept(socket_RV, (struct sockaddr *)&adresse, &lgadresse);
      close(socket_RV);
      printf("Connexion service etablie. User: %s - %d; Machine: %s\n", getlogin(), geteuid(), nom);

      /* ----------------------------------------------------------- 
       * Connection pour parler et echanger des donnees textuelles 
       * ----------------------------------------------------------- */
      if ((socket_RV_talk=socket(AF_INET, SOCK_STREAM, 0)) ==-1) {
        perror("socket rendez-vous");
        exit(1);  
      }
      // Construction de l'adresse
      adr.sin_family=AF_INET;
      adr.sin_port=htons(talkport);
      adr.sin_addr.s_addr = htonl(INADDR_ANY);
      // Liaison
      if (bind(socket_RV_talk, (struct sockaddr *) &adr, sizeof(adr))==-1) {
        perror("Impossible d'etablir une liaison");
        exit(1);
      }
      // Ecoute
      if (listen(socket_RV_talk,1)==-1) {
        perror("Impossible d'ecouter");
        exit(1);
      }
      socket_talk=accept(socket_RV_talk, (struct sockaddr *)&adresse, &lgadresse);
      close(socket_RV_talk);

      printf("Connexion talk etablie sur machine: %s\n", nom);


    char c = 'X';
    char * d="Sur Quel plan?\n";
		char * e="Quelle taille ?\n";
    char *talker = (char*)malloc(MAXNAME);
    char *chat =  (char*)malloc(MAXTEXT);
    char *begchat;
    int code, colorplan;

    image_desc * img=malloc(sizeof(image_desc));

    double* hist = malloc(img->width*sizeof(double));
    double** histo = malloc(3*sizeof(double)); //Allocation mémoire de la première dimension du tableau
		double* histRep = malloc(2*sizeof(double));
    List * list=NULL;
    char * fileName="./input/ensta.tga";
    char * fname;
    int i;
		int taille;
    FILE * fp;
    ssize_t csize;

    read_header(socket_talk, talker);
    printf("%s is connected\n", talker);
    char cwrite;

    for (i=0; i<3; i++){
    	histo[i] = malloc(img->width*sizeof(double)); // Allocation mémoire de la deuxième dimension du tableau
    }
  
    while(c!=EOF) {
		begchat = chat;
		
		//Dialogue avec le client//	
        printf("\nFaites un choix \n\n");
        printf(" 1: Acquisition d images \n 2: Affichage des détails \n 3: Faire histogramme sur un des plans de couleur (a choisir:bleu, vert ou rouge) \n 5: Histogramme combiné \n 6: Histogramme taux de presence de chaque couleur \n 7 : histogramme de répartition des images par taille \n 8 : Quitter \n\n");
    
		code = 0;
		taille = 0;
		colorplan = 0;
  
		do {
			read(socket_talk, &c, 1);
			*chat = c;
			chat++;
		} while (c!='\n');
		*(chat) = '\0';
		chat = begchat;
		print_msg(talker, chat);
      		
		if(strcmp(chat, "1\n") == 0){
		    code = 1;}
		if(strcmp(chat, "2\n") == 0){
		    code = 2;}
		if(strcmp(chat, "3\n") == 0){
		    code = 3;}
		if (strcmp(chat, "bleu\n") == 0){
		    code = 4; colorplan = BLUEPLANE;}
		if (strcmp(chat, "vert\n") == 0){
		    code = 4; colorplan = GREENPLANE;}
		if (strcmp(chat, "rouge\n") == 0){
		    code = 4; colorplan = REDPLANE;}
		  
		if(strcmp(chat, "5\n") == 0){
		    code = 5;}
		if(strcmp(chat, "6\n") == 0){
		    code = 6;}
		if(strcmp(chat, "7\n") == 0){
		    code = 7;}
		if(atoi(chat)>= 10){
		    taille=atoi(chat);
				code=7;}
		if(strcmp(chat, "8\n") == 0){
			printf("FIN DU PROGRAMME \n");
			break;}
		    
		printf("Le code est: %d plan %d, taille : %d\n\n", code, colorplan, taille);      
		 
		    switch(code){
				case 0:
				printf("Essaye encore !\n\n");
				break;
		    case 1:	
				list = acquerir("./input/"); // Comme ceci ca marchera peut etre mieux, sinon on fait comme d'hab
		      break;
		    case 2:
		      	affichage(list);
		      break;
		    case 3:	
				write(socket_talk, d, 15);
		      break;
			case 4:
				hist=histColorPlan(img, fileName, colorplan);
				break;
			case 5:		
				histo=histCombinated(img, fileName);
				break;
			case 6:
				hist=histTauxPC(img, fileName);                 //Pour un seul fichier spécifié au dessus
				break;
			case 7 :		
				if(taille==0){
					write(socket_talk, e, 16);
					printf("histoRep\n");
				}
				else{
					histRep=histRepImgTaille(list, taille);
				}
				break;								   			    
		  default:
		      printf("echec \n");}

		if (startswith("envoie",chat)) {
			printf("Je vais t'envoyer une image\n");
			send_img(socket_service);
		}	
		else if (startswith("info", chat)) {
			do
			{
				cwrite=getchar();
				write(socket_talk, &cwrite, 1);
			}
			while (cwrite!='\n'); 
		}
		else {
			printf("\nRien de special\n");
		}
  }
  close(socket_service);
  close(socket_talk);	  
  return 0;
}


void send_img(int socket_service) {
  int written_size;
  int fd = open("./img_src/ensta.tga", O_RDONLY);
  long img_size = lseek(fd, 0L, SEEK_END);
  lseek(fd, 0L, SEEK_SET);
  char *buffer = malloc(img_size);
  int readinfile = read(fd, buffer, img_size);
  close(fd);
  write(socket_service, &img_size, sizeof(long)); 
  printf("Taille de l'image : %ld\n", img_size);
  do {
		written_size = write(socket_service, buffer,img_size);
		printf("Envoi termine. Taille (octets): %ld. Lu = %d, Envoye = %d\n", 
					 img_size, readinfile, written_size);
  } while (written_size<img_size && written_size<=0);
  printf("Envoi termine : %d.\n", written_size);
}


void print_msg(char *talker, char * chat) {
  fprintf(stdout, "%s-dit-au-serveur:%s\n", talker, chat);
  fflush(stdout);
}

void read_header(int sock, char * username) {
  int loglen ;
  read(sock, &loglen, 1);
  read(sock, username, loglen);
}



