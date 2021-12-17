void doTee(int ,char* []);
void doPwd(int ,char* []);
void doCp(int ,char* []);
void doCd(int ,char* []);
void doCat(int ,char* []);
void file_in(char*);
void file_out(char*);
void outnum(int);

struct Terminal {
	char user[100]; // 用户名
	char mach[100]; // 计算机名
	char root[100]; // 根目录
	char wdir[100]; // 工作目录
	char strin[10000]; // 重定向标准输入
	char strout[10000]; // 重定向标准输出
};

extern Terminal gTerm;
extern char rootin[1000];
extern char rootout[1000];
extern char a[7];
extern int id;
