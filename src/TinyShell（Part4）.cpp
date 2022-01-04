#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "tinyshell.h" 
using namespace std;

struct Terminal {
	char user[MAXLINE]; // 用户名
	char mach[MAXLINE]; // 计算机名
	char root [MAXLINE]; // 根目录
	char wdir [MAXLINE]; // 工作目录
	char strin[MAXFILE]; // 重定向标准输入
	char strout[MAXFILE]; // 重定向标准输出
};
Terminal gTerm;

char input [MAXLEN];//用户输入的命令 
char commands [MAXNUM][MAXLEN];//拆分后的结果 
char *poin[MAXLEN];//辅助参数数组 
int count;//拆分数量 
int i, j;
char before[MAXLINE];//按'|'拆分后前面的命令 
char after[MAXLINE];//按'|'拆分后后面的命令 

//初始化 
void Initialize(){
	memset (gTerm.strin, 0, sizeof(gTerm.strin));
	memset (gTerm.strout, 0, sizeof(gTerm.strin));
	memset (before, 0, sizeof(before));
	memset (after, 0, sizeof(after));
}

//接受字符储存在input中
int get(char input[]){
	//初始化
	memset(input, 0, MAXLINE);
	cin.getline(input, MAXLINE, '\n');
	return strlen(input); 
}

//解析字符串
void cut(char *input){
	//初始化
	 for (i = 0; i < MAXNUM; i++){
	 	poin[i] = NULL;
	 	memset (commands [i], 0, MAXLEN);
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


//显示命令提示符 
void display(){
	cout << "\e[92;1m" << gTerm.user << "@" << gTerm.mach;
	cout << "\e[94;1m"<< gTerm.wdir;
	cout << "\e[0m:";
	cout << "$ ";
}

//识别指令
void identify(char input[MAXFILE]){
	cut (input);
	 //单独处理quit命令
    if(strcmp(commands[0],"quit") == 0){
        cout << "Thanks for using TinyShell!" << endl;
        exit(0);  
    }
    //单独处理help命令 
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
    	doDiff(count + 1, poin);
   	}
   	else if (strcmp (commands[0] , "grep") == 0){
    	doGrep(count + 1, poin);
    }
    else if (strcmp (commands[0] , "tee") == 0){
    	doTee(count + 1, poin);
    }
    else if (strcmp (commands[0] , "cat") == 0){
    	doCat(count + 1, poin);
    }
    else if (strcmp (commands[0] , "cp") == 0){
    	doCp(count + 1, poin);
    }
    else if (strcmp (commands[0] , "cd") == 0){
    	doCd(count + 1, poin);
    }
    else if (strcmp (commands[0] , "pwd") == 0){
    	doPwd(count + 1, poin);
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

//处理复合命令 
void pipe(char input[MAXFILE]){
	for (i = 0; input[i] != '\0'; i++){
		if (input[i] == '|') break;
	}
	//分离指令
	memset (before, 0, MAXFILE);
	memset (after, 0, MAXFILE);
	for (j = 0; j < i - 1; j++){
		before [j] = input [j];
	} 
	for (j = 0; j < strlen(input) - i - 1; j++){
		after [j] = input [j + i + 2];
	} 
}

//记录复合次数 
int countpipe(char input[MAXFILE]){
	int temp = 0;
	for (i = 0; input[i] != '\0'; i++){
		if (input[i] == '|') temp++;
	}
	return temp;
}

//Echo命令 
void doEcho(int argc, char * argv[]){
	//-n 不换行输出 
	if (strcmp(argv[1] , "-n") == 0){
	int temp = 0;
		for (int k = 2; k <= argc; k++){
			temp += sprintf(gTerm.strout + temp, "%s ", argv[k]);
		}
	}
	//彩色输出 
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
	//--help指令 
	else if (strcmp(argv[1] , "--help") == 0) {
		sprintf(gTerm.strout, "Echo the STRING(s) to standard output.\n"
		"-n     do not output the trailing newline\n"
		"-r     Echo in color red\n"
		"-g     Echo in color green\n"
		"-b     Echo in color blue\n");
	}
	//默认换行输出 
	else {
		int temp = 0;
		for (int k = 1; k <= argc; k++){
			temp += sprintf(gTerm.strout + temp, "%s ", argv[k]);
		}
		sprintf(gTerm.strout + temp, "\n");
	}
}
 
int main(int argc, char * argv[]){
	memset(gTerm.wdir, '/', 1);
	cout << "Machine Name:";
	cin.getline(gTerm.mach, MAXLINE, '\n');
	cout << "Root Directory:";
	cin.getline(gTerm.root, MAXLINE, '\n');
	cout << "Login:";
	cin.getline(gTerm.user, MAXLINE, '\n');
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

void doLen(int argc, char * argv[]){
	int temp = strlen (gTerm.strin);
	sprintf(gTerm.strout, "%d\n", temp);
}

