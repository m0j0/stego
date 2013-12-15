#pragma once

#include "bit_stream.h"

void crypto_encrypt(stegosystem &st);
void crypto_decrypt(stegosystem &st);
void generate_crypto_key(stegosystem &st, unsigned char *key);
void generate_stego_key(stegosystem &st, unsigned char *key);