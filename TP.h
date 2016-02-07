#include <stdint.h>
#include "mem_targa.h"

/*------------------------------------------------------------------------------------
 * Structures
 *------------------------------------------------------------------------------------
 */

/* structure contenant les informations concernant un fichier targa */
typedef struct ImageInfo_ {
	char * nomFich ; // nom du fichier contenant l'image
	int tailleFich ; // taille du fichier en kilo octets
	int largeImg ; // largeur de l'image en pixels
	int longImg ; // hauteur de l'image en pixels
} ImageInfo;

/* structure liste de ImageInfo */
typedef struct s_List_ List;
struct s_List_ {
    List *next; // pointeur sur l element suivant 
    ImageInfo data; // pointeur sur une donnée 
};

List * acquerir(const char * chemin);
void affichage(List *list);
double *histTauxPC (image_desc * img, char *f);
double *histColorPlan (image_desc * img, char * fName, int colorplan);
double **histCombinated(image_desc * img, char * fName);
double *histRepImgTaille(List * list, int taille);
