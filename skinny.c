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

int subcell(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     * Written by Henriikka
     *
     *
     */
    return -1;
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

int shift_rows(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     * Written by Henriikka
     *
     */
    return -1;
}

int mix_col(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    /**
     * Written by Ibtehaz
     *
     */
    return -1;
}
