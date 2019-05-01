#ifndef ONEACE_H
#define ONEACE_H


#include "commondatatype.h"

class ONEACE
{
    protected:    
        void genPrime(mpz_t &mpzPrime, int bits);
    public:
        void genRandom(mpz_t &random);
        void Setup();
        KEY Gen();
        Cipher Enc(const mpz_t &msg, const mpz_t &ek);
        CipherSan San(Cipher &cipher, const mpz_t &rk);
        char * Dec(CipherSan &c_san, const mpz_t &dk);
    public:
        PP pp;
        MSK msk;
};

#endif // ONEACE_H
