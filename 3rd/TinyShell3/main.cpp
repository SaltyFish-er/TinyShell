#include <iostream>
#include <cstring>
#include "heads.h"
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

Terminal gTerm;

char rootin[1000];
char rootout[1000];
char a[7];
int id=0;

void file_in(char* input){
	strcpy(rootin,gTerm.root);
	if (input[0]=='/'){
		strcat(rootin,input);
	}
	else {
		strcat(rootin,gTerm.wdir);
		strcat(rootin,input);	
	}
}

void file_out(char* input){
	strcpy(rootout,gTerm.root);
	if (input[0]=='/'){
		strcat(rootout,input);
	}
	else {
		strcat(rootout,gTerm.wdir);
		strcat(rootout,input);	
	}
}

void outnum(int i){//输出行数数字前后的空格
	while (i>9){
		a[id]=i%10;
		i=i/10;
		id++;
	}
	a[id]=i;
	for (int x=1;x<6-id;x++){
		strcat(gTerm.strout," ");
	}
	for (int y=id;y>=0;y--){
		char num[2];
		num[0]=a[y]+'0';
		num[1]='\0';
		strcat(gTerm.strout,num);
	}
	strcat(gTerm.strout,"  ");
	id=0;
}

int main(int argc, char** argv) {
	return 0;
}
