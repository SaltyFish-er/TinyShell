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
    int len1,out=0;//out记录strout到哪了
    char *a;//存模式串
    int pan1=2,pan2=2;//判断A和B命令中数字
    int k=0,ashu=0,bshu=0,temp;//ashu和bshu记录A和B命令中数字
    bool q=true,c=false,H=false,nq=false,i=false,A=false,B=false;//用于判断各命令启动
    char b[]="--help";
    for(;k<6;k++){
        if(argv[1][k]!=b[k]){
        q=false;
        break;
        }
    }//判断是否是--help命令
    k=0;
    if(q==true)
    {
        char f[]="help:\n-c：输出符合样式的行数\n-h：在显示符合样式的那一行之前，不标示该行所属的文件名称\n-H：在显示符合样式的那一行之前，标示该行所属的文件名称\n-i：忽略字符大小写的差别-n：在显示符合样式的那一行之前，标示出该行的编号\n-A[行数]：除了显示符合范本样式的那一列之外，同时显示该行之后的[行数]行内容\n-B[行数]：除了显示符合样式的那一行之外，同时显示该行之前的[行数]内容\n输出标准样式：grep -h -H -I -n -c -A[行数] -B[行数] 模式串 文件\n";
        strcat(gTerm.strout,f);
        return;
    }
    ifstream fin;
    int z=1;//记录到第几个命令
    while(1)//判断各个{-x}是否被启动
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
                    for(;pan1>=2;pan1--){//计算出ashu
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
                    for(;pan2>=2;pan2--){//计算出bshu
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
    string s;//s用来存储当前文本读入的字符串
    int a4=0,a5=0,num=0,xu=0,chang[100],y,count=0,zu=0,after=0;//chang【n】记录每个子串多长，num记录多少个子串，xu记录当前到哪个子串
    string du[100];//存子串
    for(int a3=0;a3<len1;a3++)//将含通配符*的模式串分解为不含*的几个子串du[]
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
    int w=1,zhi=0,wei=0,w1=0,w2=0;//zhi和wei用于b命令的计数，w1w2用于记录应输出多少个bline字符串
    string bline[bshu+1];//存前面n个字符串，以符合-B的功能
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
        bool op=true;//判断是否要打开文件
        if(argv[z][0]=='-')//判断是否从标准输入中读入
        op=false;
        if(op==true)
        fin.open(openfile);
        if(!fin)
        {
            cerr<<"no such file or directory"<<endl;
            continue;
        }
        bool run=true;//判断文件是否读完
        int len=0,len2=0;//len记录标准输入的尾，len2记录s当前位置
        after=0;
        while(run)
        {   
            len2=0;
            s[0]=0;
            if(op==true){
                if(!getline(fin,s,'\n'))//每次从文件中读入一行到s中，行结束标志为\n
                {
                    run=false;
                    break;
                }
            }
            else
            {
                while(gTerm.strin[len]!='\n'&&gTerm.strin[len]!=EOF&&gTerm.strin[len]!=0)//每次从标准输入中读入一行到s中，行结束标志为\n
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
            }//k记录当前字符串的长度
            bline[zhi]=s;
            zhi++;
            if(zhi==bshu+1){//若bline存到尾，再从头开始存，以重复利用
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
                    if(xu==num-1&&xu>0||(num==1&&a[0]=='*'))//若子串为最后一串或为*xxx类型，从后往前匹配
                    {
                        if(y==0){//y用于判断是否第一次到最后一个字串
                            zu=j;
                        j=k-chang[xu];
                        y++;
                        }
                        else if(j>zu)//往前找
                        j--;
                        if(j<=zu)//若找不到最后一个字串
                        {
                            break;
                        }
                    }
                    else if((xu<num-1||xu==0)){//不然正常从前面开始
                        j++;
                    }
                    for(n=j,t=0;t<chang[xu]&&n<k&&xu<num;)//开始匹配
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
            flag1://开始输出
            if(end!=0||(len1==1&&(a[0]==s[0]||a[0]=='*'||a[0]=='.'))){
                    if(c==true)//若c命令为真，不输出，仅计数
                    {
                        count++;
                        continue;
                    }
                    if(B==true)//若—Bn被激发，输出未输出的前n个字符串
                    {
                        if(wei==0&&w-w1-1<zhi-1&&A==true)//防止与A输出的字符串重复
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
                    if(H==true)//若-H被激发，输出文件名
                    {
                        strcat(gTerm.strout,argv[z]);
                        out=strlen(gTerm.strout);
                        gTerm.strout[out]=':';
                        out++;
                        gTerm.strout[out]=0;
                    }
                    if(nq==true)//若-n被激发，输出行序数
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
                    }//后面开始正式输出本行
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
                if(A==true&&after>0)//若-An被激发，开始输出后n行
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
        if(c==true&&z<argc){//若-c被激发，仅输出符合的个数
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
