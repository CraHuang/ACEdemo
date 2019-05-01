#include "oneace.h"
#include<ctime>
#include<cstdlib>


//问题：产生的不是真正的随机数，每次都一样 （未解决）,仍未找到方法
void  ONEACE::genRandom(mpz_t &random)
{
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    struct timespec time1 = {0,0};
    clock_gettime(CLOCK_REALTIME,&time1);
    mpz_t seed;
    mpz_init_set_si(seed,time1.tv_nsec);
    gmp_randseed(grt,seed);
    mpz_urandomm(random, grt, pp.q);

}

void ONEACE::genPrime(mpz_t &mpzPrime, int bits)
{
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    struct timespec time1 = {0,0};
    clock_gettime(CLOCK_REALTIME,&time1);
    mpz_t seed;
    mpz_init_set_si(seed,time1.tv_nsec);
    gmp_randseed(grt,seed);
    mpz_urandomb(mpzPrime,grt,bits);
    mpz_nextprime(mpzPrime, mpzPrime);


}

/*
void  ONEACE::Setup()
{
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    struct timespec time1 = {0,0};
    clock_gettime(CLOCK_REALTIME,&time1);
    mpz_t seed;
    mpz_init_set_si(seed,time1.tv_nsec);
    gmp_randseed(grt,seed);
    mpz_init(pp.q);
    mpz_urandomb(pp.q,grt,BIGNUM_LEN);
    mpz_nextprime(pp.q, pp.q);
    mpz_init(pp.g);
    mpz_urandomm(pp.g, grt, pp.q);
    mpz_init(msk.alpha);
    mpz_urandomm(msk.alpha, grt, pp.q);
    mpz_init(msk.x);
    mpz_urandomm(msk.x, grt, pp.q);
    mpz_init(pp.h);
    mpz_powm(pp.h, pp.g, msk.x, pp.q);	//h = g^x
}
*/

void  ONEACE::Setup()
{
    //初始化公共参数及主密钥
    mpz_init(pp.q);
    genPrime(pp.q, BIGNUM_LEN); //q为大素数，循环群(明文空间)的阶
    mpz_init(pp.g);
    genRandom(pp.g); //g为生成元，0~q之间的随机数
    mpz_init(msk.alpha);
    genRandom(msk.alpha); //alpha是0~q之间的随机数
    mpz_init(msk.x);

    //如果msk == x,则重复随机化过程**********OK,一定程度上缓解了随机数不随机的问题
    do{
        genRandom(msk.x); //x是0~q之间的随机数
    }while(mpz_cmp(msk.alpha, msk.x) == 0);

    mpz_init(pp.h);
    mpz_powm(pp.h, pp.g, msk.x, pp.q);	//h = g^x
}


KEY ONEACE::Gen()
{
    KEY key;
    mpz_init(key.ek);
    mpz_init(key.dk);
    mpz_init(key.rk);

    mpz_set(key.ek, msk.alpha);
    mpz_set(key.dk, msk.x);
    mpz_set(key.rk, msk.alpha);

   //算法理解错误：dk = -x 不是负数的意思，而是加法群里的逆元 ，但是加法逆元也就是相反数！!!
    mpz_neg(key.dk, key.dk); //把dk变成-dk,相当于dk取反
    mpz_neg(key.rk, key.rk);

    return key;
}

Cipher ONEACE::Enc(const mpz_t &msg, const mpz_t &ek)
{
    Cipher cipher;
    mpz_init(cipher.c0);
    mpz_init(cipher.c1);
    mpz_init(cipher.c2);
    mpz_init(cipher.c3);
    //计算c1的辅助变量
    mpz_t c1a,c1b;
    mpz_init(c1a);
    mpz_init(c1b);

    //生成随机数r1,r2
    mpz_t r1,r2;
    mpz_init(r1);
    mpz_init(r2);
    genRandom(r1);
    //防止r1==r2
    do{
        genRandom(r2);
    }while(mpz_cmp(r1, r2) == 0);

    //计算密文
    //c0
    mpz_powm(cipher.c0, pp.g, r1, pp.q);
    //c1
    mpz_powm(c1a, pp.g, ek, pp.q);
    mpz_powm(c1b, pp.h, r1, pp.q);
    mpz_mul(cipher.c1, c1a, c1b);
    mpz_mod(cipher.c1, cipher.c1, pp.q);
    //c2
    mpz_powm(cipher.c2, pp.g, r2, pp.q);
    //c3
    mpz_powm(cipher.c3, pp.h, r2, pp.q);
    mpz_mul(cipher.c3, cipher.c3, msg);
    mpz_mod(cipher.c3, cipher.c3, pp.q);

    mpz_clear(c1a);
    mpz_clear(c1b);
    mpz_clear(r1);
    mpz_clear(r2);

    return cipher;
}

CipherSan ONEACE::San(Cipher &cipher, const mpz_t &rk)
{
    CipherSan ciphersan;
    mpz_init(ciphersan.csan0);
    mpz_init(ciphersan.csan1);
    //计算消毒密文的辅助变量
    mpz_t temp1,temp2,temp3,temp4;
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_init(temp3);
    mpz_init(temp4);

    //生成随机数s1,s2
    mpz_t s1,s2;
    mpz_init(s1);
    mpz_init(s2);
    genRandom(s1);
    //防止s1==s2
    do{
         genRandom(s2);
    }while(mpz_cmp(s1, s2) == 0);

    //计算消毒密文
    //c01
    mpz_powm(temp1, cipher.c0, s1, pp.q);
    mpz_powm(temp2, pp.g, s2, pp.q);
    mpz_mul(ciphersan.csan0, temp1, temp2);
    mpz_mul(ciphersan.csan0, ciphersan.csan0, cipher.c2);
    mpz_mod(ciphersan.csan0, ciphersan.csan0, pp.q);
    //c11
    mpz_powm(temp3, pp.g, rk, pp.q);
    mpz_mul(temp3, temp3, cipher.c1);
    mpz_powm(temp3, temp3, s1, pp.q);
    mpz_powm(temp4, pp.h, s2, pp.q);
    mpz_mul(ciphersan.csan1, temp3, temp4);
    mpz_mul(ciphersan.csan1, ciphersan.csan1, cipher.c3);
    mpz_mod(ciphersan.csan1, ciphersan.csan1, pp.q);

    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);
    mpz_clear(temp4);
    mpz_clear(s1);
    mpz_clear(s2);

    return ciphersan;
}

char * ONEACE::Dec(CipherSan &ciphersan, const mpz_t &dk)
{
    mpz_t m;
    mpz_init(m);

    mpz_powm(m, ciphersan.csan0, dk, pp.q);
    mpz_mul(m, ciphersan.csan1, m);
    mpz_mod(m, m, pp.q);

    size_t len = mpz_sizeinbase(m,10);
    char * res = new char[len+1];
    mpz_get_str(res, 10, m);

    return res;
}
