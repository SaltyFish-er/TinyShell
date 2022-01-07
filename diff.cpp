#define _CRT_SECURE_NO_WARNINGS

#include <iostream> //cin cout
#include <fstream>  //open close
#include <string>   //string
#include <stdio.h>  //freopen
#include <sstream>  //stringstream
#include <cstring>
#include "heads.h"
using namespace std;

const int MAXROW = 2047;
const int SEARCHDEPTH = 5;

//按行读入的文件
struct fileInLine {
	string file[MAXROW];
	int lineCount=0;
};
 fileInLine file_input[2];

//两个文件的匹配情况
 int match[MAXROW];

//输出帮助文档
void diff_help() {
	cout << "Usage: diff {-b}{-B}{-I[字符串]}{-q}{-w}[文件1][文件2]"<<endl;
	cout << "逐行比较两个文件" << endl;
	cout << endl;
	cout << "  --help：显示帮助。\n\
  -b：不检查空格字符的不同。\n\
  -B：不检查空白行。\n\
  -i：不检查大小写的不同。\n\
  -q：仅显示有无差异，不显示详细的信息。无差异时不显示，有差异时显示“File[文件 1] and [文件 2] differ”\n\
  -w：忽略全部的空格字符。\n\
  -I[字符串]：若两个文件在某几行有所不同，但这几行同时都包含了选项中指定字符串，则不显示这两个文件的差异。"<< endl;
}

//获取文件绝对路径
//如果输入为"-"直接返回
//函数中new给absolutePath的空间将在read函数中delete
char* getPath(const char* path) {
	char* absolutePath = new char[1005];
	if (path[0] == '\\' || path[0] == '/') {
		char root[1005];
		strcpy(root, gTerm.root);
		strcat(root, path);
		strcpy(absolutePath, root);
	}
	else if (!strcmp(path, "-")) {
		strcpy(absolutePath, path);
	}
	else {
		char wdir[1005];
		strcpy(wdir, gTerm.root);
		strcat(wdir, gTerm.wdir);
		strcat(wdir, path);
		strcpy(absolutePath, wdir);
	}
	return absolutePath;
}

//将src从索引l到r的部分拷贝至dest [l,r)
void str_copy(char* dest, const char* src, int l, int r) {
	int length = r - l;
	if (strlen(dest) < length) { //for debug
		cerr << "Error: str_copy dest is too short" << endl;
		return;
	}
	for (int i = l,j = 0;i < r;i++,j++) {
		dest[j] = src[i];
	}
	dest[length] = '\0';
	return;
}

//将输入文件的信息按行读到file_input[2]
//如果成功则返回1，出错则返回0
bool read(int argc,char* argv[]) { //debug 结束后修改argv为char*类型
	char* in_file[2];
	in_file[0] = getPath(argv[argc - 2]);
	in_file[1] = getPath(argv[argc - 1]);
	//cerr<<"in_file[0] is "<<in_file[0]<<endl;
	//cerr<<"in_file[1] is "<<in_file[1]<<endl;
	//将读入文件的行数初始化
	file_input[0].lineCount = 0;
	file_input[1].lineCount = 0;
	ifstream file;
	for (int i = 0;i < 2;i++) {
		if (!strcmp(in_file[i],"-")) {
			char temp[1005];
			for (int x = 0, j = 0, k = 0;;) {
				if (gTerm.strin[x] == '\n') {
					temp[k] = '\0';
					file_input[i].file[j] = temp;
					file_input[i].lineCount++;
					k = 0;
					x++, j++;
				}
				else if (gTerm.strin[x] == '\0') {
					temp[k] = '\0';
					file_input[i].file[j] = temp;
					file_input[i].lineCount++;
					break;
				}
				else {
					temp[k] = gTerm.strin[x];
					x++, k++;
				}
			}
		}
		else {
			file.open(in_file[i]);
			if (file.is_open()) {
				int j = 0;
				while (getline(file, file_input[i].file[j])) {
					//cerr << file_input[i].file[j] << endl; //test for debug
					file_input[i].lineCount++;
					j++;
				}
			}
			else {
				//cerr << "wdir is:"<<gTerm.wdir<<endl;
				cerr << "diff: " << argv[argc - 2 + i] << " No such file or directory" <<endl;
				delete in_file[0];
				delete in_file[1];
				return false;
			}
			file.close();
			file.clear();
		}
	}
	delete in_file[0];
	delete in_file[1];
	return true;
}

