#include <stdint.h>
#include "skinny.h"

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
    // const unsigned char *key = k;
    // const unsigned char *plain_text = p;
    // const unsigned char *output = c;
    return;
}

uint8_t subCells(uint8_t *bits)
{
    /**
     * Written by Henriikka
     *
     *
     */

    // bits = 8 bit array x_7 - x_0

    int i = 0;
    while (i < 4)
    {
        bits[4] = bits[4] ^ ~(bits[7] | bits[6]);
        bits[0] = bits[0] ^ ~(bits[3] | bits[2]);

        if (i < 3)
        {
            // Bit permutation used for 3 first rounds
            uint8_t bit_array = {bits[2], bits[1], bits[7], bits[6], bits[4], bits[0], bits[3], bits[5]};

            bits = bit_array;
        }
        else
        {
            // Bit permutation for last round
            uint8_t bit_array = {bits[7], bits[6], bits[5], bits[4], bits[3], bits[1], bits[2], bits[0]};

            bits = bit_array;
        }
    }

    return bits;
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

uint8_t shift_rows(uint8_t *matrix)
{
    /**
     * Written by Henriikka
     *
     */

    // Rotate cells to match P in paper (ShiftRows)
    uint8_t shifted = {matrix[0], matrix[1], matrix[2], matrix[3], matrix[7], matrix[4], matrix[5], matrix[6], matrix[10],
                       matrix[11], matrix[8], matrix[9], matrix[13], matrix[14], matrix[15], matrix[12]};

    return shifted;
}

int *mix_col(int last_output[])
{
    /**
     * Written by Ibtehaz
     * Convert the last_output into a linear array, of binary, in chunks of 8
     * DEV Comment
     * last_output = one dimensional array from the previous round.
     * We need to multiply it cell by cell and generate a new_array?
     */
    int fixed_array[16] = {1, 0, 1, 1,
                           1, 0, 0, 0,
                           0, 1, 1, 0,
                           1, 0, 1, 0};
    int new_array[16] = {
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

    return new_array;
}
