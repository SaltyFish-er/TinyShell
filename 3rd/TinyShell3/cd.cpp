//切换工作目录 
#include <iostream>//done
#include <cstring>//tested
#include <fstream>//wrong
#include "heads.h"
using namespace std;

char temp[1000];
char temmp[1000];//反向的 
char curpath[1000];
int idd=0;
int cid=0;
int meetslash=0;

void doCd(int argc,char* argv[]){
	if (strcmp(argv[1],"--help")==0){
		ifstream fin("cd --help.txt");
		for (string str;getline(fin,str);){
			cout<<str<<endl;
		}
		fin.close();
	}
	if (argv[1][0]!='/'){//不是绝对路径 
		strcpy(temp,gTerm.wdir);
		if (strlen(gTerm.wdir)>1){
			strcat(temp,"/");
		}
		strcat(temp,argv[1]);
	}
	else {
		strcpy(temp,argv[1]);
	}
	if (strcmp(argv[1],"..")==0){
		for (int i=strlen(temp)-1;i>=0;i--){
			if (meetslash<2&&temp[i]=='/'){
				meetslash++;
				if (meetslash==2){
					i--;
				}
			}
			if (meetslash>=2){
				temmp[idd]=temp[i];
				idd++;
			}
		}
		for (int x=strlen(temmp)-1;x>=0;x--){
			if (temmp[x]=='/'&&temmp[x-1]=='/'){
				continue;
			}
			else {
				curpath[cid]=temmp[x];
				cid++;
			}
		}
		strcpy(gTerm.wdir,curpath);
	}
	else if (strcmp(argv[1],".")==0){
		strcpy(gTerm.wdir,gTerm.wdir);
	}
	else if (argv[1][0]=='-'){
		cerr<<"-bash: cd: "<<argv[1]<<": invalid option"<<endl;
		cerr<<"cd: usage: cd [-L|[-P [-e]] [-@]] [dir]"<<endl;
	}
	else {
		for (int i=strlen(temp)-1;i>=0;i--){
			if (temp[i]!='.'&&temp[i]!='/'){//字母，正常输出 
				temmp[idd]=temp[i];
				idd++;
			}
			else if (temp[i]=='/'&&temp[i-1]!='.'){//单/，正常输出
				temmp[idd]=temp[i];
				idd++;
			}
			else if (temp[i]=='.'&&temp[i-1]=='.'&&temp[i-2]!='.'){//一个dot 
				i--;
			}
			else if (temp[i]=='/'&&temp[i-1]=='.'&&temp[i-2]=='.'){//两个dots
				temmp[idd]=temp[i];
				idd++;
				i=i-4;
				while (temp[i-1]!='/'){
					i--;
				}
				
			}
		}//temmp生成完毕
		for (int x=strlen(temmp)-1;x>=0;x--){
			if (temmp[x]=='/'&&temmp[x-1]=='/'){
				continue;
			}
			else {
				curpath[cid]=temmp[x];
				cid++;
			}
		}
		strcpy(gTerm.wdir,curpath);
	}
}
