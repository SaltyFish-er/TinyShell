//�����ļ����������׼��� 
#include <iostream>//done
#include <cstring>//tested
#include <fstream>//wrong
#include "heads.h"
using namespace std;

bool func[4]={false,false,false,false};
int file_lo=1;
char tempp[100000]={'\0'};
int linenum=1;
bool blank=false;
bool valid=true;

void find_func(int argc,char* argv[]){
	for (int i=0;i<argc;i++){
		if (strcmp(argv[i],"-b")==0){
			func[0]=true;
			file_lo++;
		}
		else if (strcmp(argv[i],"-E")==0){
			func[1]=true;
			file_lo++;
		}
		else if (strcmp(argv[i],"-n")==0){
			func[2]=true;
			file_lo++;
		}
		else if (strcmp(argv[i],"-s")==0){
			func[3]=true;
			file_lo++;
		}
		else if (argv[i][0]=='-'&&strcmp(argv[1],"--help")!=0){
			valid=false;
		}
	}
}

void doCat(int argc,char* argv[]){
	find_func(argc,argv);
	if (valid){
	if (strcmp(argv[1],"--help")==0){
		ifstream fin("cat --help.txt");
		for (string str;getline(fin,str);){
			cout<<str<<endl;
		}
		fin.close();
	}
	else {
		for (int i=file_lo;i<argc;i++){
			if (strcmp(argv[i],"-")==0){
				strcat(tempp,gTerm.strin);
			}
			else {
				file_in(argv[i]);
				ifstream fin(rootin);
				if (!fin.is_open()){
					cerr<<"cat: "<<rootin<<" : No such file or directory"<<endl;
					fin.close();
				}
				else {
					ofstream fout("tempp.txt",ios::app);
					for (string str;getline(fin,str);){
						fout<<str<<endl;
					}//�ѽ��ļ�ȫ���洢��tempp.txt��
					fin.close();
					fout.close();
				}
			}
		}
		ifstream fin("tempp.txt");
		if (func[0]){//��-b���Էǿհ��б��
			if (func[3]){//��-s��ѹ���հ��� 
				if (func[1]){//��-E��-b -s -E 
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){//���ǿհ��У���� 
							outnum(linenum);
							linenum++;
							blank=false;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"$");
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0&&!blank){//��һ���հ��У������ 
							strcat(gTerm.strout,"        $");
							strcat(gTerm.strout,"\n");
							blank=true;
						}
						else if (strlen(str.c_str())==0&&blank){//ǰ��Ҳ�ǿհ��У������ 
							continue;
						}
					}
				}
				else {//��-E��-b -s 
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){
							outnum(linenum);
							linenum++;
							blank=false;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0&&!blank){
							strcat(gTerm.strout,"\n");
							blank=true;
						}
						else if (strlen(str.c_str())==0&&blank){
							continue;
						}
					}
				}
			}
			else {//��-s
				if (func[1]){//��-E��-b -E
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){
							outnum(linenum);
							linenum++;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"$");
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0){
							strcat(gTerm.strout,"        $");
							strcat(gTerm.strout,"\n");
						}
					}
				}
				else {//��-E��-b 
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){
							outnum(linenum);
							linenum++;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0){
							strcat(gTerm.strout,"\n");
						}
					}
				}
			}
		}
		else if (func[2]){//��-n������������б�� 
			if (func[3]){//��-s��ѹ���հ��� 
				if (func[1]){//��-E��-n -s -E 
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){//���ǿհ��У���� 
							outnum(linenum);
							linenum++; 
							blank=false;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"$");
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0&&!blank){//��һ���հ��У���� 
							outnum(linenum);
							linenum++;
							strcat(gTerm.strout,"$");
							strcat(gTerm.strout,"\n");
							blank=true;
						}
						else if (strlen(str.c_str())==0&&blank){//ǰ��Ҳ�ǿհ��У������ 
							continue;
						}
					}
				}
				else {//��-E��-n -s 
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){//���ǿհ��У���� 
							outnum(linenum);
							linenum++; 
							blank=false;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0&&!blank){//��һ���հ��У���� 
							outnum(linenum);
							linenum++;
							strcat(gTerm.strout,"\n");
							blank=true;
						}
						else if (strlen(str.c_str())==0&&blank){//ǰ��Ҳ�ǿհ��У������ 
							continue;
						}
					}
				}
			}
			else {//��-s 
				if (func[1]){//��-E��-n -E 
					for (string str;getline(fin,str);){
						outnum(linenum);
						linenum++;
						strcat(gTerm.strout,str.c_str());
						if (strlen(str.c_str())==0){
							strcat(gTerm.strout,"$");
						}
						else {
							strcat(gTerm.strout,"$");
						}
						strcat(gTerm.strout,"\n");
					}
				}
				else {//��-E��-n 
					for (string str;getline(fin,str);){
						outnum(linenum);
						linenum++;
						strcat(gTerm.strout,str.c_str());
						strcat(gTerm.strout,"\n");
					}
				}
			}
		} 
		else if (!func[0]&&!func[2]){//û��-b and -n
			if (func[3]){//��-s
				if (func[1]){//��-E��-s -E 
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){
							blank=false;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"$");
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0&&!blank){
							strcat(gTerm.strout,"$");
							strcat(gTerm.strout,"\n");
							blank=true;
						}
						else if (strlen(str.c_str())==0&&blank){
							continue;
						}
					}
				}
				else {//��-E��-s 
					for (string str;getline(fin,str);){
						if (strlen(str.c_str())!=0){
							blank=false;
							strcat(gTerm.strout,str.c_str());
							strcat(gTerm.strout,"\n");
						}
						else if (strlen(str.c_str())==0&&!blank){
							strcat(gTerm.strout,"\n");
							blank=true;
						}
						else if (strlen(str.c_str())==0&&blank){
							continue;
						}
					}
				}
			}
			else {//��-s 
				if (func[1]){//��-E��-E 
					for (string str;getline(fin,str);){
						strcat(gTerm.strout,str.c_str());
						strcat(gTerm.strout,"$\n");						
					}
				}
				else {//��-E��ɶ��û�� 
					for (string str;getline(fin,str);){
						strcat(gTerm.strout,str.c_str());
						strcat(gTerm.strout,"\n");
					}
				}
			}
		}
		fin.close();
	}
	remove("tempp.txt");
	}//valid������˽��������������� 
	else {
		cerr<<"cat: invalid option"<<endl;
		cerr<<"Try 'cat --help' for more information."<<endl;
	}
	for (int i=0;i<4;i++){
		func[i]=false;
	}
	file_lo=1;
	tempp[100000]={'\0'};
	linenum=1;
	blank=false;
	valid=true;
}