//以区分/不区分大小写的模式判断两个字符是否一致
//判断空格是否一致
bool cmpChar(char a, char b,bool params[6]) {
	if (a == b) {
		return true;
	}
	if (params[2]) {
		if (a <= 90 && a >= 65 && (a + 32 == b)) {
			return true;
		}
		if (a >= 97 && a <= 121 && (a - 32 == b)) {
			return true;
		}
	}
	if (params[0]) {
		if (a == 9 && b == 32) {
			return true;
		}
		if (a == 32 && b == 9) {
			return true;
		}
	}
	return false;
}

//判断key是否为dest的子串
bool isSubstring(string dest, string key, bool params[6]) {
	int length1 = dest.length();
	int length2 = key.length();
	if (length2 > length1) {
		return false;
	}
	for (int i = 0;i < length1;i++) {
		for (int j = 0;j < length2;j++) {
			if (!cmpChar(dest[i+j],key[j],params)) {
				break;
			}
			else {
				if (j == length2 - 1) {
					return true;
				}
			}
		}
	}
	return false;
}

//获取字符中最后一个非空格字符的索引
int getLastChar(string str) {
	int length = str.length();
	for (int i = length - 1;i >= 0;i--) {
		if (str[i] != ' '&&str[i]!=9) {
			return i;
		}
	}
	return 0;
}

//比较两个字符串是否不同
//这里需要处理的命令选项有 -w -b -i -I
bool cmpLine(bool params[6], char* key, string line1, string line2) {
	int length1 = line1.length();
	int length2 = line2.length();
	//处理-w -i -b
	//-b的功能真包含于-w：如果有-w时，可认为有无-b无所谓
	//分三种情况进行讨论处理：(1) {-w -i -b} || {-w -i} || {-w -b} || {-w}
	//(2){-b -i} || {-b}  (3)//{-i} || { }
	bool flag = true;
	if (params[4]) { // {-w -i -b} || {-w -i} || {-w -b} || {-w}
		int i = 0,j = 0;
		for (;i < length1 && j < length2;) {
			if (line1[i] == ' '||line1[i]==9) {
				i++;
			}
			else if (line2[j] == ' '||line2[i]==9) {
				j++;
			}
			else {
				if (!cmpChar(line1[i], line2[j], params)) {
					flag = false;
					break;
				}
				i++, j++;
			}
		}
		if (i < length1) {
			for (;i < length1;i++) {
				if (line1[i] != ' '&&line1[i]!=9) {
					flag = false;
					break;
				}
			}
		}
		if (j < length2) {
			for (;j < length2;j++) {
				if (line2[j] != ' '&&line2[j]!=9) {
					flag = false;
					break;
				}
			}
		}
	}

	else if (params[0]&&!params[4]) { // {-b -i} || {-b}
		int lastChar1 = getLastChar(line1);
		int lastChar2 = getLastChar(line2);
		int i = 0, j = 0;
		for (;i < length1 && j < length2;) {
			if (!cmpChar(line1[i], line2[j], params)) {
				flag = false;
				break;
			}
			else {
				if (line1[i] == ' '||line1[i]==9) {
					while (line1[i] == ' '||line1[i]==9) {
						i++;
					}
					while (line2[j] == ' '||line2[j]==9) {
						j++;
					}
				}
				else {
					i++, j++;
				}
			}
		}
		if (i <= lastChar1 || j <= lastChar2) {
			flag = false;
		}
	}

	else{ //{-i} || { }
		if (length1 != length2) {
			flag = false;
		}
		else{
			for (int i = 0, j = 0;i < length1 && j < length2;i++, j++) {
				if (!cmpChar(line1[i], line2[j], params)) {
					flag = false;
					break;
				}
			}
		}
	}

	//处理-I选项
	//先判断是否匹配，如果匹配返回true
	//否则判断不匹配的行是否满足字符串匹配
	if (flag) {
		return true;
	}
	else {
		if (params[5]) {
			if (isSubstring(line1, key, params) && isSubstring(line2, key, params)) {
				return true;
			}
		}
	}
	return false;
}

