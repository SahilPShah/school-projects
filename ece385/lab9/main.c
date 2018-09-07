/************************************************************************
Lab 9 Nios Software

Dong Kai Wang, Fall 2017
Christine Chen, Fall 2013

For use with ECE 385 Experiment 9
University of Illinois ECE Department
************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "aes.h"

unsigned int RotWord(unsigned int word);
unsigned int SubWord(unsigned int c, int i);
// Pointer to base address of AES module, make sure it matches Qsys
volatile unsigned int *AES_PTR = (unsigned int *) 0x00000040;


// Execution mode: 0 for testing, 1 for benchmarking
int run_mode = 0;


void printState(unsigned char* state){
	//int i=0;
	//for(i=0;i<16;i++){
		//printf("state[%d] = %02x\n", i, state[i]);
	//}
	//printf("\n");
}
/** charToHex
 *  Convert a single character to the 4-bit value it represents.
 *
 *  Input: a character c (e.g. 'A')
 *  Output: converted 4-bit value (e.g. 0xA)
 */
unsigned char charToHex(unsigned char c)
{
	unsigned char hex = c;

	if (hex >= '0' && hex <= '9')
		hex -= '0';
	else if (hex >= 'A' && hex <= 'F')
	{
		hex -= 'A';
		hex += 10;
	}
	else if (hex >= 'a' && hex <= 'f')
	{
		hex -= 'a';
		hex += 10;
	}
	return hex;
}

/** charsToHex
 *  Convert two characters to byte value it represents.
 *  Inputs must be 0-9, A-F, or a-f.
 *
 *  Input: two characters c1 and c2 (e.g. 'A' and '7')
 *  Output: converted byte value (e.g. 0xA7)
 */
unsigned char charsToHex(unsigned char c1, unsigned char c2)
{
	unsigned char hex1 = charToHex(c1);
	unsigned char hex2 = charToHex(c2);
	//unsigned char a = (hex1 << 4) + hex2;
	return (hex1 << 4) + hex2;
}


/** Key Expansion
 *  Create Key Schedule
 *
 *  Input: 		  key_in - Pointer to 16x 8-bit char array that contains the input key in hex format
 *  Output: key_schedule - Pointer to 4x 44-byte 1-D char matrix containing all 11 round keys
 */
void KeyExpansion(unsigned char *key_hex, unsigned char *key_schedule )
{
	unsigned int key_word[4];
	unsigned int key_temp;
	unsigned int key_schedule_word[44];
	int i;
	int j;

	for(i = 0; i < 4; i++)
	{
		key_word[i] = 0;			//initialize key_word
	}

	for(j = 0; j < 4; j = j + 1)
	{
		for(i = 0; i < 4; i = i + 1)
		{
			key_word[j] = key_word[j] + (key_hex[(4*j) + i] << (8*(3-i))); //making the char key into an int key
		}
	}


	for(i = 0; i < 4; i = i + 1)
	{
		key_schedule_word[i] = key_word[i];		//original cipher key as 1st round key
	}

	for(i = 4; i < 44; i = i + 1)
	{
		key_temp = key_schedule_word[i - 1];
		if((i % 4) == 0)
		{
			key_temp = SubWord(RotWord(key_temp), i) ^ Rcon[i/4];
		}
		key_schedule_word[i] = key_temp ^ key_schedule_word[i - 4];
	}
	for(j = 0; j < 44; j = j + 1)
	{
		key_schedule[(4*j) + 0] = ((key_schedule_word[j] & 0xff000000) >> 24);
		key_schedule[(4*j) + 1] = ((key_schedule_word[j] & 0x00ff0000) >> 16);
		key_schedule[(4*j) + 2] = ((key_schedule_word[j] & 0x0000ff00) >> 8);
		key_schedule[(4*j) + 3] = ((key_schedule_word[j] & 0x000000ff) >> 0);

//		printf("word: %d \n" , j);
//	  printf("char = %02x ", 	key_schedule[(4*j) + 0]);
//		printf("char = %02x ", 	key_schedule[(4*j) + 1]);
//		printf("char = %02x ", 	key_schedule[(4*j) + 2]);
//		printf("char = %02x ", 	key_schedule[(4*j) + 3]);
//	  printf("\n");
	}

}

unsigned int RotWord(unsigned int word)
{
	unsigned int temp1 = 0;
	unsigned int temp2 = 0;
	unsigned int temp3 = 0;

	temp1 = word & 0xff000000;
	temp2 = word & 0x00ffffff;
	temp3 = (temp1 >> 24) + (temp2 << 8);
	return temp3;
}

