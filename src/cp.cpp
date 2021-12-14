//复制文件 
#include <iostream>//done
#include <cstring>//tested
#include <fstream>//wrong
#include "heads.h"
using namespace std;

void doCp(int argc,char* argv[]){
	if (strcmp(argv[1],"--help")==0){
		ifstream fin("cp --help.txt");
		for (string str;getline(fin,str);){
			cout<<str<<endl;
		}
		fin.close();
	}
	else if (strcmp(argv[1],"-n")==0){//不覆盖已存在的文件
		if (strcmp(argv[2],"-")!=0){//读取文件
			file_in(argv[2]);
			ifstream fin(rootin,ios::binary);
			if (!fin.is_open()){
				cerr<<"cp: "<<rootin<<" : No such file or directory"<<endl;
				fin.close();
			}
			else {			
			if (strcmp(argv[3],"-")!=0){//写入文件
				file_out(argv[3]);
				ofstream fout(rootout,ios::binary|ios::app);//ios::binary,ios::app都表示数 
				while (!fin.eof()){
					char s;
					fin.read(&s,sizeof(char));
					if (fin.eof()){
						break;
					}
					fout.write(&s,sizeof(char));
				}
				fout.close();
			}
			else {//写入strout
				for (string str;getline(fin,str);){
					strcat(gTerm.strout,str.c_str());
				}
			}
			fin.close();
			}
		}
		if (strcmp(argv[2],"-")==0){//读取strin
			if (strcmp(argv[3],"-")!=0){//写入文件
				file_out(argv[3]);
				ofstream fout(rootout,ios::binary|ios::app);
				fout.write((char*)&gTerm.strin,sizeof(gTerm.strin));
				fout.close();
			}
			else {//写入strout
				strcpy(gTerm.strout,gTerm.strin);
			}
		}
	}
	else if (argv[1][0]=='-'&&argv[1][1]!='n'){
		cerr<<"cp: missing file operand"<<endl;
		cerr<<"Try 'cp --help' for more information."<<endl;
	}
	else {//无-n，1为输入文件，2为输出文件 
		if (strcmp(argv[1],"-")!=0){//读取文件
			file_in(argv[1]);
			ifstream fin(rootin);
			if (!fin.is_open()){
				cerr<<"cp: "<<rootin<<" : No such file or directory"<<endl;
				fin.close();
			}
			else {
			if (strcmp(argv[2],"-")!=0){//写入文件
				file_out(argv[2]);
				ofstream fout(rootout,ios::binary);//ios::binary,ios::app都表示数 
				while (!fin.eof()){
					char s;
					fin.read(&s,sizeof(char));
					if (fin.eof()){
					break;
					}
					fout.write(&s,sizeof(char));
				}
				fout.close();
			}
			else {//写入strout
				for (string str;getline(fin,str);){
					strcat(gTerm.strout,str.c_str());
				}
			}
			fin.close();
			}
		}
		if (strcmp(argv[1],"-")==0){//读取strin
			if (strcmp(argv[2],"-")!=0){//写入文件
				file_out(argv[2]);
				ofstream fout(rootout,ios::binary);
				fout.write((char*)&gTerm.strin,sizeof(gTerm.strin));
				fout.close();
			}
			else {//写入strout
				strcpy(gTerm.strout,gTerm.strin);
			}
		}
	}
}
