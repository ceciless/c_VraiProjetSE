#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "TP.h"
//#include "fun_targa.h"

int main(int argc, char * argv[])
{
	List *list =  NULL;
	list = acquerir("/home23/rohema/TD_OS/TD5");

	affichage(list);
}
