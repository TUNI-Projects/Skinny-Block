#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "skinny.h"

unsigned char *subCells(unsigned char *bits);
int add_constant(int last_round[], int round_number);
int add_round_tweakey(unsigned char *c, const unsigned char *p, const unsigned char *k);
unsigned char shift_rows(unsigned char matrix[]);
unsigned char mix_col(unsigned char last_output[]);
void print_stuff(unsigned char *data);
void dec_to_bin(int dec, unsigned int binary[]);
void debug();
/**
 *  Skinny Round Constants in Hexadecimal
 *  From Table 2
 */

static const unsigned char round_1_16[] = {
    0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F, 0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E};

static const unsigned char round_17_32[] = {
    0x1D, 0x3A, 0x35, 0x2B, 0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E, 0x1C, 0x38};

static const unsigned char round_33_48[] = {
    0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A, 0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04};

static const unsigned char round_49_62[] = {
    0x09, 0x13, 0x26, 0x0C, 0x19, 0x32, 0x25, 0x0A, 0x15, 0x2A, 0x14, 0x28, 0x10, 0x20};

void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     * SKINNY-128-384 block cipher encryption.
     * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
     */
    // unsigned char x = 0x8;
    // int y = (int)x;

    // printf("%x\n", x);
    // printf("%d\n", y);
    // char *z[8]; // i need a pointer to pass;
    // dec_to_bin(y, z);

    unsigned char *key = k;
    unsigned char *plain_text = p;
    unsigned char *output = c;

    subCells(plain_text);
    mix_col(plain_text);
    shift_rows(plain_text);
    // unsigned char x = strtol("10010011", NULL, 2); < converts binary to hex.
    // debug();
}

unsigned char *subCells(unsigned char *plain_text)
{
    /**
     * Written by Henriikka
     * Step 1: Run the loop 16 times over the plain-text.
     * Step 2: Convert each byte to binary.
     * This function is generating some weird value after bit ops. I think the bit ops are wrong.
     *
     */
    for (int index = 0; index < 16; index++)
    {
        unsigned int bits[8];
        dec_to_bin(plain_text[index], bits);

        // Debug stuff
        printf("-----------------------\n");
        printf("-----------------------\n");
        printf("Original bits, before the loop start \n");
        for (int i = 0; i < 8; i++)
        {
            printf("Index: %d, %x \n", i + 1, bits[i]);
        }
        printf("-------------\n");
        printf("-------------\n");
        // Debug stuff

        for (int i = 0; i < 4; i++)
        {
            // this result is weird i think. I am not sure.
            bits[4] = bits[4] ^ ~(bits[7] | bits[6]);
            bits[0] = bits[0] ^ ~(bits[3] | bits[2]);
            // this result is weird i think. I am not sure.

            if (i < 3)
            {
                // Bit permutation used for 3 first rounds
                unsigned int new_bit[] = {bits[2], bits[1], bits[7], bits[6], bits[4], bits[0], bits[3], bits[5]};
                for (int j = 0; j < 8; j++)
                {
                    bits[j] = new_bit[j];
                }
            }
            else
            {
                // Bit permutation for last round
                unsigned int new_bit[] = {bits[7], bits[6], bits[5], bits[4], bits[3], bits[1], bits[2], bits[0]};
                for (int j = 0; j < 8; j++)
                {
                    bits[j] = new_bit[j];
                }
            }
        }
        printf("-------------\n");
        printf("-------------\n");
        printf("Final Result \n");
        for (int i = 0; i < 8; i++)
        {
            printf("Index: %d, %d \n", i + 1, bits[i]);
        }
        printf("-------------\n");
        printf("-------------\n");
    }
    // TODO: need to fix this return statement.
    return 0xdf;
}

int add_constant(int last_round[], int round_number)
{
    /**
     * Written by Ibtehaz
     *
     */
    unsigned char current_round_array;
    if (round_number < 17)
    {
        current_round_array = round_1_16;
    }
    else if (round_number < 33)
    {
        current_round_array = round_17_32;
    }
    else if (round_number < 49)
    {
        current_round_array = round_33_48;
    }
    else
    {
        current_round_array = round_49_62;
    }
    return -1;
}

