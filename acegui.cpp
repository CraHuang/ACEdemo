#include "acegui.h"
#include "ui_acegui.h"
#include<cstdio>
#include <QFileDialog>
#include<QComboBox>
#include <QMessageBox>
#include <QTextStream>

ACEGUI::ACEGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ACEGUI)
{
    ui->setupUi(this);
    ui->lineEdit_userinput->setPlaceholderText(tr("please input user numbers"));
    ui->lineEdit_UserId->setPlaceholderText(tr("Input DataUser Id"));
    ui->lineEdit_OwnerId->setPlaceholderText(tr("Input DataOwner Id"));
//    ui->textEdit_showP->setPlaceholderText("Show Access Control Matrix");
    ui->lineEdit->setPlaceholderText(tr("choose file"));
    ui->lineEdit_2->setPlaceholderText(tr("choose file"));

//    //初始化ACE环境
//    QString input = ui->lineEdit_userinput->text(); //从部件中获取用户输入的字符
//    int userNum = input.toInt(); //将QString转为int
//    keyset = ace.Setup(userNum);
    //怪不得错误，这时候获取到的是空字符串
}

ACEGUI::~ACEGUI()
{
    delete ui;
}


void ACEGUI::on_ownerOpenButton_clicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    ui->lineEdit->setText(fileName);
}

void ACEGUI::on_encryptButton_clicked(bool checked)
{
    QString fileName = ui->lineEdit->text();
    //点击加密时检查是否选择了文件
    if (fileName == nullptr) {
        QMessageBox::warning(this, "Warning", "Please choose file first! ");
        return;
    }
    //将QString转为char*
    char * path;
    QByteArray ba = fileName.toLatin1();
    path = ba.data();

    //获取用户身份
    QString input = ui->lineEdit_OwnerId->text(); //从部件中获取用户输入的字符
    int user_i = input.toInt(); //将QString转为int

    int mode = 0; //sender
    FILE *fpread = fopen(path,"r");

    //读取打开文件内容到msg
    mpz_t msg;
    mpz_init(msg);
    //mpz_set_ui(msg, 0); //初始化明文内容为0
    mpz_inp_str(msg,fpread,10);
    //对msg加密
    mpz_t *userKey = ace.Gen(user_i, mode, keyset);
    mpz_t *santizeKey = ace.Gen(user_i, 2, keyset);
    Cipher * cipherset = ace.Enc(msg, userKey, user_i);
    //将密文写进指定文件
    FILE *fpenc = fopen("D:/ACEdata/DataOwner/cipher.txt","w+");
    for(int i = 0; i < ace.USERN; i++)
    {
        mpz_out_str(fpenc,10,cipherset[i].c0);
        fputc(10,fpenc);
        mpz_out_str(fpenc,10,cipherset[i].c1);
        fputc(10,fpenc);
        mpz_out_str(fpenc,10,cipherset[i].c2);
        fputc(10,fpenc);
        mpz_out_str(fpenc,10,cipherset[i].c3);
        fputc(10,fpenc);
    }
    fclose(fpenc);

    //对密文进行消毒
    CipherSan *ciphersanset = ace.San(cipherset, santizeKey);
    //将消毒密文写进指定文件
    FILE *fpsan = fopen("D:/ACEdata/CLOUD/cipherSan.txt","w+");
    for(int i = 0; i < ace.USERN; i++)
    {
        mpz_out_str(fpsan,10,ciphersanset[i].csan0);
        fputc(10,fpenc);
        mpz_out_str(fpsan,10,ciphersanset[i].csan1);
        fputc(10,fpenc);
    }
    fclose(fpsan);

    QMessageBox::information(this, "提示", "加密完成！ ");
}

//和on_ownerOpenButton_clicked是同样的，待改进
void ACEGUI::on_userOpenButton_clicked(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    ui->lineEdit_2->setText(fileName);
}

