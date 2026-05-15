#include "security_hardware.h"
#include <string.h>
 
/* -----------------------------------------------------------------------
 * AES-128 Key Definition
 *
 * NIST FIPS-197 Appendix A.1 test key — swap out before deployment.
 * The MPU region covering this symbol must be "Privileged Access Only".
 * ----------------------------------------------------------------------- */
const uint8_t gAES_Key[AES_KEY_SIZE] = {
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
 
/* -----------------------------------------------------------------------
 * AES-128 Internal Tables  (FIPS-197 compliant)
 * ----------------------------------------------------------------------- */
 
/* Forward S-box */
static const uint8_t sbox[256] = {
    0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
    0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
    0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
    0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
    0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
    0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
    0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
    0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
    0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
    0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
    0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
    0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
    0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
    0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
    0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
    0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16
};
 
/* Round constants (one per AES-128 round, rounds 1-10) */
static const uint8_t rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};
 
/* -----------------------------------------------------------------------
 * GF(2^8) multiply-by-2  (xtime)
 * Irreducible polynomial: x^8 + x^4 + x^3 + x + 1  (0x1B)
 * ----------------------------------------------------------------------- */
static inline uint8_t xtime(uint8_t x) {
    return (x << 1) ^ ((x & 0x80U) ? 0x1BU : 0x00U);
}
 
/* -----------------------------------------------------------------------
 * Key Expansion  (FIPS-197 Section 5.2, 11 round keys × 16 bytes)
 * ----------------------------------------------------------------------- */
static void key_expansion(const uint8_t *key, uint8_t *rk) {
    memcpy(rk, key, 16);
 
    for (int i = 1; i <= 10; i++) {
        const uint8_t *prev = rk + (i - 1) * 16;
        uint8_t       *curr = rk +  i      * 16;
 
        /* SubWord(RotWord(last 4 bytes of prev round)) XOR Rcon */
        curr[0] = sbox[prev[13]] ^ rcon[i - 1] ^ prev[0];
        curr[1] = sbox[prev[14]]               ^ prev[1];
        curr[2] = sbox[prev[15]]               ^ prev[2];
        curr[3] = sbox[prev[12]]               ^ prev[3];
 
        /* Remaining three words: XOR with word 4 bytes earlier */
        for (int j = 4; j < 16; j++) {
            curr[j] = curr[j - 4] ^ prev[j];
        }
    }
}
 
/* -----------------------------------------------------------------------
 * AES Round Functions
 * State layout: flat 16-byte array, column-major.
 *   state[r + 4*c]  =  state[row r, column c]
 * Input/output bytes map directly: state[i] = plaintext[i]  (FIPS-197 §3.4)
 * ----------------------------------------------------------------------- */
static void add_round_key(uint8_t *state, const uint8_t *rk) {
    for (int i = 0; i < 16; i++) { state[i] ^= rk[i]; }
}
 
static void sub_bytes(uint8_t *state) {
    for (int i = 0; i < 16; i++) { state[i] = sbox[state[i]]; }
}
 
/* ShiftRows: row r is shifted left cyclically by r positions */
static void shift_rows(uint8_t *s) {
    uint8_t t;
    /* Row 1 (indices 1,5,9,13): left shift by 1 */
    t = s[1];  s[1] = s[5];  s[5] = s[9];  s[9] = s[13]; s[13] = t;
    /* Row 2 (indices 2,6,10,14): left shift by 2 (swap pairs) */
    t = s[2];  s[2] = s[10]; s[10] = t;
    t = s[6];  s[6] = s[14]; s[14] = t;
    /* Row 3 (indices 3,7,11,15): left shift by 3 = right shift by 1 */
    t = s[15]; s[15] = s[11]; s[11] = s[7]; s[7] = s[3]; s[3] = t;
}
 
/*
 * MixColumns: each column is multiplied by the MDS matrix in GF(2^8).
 * Matrix:  [2 3 1 1]
 *          [1 2 3 1]
 *          [1 1 2 3]
 *          [3 1 1 2]
 */
static void mix_columns(uint8_t *s) {
    for (int c = 0; c < 4; c++) {
        uint8_t *col = s + 4 * c;
        uint8_t a0 = col[0], a1 = col[1], a2 = col[2], a3 = col[3];
        col[0] = xtime(a0) ^ (xtime(a1) ^ a1) ^ a2            ^ a3;
        col[1] = a0        ^ xtime(a1)         ^ (xtime(a2)^a2)^ a3;
        col[2] = a0        ^ a1                ^ xtime(a2)     ^ (xtime(a3)^a3);
        col[3] = (xtime(a0)^a0) ^ a1           ^ a2            ^ xtime(a3);
    }
}
 
/* -----------------------------------------------------------------------
 * AES128_Encrypt  (public API)
 * ----------------------------------------------------------------------- */
void AES128_Encrypt(const uint8_t *plaintext,
                    const uint8_t *key,
                    uint8_t       *ciphertext)
{
    /* 11 round keys × 16 bytes = 176 bytes on the stack */
    uint8_t rk[11 * 16];
    uint8_t state[16];
 
    key_expansion(key, rk);
    memcpy(state, plaintext, 16);   /* load plaintext into state */
 
    add_round_key(state, rk);       /* initial round */
 
    for (int round = 1; round <= 9; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, rk + round * 16);
    }
 
    /* Final round: no MixColumns */
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, rk + 10 * 16);
 
    memcpy(ciphertext, state, 16);  /* write result */
}
 
/* -----------------------------------------------------------------------
 * CRC-32  (IEEE 802.3, reflected polynomial 0xEDB88320)
 * ----------------------------------------------------------------------- */
uint32_t CRC32_Calculate(const uint8_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFFUL;
 
    while (length--) {
        crc ^= *data++;
        for (int bit = 0; bit < 8; bit++) {
            crc = (crc >> 1) ^ ((crc & 1U) ? 0xEDB88320UL : 0UL);
        }
    }
 
    return crc ^ 0xFFFFFFFFUL;
}
 