int add_round_tweakey(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     *
     *
     */
    return -1;
}

unsigned char shift_rows(unsigned char matrix[])
{
    /**
     * Written by Henriikka
     *
     */
    // Debug stuff
    printf("-----------------------\n");
    printf("-----------------------\n");
    for (int i = 0; i < 16; i++)
    {
        printf("Index: %d, %x \n", i, matrix[i]);
    }
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    // Debug stuff
    // Rotate cells to match P in paper (ShiftRows)
    unsigned char shifted[] = {matrix[0], matrix[1], matrix[2], matrix[3], matrix[7], matrix[4], matrix[5], matrix[6], matrix[10],
                               matrix[11], matrix[8], matrix[9], matrix[13], matrix[14], matrix[15], matrix[12]};

    for (int i = 0; i < 16; i++)
    {
        printf("Index: %d, %x \n", i, shifted[i]);
    }
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    return shifted;
}

unsigned char mix_col(unsigned char last_output[])
{
    /**
     * Written by Ibtehaz
     * ** comment **
     * For individual function, I am gonna test it against the original hex value and adding it against the array.
     * I will do integration testing against the full system later
     *
     * This function works with hex summation.
     * the input has to be unsigned char and output currently is in decimal.
     * ** comment **
     */
    // Debug stuff
    printf("-----------------------\n");
    printf("-----------------------\n");
    for (int i = 0; i < 16; i++)
    {
        printf("Index: %d, %x \n", i + 1, last_output[i]);
    }
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    // Debug stuff

    int fixed_array[16] = {1, 0, 1, 1,
                           1, 0, 0, 0,
                           0, 1, 1, 0,
                           1, 0, 1, 0};

    printf("%x --- %x --- %x \n", last_output[0], last_output[1], last_output[3]);

    unsigned char new_array[16] = {
        last_output[0] + last_output[1] + last_output[3],
        last_output[2],
        last_output[0] + last_output[2] + last_output[3],
        last_output[0],
        last_output[4] + last_output[5] + last_output[7],
        last_output[6],
        last_output[4] + last_output[6] + last_output[7],
        last_output[4],
        last_output[8] + last_output[9] + last_output[11],
        last_output[10],
        last_output[8] + last_output[10] + last_output[11],
        last_output[8],
        last_output[12] + last_output[13] + last_output[15],
        last_output[14],
        last_output[14] + last_output[14] + last_output[15],
        last_output[12]};

    for (int i = 0; i < 16; i++)
    {
        printf("Index: %d, %x \n", i + 1, new_array[i]);
    }
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    printf("-----------------------\n");
    return new_array;
}

void dec_to_bin(int dec, unsigned int binary[])
{
    /**
     * convert decimals to binary for bitwise ops.
     * :returns char [8]
     * question:
     * how to return AN ARRAY.
     */
    printf("$$$$$$$$$\ndecimal to convert: %d\n", dec);
    // unsigned int binary[8];
    int mid_val[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int reverse[] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int index = 0; dec > 0; index++)
    {
        // mid_val[index] = (dec % 2) + '0';
        mid_val[index] = (dec % 2);
        dec = dec / 2;
    }

    // reverse the order of bin in the bin array.
    for (int i = 0; i < 8; i++)
    {
        reverse[8 - 1 - i] = mid_val[i];
    }

    // put everything inside the original pointer
    for (int i = 0; i < 8; i++)
    {
        binary[i] = reverse[i];
    }

    for (int i = 0; i < 8; i++)
    {
        printf("%d", binary[i]);
    }
    printf("\n$$$$$$$$$\n");
}

/**
 * ---------------------
 * Debug stuff
 * ---------------------
 */

void print_stuff(unsigned char *data)
{
    /**
     * Function to print random stuff
     */
    for (int i = 0; i < 16; i++)
    {
        printf("Index: %d, %x \n", i + 1, data[i]);
    }
    printf("\n");
}

void debug()
{
    int x = 10;
    char z = '1';

    printf('%x \n', x); // segmentation fault, why?
}
