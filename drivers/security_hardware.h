#ifndef DRIVERS_SECURITY_HARDWARE_H
#define DRIVERS_SECURITY_HARDWARE_H
 
#include <stdint.h>
 
/* -----------------------------------------------------------------------
 * AES-128 Constants
 * ----------------------------------------------------------------------- */
#define AES_BLOCK_SIZE  16U   /* bytes */
#define AES_KEY_SIZE    16U   /* bytes */
 
/*
 * gAES_Key - The 128-bit encryption key.
 *
 * SECURITY: The MPU must mark the SRAM region containing this symbol as
 * "Privileged Access Only" to satisfy EX-02.  Any unprivileged task (e.g.
 * the UART task) that reads this address will trigger HardFault_Handler.
 *
 * Set to the NIST FIPS-197 Appendix A.1 test-vector key so FT-02 can be
 * validated against the published ciphertext.  Replace before deployment.
 */
extern const uint8_t gAES_Key[AES_KEY_SIZE];
 
/* -----------------------------------------------------------------------
 * AES-128 ECB Encryption
 *
 * Encrypts one 16-byte block (ECB mode, FIPS-197 compliant).
 * The software implementation produces bit-for-bit identical output to the
 * onboard AES-128 hardware accelerator.
 *
 * Hardware path: replace the body of AES128_Encrypt() with calls to
 *   DL_AES_setKey128(), DL_AES_loadInputData(), DL_AES_startEncryption(),
 *   DL_AES_getResult() from the TI MSPM0 DriverLib.
 *
 * FT-02 test vector (NIST FIPS-197 Appendix B):
 *   Plaintext  : 32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34
 *   Key        : 2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c
 *   Ciphertext : 39 25 84 1d 02 dc 09 fb dc 11 85 97 19 6a 0b 32
 * ----------------------------------------------------------------------- */
void AES128_Encrypt(const uint8_t *plaintext,
                    const uint8_t *key,
                    uint8_t       *ciphertext);
 
/* -----------------------------------------------------------------------
 * CRC-32  (IEEE 802.3 / Ethernet, polynomial 0xEDB88320)
 *
 * Computed over every outgoing packet in the UART task.
 * If the CRC of the received packet does not match the transmitted value,
 * the packet must be rejected (test EX-03).
 * ----------------------------------------------------------------------- */
uint32_t CRC32_Calculate(const uint8_t *data, uint32_t length);
 
#endif /* DRIVERS_SECURITY_HARDWARE_H */
 