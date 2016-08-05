
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "bitrotate.h"

void sub_9E9D8(unsigned char *input, unsigned char*output);

unsigned char* sub_87444(unsigned char *input, int size, unsigned char* arr1) {
	char arr2[256];
	char arr3[256];
	int roundedsize, totalsize;

	for (int j = 0; j < 8; j++)
		for (int i = 0; i < 32; i++)
			arr2[32 * j + i] = rotl8(arr1[i], j);//rotate byte left

	memmove(input + 32, input, size);
	memcpy(input, arr2, 32);

	roundedsize = size + (256 - (size % 256));
	totalsize = roundedsize + 32;

	if (roundedsize > size)
	{
		memset(input + 32 + size, 0, roundedsize - size); //pad data with zeroes
	}
	input[totalsize - 1] = 256 - (size % 256);

	for (int offset = 32; offset < totalsize; offset += 256)
	{
		for (int i = 0; i < 256; i++)
			input[offset + i] ^= arr2[i];
		sub_9E9D8(input + offset, arr3);
		//ingress_encrypt(input + offset, arr3);
		memcpy(arr2, arr3, 256);
		memcpy(input + offset, arr3, 256);
	}

	return input;
}

unsigned char* load_file(char const* path, size_t* plength)
{
	unsigned char* buffer = 0;
	size_t length;
	FILE * f = fopen(path, "rb"); //was "rb"

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (unsigned char*) malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, f);
		}
		fclose(f);
		*plength = length;
	}

	return buffer;
}


void print_hex(const unsigned char *s, size_t l)
{
	for (size_t i = 0; i<l; i++) {
		printf("%02x", (unsigned int)s[i]);
	}
	printf("\n");
}

void verify(const char* plain, const char* enc){
	size_t input_size;
	unsigned char* input = load_file("dump0.bin", &input_size);
	size_t output_size;
	unsigned char* output = load_file("dump0_encrypted.bin", &output_size);

	size_t transformed_output_size = 32 + input_size + (256 - (input_size % 256));
	unsigned char* transformed_output = (unsigned char*) malloc(transformed_output_size);
	memcpy(transformed_output, input, input_size);

	sub_87444(transformed_output, input_size, output);

	printf("%s: ", plain);
	if (output_size == transformed_output_size && memcmp(output, transformed_output, output_size) == 0) {
		printf("ok %d\n",output_size);
	}else{
		printf("not ok\n");
	}
}

int main() {
	verify("../dumps/dumps_mandra/dump0.bin", "../dumps/dumps_mandra/dump0_encrypted.bin");
	verify("../dumps/dumps_mandra/dump1.bin", "../dumps/dumps_mandra/dump1_encrypted.bin");
	verify("../dumps/dumps_mandra/dump2.bin", "../dumps/dumps_mandra/dump2_encrypted.bin");
	verify("../dumps/dumps_mandra/dump3.bin", "../dumps/dumps_mandra/dump3_encrypted.bin");
	verify("../dumps/dumps_mandra/dump4.bin", "../dumps/dumps_mandra/dump4_encrypted.bin");
	verify("../dumps/dumps_mandra/dump5.bin", "../dumps/dumps_mandra/dump5_encrypted.bin");
	verify("../dumps/dumps_mandra/dump6.bin", "../dumps/dumps_mandra/dump6_encrypted.bin");
	verify("../dumps/dumps_mandra/dump7.bin", "../dumps/dumps_mandra/dump7_encrypted.bin");
	verify("../dumps/dumps_mandra/dump8.bin", "../dumps/dumps_mandra/dump8_encrypted.bin");
	verify("../dumps/dumps_mandra/dump9.bin", "../dumps/dumps_mandra/dump9_encrypted.bin");



}
