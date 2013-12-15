typedef struct {
    unsigned long state[8], length, curlen;
    unsigned char buf[64];
}
sha_state;


void sha_init(sha_state * md);
void sha_process(sha_state * md, unsigned char *buf, int len);
void sha_done(sha_state * md, unsigned char *hash);