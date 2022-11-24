#include <stdint.h>
#include "skinny.h"

/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    const unsigned char *key = k;
    const unsigned char *plain_text = p;
    const unsigned char *output = c;
}

uint8_t subCells(uint8_t * bits)
{
    /**
     * Written by Henriikka
     *
     *
     */

    // bits = 8 bit array x_7 - x_0

    int i = 0;
    while ( i < 4){
        bits[4] = bits[4] ^ ~(bits[7] | bits[6]);
        bits[0] = bits[0] ^ ~(bits[3] | bits[2]);

        if (i < 3){
             //Bit permutation used for 3 first rounds
            uint8_t bit_array = {bits[2], bits[1], bits[7], bits[6], bits[4], bits[0], bits[3], bits[5]};

            bits = bit_array;
  
        }
        else{
             //Bit permutation for last round
            uint8_t bit_array = {bits[7], bits[6], bits[5], bits[4], bits[3], bits[1], bits[2], bits[0]};

            bits = bit_array;
        }
    }

  
    return bits;
}

int add_constant(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     * Written by Ibtehaz
     *
     */
    return -1;
}

int add_round_tweakey(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     *
     *
     */
}

uint8_t shift_rows(uint8_t * matrix){
    /**
     * Written by Henriikka
     *
     */

    // Rotate cells to match P in paper (ShiftRows)
    uint8_t shifted = { matrix[0], matrix[1], matrix[2], matrix[3], matrix[7], matrix[4], matrix[5], matrix[6], matrix[10], 
                        matrix[11], matrix[8], matrix[9], matrix[13], matrix[14], matrix[15], matrix[12] };

    return shifted;
}

int mix_col(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     * Written by Ibtehaz
     *
     */
    return -1;
}
