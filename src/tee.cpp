//从标准输入读取数据，将其内容输出到标准输出，同时保存至多个文件 
#include <iostream>//done
#include <cstring>//tested
#include <fstream>//wrong
#include "heads.h"
using namespace std;

void doTee(int argc,char* argv[]){
	if (strcmp(argv[1],"--help")==0){
		ifstream fin("tee --help.txt");
		for (string str;getline(fin,str);){
			cout<<str<<endl;
		}
		fin.close();
	}
	else if (strcmp(argv[1],"-a")==0){
		strcpy(gTerm.strout,gTerm.strin);
		for (int i=2;i<argc;i++){
			file_in(argv[i]);
			ofstream fout(rootin,ios::app);
		//	ofstream fout(argv[i],ios::app);测试用 
			fout<<gTerm.strin;//输入到文件，且没有覆盖 
			fout.close();
		}
	}
	else if (argv[1][0]=='-'&&argv[1][1]!='a'){
		cerr<<"tee: invalid option -- '"<<argv[1]<<"'"<<endl;
		cerr<<"Try 'tee --help' for more information."<<endl;
	}
	else {
		strcpy(gTerm.strout,gTerm.strin); 
		for (int i=1;i<argc;i++){
			file_in(argv[i]);
			ofstream fout(rootin);
			fout<<gTerm.strin;//输入到文件
			fout.close();
		}
	}
}
