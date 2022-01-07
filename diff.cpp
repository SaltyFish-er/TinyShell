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

//���ж�����ļ�
struct fileInLine {
	string file[MAXROW];
	int lineCount=0;
};
 fileInLine file_input[2];

//�����ļ���ƥ�����
 int match[MAXROW];

//��������ĵ�
void diff_help() {
	cout << "Usage: diff {-b}{-B}{-I[�ַ���]}{-q}{-w}[�ļ�1][�ļ�2]"<<endl;
	cout << "���бȽ������ļ�" << endl;
	cout << endl;
	cout << "  --help����ʾ������\n\
  -b�������ո��ַ��Ĳ�ͬ��\n\
  -B�������հ��С�\n\
  -i��������Сд�Ĳ�ͬ��\n\
  -q������ʾ���޲��죬����ʾ��ϸ����Ϣ���޲���ʱ����ʾ���в���ʱ��ʾ��File[�ļ� 1] and [�ļ� 2] differ��\n\
  -w������ȫ���Ŀո��ַ���\n\
  -I[�ַ���]���������ļ���ĳ����������ͬ�����⼸��ͬʱ��������ѡ����ָ���ַ���������ʾ�������ļ��Ĳ��졣"<< endl;
}

//��ȡ�ļ�����·��
//�������Ϊ"-"ֱ�ӷ���
//������new��absolutePath�Ŀռ佫��read������delete
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

//��src������l��r�Ĳ��ֿ�����dest [l,r)
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

