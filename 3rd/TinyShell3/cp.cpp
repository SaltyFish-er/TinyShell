//�����ļ� 
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
	else if (strcmp(argv[1],"-n")==0){//�������Ѵ��ڵ��ļ�
		if (strcmp(argv[2],"-")!=0){//��ȡ�ļ�
			file_in(argv[2]);
			ifstream fin(rootin,ios::binary);
			if (!fin.is_open()){
				cerr<<"cp: "<<rootin<<" : No such file or directory"<<endl;
				fin.close();
			}
			else {			
			if (strcmp(argv[3],"-")!=0){//д���ļ�
				file_out(argv[3]);
				ofstream fout(rootout,ios::binary|ios::app);//ios::binary,ios::app����ʾ�� 
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
			else {//д��strout
				for (string str;getline(fin,str);){
					strcat(gTerm.strout,str.c_str());
				}
			}
			fin.close();
			}
		}
		if (strcmp(argv[2],"-")==0){//��ȡstrin
			if (strcmp(argv[3],"-")!=0){//д���ļ�
				file_out(argv[3]);
				ofstream fout(rootout,ios::binary|ios::app);
				fout.write((char*)&gTerm.strin,sizeof(gTerm.strin));
				fout.close();
			}
			else {//д��strout
				strcpy(gTerm.strout,gTerm.strin);
			}
		}
	}
	else if (argv[1][0]=='-'&&argv[1][1]!='n'){
		cerr<<"cp: missing file operand"<<endl;
		cerr<<"Try 'cp --help' for more information."<<endl;
	}
	else {//��-n��1Ϊ�����ļ���2Ϊ����ļ� 
		if (strcmp(argv[1],"-")!=0){//��ȡ�ļ�
			file_in(argv[1]);
			ifstream fin(rootin);
			if (!fin.is_open()){
				cerr<<"cp: "<<rootin<<" : No such file or directory"<<endl;
				fin.close();
			}
			else {
			if (strcmp(argv[2],"-")!=0){//д���ļ�
				file_out(argv[2]);
				ofstream fout(rootout,ios::binary);//ios::binary,ios::app����ʾ�� 
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
			else {//д��strout
				for (string str;getline(fin,str);){
					strcat(gTerm.strout,str.c_str());
				}
			}
			fin.close();
			}
		}
		if (strcmp(argv[1],"-")==0){//��ȡstrin
			if (strcmp(argv[2],"-")!=0){//д���ļ�
				file_out(argv[2]);
				ofstream fout(rootout,ios::binary);
				fout.write((char*)&gTerm.strin,sizeof(gTerm.strin));
				fout.close();
			}
			else {//д��strout
				strcpy(gTerm.strout,gTerm.strin);
			}
		}
	}
}
