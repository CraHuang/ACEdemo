#ifndef COMMONDATATYPE_H
#define COMMONDATATYPE_H
#include<gmp.h>

#define BIGNUM_LEN 1024


//主密钥
struct MSK
{
    mpz_t alpha; //0~q之间的随机数
    mpz_t x; //0~q之间的随机数
};

//公共参数
struct PP
{
    mpz_t q; //素数阶(大素数)
    mpz_t g; //生成元
    mpz_t h; //h = g^x
};

//模式：sen = 0, rec = 1, san = 2
enum MODE {sen, rec, san};

//1ACE:单身份密钥空间
struct KEY
{
    mpz_t ek;
    mpz_t dk;
    mpz_t rk;
};

//1ACE:密文
struct Cipher
{
    mpz_t c0;
    mpz_t c1;
    mpz_t c2;
    mpz_t c3;
};

//1ACE:消毒密文
struct CipherSan
{
    mpz_t csan0;
    mpz_t csan1;
};

#endif // COMMONDATATYPE_H
