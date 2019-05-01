#ifndef ACE_H
#define ACE_H

#include "oneace.h"

class ACE
{
    public:
        int** genP();
        KEY* Setup(int userNum);
        mpz_t * Gen(int user_i, int mode, KEY* msk);
        Cipher* Enc(const mpz_t &msg, const mpz_t * ek_i, int user_i);
        CipherSan* San(Cipher* ciphersetr, const mpz_t * rk);
        char * Dec(CipherSan* ciphersanset, const mpz_t * dk_i, int user_i);
    public:
        ONEACE* oneace; //根据用户数量来初始化oneace
        int USERN; //用户数量
        int **P; //访问控制矩阵
};


#endif // ACE_H
