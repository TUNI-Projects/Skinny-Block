#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "skinny.h"
#include <string.h>
#include <math.h>

// Algorithm Func
void subCells(unsigned char plain_text[]);
void add_constant(unsigned char *plain_text, int round_number);
int add_round_tweakey(unsigned char *c, const unsigned char *p, const unsigned char *k);
unsigned char shift_rows(unsigned char matrix[]);
unsigned char mix_col(unsigned char last_output[]);

// Utility Func
void dec_to_bin(int dec, unsigned int binary[]);
void bin_2_hex(unsigned int bits[], unsigned char hex[]);
unsigned char hex_converter(int val);
void debug();
void bin_2_hex_v2(unsigned int bits[]);
int dec_converter(unsigned char hex_val);
int hex_2_dec(unsigned char hex[]);
int bin_2_dec(unsigned int bits[]);

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
     * -----
     * Dec 06
     * 1. Add Round Tweakey and Initialization func. I have to write them
     * 2. How to store the results. :/
     */
    unsigned char key[48]; // original key.
    unsigned char plain_text[16];

    // copy p into plain_text
    for (int i = 0; i < 16; i++)
    {
        plain_text[i] = p[i];
    }

    // copy key
    for (int i = 0; i < 48; i++)
    {
        key[i] = k[i];
    }

    // this is already properly initialized and I can change this val
    unsigned char output[16];

    subCells(plain_text);
    add_constant(plain_text, 16);
    add_constant(plain_text, 18);
    add_constant(plain_text, 48);
    add_constant(plain_text, 60);
    mix_col(plain_text);
    shift_rows(plain_text);
}

