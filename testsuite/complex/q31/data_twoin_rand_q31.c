#include "stdio.h"
#include "math.h"
void main(void)
{
  int i = 0;
  int32_t valA = 0, valB = 0;
  int count =1;
  FILE *fptr;
  FILE *gptr;

  fptr = fopen("input_mult_q31_a.txt", "w");
  gptr = fopen("input_mult_q31_b.txt", "w");

  for(i = 0; i < 32768; i++)
  { 
	valB = (rand()*rand());
	fwrite(&valA, sizeof(int32_t), 1, fptr);
	fwrite(&valB, sizeof(int32_t), 1, gptr);
	printf("%d %d %d\n", count, valA, valB);
	valA += 131072;
	count++;
  }

  fclose(fptr);
  fclose(gptr);
}
