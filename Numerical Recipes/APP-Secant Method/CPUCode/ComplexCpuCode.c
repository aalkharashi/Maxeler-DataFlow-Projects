/**
 * Abdulwahab Alkharashi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <MaxSLiCInterface.h>
#include "Maxfiles.h"

int check(float *actualOut, float *expectedOut, int size)
{
	int status = 0;
	for (int i = 0; i < size; i++) {
		printf(
			"[%d]\treal: %f\timag: %f\n",
			i,
			actualOut[i * 2],
			actualOut[i * 2 + 1]);

		if (actualOut[i * 2] != expectedOut[i * 2] ||
		    actualOut[i * 2 + 1] != expectedOut[i * 2 + 1])
		{
			fprintf(
				stderr,
				"Error detected, expected real: %f\timag: %f\n",
				expectedOut[i * 2],
				expectedOut[i * 2 + 1]);
			status = 1;
		}
	}
	return status;
}

void ComplexCPU(int size, float *cplxIn1, float *cplxIn2, float *actualOut)
{
	for (int i = 0; i < size; i++) {
		actualOut[i] = 3 * (cplxIn1[i] + cplxIn2[i]);
	}
}

int main()
{
	const int size = 2;
	float cplxIn1[] = {
		1, 2, 
		3, 4 };

	float cplxIn2[] = { 5, 6, 7, 8 };

	float expectedOut[] = {
		18, 24, 
		30, 36 };

	float *actualOut = malloc(sizeof(expectedOut));
	memset(actualOut, 0, sizeof(expectedOut));

	ComplexCPU(
		size,
		cplxIn1,
		cplxIn2,
		actualOut);

	printf("Running DFE.\n");
	Complex(
		size,
		cplxIn1,
		cplxIn2,
		actualOut);

	int status = check(actualOut, expectedOut, size);
	if (status)
		printf("Test failed.\n");
	else
		printf("Test passed OK!\n");

	return status;
}
