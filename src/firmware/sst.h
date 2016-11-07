enum PHASES {
    STOPPED = 1, STARTED = 2
};

struct sst {
    long int start;
    long int stop;
    volatile uint8_t* port;
    int bit;
    enum PHASES phase;
};

extern void sst_activate(struct sst* self, long int currentTime);