unsigned int SubWord(unsigned int c, int i)
{
	unsigned char temp1 = (c & 0xff000000) >> 24;
	unsigned char temp2 = (c & 0x00ff0000) >> 16;
	unsigned char temp3 = (c & 0x0000ff00) >> 8;
	unsigned char temp4 = (c & 0x000000ff) >> 0;
	unsigned int substitute;

	temp1 = aes_sbox[temp1];
	temp2 = aes_sbox[temp2];
	temp3 = aes_sbox[temp3];
	temp4 = aes_sbox[temp4];

	substitute = ((temp1 << 24) +  (temp2 << 16) +  (temp3 << 8) +  (temp4 << 0));
	return substitute;
}

void AddRoundKey(unsigned char *state, unsigned int Round, unsigned char *curr_key)
{
	int word_start = Round * 16;
//	printf("word start = %d " , word_start);				//curr_key will be an array of Bytes NOT words
//	printf("Round = %u \n " , Round);
	int j;
	for(j = word_start; j < word_start + 16; j++)
	{
		//printf(" before state[%d] = %02x \n " , j - word_start, state[(j - word_start)] );													//for each byte in the Round
		state[(j - word_start)] = curr_key[j] ^ state[(j - word_start)]; 		//XOR the byte in the curr_key and the byte in state
		//printf(" after state[%d] = %02x \n " , j - word_start, state[(j - word_start)] );
	}
//		printf("\n");
//		printf("\n");
//		printf("\n");
}

void SubBytes(unsigned char *state)
{
	int i;
	int j;

	for(j = 0; j < 4; j = j + 1)
	{
		for(i = 0; i < 4; i = i + 1)
		{
			state[(4*i) + j] = aes_sbox[state[(4*i) + j]];
		}
	}
}

void ShiftRows(unsigned char *state)
{

	unsigned char temp1 = state[1];
	unsigned char temp6 = state[6];
	unsigned char temp2 = state[2];
	unsigned char temp3 = state[3];
	unsigned char temp7 = state[7];
	unsigned char temp11 = state[11];

	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = temp1;

	state[2]  = state[10];
	state[6]  = state[14];
	state[14] = temp6;
	state[10] = temp2;

	state[3] = state[15];
	state[7] = temp3;
	state[11] = temp7;
	state[15] = temp11;

}

void MixColumns(unsigned char *state)
{
	unsigned char temp_state0;
	unsigned char temp_state1;
	unsigned char temp_state2;
	unsigned char temp_state3;

	temp_state0  = gf_mul[state[0]][0] ^ gf_mul[state[1]][1] ^ state[2] ^ state[3];
	temp_state1  = state[0] ^ gf_mul[state[1]][0] ^ gf_mul[state[2]][1] ^ state[3];
	temp_state2  = state[0] ^ state[1] ^ gf_mul[state[2]][0] ^ gf_mul[state[3]][1];
	temp_state3 = gf_mul[state[0]][1] ^  state[1] ^  state[2] ^ gf_mul[state[3]][0];

	state[0] = temp_state0;
	state[1] = temp_state1;
	state[2] = temp_state2;
	state[3] = temp_state3;
////////////////////////////////////////////////////////////////////////////////
	temp_state0  = gf_mul[state[4]][0] ^ gf_mul[state[5]][1] ^ state[6] ^ state[7];
	temp_state1  = state[4] ^ gf_mul[state[5]][0] ^ gf_mul[state[6]][1] ^ state[7];
	temp_state2  = state[4] ^ state[5] ^ gf_mul[state[6]][0] ^ gf_mul[state[7]][1];
	temp_state3 = gf_mul[state[4]][1] ^  state[5] ^  state[6] ^ gf_mul[state[7]][0];

	state[4] = temp_state0;
	state[5] = temp_state1;
	state[6] = temp_state2;
	state[7] = temp_state3;
////////////////////////////////////////////////////////////////////////////////
	temp_state0  = gf_mul[state[8]][0] ^ gf_mul[state[9]][1] ^ state[10] ^ state[11];
	temp_state1  = state[8] ^ gf_mul[state[9]][0] ^ gf_mul[state[10]][1] ^ state[11];
	temp_state2  = state[8] ^ state[9] ^ gf_mul[state[10]][0] ^ gf_mul[state[11]][1];
	temp_state3 = gf_mul[state[8]][1] ^  state[9] ^  state[10] ^ gf_mul[state[11]][0];

	state[8] = temp_state0;
	state[9] = temp_state1;
	state[10] = temp_state2;
	state[11] = temp_state3;
////////////////////////////////////////////////////////////////////////////////
	temp_state0  = gf_mul[state[12]][0] ^ gf_mul[state[13]][1] ^ state[14] ^ state[15];
	temp_state1  = state[12] ^ gf_mul[state[13]][0] ^ gf_mul[state[14]][1] ^ state[15];
	temp_state2  = state[12] ^ state[13] ^ gf_mul[state[14]][0] ^ gf_mul[state[15]][1];
	temp_state3 = gf_mul[state[12]][1] ^  state[13] ^  state[14] ^ gf_mul[state[15]][0];

	state[12] = temp_state0;
	state[13] = temp_state1;
	state[14] = temp_state2;
	state[15] = temp_state3;
}