//判断一个行是不是只有空格(space/tab)
bool isLineBlank(string line) {
	for (int i = 0;i < line.length();i++) {
		if (line[i] != 9 && line[i] != 32) {
			return false;
		}
	}
	return true;
}
//根据match匹配对将结果输出到屏幕上
//这里需要处理-B命令
//注意-B命令和-w -b复合时，只有空格的行也认为是空白行
void displayMatch(bool params[]) {
	int now_i = 0, now_j = 0;
	int last_i = -1, last_j = -1;
	for (int i = 0;i < file_input[0].lineCount&&match[i]<file_input[1].lineCount;i++) {
		if (match[i] >= 0) {
			now_i = i;
			now_j = match[i];
			if (now_i == last_i + 1 && now_j == last_j + 1) {

			}
			else if (now_i == last_i + 1) {
				bool out = 0;
				if (params[1]) {
					for (int n = last_j + 1;n < now_j;n++) {
						if (file_input[1].file[n] != "") {
							if (!((params[0] || params[4]) && isLineBlank(file_input[1].file[n]))) {
								//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
								out = 1;
							}
						}
					}
				}
				else {
					out = 1;
				}
				if (out) {
					cout << last_i + 1 << "a" << last_j + 1 + 1 << endl;
					for (int n = last_j + 1;n < now_j;n++) {
						cout << ">" << file_input[1].file[n] << endl;
					}
				}
			}
			else if (now_j == last_j + 1) {
				bool out = 0;
				if (params[1]) {
					for (int m = last_i + 1;m < now_i;m++) {
						if (file_input[0].file[m] != "") {
							if (!((params[0] || params[4]) && isLineBlank(file_input[0].file[m]))) {
								//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
								out = 1;
							}
						}
					}
				}
				else {
					out = 1;
				}
				if (out) {
					cout << last_i + 1 + 1 << "d" << last_j + 1 << endl;
					for (int m = last_i + 1;m < now_i;m++) {
						cout << "<" << file_input[0].file[m] << endl;
					}
				}
			}
			else {
				bool out = 0;
				if (params[1]) {
					for (int m = last_i + 1;m < now_i;m++) {
						if (file_input[0].file[m] != "") {
							if (!((params[0] || params[4]) && isLineBlank(file_input[0].file[m]))) {
								//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
								out = 1;
							}
						}
					}
					for (int n = last_j + 1;n < now_j;n++) {
						if (file_input[1].file[n] != "") {
							if (!((params[0] || params[4]) && isLineBlank(file_input[1].file[n]))) {
								//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
								out = 1;
							}
						}
					}
				}
				else {
					out = 1;
				}
				if (out) {
					cout << last_i + 1 + 1 << "c" << last_j + 1 + 1 << endl;
					for (int m = last_i + 1;m < now_i;m++) {
						cout << "<" << file_input[0].file[m] << endl;
					}
					cout << "---" << endl;
					for (int n = last_j + 1;n < now_j;n++) {
						cout << ">" << file_input[1].file[n] << endl;
					}
				}
				
			}
			last_i = now_i;
			last_j = now_j;
		}
	}

	//对最后部分进行输出
	//获取最后一组匹配对
	int index1 = file_input[0].lineCount - 1;
	int index2 = file_input[1].lineCount - 1;
	while (index1 > 0 && match[index1] < 0) {
		index1--;
	}
	if (index1 >= 0) {
		index2 = match[index1];
	}
	index1++;
	index2++;
	if (index1 == file_input[0].lineCount && index2 == file_input[1].lineCount) {

	}
	else if (index1 == file_input[0].lineCount) {
		bool out = 0;
		if (params[1]) {
			for (int a=index2;a < file_input[1].lineCount;a++) {
				if (file_input[1].file[a] != "") {
					if (!((params[0] || params[4]) && isLineBlank(file_input[1].file[a]))) {
						//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
						out = 1;
					}
				}
			}
		}
		else {
			out = 1;
		}
		if (out) {
			cout << index1 << "a" << index2 + 1 << endl;
			for (;index2 < file_input[1].lineCount;index2++) {
				cout << ">" << file_input[1].file[index2] << endl;
			}
		}
	}
	else if (index2 == file_input[1].lineCount) {
		bool out = 0;
		if (params[1]) {
			for (int b=index1;b < file_input[0].lineCount;b++) {
				if (file_input[0].file[b] != "") {
					if (!((params[0] || params[4]) && isLineBlank(file_input[0].file[b]))) {
						//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
						out = 1;
					}
				}
			}
		}
		else {
			out = 1;
		}
		if (out) {
			cout << index1 + 1 << "d" << index2 << endl;
			for (;index1 < file_input[0].lineCount;index1++) {
				cout << "<" << file_input[0].file[index1] << endl;
			}
		}
		
	}
	else {
		bool out = 0;
		if (params[1]) {
			int a = index1;
			int b = index2;
			for (;a < file_input[0].lineCount;a++) {
				if (file_input[0].file[a]!="") {
					if (!((params[0] || params[4]) && isLineBlank(file_input[0].file[a]))) {
						//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
						out = 1;
					}
				}
			}
			for (;b < file_input[1].lineCount;b++) {
				if (file_input[1].file[b] != "") {
					if (!((params[0] || params[4]) && isLineBlank(file_input[1].file[b]))) {
						//判断此时不处于{-B命令与-w/-b复合}且{该行只有空格}的状态
						out = 1;
					}
				}
			}
		}
		else {
			out = 1;
		}
		if (out) {
			cout << index1 + 1 << "c" << index2 + 1 << endl;
			for (;index1 < file_input[0].lineCount;index1++) {
				cout << "<" << file_input[0].file[index1] << endl;
			}
			cout << "---" << endl;
			for (;index2 < file_input[1].lineCount;index2++) {
				cout << ">" << file_input[1].file[index2] << endl;
			}
		}
	}
}

