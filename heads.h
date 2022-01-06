#define MAXLINE 1005
#define MAXFILE 100005

void doDiff(int, char**);
void doGrep(int, char**);
void doTee(int, char**);
void doCat(int, char**);
void doCp(int, char**);
void doCd(int, char**);
void doPwd(int, char**);
void doEcho(int, char**);
void doLen(int, char**);

void file_in(char*);
void file_out(char*);
void outnum(int);

struct Terminal {
	char user[1005]; // �û���
	char mach[1005]; // �������
	char root [1005]; // ��Ŀ¼
	char wdir [1005]; // ����Ŀ¼
	char strin[100005]; // �ض����׼����
	char strout[100005]; // �ض����׼���
};

extern Terminal gTerm;
extern char rootin[1000];
extern char rootout[1000];
extern char a[7];
extern int id;
