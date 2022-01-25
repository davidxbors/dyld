struct uuid_t {
    unsigned long        time_low;
    unsigned short        time_mid;
    unsigned short        time_hi_and_version;
    unsigned char        clock_seq_hi_and_reserved;
    unsigned char        clock_seq_low;
    unsigned char        node[6];
};

typedef struct uuid_t uuid_t;
