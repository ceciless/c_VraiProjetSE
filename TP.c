#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "TP.h"
#include "fun_targa.h"


/*------------------------------------------------------------------------------------
 * Fonctions
 *------------------------------------------------------------------------------------
 */

/* acquisition des fichiers targa contenus dans un repertoire passe en parametre */
List * acquerir(const char * chemin){

	DIR * rep =  opendir(chemin); //ouverture du dossier
	if (rep==NULL){ //Si le dossier n'a pas pu etre ouvert
		printf("Chemin non valide ! \n\n\n");
		exit(1);
	}
	
	struct dirent * lecture = NULL;
	List *list = malloc(sizeof(List)); // tete de liste
	List *node = NULL; // noeud courant
	List * prev = NULL; // noeud precedent
	
	printf("La liste est: \n\n");
	
	
	while((lecture=readdir(rep))!=NULL){ //parcourir les fichiers du repertoire
		int len = strlen(lecture->d_name);
		if (!strcmp(lecture->d_name+(len-4),".tga")){ //"choisir" les fichiers .tga
		
			ImageInfo image;

			FILE * f=fopen(lecture->d_name, "r");
			
			
			if(f==NULL){
				printf("FIN\n");}
			else {
				targa_header * header= malloc(sizeof(targa_header));
				fread(header, sizeof(targa_header), 1, f);

				image.nomFich = lecture->d_name; // nom du fichier
				
		    printf("%s\n",image.nomFich);
				fseek(f, 0, SEEK_END); // taille du fichier
				image.tailleFich = ftell(f)/1024;

				image.largeImg = header->width; // largeur et longueur
				image.longImg = header->height;
		    
				if (node==NULL){  // si premier noeud
					
					node = list;
					node->data=image;
					node->next=NULL;
					prev = node;
				}
				else
				{
					node = malloc(sizeof(List));
					prev->next= node;
				  	node->data=image;
					node->next=NULL;
					prev=node;
				}
  		}
		}
	}
	
	closedir(rep);
	return list;
}

/* affichage de la liste contenant les informations des fichiers targa */
void affichage(List *list){
	
	List * maListe = list;
	ImageInfo image = maListe->data;
	
	if(maListe!=NULL){
		printf("Fichier %s Taille: %ld Ko  Dimensions: (%d, %d).\n", image.nomFich, image.tailleFich, image.largeImg, image.longImg);
		maListe=maListe->next;
	}

	while(maListe!=NULL){	
    image= maListe->data;
		printf("Fichier %s Taille: %ld Ko  Dimensions: (%d, %d).\n", image.nomFich, image.tailleFich, image.largeImg, image.longImg);
		maListe=maListe->next;
	}
}


/* affichage de l'histogramme par plan de couleur*/
double * histColorPlan (image_desc * img, char * fName, int colorplan){

    double *TabHist = malloc(img->width*sizeof(double));
    int i,j;
    int c;
    int planeSize;
    int somme = 0;
    double percent;
		int compteur = 0;

    targa_header *i_head = malloc(sizeof(targa_header));
    c = readImage(img, i_head,fName);
    planeSize = sizeof(uint8_t) * img->height * img->width;
    
    for (i=0; i<img->width; i++) {
				TabHist[i] = 0;
    }
    
    if (colorplan == 1){
        printf("Plan de couleur : Bleu; hauteur : %d\n\n",img->height);
      
        for (i=0; i<img->width; i++) {
						compteur=0;
            for (j=0; j<img->height; j++) {
								
                if (img->pBlue[i+j*img->width] == 255){
                	compteur++;
                }
            }
           	TabHist[i] = 100*(double)compteur/img->height;
        }
    }
     
    if (colorplan == 2){
        printf("Plan de couleur : Vert\n\n");

        for (i=0; i<img->width; i++) {
						compteur=0;
            for (j=0; j<img->height; j++) {
                if (img->pGreen[i+j*img->width] == 255){
                	compteur++;
                }
            }
           	TabHist[i] = 100*(double)compteur/img->height;
        }
     }
     
    if (colorplan == 4){
        printf("Plan de couleur : Rouge\n\n");
        
        for (i=0; i<img->width; i++) {
						compteur = 0;
            for (j=0; j<img->height; j++) {
                if (img->pRed[i+j*img->width] == 255){
                	compteur++;
                }
            }
           	TabHist[i] = 100*(double)compteur/img->height;
        }
    }
		
		printf("\nHistogramme sur plan couleur effectue, Voici quelques valeurs\n\n");
		
		for(i=0; i < 20; i++){
			printf("TabHist[%d] = %.2lf% \n", i, TabHist[i]); //Affichage de l'histogramme calculé, 2 chiffres après la virgule
		}
		
return TabHist;
}

