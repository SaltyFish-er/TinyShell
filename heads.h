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
	char user[1005]; // 用户名
	char mach[1005]; // 计算机名
	char root [1005]; // 根目录
	char wdir [1005]; // 工作目录
	char strin[100005]; // 重定向标准输入
	char strout[100005]; // 重定向标准输出
};

extern Terminal gTerm;
extern char rootin[1000];
extern char rootout[1000];
extern char a[7];
extern int id;
