#include "ace.h"
#include<iostream>

//根据用户输入生成访问控制矩阵
int ** ACE::genP()
{
    int **P = (int**)malloc(USERN * sizeof (int*));
    for (int i = 0; i < USERN; i++) {
        P[i] = (int*)malloc(USERN * sizeof (int));
        for (int j = 0; j < USERN; j++) {
            if(j <= i){
                P[i][j] = 1;
            }else {
                P[i][j] = 0;
            }
        }
    }

    return P;
}

KEY* ACE::Setup(int userNum)
{
    USERN = userNum;
    oneace = new ONEACE[USERN];
    P = genP();
    KEY *msk = new KEY[USERN];
    for(int i = 0; i < USERN; i++)
    {
        oneace[i].Setup();
        msk[i] = oneace[i].Gen();
    }

    return msk;
}

//这里返回的是用户密钥
/*
i:[0,,,n+1] //暂定i:[0,n)
ek0 = pp //暂不考虑
用户界面体现：
用户输入身份i及模式mode,获得相应密钥
*/
mpz_t * ACE::Gen(int user_i, int mode, KEY* msk)
{
    int num_ek_i = 0;
    for(int j = 0; j < USERN; j++)
    {
        if(P[user_i][j] == 1)
        {
            num_ek_i++;
        }
    }
    if( sen == mode){ //返回sender i的加密密钥组
        mpz_t *ek_i = new mpz_t[num_ek_i];
        for(int j = 0; j < USERN; j++)
        {
            if(P[user_i][j] == 1)
            {
                mpz_init(ek_i[j]);
                mpz_set(ek_i[j], msk[j].ek);
            }
        }
        return ek_i;
    }else if(rec == mode){
        //暂无法实现返回单一receiver i的解密密钥
        //返回具有解密权限的解密密钥组
        mpz_t *dk_i = new mpz_t[USERN];
        for(int j = 0; j < USERN; j++)
        {
            mpz_init(dk_i[j]);
            mpz_set(dk_i[j], msk[j].dk);
        }
        return dk_i;
    }else if(san == mode){ //返回sanitizer的消毒密钥组 rk
        mpz_t *rk = new mpz_t[USERN];
        for(int j = 0; j < USERN; j++)
        {
            mpz_init(rk[j]);
            mpz_set(rk[j], msk[j].rk);
        }
        return rk;
    }

}
/*
ek_i就体现了用户身份为i
为方便，输入参数加上身份i，与ek_i是对应的
*/
Cipher* ACE::Enc(const mpz_t &msg, const mpz_t * ek_i, int user_i)
{
    Cipher *cipherset = new Cipher[USERN];
    mpz_t r;
    mpz_init(r);
    for(int j = 0; j < USERN; j++)
    {
        if(P[user_i][j] == 1){ //ekj属于ek_i
            cipherset[j] = oneace[j].Enc(msg, ek_i[j]);
        }else{ //ekj不属于ek_i，输出随机密文

//            mpz_set_str(r,"167823564564344543",10);
            oneace[j].genRandom(r);
//            cipherset[j] = oneace[j].Enc(oneace[j].pp.q, oneace[j].pp.h);
            cipherset[j] = oneace[j].Enc(r, oneace[j].msk.alpha);

        }
    }
    mpz_clear(r);
    return cipherset;
}

CipherSan* ACE::San(Cipher* cipherset, const mpz_t * rk)
{
    CipherSan *ciphersanset = new CipherSan[USERN];
    for(int i = 0; i < USERN; i++)
    {
        ciphersanset[i] = oneace[i].San(cipherset[i], rk[i]);
    }

    return ciphersanset;
}

/*
dk_i就体现了用户身份为i
为方便，输入参数加上身份i，与dk_i是对应的
*/
char * ACE::Dec(CipherSan* ciphersanset, const mpz_t * dk_i, int user_i)
{
    char *m;
    m = oneace[user_i].Dec(ciphersanset[user_i], dk_i[user_i]);

    return m;
}