//比较file_input两个文件的不同，将匹配结果保存
//这里需要对-q命令选项进行预处理
//然后调用cmpLine处理-w -b -i -I
//两个文件不同则返回0，相同则返回1
bool cmp(bool params[6],char* key) {
	bool same = 1;
	int last_i = -1, last_j = -1;
	int temp_i = 10000, temp_j = 10000;
	bool matched = 0;//记录是否存在一个尚未更新的匹配对
	for (int i = 0, j = 0;i < file_input[0].lineCount && j < file_input[1].lineCount;) {
		if (cmpLine(params, key, file_input[0].file[i], file_input[1].file[j])) {
			if (i + j < temp_i + temp_j) {
				//此时匹配的可以优化
				temp_i = i;
				temp_j = j;
				matched = 1;
				if (i + 1 >= file_input[0].lineCount) {
					//如果达到上界，那么更新匹配对
					match[temp_i] = temp_j;
					matched = 0;
					last_i = temp_i;
					last_j = temp_j;
					temp_i = 10000; //设置为最大
					temp_j = 10000;
					i = last_i + 1;
					j = last_j + 1;
				}
				else {
					//否则继续向下试探
					i = i + 1;
					j = last_j + 1;
				}
			}
			else {
				//此时匹配的已经不能优化了
				if (i + 1 < file_input[0].lineCount) {
					//如果i还可以加，那么继续向下试探
					i++;
				}
				else {
					//如果i已经不能加了，更新匹配
					match[temp_i] = temp_j;
					matched = 0;
					last_i = temp_i;
					last_j = temp_j;
					temp_i = 10000; //设置为最大
					temp_j = 10000;
					i = last_i + 1;
					j = last_j + 1;
				}
				
			}
		}
		else {
			if (j - last_j < SEARCHDEPTH && j + 1 < file_input[1].lineCount) {
				if (!matched) {
					if (params[3]) {
						return false;
					}
					same = 0;
				}
				j++;
			}
			else {
				//j已经查了很多了，还是没找到（更好的）匹配
				//应该更改i了
				if (i + 1 < file_input[0].lineCount) {
					//i还可以再加
					if (i < temp_i + temp_j - last_j) {
						//这个时候也许还能找到更好的匹配(可能是match了，也可能没有)
						//i++，还原j继续查找
						//如果没match，将same置0
						if (!matched) {
							same = 0;
						}
						i++;
						j = last_j + 1;
					}
					else {
						//这个时候一定match了，而且i已经过大，不可能再找到更好的匹配了
						//所以直接更新match，并将各个参量还原
						match[temp_i] = temp_j;
						matched = 0;
						last_i = temp_i;
						last_j = temp_j;
						temp_i = 10000; //设置为最大
						temp_j = 10000;
						i = last_i + 1;
						j = last_j + 1;
					}
				}
				else {
					//i已经到Max了，再加出界了
					if (!matched) {
						//如果没match，说明遍历已经结束
						//直接i++出界结束本次匹配
						i++;
					}
					else {
						//如果match了，说明已经不可能有更好的匹配了
						//更新match，并将各个参量还原
						match[temp_i] = temp_j;
						matched = 0;
						last_i = temp_i;
						last_j = temp_j;
						temp_i = 10000; //设置为最大
						temp_j = 10000;
						i = last_i + 1;
						j = last_j + 1;
					}
				}
			}
		}
	}
	return same;
}

