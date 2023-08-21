#include "stdio.h"
#include <stdlib.h>


int main(void)
{
  int i;
  int datasize = 30000;
  short valy, valx;
  FILE *fptr, *gptr;
  float zz;


  fptr = fopen("input_q15_y.bin", "w");
  gptr = fopen("input_q15_x.bin", "w");

  for(i = 0; i < (datasize); i++)
  {
    do{
          valy = rand()%(65536);
	    valx = rand()%(65536);
        zz = (float)valy/valx;
    }while (abs(zz) < 1);
	printf("%d %d %d\n", i, valy, valx);
	fwrite(&valy, sizeof(short), 1, fptr);
	fwrite(&valx, sizeof(short), 1, gptr);
  }
//  valy = 1234;
//  valx = 0;
//  printf("%d %d %d\n",i ,valy, valx);
//  fwrite(&valy, sizeof(short), 1, fptr);
//  fwrite(&valx, sizeof(short), 1, gptr);
//  valy = -1234;
//  valx = 0;
//  i++;
//  printf("%d %d %d\n", i, valy, valx);
//  fwrite(&valy, sizeof(short), 1, fptr);
//  fwrite(&valx, sizeof(short), 1, gptr);

  fclose(fptr);
  fclose(gptr);

  return 0;
}
