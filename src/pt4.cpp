#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "heads.h" 
using namespace std;

char input [128];//�û���������� 
char commands [64][128];//��ֺ�Ľ�� 
char *poin[128];//������������ 
int count;//������� 
int i, j;
char before[64];//��'|'��ֺ�ǰ������� 
char after[256];//��'|'��ֺ��������� 

//��ʼ�� 
void Initialize(){
	memset (gTerm.strin, 0, sizeof(gTerm.strin));
	memset (gTerm.strout, 0, sizeof(gTerm.strin));
	memset (before, 0, sizeof(before));
	memset (after, 0, sizeof(after));
}

//�����ַ�������input��
int get(char input[]){
	//��ʼ��
	memset(input, 0, 64);
	cin.getline(input, 64, '\n');
	return strlen(input); 
}

//�����ַ���
void cut(char *input){
	//��ʼ��
	 for (i = 0; i < 64; i++){
	 	poin[i] = NULL;
	 	memset (commands [i], 0, 128);
	 }
	 count = 0;
	 int len = strlen(input);
	 int temp = 0;
	 for (i = 0, j = 0; i < len; i++){
	 	if (input[i] != ' '){
	 		commands[count][j] = input [i];
	 		j = j + 1;
	 	}
	 	else if (j != 0){
	 		commands[count][j] = '\0';
	 		count = count + 1;
	 		j = 0;
	 	}
	 } 
	if (j != 0) commands [count][j] = '\0';
	for ( i = 0; i < len; i++){
		poin[i] = &commands [i][0];
	}
}


//��ʾ������ʾ�� 
void display(){
	cout << "\e[92;1m" << gTerm.user << "@" << gTerm.mach;
	cout << "\e[94;1m"<< gTerm.wdir;
	cout << "\e[0m:";
	cout << "$ ";
}

//���������� 
void pipe(char input[256]){
	for (i = 0; input[i] != '\0'; i++){
		if (input[i] == '|') break;
	}
	//����ָ��
	memset (before, 0, 256);
	memset (after, 0, 256);
	for (j = 0; j < i - 1; j++){
		before [j] = input [j];
	} 
	for (j = 0; j < strlen(input) - i - 1; j++){
		after [j] = input [j + i + 2];
	} 
}

//��¼���ϴ��� 
int countpipe(char input[256]){
	int temp = 0;
	for (i = 0; input[i] != '\0'; i++){
		if (input[i] == '|') temp++;
	}
	return temp;
}

//Echo���� 
void doEcho(int argc, char * argv[]){
	//-n ��������� 
	if (strcmp(argv[1] , "-n") == 0){
	int temp = 0;
		for (int k = 2; k <= argc; k++){
			temp += sprintf(gTerm.strout + temp, "%s ", argv[k]);
		}
	}
	//��ɫ��� 
	else if (strcmp(argv[1], "-r") == 0){
		int temp = sprintf(gTerm.strout, "\e[91;1m");
		for (int k = 2; k <= argc; k++){
			temp += sprintf(gTerm.strout + temp, "%s ", argv[k]);
		}
		sprintf(gTerm.strout + temp, "\n");
	}
	else if (strcmp(argv[1], "-g") == 0){
		int temp = sprintf(gTerm.strout, "\e[92;1m");
		for (int k = 2; k <= argc; k++){
			temp += sprintf(gTerm.strout + temp, "%s ", argv[k]);
		}
		sprintf(gTerm.strout + temp, "\n");
	}
	else if (strcmp(argv[1], "-b") == 0){
		int temp = sprintf(gTerm.strout, "\e[94;1m");
		for (int k = 2; k <= argc; k++){
			temp += sprintf(gTerm.strout + temp, "%s ", argv[k]);
		}
		sprintf(gTerm.strout + temp, "\n");
	}
	//--helpָ�� 
	else if (strcmp(argv[1] , "--help") == 0) {
		sprintf(gTerm.strout, "Echo the STRING(s) to standard output.\n"
		"-n     do not output the trailing newline\n"
		"-r     Echo in color red\n"
		"-g     Echo in color green\n"
		"-b     Echo in color blue\n");
	}
	//Ĭ�ϻ������ 
	else {
		int temp = 0;
		for (int k = 1; k <= argc; k++){
			temp += sprintf(gTerm.strout + temp, "%s ", argv[k]);
		}
		sprintf(gTerm.strout + temp, "\n");
	}
}

void doLen(int argc, char * argv[]){
	int temp = strlen (gTerm.strin);
	sprintf(gTerm.strout, "%d\n", temp);
}

//ʶ��ָ��
void identify(char input[256]){
	cut (input);
	 //��������quit����
    if(strcmp(commands[0],"quit") == 0){
        cout << "Thanks for using TinyShell!" << endl;
        exit(0);  
    }
    //��������help���� 
	else if(strcmp(commands[0], "help") == 0){
		cout << "For detail information about a specific command,"
		" please enter 'command' --help\n";
		cout <<"diff 	compare two textfiles by each line\n";
		cout <<"grep 	search a specific string in a textfile\n";
		cout <<"tee  	read data in strin, output them to strout, while saving in multiple textfiles\n";
		cout <<"cat  	connect textfiles and output to strout\n";
		cout <<"cp   	copy textfiles\n";
		cout <<"cd  	switch working directory\n";
		cout <<"pwd  	print working directory\n";
		cout <<"echo 	output string to strout\n";
		cout <<"|   	connect multiple commands\n";
		cout <<"quit 	quit TinyShell\n";
		cout <<"len  	output the length of strin (including \\n)\n";	 
	}
	else if (strcmp (commands[0] , "diff") == 0){
    	doDiff(count+1, poin);
   	}
   	else if (strcmp (commands[0] , "grep") == 0){
    	doGrep(count+1, poin);
    }
    else if (strcmp (commands[0] , "tee") == 0){
    	doTee(count+1, poin);
    }
    else if (strcmp (commands[0] , "cat") == 0){
    	doCat(count+1, poin);
    }
    else if (strcmp (commands[0] , "cp") == 0){
    	doCp(count+1, poin);
    }
    else if (strcmp (commands[0] , "cd") == 0){
    	doCd(count+1, poin);
    }
    else if (strcmp (commands[0] , "pwd") == 0){
    	doPwd(count+1, poin);
    }
    else if (strcmp (commands[0] , "echo") == 0){
    	doEcho(count, poin);
    }
    else if (strcmp (commands[0] , "len") == 0){
    	doLen(count, poin);
    }
    else if (input[0] != 0){
    	cerr << "Command '" << commands[0] << "' not found\nYou can enter help for information about supported commands\n";
    }
}

int main(int argc, char * argv[]){
	memset(gTerm.wdir, '/', 1);
	cout << "Machine Name:";
	cin.getline(gTerm.mach, 64, '\n');
	cout << "Root Directory:";
	cin.getline(gTerm.root, 64, '\n');
	cout << "Login:";
	cin.getline(gTerm.user, 64, '\n');
	while (1){
    	Initialize();
    	display();
    	get (input);
    	int times = countpipe(input);
    	if (times){
    		for (int t = 1; t <= times; t++){
    			pipe (input);
				identify (before);
				strcpy (gTerm.strin, gTerm.strout);
				memset (gTerm.strout, 0, sizeof(gTerm.strout));
				strcpy (input, after);
    		}
    		identify (input);
    	}
    	else {
    		identify (input);
    	}
    	cout << gTerm.strout;
    }
    return 0;	
} 