void doDiff(int argc, char* argv[]) { //debug 结束后修改argv为char*类型
	//处理--help命令
	//cout<<argc<<endl;
	if (!strcmp(argv[1], "--help")) {
		if (argc == 2) {
			diff_help();
			return;
		}
		else {
			cerr << "diff: --help is not compatible with other parameters " << endl;
			return;
		}
	}
	
	//获取命令选项并记录
	char strLookup[1005];
	const char* paramString[] = { "-b","-B","-i","-q","-w","-I" };
	bool params[6];
	memset(params, 0, sizeof(params));
	bool flag = 0; //记录每个命令选项是否合法
	for (int i = 1;i < argc - 2;i++) {
		//检索argc[i]是否为非-I的其他选项说明
		flag = 0;
		for (int j = 0;j < 5;j++) {
			if (!strcmp(argv[i], paramString[j])) {
				//cerr << "argv["<<i << "]: " << argv[i] << endl;//test for debug
				if (params[j]) {
					cerr << "diff: Enter {" << paramString[j] << "} parameter too many times";
					return;
				}
				else {
					flag = 1;
					params[j] = true;
				}
			}
		}
		//检索argc[i]是否为-I
		if (!memcmp(argv[i], paramString[5], 2)) {
			//cerr << "argv["<<i << "]: " << argv[i] << endl;//test for debug
			if (params[5]) {
				cerr << "diff: Enter {-I[string]} too many times" << endl;
				return;
			}
			else {
				flag = 1;
				params[5] = true;
				str_copy(strLookup, argv[i], 2, strlen(argv[i]));
			}
		}
		//如果argc[i]不符合选项说明，则退出
		if (!flag) {
			cerr << "diff: parmerter {" << argv[i] << "} is illegal\nTry 'diff --help' for more information "  << endl;
			return;
		}
	}

	//调用read()函数读取输入文件的信息至file_input[2]
	//重定向输出至strout
	//调用cmp()函数进行差异的比较并输出
	if (read(argc, argv)) {
		//定义ss流，保存原来cout标准输出流到oldcout
		//将cout的输出缓冲区替换为ss的
		stringstream ss;
		streambuf* oldcout = cout.rdbuf();
		cout.rdbuf(ss.rdbuf());
		memset(match, -1, sizeof(match));
		if (!cmp(params, strLookup)) {
			if (params[3]) {
				cout << "File " << argv[argc - 2] << " and " << argv[argc - 1] << " differ"<<endl;
			}
			else {
				displayMatch(params);
			}
		}
		//将ss流的缓冲区字符串复制给strout
		//并将输出重定向为原来的cout
		strcpy(gTerm.strout, ss.str().c_str());
		cout.rdbuf(oldcout);
	}
	return;
}