void subCells(unsigned char plain_text[])
{
    /**
     * Written by Henriikka
     */

    for (int index = 0; index < 16; index++)
    {
        unsigned int bits[8];
        dec_to_bin(plain_text[index], bits);

        for (int i = 0; i < 4; i++)
        {
            // this result is weird i think. I am not sure.
            unsigned int x = bits[7] | bits[6];
            if (x == 0)
            {
                x = 1;
            }
            else
            {
                x = 0;
            }
            bits[4] = bits[4] ^ x;

            // compliment wasnt working !
            x = bits[3] | bits[2];
            if (x == 0)
            {
                x = 1;
            }
            else
            {
                x = 0;
            }
            bits[0] = bits[0] ^ x;
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
        plain_text[index] = bin_2_dec(bits);
    }
    // i need to make sure value is persistent inside plain_text.
}

void add_constant(unsigned char *plain_text, int round_number)
{
    /**
     * Written by Ibtehaz
     * in Table-2 round_number is mentioned from 1, instead of usual 0.
     * So we will decrease round_number by 1.
     */
    
    // in Table-2 round_number is mentioned from 1, instead of usual 0.
    //  So we will decrease round_number by 1.
    //  In the main skinny function, we will also need to start the loop from 1, instead of 0.
    round_number = round_number - 1;
    unsigned char *current_round_array = NULL;
    int index = 0;
    if (round_number < 16)
    {
        current_round_array = round_1_16;
        index = round_number;
    }
    else if (round_number < 32)
    {
        current_round_array = round_17_32;
        index = round_number - 16;
    }
    else if (round_number < 48)
    {
        current_round_array = round_33_48;
        index = round_number - 32;
    }
    else
    {
        current_round_array = round_49_62;
        index = round_number - 48;
    }
    unsigned char constant = current_round_array[index];
    // -----------------------
    unsigned char c2 = 0x2;

    unsigned int c1_c0[8];
    dec_to_bin(constant, c1_c0);
    unsigned int c0[] = {0, 0, 0, 0, c1_c0[7 - 3], c1_c0[7 - 2], c1_c0[7 - 1], c1_c0[7 - 0]};
    unsigned int c1[] = {0, 0, 0, 0, 0, 0, c1_c0[7 - 5], c1_c0[7 - 4]};

    unsigned char c0_hex[2];
    bin_2_hex(c0, c0_hex);
    int c0_dec = hex_2_dec(c0_hex);

    unsigned char c1_hex[2];
    bin_2_hex(c1, c1_hex);
    int c1_dec = hex_2_dec(c1_hex);

    printf("c0 --> ");
    for (int i = 0; i < 8; i++)
    {
        printf("%u ", c0[i]);
    }
    printf(" || dec: %d\n", c0_dec);

    printf("c1 --> ");
    for (int i = 0; i < 8; i++)
    {
        printf("%u ", c1[i]);
    }
    printf(" || dec: %d\n", c1_dec);
    printf("c2 --> %d\n", c2);

    printf("0 -> %x, 1 -> %x, 2 -> %x \n", plain_text[0], plain_text[1], plain_text[2]);
    plain_text[0] = plain_text[0] + c0_dec;
    plain_text[1] = plain_text[1] + c1_dec;
    plain_text[2] = plain_text[2] + c2;
    printf("0 -> %x, 1 -> %x, 2 -> %x \n\n", plain_text[0], plain_text[1], plain_text[2]);
}

int add_round_tweakey(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     * TODO:
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

/**
 * ---------------------
 * Utility Stuff
 * ---------------------
 */

void dec_to_bin(int dec, unsigned int binary[])
{
    /**
     * convert decimals to binary for bitwise ops.
     * :returns char [8]
     * question:
     * how to return AN ARRAY.
     */
    // printf("$$$$$$$$$\ndecimal to convert: %d\n", dec);
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

    // for (int i = 0; i < 8; i++)
    // {
    //     printf("%d", binary[i]);
    // }
    // printf("\n$$$$$$$$$\n");
}

void bin_2_hex(unsigned int bits[], unsigned char hex[])
{
    /**
     * Convert the binary to hexadecimal.
     * Code in crude format.
     */
    int hex_1 = bits[3] * pow(2, 0) + bits[2] * pow(2, 1) + bits[1] * pow(2, 2) + bits[0] * pow(2, 3);
    int hex_2 = bits[7] * pow(2, 0) + bits[6] * pow(2, 1) + bits[5] * pow(2, 2) + bits[4] * pow(2, 3);

    hex[0] = hex_converter(hex_1);
    hex[1] = hex_converter(hex_2);
}

int hex_2_dec(unsigned char hex[])
{
    /**
     * Convert Hex to Decimal !!!
     */
    return dec_converter(hex[1]) + dec_converter(hex[0]) * 16;
}

int bin_2_dec(unsigned int bits[])
{
    /**
     * Convert the binary to hexadecimal.
     * Code in crude format.
     */
    return bits[7] * pow(2, 0) + bits[6] * pow(2, 1) + bits[5] * pow(2, 2) + bits[4] * pow(2, 3) + bits[3] * pow(2, 0) + bits[2] * pow(2, 1) + bits[1] * pow(2, 2) + bits[0] * pow(2, 3);
}

unsigned char hex_converter(int val)
{
    /**
     * simple dec to hex converter from (10 - 15) to (0xA - 0xF)
     */
    switch (val)
    {
    case 10:
        return 0xA;
        break;
    case 11:
        return 0xB;
        break;
    case 12:
        return 0xC;
        break;
    case 13:
        return 0xD;
        break;
    case 14:
        return 0xE;
        break;
    case 15:
        return 0xF;
        break;
    default:
        return val;
    }
}

int dec_converter(unsigned char hex_val)
{
    /**
     *
     */
    switch (hex_val)
    {
    case 0xA:
        return 10;
        break;
    case 0xB:
        return 11;
        break;
    case 0xC:
        return 12;
        break;
    case 0xD:
        return 13;
        break;
    case 0xE:
        return 14;
        break;
    case 0xF:
        return 15;
        break;
    default:
        return hex_val;
    }
}

/**
 * ---------------------
 * Debug stuff
 * ---------------------
 */
void debug()
{
    int x = 10;
    char z = '1';
    unsigned char poi = 0x20;
    // unsigned char x = strtol("10010011", NULL, 2); < converts binary to hex.
    printf("%x \n", x); // segmentation fault, why?
    printf("%x \n", poi);
    // because of the single ' instead of "" -_-
    // unsigned char new_hex[2];

    // strcat(new_hex, hex); <<< this function copy string.
    // printf("Final destination string : |%s|", new_hex);
}
