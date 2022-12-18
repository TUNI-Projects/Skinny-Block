#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "skinny.h"
#include <string.h>
#include <math.h>

// Algorithm Func
void initialization(unsigned char plain_text[],
                    unsigned char tweakey[],
                    unsigned char pt1[],
                    unsigned char tk1[],
                    unsigned char tk2[],
                    unsigned char tk3[]);
void subCells(unsigned char plain_text[]);
void add_constant(unsigned char *plain_text, int round_number);
void add_round_tweakey(unsigned char IS[],
                       unsigned char tk1[],
                       unsigned char tk2[],
                       unsigned char tk3[],
                       unsigned char last_output[]);
void shift_rows(unsigned char matrix[]);
void mix_col(unsigned char last_output[]);

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

void skinny(unsigned char *c,
            const unsigned char *p,
            const unsigned char *k)
{
    /**
     * SKINNY-128-384 block cipher encryption.
     * Under 48-byte tweakey at k,
     * encrypt 16-byte plaintext at p
     * and store the 16-byte output at c.
     * -----
     * Dec 06
     * 1. Add Round Tweakey and Initialization func. I have to write them
     * 2. How to store the results. :/
     */
    unsigned char IS[] = {
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
    };
    unsigned char tk1[] = {
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
    };
    unsigned char tk2[] = {
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
    };
    unsigned char tk3[] = {
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
        '0',
    };
    initialization(p, k, IS, tk1, tk2, tk3);

    unsigned char plain_text[16];
    // copy p into plain_text, use this variable in everywhere else
    // except add_round_tweakey step.
    for (int i = 0; i < 16; i++)
    {
        plain_text[i] = p[i];
    }

    // this is already properly initialized and I can change this val
    unsigned char output[16];

    for (int round = 1; round <= 56; round++)
    {
        subCells(plain_text);
        add_constant(plain_text, round);
        add_round_tweakey(IS, tk1, tk2, tk3, plain_text);
        shift_rows(plain_text);
        mix_col(plain_text);
    }

    // printf("After 56!\n");
    // for (int i = 0; i < 16; i++)
    // {
    //     c[i] = plain_text[i];
    //     printf("%x", c[i]);
    // }
    // printf("\n");
}

void initialization(unsigned char plain_text[],
                    unsigned char tweakey[],
                    unsigned char pt1[],
                    unsigned char tk1[],
                    unsigned char tk2[],
                    unsigned char tk3[])
{
    /**
     * unsigned char plain_text[] - original plain_text array, len 16
     * unsigned char tweakey[] - original tweakey array, len 48
     * unsigned char pt1[] - copy of plain_text array len 16
     * unsigned char tk1[] - copy of tweakey [:16]
     * unsigned char tk2[] - copy of tweakey [16:32]
     * unsigned char tk3[] - copy of tweakey [32:]
     *
     * it basically initialize the entire enc algo. Nothing special happens here
     */
    // copy p into plain_text
    for (int i = 0; i < 16; i++)
    {
        pt1[i] = plain_text[i];
    }

    // copy tk1
    for (int i = 0; i < 16; i++)
    {
        tk1[i] = tweakey[i];
    }

    // copy tk2
    for (int i = 16; i < 32; i++)
    {
        tk2[i - 16] = tweakey[i];
    }

    // copy tk3
    for (int i = 32; i < 48; i++)
    {
        tk3[i - 32] = tweakey[i];
    }
}