/** encrypt
 *  Perform AES encryption in software.
 *
 *  Input: msg_ascii - Pointer to 32x 8-bit char array that contains the input message in ASCII format
 *         key_ascii - Pointer to 32x 8-bit char array that contains the input key in ASCII format
 *  Output:  msg_enc - Pointer to 4x 32-bit int array that contains the encrypted message
 *               key - Pointer to 4x 32-bit int array that contains the input key
 */
void encrypt(unsigned char *msg_ascii, unsigned char *key_ascii, unsigned int *msg_enc, unsigned int *key)
{
	// Implement this function

	int i = 0;
	int j = 0;

	unsigned char msg_hex[16];
	unsigned char key_hex[16];
	unsigned char key_schedule[176];
	unsigned char state[16];



	msg_hex[0] = charsToHex(msg_ascii[0], msg_ascii[1]);
	msg_hex[1] =  charsToHex(msg_ascii[2], msg_ascii[3]);
	msg_hex[2] =  charsToHex(msg_ascii[4], msg_ascii[5]);
	msg_hex[3] =  charsToHex(msg_ascii[6], msg_ascii[7]);
	msg_hex[4] =  charsToHex(msg_ascii[8], msg_ascii[9]);
	msg_hex[5] =  charsToHex(msg_ascii[10], msg_ascii[11]);
	msg_hex[6] =  charsToHex(msg_ascii[12], msg_ascii[13]);
	msg_hex[7] = charsToHex(msg_ascii[14], msg_ascii[15]);
	msg_hex[8] = charsToHex(msg_ascii[16], msg_ascii[17]);
	msg_hex[9] = charsToHex(msg_ascii[18], msg_ascii[19]);
	msg_hex[10] = charsToHex(msg_ascii[20], msg_ascii[21]);
	msg_hex[11] = charsToHex(msg_ascii[22], msg_ascii[23]);
	msg_hex[12] = charsToHex(msg_ascii[24], msg_ascii[25]);
	msg_hex[13] = charsToHex(msg_ascii[26], msg_ascii[27]);
	msg_hex[14] = charsToHex(msg_ascii[28], msg_ascii[29]);
	msg_hex[15] = charsToHex(msg_ascii[30], msg_ascii[31]);


	key_hex[0] = charsToHex(key_ascii[0], key_ascii[1]);
	key_hex[1] =  charsToHex(key_ascii[2], key_ascii[3]);
	key_hex[2] =  charsToHex(key_ascii[4], key_ascii[5]);
	key_hex[3] =  charsToHex(key_ascii[6], key_ascii[7]);
	key_hex[4] =  charsToHex(key_ascii[8], key_ascii[9]);
	key_hex[5] =  charsToHex(key_ascii[10], key_ascii[11]);
	key_hex[6] =  charsToHex(key_ascii[12], key_ascii[13]);
	key_hex[7] = charsToHex(key_ascii[14], key_ascii[15]);
	key_hex[8] = charsToHex(key_ascii[16], key_ascii[17]);
	key_hex[9] = charsToHex(key_ascii[18], key_ascii[19]);
	key_hex[10] = charsToHex(key_ascii[20], key_ascii[21]);
	key_hex[11] = charsToHex(key_ascii[22], key_ascii[23]);
	key_hex[12] = charsToHex(key_ascii[24], key_ascii[25]);
	key_hex[13] = charsToHex(key_ascii[26], key_ascii[27]);
	key_hex[14] = charsToHex(key_ascii[28], key_ascii[29]);
	key_hex[15] = charsToHex(key_ascii[30], key_ascii[31]);


	KeyExpansion(key_hex, key_schedule);				//This will create the Key Schedule

		for(i = 0; i < 16; i = i + 1)
		{
			state[i] = msg_hex[i];
		}

	AddRoundKey(state, 0, key_schedule);

	for(i = 0; i < 9; i = i + 1)
	{
		SubBytes(state);
		//printf("SubBytes: \n");
		printState(state);
		ShiftRows(state);
		//printf("ShiftRows: \n");
		printState(state);
		MixColumns(state);
		//printf("MixColumns: \n");
		printState(state);
		AddRoundKey(state, i + 1, key_schedule);
		//printf("AddRoundKey: \n");
		printState(state);
	}

	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, 10, key_schedule);

	for(j = 0; j < 4; j = j + 1)
	{
		msg_enc[j] = 0;
		for(i = 0; i < 4; i = i + 1)
		{
			msg_enc[j] = msg_enc[j] + (state[(4*j) + i] << (8*(3-i))); //making the char key into an int key
		}
	}

	for(j = 0; j < 4; j = j + 1)
	{
			key[j] = 0;
		for(i = 0; i < 4; i = i + 1)
		{
			key[j] = key[j] + (key_hex[(4*j) + i] << (8*(3-i))); //making the char key into an int key
		}
	}



}