//�������ļ�����Ϣ���ж���file_input[2]
//����ɹ��򷵻�1�������򷵻�0
bool read(int argc,char* argv[]) { //debug �������޸�argvΪchar*����
	char* in_file[2];
	in_file[0] = getPath(argv[argc - 2]);
	in_file[1] = getPath(argv[argc - 1]);
	//cerr<<"in_file[0] is "<<in_file[0]<<endl;
	//cerr<<"in_file[1] is "<<in_file[1]<<endl;
	//�������ļ���������ʼ��
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

//������/�����ִ�Сд��ģʽ�ж������ַ��Ƿ�һ��
//�жϿո��Ƿ�һ��
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

//�ж�key�Ƿ�Ϊdest���Ӵ�
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

//��ȡ�ַ������һ���ǿո��ַ�������
int getLastChar(string str) {
	int length = str.length();
	for (int i = length - 1;i >= 0;i--) {
		if (str[i] != ' '&&str[i]!=9) {
			return i;
		}
	}
	return 0;
}

//�Ƚ������ַ����Ƿ�ͬ
//������Ҫ���������ѡ���� -w -b -i -I
bool cmpLine(bool params[6], char* key, string line1, string line2) {
	int length1 = line1.length();
	int length2 = line2.length();
	//����-w -i -b
	//-b�Ĺ����������-w�������-wʱ������Ϊ����-b����ν
	//����������������۴���(1) {-w -i -b} || {-w -i} || {-w -b} || {-w}
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

	//����-Iѡ��
	//���ж��Ƿ�ƥ�䣬���ƥ�䷵��true
	//�����жϲ�ƥ������Ƿ������ַ���ƥ��
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

//�ж�һ�����ǲ���ֻ�пո�(space/tab)
bool isLineBlank(string line) {
	for (int i = 0;i < line.length();i++) {
		if (line[i] != 9 && line[i] != 32) {
			return false;
		}
	}
	return true;
}
//����matchƥ��Խ�����������Ļ��
//������Ҫ����-B����
//ע��-B�����-w -b����ʱ��ֻ�пո����Ҳ��Ϊ�ǿհ���
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
								//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
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
								//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
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
								//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
								out = 1;
							}
						}
					}
					for (int n = last_j + 1;n < now_j;n++) {
						if (file_input[1].file[n] != "") {
							if (!((params[0] || params[4]) && isLineBlank(file_input[1].file[n]))) {
								//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
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

	//����󲿷ֽ������
	//��ȡ���һ��ƥ���
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
						//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
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
						//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
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
						//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
						out = 1;
					}
				}
			}
			for (;b < file_input[1].lineCount;b++) {
				if (file_input[1].file[b] != "") {
					if (!((params[0] || params[4]) && isLineBlank(file_input[1].file[b]))) {
						//�жϴ�ʱ������{-B������-w/-b����}��{����ֻ�пո�}��״̬
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

//�Ƚ�file_input�����ļ��Ĳ�ͬ����ƥ��������
//������Ҫ��-q����ѡ�����Ԥ����
//Ȼ�����cmpLine����-w -b -i -I
//�����ļ���ͬ�򷵻�0����ͬ�򷵻�1
bool cmp(bool params[6],char* key) {
	bool same = 1;
	int last_i = -1, last_j = -1;
	int temp_i = 10000, temp_j = 10000;
	bool matched = 0;//��¼�Ƿ����һ����δ���µ�ƥ���
	for (int i = 0, j = 0;i < file_input[0].lineCount && j < file_input[1].lineCount;) {
		if (cmpLine(params, key, file_input[0].file[i], file_input[1].file[j])) {
			if (i + j < temp_i + temp_j) {
				//��ʱƥ��Ŀ����Ż�
				temp_i = i;
				temp_j = j;
				matched = 1;
				if (i + 1 >= file_input[0].lineCount) {
					//����ﵽ�Ͻ磬��ô����ƥ���
					match[temp_i] = temp_j;
					matched = 0;
					last_i = temp_i;
					last_j = temp_j;
					temp_i = 10000; //����Ϊ���
					temp_j = 10000;
					i = last_i + 1;
					j = last_j + 1;
				}
				else {
					//�������������̽
					i = i + 1;
					j = last_j + 1;
				}
			}
			else {
				//��ʱƥ����Ѿ������Ż���
				if (i + 1 < file_input[0].lineCount) {
					//���i�����Լӣ���ô����������̽
					i++;
				}
				else {
					//���i�Ѿ����ܼ��ˣ�����ƥ��
					match[temp_i] = temp_j;
					matched = 0;
					last_i = temp_i;
					last_j = temp_j;
					temp_i = 10000; //����Ϊ���
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
				//j�Ѿ����˺ܶ��ˣ�����û�ҵ������õģ�ƥ��
				//Ӧ�ø���i��
				if (i + 1 < file_input[0].lineCount) {
					//i�������ټ�
					if (i < temp_i + temp_j - last_j) {
						//���ʱ��Ҳ�����ҵ����õ�ƥ��(������match�ˣ�Ҳ����û��)
						//i++����ԭj��������
						//���ûmatch����same��0
						if (!matched) {
							same = 0;
						}
						i++;
						j = last_j + 1;
					}
					else {
						//���ʱ��һ��match�ˣ�����i�Ѿ����󣬲��������ҵ����õ�ƥ����
						//����ֱ�Ӹ���match����������������ԭ
						match[temp_i] = temp_j;
						matched = 0;
						last_i = temp_i;
						last_j = temp_j;
						temp_i = 10000; //����Ϊ���
						temp_j = 10000;
						i = last_i + 1;
						j = last_j + 1;
					}
				}
				else {
					//i�Ѿ���Max�ˣ��ټӳ�����
					if (!matched) {
						//���ûmatch��˵�������Ѿ�����
						//ֱ��i++�����������ƥ��
						i++;
					}
					else {
						//���match�ˣ�˵���Ѿ��������и��õ�ƥ����
						//����match����������������ԭ
						match[temp_i] = temp_j;
						matched = 0;
						last_i = temp_i;
						last_j = temp_j;
						temp_i = 10000; //����Ϊ���
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

void doDiff(int argc, char* argv[]) { //debug �������޸�argvΪchar*����
	//����--help����
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
	
	//��ȡ����ѡ���¼
	char strLookup[1005];
	const char* paramString[] = { "-b","-B","-i","-q","-w","-I" };
	bool params[6];
	memset(params, 0, sizeof(params));
	bool flag = 0; //��¼ÿ������ѡ���Ƿ�Ϸ�
	for (int i = 1;i < argc - 2;i++) {
		//����argc[i]�Ƿ�Ϊ��-I������ѡ��˵��
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
		//����argc[i]�Ƿ�Ϊ-I
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
		//���argc[i]������ѡ��˵�������˳�
		if (!flag) {
			cerr << "diff: parmerter {" << argv[i] << "} is illegal\nTry 'diff --help' for more information "  << endl;
			return;
		}
	}

	//����read()������ȡ�����ļ�����Ϣ��file_input[2]
	//�ض��������strout
	//����cmp()�������в���ıȽϲ����
	if (read(argc, argv)) {
		//����ss��������ԭ��cout��׼�������oldcout
		//��cout������������滻Ϊss��
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
		//��ss���Ļ������ַ������Ƹ�strout
		//��������ض���Ϊԭ����cout
		strcpy(gTerm.strout, ss.str().c_str());
		cout.rdbuf(oldcout);
	}
	return;
}
