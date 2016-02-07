#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mem_targa.h"

void freeImage(image_desc *pDesc)
{
  free (pDesc->pBlue); pDesc->pBlue = NULL;
  free (pDesc->pGreen); pDesc->pGreen = NULL;
  free (pDesc->pRed); pDesc->pRed = NULL;
}

int readImage(image_desc *pDesc, targa_header *pHead, char * fName)
{
  FILE * fDesc;
  int i;
  /* Open fDesc file */
  fDesc = fopen(fName,"r");

  /* Lecture du header */
  fread(pHead, sizeof(targa_header), 1, fDesc);
  //head = *pHead; // on le met dans une vari. temporarire pour faciliter l'acces aux champs

  printf("[mem_targa] Header : %u %u %u %u %u\n", pHead->idlength, pHead->colourmaptype, pHead->datatypecode, pHead->width, pHead->height);

  // Initialisation de la structure image
  pDesc->width = pHead->width;
  pDesc->height = pHead->height;

  mallocImage(pDesc, pHead->width, pHead->height);

  
  // Remplir pixel par pixel les 3 couches de couleurs
  for (i=0; i<pHead->width*pHead->height; i++) {
	// read 1 int : blue
	fread(pDesc->pBlue+i, sizeof(uint8_t), 1, fDesc);
	// read 1 int : green
	fread(pDesc->pGreen+i, sizeof(uint8_t), 1, fDesc);
	// read 1 int : red
	fread(pDesc->pRed+i, sizeof(uint8_t), 1, fDesc);
  }
  return 1;
  
}

int mallocImage(image_desc *pDesc, uint16_t width, uint16_t height)
{
  // Note : test of memory failures are omitted here.
  int planeSize;
  if (pDesc == NULL)
	  pDesc = (image_desc*)malloc(sizeof(image_desc));
	  
  pDesc->width = width;
  pDesc->height = height;
  planeSize = sizeof(uint8_t) * height * width;
  /* Allocate memory for each image plane */

  pDesc->pBlue = calloc (planeSize, sizeof(uint8_t)); 
  pDesc->pGreen = calloc (planeSize, sizeof(uint8_t));
  pDesc->pRed = calloc (planeSize, sizeof(uint8_t));

  return planeSize;
}


int writeImage (image_desc iDesc, targa_header head, char * fName)
{
  FILE *fDesc;
  uint8_t *pBlue, *pGreen, *pRed, *pBlueEnd;
  int i;
  
  /* Open output image file */
  if ((fDesc = fopen (fName, "w")) == NULL)
  {
	fprintf (stderr, "Cannot create the file \"%s\".\n", fName);
	return 0;
  }

  /* Ecrire le header dans fDesc*/
  head.width = iDesc.width;
  head.height = iDesc.height;
  printf("[read mem_targa] Header : %u %u %u %u %u \n", 
		 head.idlength, head.colourmaptype, head.datatypecode, head.width, head.height);

  fwrite (&head, sizeof (targa_header), 1, fDesc);

// Variables temporaires 
  pBlue = iDesc.pBlue;
  pGreen = iDesc.pGreen;
  pRed = iDesc.pRed;
  /* On va lire head.width*head.height pixels pour les mettre dans le fichier fDesc */
  for (i=0; i<head.width*head.height; i++) 
  {
	fwrite(pBlue+i, sizeof(uint8_t), 1, fDesc);
	fwrite(pGreen+i, sizeof(uint8_t), 1, fDesc);
	fwrite(pRed+i, sizeof(uint8_t), 1, fDesc);
  }
  fclose (fDesc);
  return 1;
}