/* affichage des histogrammes des trois plans de couleur*/
double ** histCombinated(image_desc * img, char * fName){

    double **TabHist;
    int i,j;
    int *histo = malloc(img->width*sizeof(int));
    int c;
    int planeSize;
    int somme = 0;
    double percent;
		int compteur;
	  
		
    targa_header *i_head = malloc(sizeof(targa_header));
    c = readImage(img, i_head,fName);
    planeSize = sizeof(uint8_t) * img->height * img->width;
    
    TabHist = malloc(3*sizeof(double)); // allocation de la première dimension
    for (i=0; i<3; i++){
        TabHist[i] = malloc(img->width*sizeof(double)); // allocation de la deuxième dimension
    }    
  
				// On met toutes les cases à 0
		for(i = 0; i<3; i++){
		  for (j=0; j<img->width; j++) {
		      histo[j] = 0;
					TabHist[i][j] = 0;																	
		  }
		}
    
    	//Calcul de l'histogramme pour les plans bleu, vert et rouge

	  for (i=0; i<img->width; i++) {
				compteur=0;
	      for (j=0; j<img->height; j++) {												//PLAN BLEU
					
	          if (img->pBlue[i+j*img->width] == 255){
	          	compteur++;
	          }
	      }
	     	TabHist[0][i] = 100*(double)compteur/img->height;
	  }


    compteur = 0;
    
    for (i=0; i<img->width; i++) {
				compteur=0;
        for (j=0; j<img->height; j++) {												//PLAN VERT
            if (img->pGreen[i+j*img->width] == 255){
            	compteur++;
            }
        }
       	histo[i] = compteur;
       	TabHist[1][i] = 100*(double)compteur/(double)img->height;

    }

    compteur = 0;

    for (i=0; i<img->width; i++) {
				compteur = 0;
        for (j=0; j<img->height; j++) {												//PLAN ROUGE
            if (img->pRed[i+j*img->width] == 255){
            	compteur++;
            }
        }
       	histo[i] = compteur;
       	TabHist[2][i] = 100*(double)compteur/(double)img->height;
    }

	for(i = 0; i<3; i++){
				for(j = 0; j<20; j++){
						printf("TabHist[%d][%d] = %.2lf% \n", i,j, TabHist[i][j]); //Affichage de l'histogramme calculé, 2 chiffres après la virgule  
				}
	}
    return TabHist;
}

/*affichage de l'histogramme  avec taux de présence de chaque couleur*/

double * histTauxPC (image_desc * img, char * fName){

    int *histo = malloc(img->width*sizeof(int));
    int compteur = 0;
    double *histTPC = malloc(3*sizeof(double));
    int i,j;
    int c;
    int planeSize;
    int somme1, somme2, somme3;
		somme1 = somme2 = somme3 = 0;
    
    double percent;

    targa_header *i_head = malloc(sizeof(targa_header));


    c = readImage(img, i_head,fName);
    planeSize = sizeof(uint8_t) * img->height * img->width;
    
    for (i=0; i<3; i++) {
        histTPC[i] = 0;
    }

    //Partie bleue
       
    for (i=0; i<img->width; i++) {
    		histo[i] = 0;
    		compteur=0;
        for (j=0; j<img->height; j++) {
            if (img->pBlue[i+j*img->width] == 255){
            	compteur++;
            }
        }
        histo[i] = compteur;
       	somme1 = somme1 + histo[i];				
    }
    
    percent = 100*somme1/(double)planeSize;    
    histTPC[0]= percent;
 
		printf("\nLe pourcentage de bleu: %lf% \n", histTPC[0]);
		  
    //Partie verte
    
    compteur=0;
    
    for (i=0; i<img->width; i++) {
    		histo[i] = 0;
    		compteur=0;
        for (j=0; j<img->height; j++) {
            if (img->pGreen[i+j*img->width] == 255){
            	compteur=compteur+1;
            }
        }
        histo[i] = compteur;
       	somme2 = somme2 + histo[i];
    }
    
    percent = 100*somme2/(double)planeSize;//((int)(img->width * img->height));
    histTPC[1] = percent;

		printf("Le pourcentage de vert: %lf% \n", histTPC[1]);
		     
    //Partie rouge
    
		compteur=0;    
    for (i=0; i<img->width; i++) {
    		histo[i] = 0;
    		compteur=0;
        for (j=0; j<img->height; j++) {
            if (img->pRed[i+j*img->width] == 255){
            	compteur=compteur+1;
            }
        }
        histo[i] = compteur;
       	somme3 = somme3 + histo[i];
    }
    
    percent = 100*somme3/(double)planeSize;
    histTPC[2] = percent;
    
		printf("Le pourcentage de rouge: %lf% \n", histTPC[2]);

return histTPC;
}

/*affichage des images de tailles supérieures ou inférieures à une valeur*/
double *histRepImgTaille(List * list, int taille){

    double taux1 ;
    double taux2 ;
		int size ;
    double *histo = malloc(2*sizeof(double));
		int i;
    
    printf("\nHistogramme taux images inférieures a %dKo\n", taille);
    
    while (list!= NULL){
        if (list->data.tailleFich <= taille){
						printf("taille %d\n", list->data.tailleFich);
            taux1 = taux1 + 1;
						size = size + 1;
        }
        else{
						printf("taille %d\n", list->data.tailleFich);
            taux2 = taux2 + 1;
						size = size + 1;
        }
        list = list->next;
    }
    histo[0] = 100*taux1/(double)size;
    histo[1] = 100*taux2/(double)size;
    
	  for(i = 0; i<2; i++){
			printf("\nHisto[%d] = %.2lf %\n", i, histo[i]);
		}
    return histo;
}

