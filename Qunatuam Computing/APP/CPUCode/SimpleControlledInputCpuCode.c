/**
 * Document: MaxCompiler Tutorial (maxcompiler-tutorial.pdf)
 * Chapter: 11      Example: 1      Name: Simple controlled Input
 * MaxFile name: SimpleControlledInput
 * Summary:
 *    CPU code for a design using a controlled input. Inputs a and c are
 *    continuous data streams that will pass inputs to the core whenever there
 *    is data available. Input b will only pass inputs to the core when the
 *    current value of input stream c is 1.
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

void generateInputData(int size, uint32_t *inA, uint32_t *inB, int8_t *inC)
{
	srand(12345);
	int bInd = 0;
	for (int i = 0; i < size; i++) {
		inA[i] = rand();
		if (i % 2 == 1)
			inB[bInd++] = rand();
	}

	// since inC is a stream of dfeBool, every bit represents one stream value
	for (int i = 0; i < size / 8; i++)
		inC[i] = rand() % 256;
}

// find the value of the bit corresponding to the current i
int shouldRead(int8_t *inC, int i)
{
	int value = inC[i / sizeof(inC)];
	int bitNumber = i % sizeof(inC);
	int shouldRead = (value & (1 << bitNumber)) >> bitNumber;
	return shouldRead;
}

void SimpleControlledInputCPU(
	int size,
	uint32_t *inA,
	uint32_t *inB,
	int8_t *inC,
	uint32_t *outY)
{
	int bInd = 0;
	for (int i = 0; i < size; i++) {
		outY[i] = inA[i] + (shouldRead(inC, i) ? inB[bInd++] : 0);
	}
}

int check(int size, uint32_t *outData, uint32_t *expected)
{
	int status = 0;
	for (int i = 0; i < size; i++) {
		if (outData[i] != expected[i]) {
			fprintf(stderr, "ERROR @ %d, expected %u, got %u\n",
				i, expected[i], outData[i]);
			status = 1;
		}
	}
	return status;
}

int main()
{
	const int size = 64;
	int sizeBytes = size * sizeof(uint32_t);
	uint32_t *inA  = malloc(sizeBytes);
	uint32_t *inB  = malloc(sizeBytes);
	int8_t   *inC  = malloc(sizeBytes);
	uint32_t *outY = malloc(sizeBytes);
	uint32_t *expected = malloc(sizeBytes);

	generateInputData(size, inA, inB, inC);

	printf("Running DFE.\n");
	SimpleControlledInput(size, inA, inB, inC, outY);

	SimpleControlledInputCPU(size, inA, inB, inC, expected);

	int status = check(size, outY, expected);
	if (status)
		printf("Test failed.\n");
	else
		printf("Test passed OK!\n");
	return status;
}

