#include<iostream>
#include<fstream>
#include<string.h>
#include<string>
#include<stdio.h>
#include "heads.h"
using namespace std;
extern Terminal gTerm;
void doGrep(int argc,char *argv[])
{
    int len1,out=0;//out��¼strout������
    char *a;//��ģʽ��
    int pan1=2,pan2=2;//�ж�A��B����������
    int k=0,ashu=0,bshu=0,temp;//ashu��bshu��¼A��B����������
    bool q=true,c=false,H=false,nq=false,i=false,A=false,B=false;//�����жϸ���������
    char b[]="--help";
    for(;k<6;k++){
        if(argv[1][k]!=b[k]){
        q=false;
        break;
        }
    }//�ж��Ƿ���--help����
    k=0;
    if(q==true)
    {
        char f[]="help:\n-c�����������ʽ������\n-h������ʾ������ʽ����һ��֮ǰ������ʾ�����������ļ�����\n-H������ʾ������ʽ����һ��֮ǰ����ʾ�����������ļ�����\n-i�������ַ���Сд�Ĳ��-n������ʾ������ʽ����һ��֮ǰ����ʾ�����еı��\n-A[����]��������ʾ���Ϸ�����ʽ����һ��֮�⣬ͬʱ��ʾ����֮���[����]������\n-B[����]��������ʾ������ʽ����һ��֮�⣬ͬʱ��ʾ����֮ǰ��[����]����\n�����׼��ʽ��grep -h -H -I -n -c -A[����] -B[����] ģʽ�� �ļ�\n";
        strcat(gTerm.strout,f);
        return;
    }
    ifstream fin;
    int z=1;//��¼���ڼ�������
    while(1)//�жϸ���{-x}�Ƿ�����
    {
        if(argv[z][0]=='-')
        {
            switch(argv[z][1])
            {
                case 'c':
                            c=true;
                            z++;
                            break;
                case 'H':
                    H=true;
                    z++;
                break;
                case 'h':
                    z++;
                break;
                case 'i':
                    i=true;
                    z++;
                break;
                case 'n':
                    nq=true;
                    z++;
                break;
                case 'A':
                {
                    pan1=2;
                    while(argv[z][pan1]>='0'&&argv[z][pan1]<='9')
                    {
                        pan1++;
                    }
                    pan1--;
                    if(pan1==1)
                    {
                        cerr<<"grep:No such command '"<<argv[z]<<"'"<<endl;
                        return;
                    }
                    A=true;
                    temp=1;
                    for(;pan1>=2;pan1--){//�����ashu
                        ashu+=(argv[z][pan1]-'0')*temp;
                        temp*=10;
                    }
                    z++;
                    break;
                }
                case 'B':
                {
                    pan2=2;
                    while(argv[z][pan2]>='0'&&argv[z][pan2]<='9')
                    {
                        pan2++;
                    }
                    pan2--;
                    if(pan2==1)
                    {
                        cerr<<"grep:No such command '"<<argv[z]<<"'"<<endl;
                        return;
                    }
                    B=true;
                    temp=1;
                    for(;pan2>=2;pan2--){//�����bshu
                    bshu+=(argv[z][pan2]-'0')*temp;
                    temp*=10;
                    }
                    z++;
                    break;
                }
                default:
                cerr<<"grep:No such command '"<<argv[z]<<"'"<<endl;
                return;
            }
        }
        else
        {
            break;
        }
    }
    a=argv[z];
    len1=strlen(a);
    int t=1,n=1,beg=0,end=0;
    string s;//s�����洢��ǰ�ı�������ַ���
    int a4=0,a5=0,num=0,xu=0,chang[100],y,count=0,zu=0,after=0;//chang��n����¼ÿ���Ӵ��೤��num��¼���ٸ��Ӵ���xu��¼��ǰ���ĸ��Ӵ�
    string du[100];//���Ӵ�
    for(int a3=0;a3<len1;a3++)//����ͨ���*��ģʽ���ֽ�Ϊ����*�ļ����Ӵ�du[]
    {
        if(a[a3]=='*'&&a3==0)
        a4++;
        if((a[a3]=='*'&&a3!=0)||a3==len1-1)
        {
            if(a3==len1-1&&a[a3]!='*')
            a3++;
            for(a5=0;a5<a3-a4;a5++)
            {
                du[num][a5]=a[a5+a4];
            }
            chang[num]=a3-a4;
            a4=a3+1;
            num++;
        }
    }
    char openfile[1005]={0};
    char rootdir[1005]={0};
    char workdir[1005]={0};
    strcat(rootdir,gTerm.root);
    if(gTerm.wdir[0]!='/')
    {workdir[0]='/';
    workdir[1]=0;}
    else{
        workdir[0]=0;
    }
    strcat(workdir,gTerm.wdir);
    int worklen=strlen(workdir);
    if(workdir[worklen-1]!='/')
    {workdir[worklen]='/';
    workdir[worklen+1]=0;}
    else{
        workdir[worklen]=0;
    }
    z++;
    int w=1,zhi=0,wei=0,w1=0,w2=0;//zhi��wei����b����ļ�����w1w2���ڼ�¼Ӧ������ٸ�bline�ַ���
    string bline[bshu+1];//��ǰ��n���ַ������Է���-B�Ĺ���
    gTerm.strout[0]=0;
    for(;z<argc;z++){
        openfile[0]=0;
        if(argv[z][0]=='/')
        {
            strcat(openfile,rootdir);
            strcat(openfile,argv[z]);
        }
        else if(argv[z][0]!='-'){
            strcat(openfile,rootdir);
            strcat(openfile,workdir);
            strcat(openfile,argv[z]);
        }
        w=0;
        w1=0;
        w2=0;
        zhi=0;
        wei=0;
        char *temp=NULL;
        bool op=true;//�ж��Ƿ�Ҫ���ļ�
        if(argv[z][0]=='-')//�ж��Ƿ�ӱ�׼�����ж���
        op=false;
        if(op==true)
        fin.open(openfile);
        if(!fin)
        {
            cerr<<"no such file or directory"<<endl;
            continue;
        }
        bool run=true;//�ж��ļ��Ƿ����
        int len=0,len2=0;//len��¼��׼�����β��len2��¼s��ǰλ��
        after=0;
        while(run)
        {   
            len2=0;
            s[0]=0;
            if(op==true){
                if(!getline(fin,s,'\n'))//ÿ�δ��ļ��ж���һ�е�s�У��н�����־Ϊ\n
                {
                    run=false;
                    break;
                }
            }
            else
            {
                while(gTerm.strin[len]!='\n'&&gTerm.strin[len]!=EOF&&gTerm.strin[len]!=0)//ÿ�δӱ�׼�����ж���һ�е�s�У��н�����־Ϊ\n
                {
                    s+=gTerm.strin[len];
                    len2++;
                    len++;
                    
                }
                if(gTerm.strin[len]==EOF||gTerm.strin[len]==0)
                {
                    run=false;
                }
                len++;
                s+='\0';
                
            }
            k=0;
            while(s[k]!='\0'&&s[k]!='\n'){
            k++;
            }//k��¼��ǰ�ַ����ĳ���
            bline[zhi]=s;
            zhi++;
            if(zhi==bshu+1){//��bline�浽β���ٴ�ͷ��ʼ�棬���ظ�����
                zhi=0;
                wei=1;
            }
            y=0;
            w++;
            xu=0;
            beg=0;
            end=0;
            for(int j=-1;j<k;)
            {
                    if(xu==num-1&&xu>0||(num==1&&a[0]=='*'))//���Ӵ�Ϊ���һ����Ϊ*xxx���ͣ��Ӻ���ǰƥ��
                    {
                        if(y==0){//y�����ж��Ƿ��һ�ε����һ���ִ�
                            zu=j;
                        j=k-chang[xu];
                        y++;
                        }
                        else if(j>zu)//��ǰ��
                        j--;
                        if(j<=zu)//���Ҳ������һ���ִ�
                        {
                            break;
                        }
                    }
                    else if((xu<num-1||xu==0)){//��Ȼ������ǰ�濪ʼ
                        j++;
                    }
                    for(n=j,t=0;t<chang[xu]&&n<k&&xu<num;)//��ʼƥ��
                    {
                        if((du[xu][t]==s[n]||du[xu][t]=='.'||(du[xu][t]>='a'&&du[xu][t]<='z'&&s[n]==du[xu][t]+'A'-'a'&&i==true)||(du[xu][t]>='A'&&du[xu][t]<='Z'&&s[n]==du[xu][t]+'a'-'A'&&i==true))&&t!=chang[xu]-1)
                        {
                            t++;
                            n++;
                        }
                        else if((du[xu][t]==s[n]||du[xu][t]=='.'||(du[xu][t]>='a'&&du[xu][t]<='z'&&s[n]==du[xu][t]+'A'-'a'&&i==true)||(du[xu][t]>='A'&&du[xu][t]<='Z'&&s[n]==du[xu][t]+'a'-'A'&&i==true))&&t==chang[xu]-1){
                            if(xu==0)
                            {
                                beg=j;
                            }
                            if(xu==num-1)
                            {
                                end=n;
                                goto flag1;
                            }
                            xu++;
                            j=n;
                            break;
                        }
                        else if(du[xu][t]!=s[n])
                        break;
                    }
            }
            flag1://��ʼ���
            if(end!=0||(len1==1&&(a[0]==s[0]||a[0]=='*'||a[0]=='.'))){
                    if(c==true)//��c����Ϊ�棬�������������
                    {
                        count++;
                        continue;
                    }
                    if(B==true)//����Bn�����������δ�����ǰn���ַ���
                    {
                        if(wei==0&&w-w1-1<zhi-1&&A==true)//��ֹ��A������ַ����ظ�
                            w2=w-w1-1;
                            else if(w-w1-1>=zhi-1&&wei==0&&A==true){
                                w2=zhi-1;
                            }
                            else if(wei==0&&A==false)
                            {
                                w2=zhi-1;
                            }
                            else if(wei==1&&w-w1-1<bshu&&A==true)
                            {
                                w2=w-w1-1;
                            }
                            else{
                                w2=bshu;
                            }
                        if(wei==1&&((A==true&&after!=ashu)||A==false))
                        {
                            for(int aa=0;aa<w2;aa++)
                            {
                                if(H==true){
                                    strcat(gTerm.strout,argv[z]);
                                    out=strlen(gTerm.strout);
                                    gTerm.strout[out]='-';
                                    out++;
                                    gTerm.strout[out]=0;
                                }
                                if(nq==true){
                                    char f[]="\e[92;1m";
                                    strcat(gTerm.strout,f);
                                    out=strlen(gTerm.strout);
                                    char fen[32];
                                    int shu=1,ti=w-w2+aa;
                                    while(ti/10!=0)
                                    {
                                        shu++;
                                        ti/=10;
                                    }
                                    ti=w-w2+aa;
                                    fen[shu]=0;
                                    for(;shu>0;shu--)
                                    {
                                        fen[shu-1]=ti%10+'0';
                                        ti/=10;
                                    }
                                    strcat(gTerm.strout,fen);
                                    char f1[]="\e[94;1m-\e[0m";
                                    strcat(gTerm.strout,f1);
                                    out=strlen(gTerm.strout);
                                    }
                                    int chang=0;
                                    char f2[MAXLINE];
                                    while(bline[(2*bshu+zhi-w2+1+aa)%(bshu+1)][chang]!='\0'){
                                        f2[chang]=bline[(2*bshu+zhi-w2+1+aa)%(bshu+1)][chang];
                                        chang++;
                                    }
                                    f2[chang]=0;
                                    strcat(gTerm.strout,f2);
                                    out=strlen(gTerm.strout);
                                    gTerm.strout[out]='\n';
                                    out++;
                                    gTerm.strout[out]=0;
                            }
                        }
                        else if(wei==0&&((A==true&&after!=ashu)||A==false)){
                            for(int aa=0;aa<w2;aa++)
                            {
                                if(H==true){
                                    strcat(gTerm.strout,argv[z]);
                                    out=strlen(gTerm.strout);
                                    gTerm.strout[out]='-';
                                    out++;
                                    gTerm.strout[out]=0;
                                }
                                if(nq==true){
                                    char f[]="\e[92;1m";
                                    strcat(gTerm.strout,f);
                                    char fen[32];
                                    int shu=1,ti=w-w2+aa;
                                    while(ti/10!=0)
                                    {
                                        shu++;
                                        ti/=10;
                                    }
                                    ti=w-w2+aa;
                                    fen[shu]=0;
                                    for(;shu>0;shu--)
                                    {
                                        fen[shu-1]=ti%10+'0';
                                        ti/=10;
                                    }
                                    strcat(gTerm.strout,fen);
                                    char f1[]="\e[94;1m-\e[0m";
                                    strcat(gTerm.strout,f1);
                                    out=strlen(gTerm.strout);
                                    }
                                    int chang=0;
                                    while(bline[zhi-w2-1+aa][chang]!='\0'){
                                        gTerm.strout[out]=bline[zhi-w2-1+aa][chang];
                                        chang++;
                                        out++;
                                    }
                                    gTerm.strout[out]='\n';
                                    out++;
                                    gTerm.strout[out]=0;
                            }
                        }
                        zhi=0;
                        wei=0;
                    }
                    after=ashu;
                    if(H==true)//��-H������������ļ���
                    {
                        strcat(gTerm.strout,argv[z]);
                        out=strlen(gTerm.strout);
                        gTerm.strout[out]=':';
                        out++;
                        gTerm.strout[out]=0;
                    }
                    if(nq==true)//��-n�����������������
                    {
                        char f[]="\e[92;1m";
                        strcat(gTerm.strout,f);
                        char fen[32];
                                    int shu=1,ti=w;
                                    while(ti/10!=0)
                                    {
                                        shu++;
                                        ti/=10;
                                    }
                                    ti=w;
                                    fen[shu]=0;
                                    for(;shu>0;shu--)
                                    {
                                        fen[shu-1]=ti%10+'0';
                                        ti/=10;
                                    }
                                    strcat(gTerm.strout,fen);
                        char f1[]="\e[94;1m:\e[0m";
                        strcat(gTerm.strout,f1);
                        out=strlen(gTerm.strout);
                    }//���濪ʼ��ʽ�������
                    if(a[0]=='*')
                    {
                        beg=0;
                    }
                    if(a[len1-1]=='*')
                    {
                        end=k-1;
                    }
                    for(int a1=0;a1<k;a1++){
                        if(a1!=beg&&a1!=end){
                            gTerm.strout[out]=s[a1];
                            out++;
                            gTerm.strout[out]=0;
                        }
                        if(a1==beg)
                        {
                            char f[]="\e[91;1m";
                            strcat(gTerm.strout,f);
                            out=strlen(gTerm.strout);
                            gTerm.strout[out]=s[a1];
                            out++;
                            gTerm.strout[out]=0;
                        }
                        if(a1==end)
                        {
                            if(end!=beg)
                           { gTerm.strout[out]=s[a1];
                            out++;
                            gTerm.strout[out]=0;}
                            char f[]="\e[0m";
                            strcat(gTerm.strout,f);
                            out=strlen(gTerm.strout);
                            gTerm.strout[out]=0;
                        }
                    }
                    gTerm.strout[out]='\n';
                    out++;
                    gTerm.strout[out]=0;
            }
            else{
                if(c!=true)
                if(A==true&&after>0)//��-An����������ʼ�����n��
                {
                    if(H==true){
                        strcat(gTerm.strout,argv[z]);
                        out=strlen(gTerm.strout);
                        gTerm.strout[out]='-';
                        out++;
                        gTerm.strout[out]=0;
                    }
                    if(nq==true)
                    {
                        char f[]="\e[92;1m";
                        strcat(gTerm.strout,f);
                        char fen[32];
                                    int shu=1,ti=w;
                                    while(ti/10!=0)
                                    {
                                        shu++;
                                        ti/=10;
                                    }
                                    ti=w;
                                    fen[shu]=0;
                                    for(;shu>0;shu--)
                                    {
                                        fen[shu-1]=ti%10+'0';
                                        ti/=10;
                                    }
                                    strcat(gTerm.strout,fen);
                        char f1[]="\e[94;1m-\e[0m";
                        strcat(gTerm.strout,f1);
                        out=strlen(gTerm.strout);
                    }
                    int chang=0;
                    while(s[chang]!='\0'){
                        gTerm.strout[out]=s[chang];
                        chang++;
                        out++;
                    }
                    gTerm.strout[out]='\n';
                    out++;
                    gTerm.strout[out]=0;
                    after--;
                    w1=w;
                }
            }
        }
        if(c==true&&z<argc){//��-c����������������ϵĸ���
            strcat(gTerm.strout,argv[z]);
            out=strlen(gTerm.strout);
            gTerm.strout[out]=':';
            out++;
            gTerm.strout[out]=0;
            char fen[32];
            int shu=1,ti=count;
            while(ti/10!=0)
            {
                shu++;
                ti/=10;
            }
            ti=count;
            fen[shu]=0;
            for(;shu>0;shu--)
            {
                fen[shu-1]=ti%10+'0';
                ti/=10;
            }
            strcat(gTerm.strout,fen);
            out=strlen(gTerm.strout);
            count=0;
            gTerm.strout[out]='\n';
            out++;
            gTerm.strout[out]=0;
        }
        if(op==true)
        fin.close();
    }
}
