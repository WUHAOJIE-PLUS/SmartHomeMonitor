#include <shadow.h>
#include <func.h>

void help(void)
{
    printf("用户密码验证程序\n 第一个参数为用户名!\n");
    exit(-1);
}

void error_quit(char *msg)
{
    perror(msg);
    exit(-2);
}

void get_setting(char *salt,char *passwd){
    int i,j;

    for(i = 0,j = 0;passwd[i] && j != 4;++i){
        if(passwd[i] == '$'){
            ++j;
        }
    }
    strncpy(salt,passwd,i);
}
int main(int argc,char *argv[])
{
    struct spwd *sp;
    char *passwd;
    char setting[512] = {0};

    if(argc != 2){
        help();
    }

   passwd = getpass("请输入密码：");
    printf("passwd:%s\n",passwd );

    //读取/etc/shadow文件中对应的用户名的
    //加密密文信息
    if((sp = getspnam(argv[1])) == NULL){
        error_quit("获取用户名和密码");
    }

    printf("from shadow file,sp->sp_pwdp: %s\n",sp->sp_pwdp);

    get_setting(setting,sp->sp_pwdp);
    setting[strlen(setting) - 1] = '\0';

    printf("setting: %s\n",setting);
    char *encode = crypt(passwd,setting);
    printf("encrypted: %s\n",encode);

    if(strcmp(sp->sp_pwdp,encode) == 0){
        printf("验证通过!\n");
    }else{
        printf("验证失败!\n");
    }

    return 0;
}