/** decrypt
 *  Perform AES decryption in hardware.
 *
 *  Input:  msg_enc - Pointer to 4x 32-bit int array that contains the encrypted message
 *              key - Pointer to 4x 32-bit int array that contains the input key
 *  Output: msg_dec - Pointer to 4x 32-bit int array that contains the decrypted message
 */
void decrypt(unsigned int *msg_enc, unsigned int *msg_dec, unsigned int *key)
{
	int temp;

	AES_PTR[0] = key[0];
	AES_PTR[1] = key[1];
	AES_PTR[2] = key[2];
	AES_PTR[3] = key[3];			//Store key into registers
	AES_PTR[4] = msg_enc[0];
	AES_PTR[5] = msg_enc[1];
	AES_PTR[6] = msg_enc[2];
	AES_PTR[7] = msg_enc[3];		//Store encrypted message into registers

	AES_PTR[14] = 0xffffffff;		//Set start flag to 1, to let the hardware know to start decrypting


	while(AES_PTR[15] != 0x0000001)			//If DONE flag is set to one, hardware is done decrypting and the software can print to the console
	{

	}

	msg_dec[0] = AES_PTR[8];
	msg_dec[1] = AES_PTR[9];
	msg_dec[2] = AES_PTR[10];
	msg_dec[3] = AES_PTR[11];

	// Implement this function
}

/** main
 *  Allows the user to enter the message, key, and select execution mode
 *
 */
int main()
{
	// Input Message and Key as 32x 8-bit ASCII Characters ([33] is for NULL terminator)
	unsigned char msg_ascii[33];
	unsigned char key_ascii[33];
	// Key, Encrypted Message, and Decrypted Message in 4x 32-bit Format to facilitate Read/Write to Hardware
	unsigned int key[4];
	unsigned int msg_enc[4];
	unsigned int msg_dec[4];

	printf("Select execution mode: 0 for testing, 1 for benchmarking: ");
	scanf("%d", &run_mode);

	if (run_mode == 0) {
		// Continuously Perform Encryption and Decryption
		while (1) {
			int i = 0;
			printf("\nEnter Message:\n");
			scanf("%s", msg_ascii);
			printf("\n");
			printf("\nEnter Key:\n");
			scanf("%s", key_ascii);
			printf("\n");
			AES_PTR[14] = 0x00000000;		//Set start flag to 0, to let the hardware know to not decrypt yet
			encrypt(msg_ascii, key_ascii, msg_enc, key);
			printf("\nEncrpted message is: \n");
			for(i = 0; i < 4; i++){
				printf("%08x", msg_enc[i]);
			}
			printf("\n");
			decrypt(msg_enc, msg_dec, key);
			printf("\nDecrypted message is: \n");
			for(i = 0; i < 4; i++){
				printf("%08x", msg_dec[i]);
			}
			printf("\n");
		}
	}
	else {
		// Run the Benchmark
		int i = 0;
		int size_KB = 2;
		// Choose a random Plaintext and Key
		for (i = 0; i < 32; i++) {
			msg_ascii[i] = 'a';
			key_ascii[i] = 'b';
		}
		// Run Encryption
		clock_t begin = clock();
		for (i = 0; i < size_KB * 64; i++)
			encrypt(msg_ascii, key_ascii, msg_enc, key);
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		double speed = size_KB / time_spent;
		printf("Software Encryption Speed: %f KB/s \n", speed);
		// Run Decryption
		begin = clock();
		for (i = 0; i < size_KB * 64; i++)
			decrypt(msg_enc, msg_dec, key);
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		speed = size_KB / time_spent;
		printf("Hardware Encryption Speed: %f KB/s \n", speed);
	}

	return 0;
}