void ACEGUI::on_decryptButton_clicked(bool checked)
{
    QString fileName = ui->lineEdit_2->text();
    //点击解密时检查是否选择了文件
    if (fileName == nullptr) {
        QMessageBox::warning(this, "Warning", "Please choose file first! ");
        return;
    }
    //将QString转为char*
    char * path;
    QByteArray ba = fileName.toLatin1();
    path = ba.data();
    FILE *fpread = fopen(path,"r");

    //获取用户身份
    QString input = ui->lineEdit_UserId->text(); //从部件中获取用户输入的字符
    int user_i = input.toInt(); //将QString转为int

    int mode = 1;  //receiver
    mpz_t *userKey = ace.Gen(user_i, mode, keyset);

    char *oneline = new char[BIGNUM_LEN +1];
    //char *oneline = (char*)malloc((BIGNUM_LEN + 1)* sizeof (char));
    CipherSan *ciphersanset = new CipherSan[ace.USERN];

    //用文件数据初始化ciphersanset
    int i = 0;
    while (fgets(oneline, BIGNUM_LEN, fpread) != nullptr) {
        mpz_init_set_str(ciphersanset[i].csan0,oneline, 10);
        fgets(oneline, BIGNUM_LEN, fpread);
        mpz_init_set_str(ciphersanset[i].csan1,oneline, 10);
        i++;
    }
/* 测试是否成功初始化ciphersanset
    FILE *fpdec = fopen("D:/ACEdata/DataUser/decryptedText.txt","w+");
    for(int i = 0; i < USERN; i++)
    {
        mpz_out_str(fpdec,10,ciphersanset[i].csan0);
        fputc(10,fpdec);
        mpz_out_str(fpdec,10,ciphersanset[i].csan1);
        fputc(10,fpdec);
    }
    fclose(fpdec);
*/

    char *m = ace.Dec(ciphersanset, userKey, user_i);

    FILE *fpdec = fopen("D:/ACEdata/DataUser/decryptedText.txt","w+");
    fputs(m, fpdec);
    fclose(fpdec);

    QMessageBox::information(this, "提示", "解密完成！ ");
}

void ACEGUI::on_initButton_clicked()
{
    //完成ACE环境的初始化
    QString input = ui->lineEdit_userinput->text(); //从部件中获取用户输入的字符
    int userNum = input.toInt(); //将QString转为int
    keyset = ace.Setup(userNum);
   // ui->textEdit_showP->setText("P has generated.");
    FILE *fpek = fopen("D:/ACEdata/ACEsys/ek.txt","w+");
    //生成访问控制矩阵文件到ACEsys
    FILE *fpP = fopen("D:/ACEdata/ACEsys/P.txt","w+");
    char ch;
    for (int i = 0; i < userNum; i++) {
        for (int j = 0; j < userNum; j++) {
            ch = ace.P[i][j] + '0';
            fputc(ch,fpP);
            fputc(32,fpP); //写入空格
        }
        fputc(10,fpP); //写入换行
    }
    fclose(fpP);

    QMessageBox::information(this, "提示", "环境初始化完成！ ");

}

void ACEGUI::on_showPpushButton_clicked()
{
    //将访问控制矩阵显示到textEdit_showP
    QString fileName = "D:/ACEdata/ACEsys/P.txt";
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QFile::Text);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit_showP->setText(text);
    file.close();
}

void ACEGUI::on_lineEdit_OwnerId_textChanged(const QString &arg1)
{
    QString input = ui->lineEdit_OwnerId->text(); //从部件中获取用户输入的字符
    int user_i = input.toInt(); //将QString转为int
    int mode = 0;
    mpz_t *userKey = ace.Gen(user_i, mode, keyset);

}


void ACEGUI::on_lineEdit_UserId_returnPressed()
{
    QString input = ui->lineEdit_OwnerId->text(); //从部件中获取用户输入的字符
    int user_i = input.toInt(); //将QString转为int
    mpz_t dk;
    mpz_init(dk);
    mpz_set(dk, keyset[user_i].dk);

    size_t len = mpz_sizeinbase(dk,10);
    char * dki = new char[len+1];

    mpz_get_str(dki, 10, dk);
    //将char*转为QString
    QString str = QString(QLatin1String(dki));

    ui->textEdit_showDK->setText(str);
}
