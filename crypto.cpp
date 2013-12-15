#include "crypto.h"

#include "aes256.h"
#include "sha256.h"
#include "base64.h"

void generate_crypto_key(stegosystem &st, unsigned char *key)
{
    sha_state md;
    sha_init(&md);
	sha_process(&md, (unsigned char *)st.crypto_key.c_str(), st.crypto_key.length());
    sha_done(&md, key);

	sha_init(&md);
	sha_process(&md, key, 16);
	sha_done(&md, key);
}

void generate_stego_key(stegosystem &st, unsigned char *key)
{
    sha_state md;
    sha_init(&md);
	sha_process(&md, (unsigned char *)st.crypto_key.c_str(), st.crypto_key.length());
    sha_done(&md, key);

	sha_init(&md);
	sha_process(&md, key + 16, 16);
	sha_done(&md, key);
}

void crypto_encrypt(stegosystem &st)
{
	unsigned char key[32];
	generate_crypto_key(st, key);

	unsigned char *tmpbuf = new unsigned char[st.stego_size + (16 - st.stego_size % 16)];

	memset(tmpbuf, 0, st.stego_size + (16 - st.stego_size % 16));
	memcpy(tmpbuf, st.stego_data.c_str(), st.stego_size);

    aes256_context ctx; 
    aes256_init(&ctx, key);
	aes256_encrypt_ecb(&ctx, (unsigned char*)tmpbuf);
    aes256_done(&ctx);

	st.stego_data = base64_encode((unsigned char *)tmpbuf, st.stego_size + (16 - st.stego_size % 16));
	st.stego_size = st.stego_data.length();

	delete [] tmpbuf;
}

void crypto_decrypt(stegosystem &st)
{
	unsigned char key[32];
	generate_crypto_key(st, key);
	
	st.stego_data = base64_decode(st.stego_data);
	st.stego_size = st.stego_data.length();
	unsigned char *tmpbuf = new unsigned char[st.stego_size + (16 - st.stego_size % 16)];
	memset(tmpbuf, 0, st.stego_size + (st.stego_size % 16));
	memcpy(tmpbuf, st.stego_data.c_str(), st.stego_size);

    aes256_context ctx; 
    aes256_init(&ctx, key);
	aes256_decrypt_ecb(&ctx, (unsigned char*)tmpbuf);
    aes256_done(&ctx);

	st.stego_data = std::string((char*)tmpbuf);
}