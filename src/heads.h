void doTee(int ,char* []);
void doPwd(int ,char* []);
void doCp(int ,char* []);
void doCd(int ,char* []);
void doCat(int ,char* []);
void file_in(char*);
void file_out(char*);
void outnum(int);

struct Terminal {
	char user[100]; // �û���
	char mach[100]; // �������
	char root[100]; // ��Ŀ¼
	char wdir[100]; // ����Ŀ¼
	char strin[10000]; // �ض����׼����
	char strout[10000]; // �ض����׼���
};

extern Terminal gTerm;
extern char rootin[1000];
extern char rootout[1000];
extern char a[7];
extern int id;
