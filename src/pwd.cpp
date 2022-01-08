//显示工作目录 
#include <iostream>//done
#include <cstring>//tested
#include <fstream>//wrong
#include "heads.h"
using namespace std;

void doPwd(int argc,char* argv[]){
	if (argc>1){
		if (strcmp(argv[1],"--help")==0){
			ifstream fin("pwd --help.txt");
			for (string str;getline(fin,str);){
				cout<<str<<endl;
			}
			fin.close();
		}
		else {
			cerr<<"-bash: pwd: "<<argv[1]<<": invalid option"<<endl;
			cerr<<"pwd: usage: pwd [-LP]"<<endl;
		}
	}
	else if (argc==1){
		cout<<gTerm.wdir<<endl;
	}
}