void add_round_tweakey(unsigned char IS[],
                       unsigned char tk1[],
                       unsigned char tk2[],
                       unsigned char tk3[],
                       unsigned char last_output[])
{
    /**
     * Add Round Tweakey Func.
     * IS = IS from documentation. This value will rotate in every step of the way
     */
    // Step 1: Giant XOR. IS = IS xor TK1 xor TK2 xor Tk3.
    /**
     * First xor tk2 and tk3 store the result in a separate array.
     * xor that array with tk1, store it in the same array.
     * xor that array with IS, store in IS.
     */
    // printf("-----------------\nAdd Round Tweakey!\n");

    for (int index = 0; index < 16; index++)
    {
        unsigned char tk_2_3 = tk2[index] ^ tk3[index];
        tk_2_3 = tk_2_3 ^ tk1[index];
        tk_2_3 = tk_2_3 ^ IS[index];
        IS[index] = tk_2_3;
    }
    /**
     * XOR m0, m1, m2, m3 with output_from_last_step row 1.
     */
    last_output[0] = last_output[0] ^ IS[0];
    last_output[1] = last_output[1] ^ IS[1];
    last_output[2] = last_output[2] ^ IS[2];
    last_output[3] = last_output[3] ^ IS[3];

    /**
     * XOR m4, m5, m6, m7 with output_from_last_step row 2.
     */
    last_output[4] = last_output[4] ^ IS[4];
    last_output[5] = last_output[5] ^ IS[5];
    last_output[6] = last_output[6] ^ IS[6];
    last_output[7] = last_output[7] ^ IS[7];

    /**
     * TK's transformation
     */
    // Tk1
    unsigned char temp_tk_1[] = {
        tk1[9], tk1[15], tk1[8], tk1[13], tk1[10], tk1[14], tk1[12],
        tk1[11], tk1[0], tk1[1], tk1[2], tk1[3],
        tk1[4], tk1[5], tk1[6], tk1[7]};
    for (int i = 0; i < 16; i++)
    {
        tk1[i] = temp_tk_1[i];
    }

    // Tk2
    unsigned char temp_tk_2[] = {
        tk2[9], tk2[15], tk2[8], tk2[13], tk2[10], tk2[14], tk2[12],
        tk2[11], tk2[0], tk2[1], tk2[2], tk2[3],
        tk2[4], tk2[5], tk2[6], tk2[7]};
    for (int i = 0; i < 16; i++)
    {
        tk2[i] = temp_tk_2[i];
    }

    // Tk3
    unsigned char temp_tk_3[] = {
        tk3[9], tk3[15], tk3[8], tk3[13], tk3[10], tk3[14], tk3[12],
        tk3[11], tk3[0], tk3[1], tk3[2], tk3[3],
        tk3[4], tk3[5], tk3[6], tk3[7]};
    for (int i = 0; i < 16; i++)
    {
        tk3[i] = temp_tk_3[i];
    }

    /**
     * First and Second Row of TK2 and TK3 will get an LFSR :)
     * 0 1 2 3
     * 4 5 6 7
     * TK2 and TK3 will have different technique.
     */
    for (int i = 0; i < 8; i++)
    {
        // TK2 operation
        unsigned int tk2_bits[8];
        unsigned int new_tk2_bits[8];
        dec_to_bin(tk2[i], tk2_bits);

        new_tk2_bits[0] = tk2_bits[7] ^ tk2_bits[5];
        for (int j = 1; j < 8; j++)
        {
            new_tk2_bits[j] = tk2_bits[j - 1];
        }
        tk2[i] = bin_2_dec(new_tk2_bits);

        // TK3 operation

        unsigned int tk3_bits[8];
        unsigned int new_tk3_bits[8];
        dec_to_bin(tk3[i], tk3_bits);

        new_tk3_bits[7] = tk3_bits[0] ^ tk3_bits[6];
        for (int j = 0; j < 7; j++)
        {
            new_tk3_bits[j] = tk3_bits[j + 1];
        }
        tk3[i] = bin_2_dec(new_tk3_bits);
    }
    // done
    return;
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
                unsigned int new_bit[] = {bits[5], bits[3], bits[0], bits[4], bits[6], bits[7], bits[1], bits[2]};
                for (int j = 0; j < 8; j++)
                {
                    bits[j] = new_bit[j];
                }
            }
            else
            {
                // Bit permutation for last round
                unsigned int new_bit[] = {bits[0], bits[2], bits[1], bits[3], bits[4], bits[5], bits[6], bits[7]};
                for (int j = 0; j < 8; j++)
                {
                    bits[j] = new_bit[j];
                }
            }
        }
        plain_text[index] = bin_2_dec(bits);
    }
    // i need to make sure value is persistent inside plain_text.
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%x", plain_text[i]);
    // }
    // printf("\n\n");
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
    // printf("-----------------\nAdd Constant!\n");
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%x ", plain_text[i]);
    // }
    // printf("\n");
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
    unsigned int c0[] = {c1_c0[7 - 0], c1_c0[7 - 1], c1_c0[7 - 2], c1_c0[7 - 3], 0, 0, 0, 0};
    unsigned int c1[] = {c1_c0[7 - 4], c1_c0[7 - 5], 0, 0, 0, 0, 0, 0};

    unsigned char c0_hex[2];
    bin_2_hex(c0, c0_hex);
    int c0_dec = hex_2_dec(c0_hex);

    unsigned char c1_hex[2];
    bin_2_hex(c1, c1_hex);
    int c1_dec = hex_2_dec(c1_hex);

    plain_text[0] = plain_text[0] + c0_dec;
    plain_text[1] = plain_text[1] + c1_dec;
    plain_text[2] = plain_text[2] + c2;

    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%x ", plain_text[i]);
    // }
    // printf("\n\n");
}

void shift_rows(unsigned char matrix[])
{
    /**
     * Written by Henriikka
     */
    // Debug stuff
    // printf("-----------------\nShift Rows!\n");
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%x ", matrix[i]);
    // }
    // printf("\n");
    // Debug stuff
    // Rotate cells to match P in paper (ShiftRows)
    unsigned char shifted[] = {matrix[0], matrix[1], matrix[2], matrix[3], matrix[7], matrix[4], matrix[5], matrix[6], matrix[10],
                               matrix[11], matrix[8], matrix[9], matrix[13], matrix[14], matrix[15], matrix[12]};

    for (int i = 0; i < 16; i++)
    {
        // copying to old array
        matrix[i] = shifted[i];
    }
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%x ", matrix[i]);
    // }
    // printf("\n\n");
}

void mix_col(unsigned char last_output[])
{
    /**
     * Written by Ibtehaz
     */
    // printf("-----------------\nMix Col!\n");
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%x ", last_output[i]);
    // }
    // printf("\n");

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
        // copying to old array
        last_output[i] = new_array[i];
    }

    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%x ", last_output[i]);
    // }
    // printf("\n\n");
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
        binary[index] = (dec % 2);
        dec = dec / 2;
    }

    // // reverse the order of bin in the bin array.
    // for (int i = 0; i < 8; i++)
    // {
    //     reverse[8 - 1 - i] = mid_val[i];
    // }

    // put everything inside the original pointer
    // for (int i = 0; i < 8; i++)
    // {
    //     binary[i] = mid_val[i];
    // }

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
    int hex_1 = bits[3] * pow(2, 3) + bits[2] * pow(2, 2) + bits[1] * pow(2, 1) + bits[0] * pow(2, 0);
    int hex_2 = bits[7] * pow(2, 3) + bits[6] * pow(2, 2) + bits[5] * pow(2, 1) + bits[4] * pow(2, 0);

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
     * Convert the binary to DECIMAL!.
     * Code in crude format.
     */
    return bits[0] * pow(2, 0) + bits[1] * pow(2, 1) + bits[2] * pow(2, 2) + bits[3] * pow(2, 3) + bits[4] * pow(2, 4) + bits[5] * pow(2, 5) + bits[6] * pow(2, 6) + bits[7] * pow(2, 7);
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
