#include"head.h"
#include"conio.h"

/*全局变量*/
struct node {
	int x, y, dir;//横坐标,纵坐标,方向(上下或者左右)
};

int dirx[] = { 0,1,0,-1 };
int diry[] = { -1,0,1,0 };
stack<node>st;//记录路径
IMAGE img[40];//存储图片
int time0;//游戏开始时间
int rem_num;//消除个数
int flag[N][N];//次数,计算能否相连要用
int mat[N][N];//游戏界面矩阵,记录了每个位置的图片编号
int tmp[N][N];//暂存矩阵，标记线
//显示模块控制模块
clock_t start, stop;//clock_t为clock()函数返回的变量类型
double duration;//记录被测函数运行时间，以秒为单位
bool if_time;//标记是否计时
int max_num;//最高连击次数
int now_num;//当前连击次数
int now_level;//当前关卡
int k = (N - 2) / 2;//大小参数，为了调节不同大小适应的方阵距离
int kr=SIZE/5;//表示当前大小的方块对应的圆半径系数
//关卡参数
int all_time;//可用初始时间
int add_time;//消除一个加1秒
int remind_price;//提示一次需要一秒代价
int all_num;//水果对数目
int cag;//水果种类，1-7
//下面是关卡参数配置
int eve_num[] = {1, 30, 35, 40, 45, 50,60 };//每一关的水果组数 
int eve_cag[] = { 1, 8, 10, 12, 13, 14 ,14};//每一关的种类
int eve_start_time[] = { 100,15,15,20,20,30,30 };//每一关的初始时间
int eve_add_time[] = { 100,2,2,1,1,1 ,1};//每一关的消除加成时间
int eve_remind_price[] = { 1, 1, 1, 1, 2, 2 ,2 };//提示代价

/*初始化*/
void all_ini(int gk) {//传入关卡值
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			mat[i][j] = 0;
		}
	}
	//默认配置
	all_num = (N - 2)*(N - 2)/2;//最多生成的组数
	if_time = 0;//一开始的不需要计时
	time0 = time(0);//开始时间
	rem_num = 0;//消除数量
	max_num = 1;//最大连击
	now_num = 1;//当前连击
	loadimage(&img[1], "./imgs/西瓜.png");
	loadimage(&img[2], "./imgs/苹果.png");
	loadimage(&img[3], "./imgs/草莓.png");
	loadimage(&img[4], "./imgs/梨.png");
	loadimage(&img[5], "./imgs/火龙果.png");
	loadimage(&img[6], "./imgs/桃子.png");
	loadimage(&img[7], "./imgs/香蕉.png");
	loadimage(&img[8], "./imgs/橙子.png");
	loadimage(&img[9], "./imgs/橘子.png");
	loadimage(&img[10], "./imgs/荔枝.png");
	loadimage(&img[11], "./imgs/葡萄.png");
	loadimage(&img[12], "./imgs/芒果.png");
	loadimage(&img[13], "./imgs/菠萝.png");
	loadimage(&img[14], "./imgs/猕猴桃.png");
	loadimage(&img[21], "./imgs/竖线.png");
	loadimage(&img[22], "./imgs/横线.png");
	loadimage(&img[23], "./imgs/下右.png");
	loadimage(&img[24], "./imgs/下左.png");
	loadimage(&img[25], "./imgs/上右.png");
	loadimage(&img[26], "./imgs/上左.png");
	loadimage(&img[27], "./noChangeSizeImgs/暂停.png");
	//关卡配置
	all_time = eve_start_time[gk];
	remind_price = eve_remind_price[gk];
	add_time=eve_add_time[gk];
	cag = eve_cag[gk];
	all_num = eve_num[gk];
}//初始化

/*初始化flag*/
void ini_flag(int x, int y,int ini_num) {//相当于x-行,y-列,最后一个是初始化的值
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			flag[i][j] = ini_num;
		}
	flag[x][y] = 0;
}//初始化flag

/*清除圆并补齐画面*/
void clearRound(int x,int y,int r,int detx,int dety) {//分别是xy点坐标，半径，偏移量
	int real_x, real_y;
	real_x = x * SIZE + SIZE / 2+detx;
	real_y = y * SIZE + SIZE / 2+dety;
	clearcircle(real_x, real_y, r);
	if(mat[y][x])
		putimage(x*SIZE, y*SIZE, &img[mat[y][x]]);//补上图片
}//清除圆并补齐画面

/*画一个圆*/
void drawRound(int x,int y,int r,int detx,int dety,int color){
	int real_x = x * SIZE + SIZE / 2;//圆心坐标
	int real_y = y * SIZE + SIZE / 2;
	circle(real_x + detx, real_y + dety, r);
	setfillcolor(color);  //填充色为蓝色
	fillcircle(real_x+detx, real_y +dety, r);
}//画一个圆

/*转换数字为字符串*/
void deal(int x, char *a) {
	stack <int>sst;
	while (x) {
		sst.push(x % 10);
		x /= 10;
	}
	int l = 0;
	while (!sst.empty()) {
		a[l++] = sst.top() + '0';
		sst.pop();
	}
	a[l] = '\0';
	if (a[0] == '\0') {
		a[0] = '0';
		a[1] = '\0';
	}
}//转换数字为字符串

/*计算和统计模块*/
void printCal() {
	char a[100];

	//计算连击
	settextcolor(WHITE);
	settextstyle(40, 40, 0);
	outtextxy(N*SIZE+40, 10, "当前关卡");
	settextstyle(100, 100, 0);
	deal(now_level, a);
	outtextxy(N*SIZE+170, 100, a);

	//统计数目
	settextcolor(WHITE);
	settextstyle(30, 30, 0);
	outtextxy(N*SIZE+40, N*SIZE-250, "剩余水果对");
	settextstyle(100, 100, 0);
	deal(all_num, a);
	outtextxy(N*SIZE + 150, N*SIZE - 170, a);
}//计算连击和统计模块

/*打印时间耗尽*/
void print_timeOver() {
	//打印时间耗尽
	settextcolor(BROWN);
	int size = k * 150 / (N - 2);//字体大小
	settextstyle(size, size, 0);
	outtextxy(N*SIZE / 2 - 4 * size, N*SIZE / 2 - 2 * size, "时间耗尽");
	system("pause");
}//打印时间耗尽

/*打印时间*/
void printTime() {
	char a[100];
	//剩余时间
	settextcolor(WHITE);
	settextstyle(40, 40, 0);
	outtextxy(N*SIZE + 40, 250, "剩余时间");
	settextstyle(100, 100, 0);
	int t = all_time + (rem_num*add_time) - (time(0) - time0);
	deal(t, a);
	if (t % 10 == 9) {
		clearrectangle(N*SIZE + 170, 350, N*SIZE + 400, 450);
	}
	outtextxy(N*SIZE + 170, 350, a);
}//打印时间

/*结束*/
void printOver() {
	//打印消除完毕
	settextcolor(BROWN);
	int size = k*150 / (N - 2);//字体大小
	settextstyle(size, size, 0);
	outtextxy(N*SIZE/2-4*size, N*SIZE / 2 - 2*size, "全部消除");
	line(size, N*SIZE / 2 - size/2, N*SIZE - size, N*SIZE / 2 -  size/2);
	//打印最高连击
	char a[100];
	settextcolor(WHITE);
	size = k*90 / (N - 2);
	settextstyle(size, size, 0);
	outtextxy(N*SIZE / 2 -6 * size, N*SIZE / 2 , "最高连击次数");
	size = k*150 / (N - 2);
	settextstyle(size, size, 0);
	deal(max_num, a);
	outtextxy(N*SIZE / 2 -size/2, N*SIZE / 2+size, a);
	//打印时间
	printTime();
	//打印消除完毕
	settextcolor(WHITE);
	size = k * 60 / (N - 2);//字体大小
	settextstyle(size, size, 0);
	outtextxy(N*SIZE / 2 - 7 * size, N*SIZE / 2 + 6 * size, "点击任意键继续...");
	system("pause");
}//结束

/*打印关卡过渡动画*/
void printNextLevel() {
	cleardevice();
	char a[100];
	char info[100] = "第";//拼接数组
	//打印时间耗尽
	settextcolor(BROWN);
	int size = k * 150 / (N - 2);//字体大小
	settextstyle(size, size, 0);
	deal(now_level, a);
	strcat(info, a);
	strcat(info, "关");
	outtextxy(MAXX / 2+200-size*2, MAXY / 2-size, info);
	Sleep(1000);
	cleardevice();
}//打印关卡过渡动画

/*打印全部通关*/
void prinfAllOk() {
	settextcolor(WHITE);
	int size = k * 150 / (N - 2);//字体大小
	settextstyle(size, size, 0);
	cleardevice();
	outtextxy(N*SIZE / 2 - 4 * size, N*SIZE / 2 - 2 * size, "恭喜你全部通关");
	system("pause");
}//打印全部通关

/*打印当前状态*/
void show_mat(int Mat[N][N]) {
	printCal();
	line(N*SIZE, 0, N*SIZE, MAXY);
	for (int i = 0; i <= N - 1; i++) {
		for (int j = 0; j <= N - 1; j++) {
			if (Mat[i][j] < 0) {
				putimage(j*SIZE, i*SIZE, &img[-Mat[i][j]+20]);
			}
			else if (Mat[i][j] == 0)
				continue;
			else 
				putimage(j*SIZE, i*SIZE, &img[Mat[i][j]]);
		}
	}
}//打印当前状态

/*搜索答案*/
void dfs(int x, int y, int qdx, int qdy, int zdx, int zdy, int step, int now, int &stats) {
	if (stats)return;//找到了就不找了
	//判断,参数分别是x,y,(x向右,y向下),起点xy和终点xy,当前转弯次数,当前方向参数，0123分别是上右下左,当前寻找的状态
	if (step == 3) {//超出步数
		if (!st.empty()) {//拿出去
			st.pop();
		}
		return;
	}
	if (zdx == x && zdy == y && step <= 2) {//终点
		stats = 1;
		return;
	}
	for (int i = now; i < now + 4; i++) {//从当前方向开始找
		int index = i % 4;
		int xx = x + dirx[index];
		int yy = y + diry[index];
		if (i == now + 2 && (x != qdx || y != qdy) || xx < 0 || yy < 0 || xx == N || yy == N || // 第一个的逻辑是不能往回走，除非是起点
			mat[yy][xx] && !(xx == zdx && yy == zdy))//不合法,最后一个代表已经被占据并且不是终点
			continue;
		if ((x != qdx || y != qdy) && (i % 2 == 0 && (zdx - qdx)*(xx - x) < 0 || i % 2 == 1 && (zdy - qdy)*(yy - y) > 0))//竖着走和横着走不可能偏离终点
			continue;
		if (now == index || x == qdx && y == qdy) {//方向相同或者是起点
			st.push({ xx,yy,i % 2 });
			dfs(xx, yy, qdx, qdy, zdx, zdy, step, index, stats);
		}
		else {
			st.push({ xx,yy,i % 2 });
			dfs(xx, yy, qdx, qdy, zdx, zdy, step + 1, index, stats);
		}
		if (stats)return;
	}
	if (stats)return;
	if (!st.empty()) {//四个方向都没有
		st.pop();
	}
	return;
}//搜索答案

/*生成矩阵*/
void create_mat(int max_num) {//采用随机生成方式,生成max_num组
	srand((unsigned)time(NULL));
	bool t[5000];//标记数组,标记有没有数字
	for (int i = 0; i <= (N-2) * (N-2) + 1; i++)//矩阵一维化
		t[i] = 0;
	for (int i = 0; ; i++) {
		for (int now = 1; now <= cag; now++) {
			int z1, z2;//找到两个没出现过的位置
			while (1) {
				z1 = rand() % ((N-2)*(N-2)) + 1;//取模和乘的优先级一样...
				if (!t[z1]) {
					t[z1] = 1;
					break;
				}
			}
			while (1) {
				z2 = rand() % ((N-2)*(N-2)) + 1;
				if (!t[z2]) {
					t[z2] = 1;
					break;
				}
			}
			int x1 = (z1 - 1) % (N - 2) + 1;
			int y1 = (z1 - 1) / (N - 2) + 1;
			int x2 = (z2 - 1) % (N - 2) + 1;
			int y2 = (z2 - 1) / (N - 2) + 1;//计算行列
			mat[y1][x1] = now;
			mat[y2][x2] = now;//全部随机生成
			if (i * cag + now == max_num) {
				show_mat(mat);
				return;
			}
		}
	}
}//生成矩阵

/*标记线*/
void deal(int x1,int x2,int y1,int y2) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			tmp[i][j] = mat[i][j];//复制矩阵
	int bef_dir = st.top().dir;//初始方向
	int bef_x = st.top().x;
	int bef_y = st.top().y;
	st.pop();//终点不需要变成线
	while (!st.empty()) {
		int xx = st.top().x;
		int yy = st.top().y;
		int dir = st.top().dir;
		st.pop();
		if (dir == 0) {//上下
			//tmp[st.top().y][st.top().x] = -1;//表示上下
			if (bef_dir==dir) {
				//跟上一个一样
				tmp[yy][xx] = -1;//上下
			}
			else {         //左右
				if (!st.empty() && st.top().y < yy || st.empty() && y1 < yy) {
					//跟下一个比定上下,此时是下一个在上面
					if (xx < bef_x)//向左走
						tmp[yy][xx] = -5;//上右
					else
						tmp[yy][xx] = -6;//上左
				}
				else {
					if (xx < bef_x)//向左走
						tmp[yy][xx] = -3;//下右
					else
						tmp[yy][xx] = -4;//下左
				}
			}
		}
		else {
			//tmp[st.top().y][st.top().x] = -2;//表示左右
			if (bef_dir == dir) {//也是左右
				tmp[yy][xx] = -2;
			}
			// x1 y1 是起点坐标
			else {
				if (!st.empty() && st.top().x < xx || st.empty() && x1 < xx) {//下一个在左边
					if (yy < bef_y) //向上走
						tmp[yy][xx] = -4;//下左
					else 
						tmp[yy][xx] = -6;//上左
				}
				else {
					if (yy < bef_y)//向上走
						tmp[yy][xx] = -3;//下右
					else
						tmp[yy][xx] = -5;//上右
				}
			}
		}
		bef_x = xx;
		bef_y = yy;
		bef_dir = dir;//更新表示上一个状态的量
	}
}//标记线

/*处理消除之后的样子以及计时模块*/
bool deal_after(int x1,int x2,int y1,int y2) {
	//计时模块
	rem_num++;
	all_num--;
	if (if_time) {
		stop = clock();//停止计时
		duration = (double)(stop - start) / CLOCKS_PER_SEC;//计算运行时间
		if (duration <= DET) {
			now_num += 1;
			max_num = max(max_num, now_num);
		}
		else {
			now_num = 1;//重置连击次数
		}
	}
	//下面显示出消除的样子
	show_mat(tmp);
	Sleep(300);//程序暂停
	mat[y1][x1] = 0;
	mat[y2][x2] = 0;
	cleardevice();
	show_mat(mat);
	if (!all_num) {
		return 1;//代表可以结束游戏了
	}
	start = clock();//开始计时
	if_time = 1;//计时标记
	return 0;
}

/*提示模块*/
bool remind(int &x1, int &y1, int &x2, int &y2) {//返回是否可以消除,返回值内包括起点终点坐标
	for (int i1 = 1; i1 < N - 1; i1++) {
		for (int j1 = 1; j1 < N - 1; j1++) {
			if (!mat[i1][j1])
				continue;//空格
			for (int i2 = 1; i2 < N - 1; i2++) {
				for (int j2 = 1; j2 < N - 1; j2++) {
					if (!mat[i2][j2] || i1 == i2 && j1 == j2||mat[i2][j2]!=mat[i1][j1])
						continue;
					int check = 0;
					dfs(j1, i1, j1, i1, j2, i2, 0, 0, check);
					if(check) {
						x1 = j1;
						y1 = i1;
						x2 = j2;
						y2 = i2;
						return 1;
					}
					else {
						while (!st.empty())//否则要清空栈
							st.pop();
					}
				}
			}
		}
	}
	return 0;
}//提示模块

/*刷新模块*/
void refresh() {
	int x, y;
	srand((unsigned)time(NULL));
	ini_flag(0, 0, 0);//初始化flag，借用flag数组标记
	for (int i = 1; i <= N - 1; i++) {
		for (int j = 1; j <= N - 1; j++) {
			if (mat[i][j]&&!flag[i][j]) {//如果里面是有数字的并且不是前面的移过来的
				while (1) {
					x = rand() % (N - 2) + 1;
					y = rand() % (N - 2) + 1;//找到两个没出现过的位置
					if (!mat[y][x]) {//空的
						mat[y][x] = mat[i][j];
						mat[i][j] = 0;
						flag[y][x] = 1;//标记点
						break;
					}
				}
			}
		}
	}
	cleardevice();
	show_mat(mat);
}//刷新模块

/*游戏暂停*/
int stop_game() {
	int stop_time = time(0);
	cleardevice();
	putimage(MAXX / 2 + 100, MAXY / 2 - 100, &img[27]);
	system("pause");//停止
	return time(0)-stop_time;//返回暂停时间
}//游戏暂停

/*游戏操作部分*/
bool play() {
	int x1=1, y1=1, x2=1, y2=1,now=1;//记录当前位置(x2,y2)和上一个位置(x1,y1),now记录当前状态，1是第一个，2是第二个
	int bef_t = time(0)-time0;
	while (all_num) {//只要还剩下
		MOUSEMSG m;
		if (bef_t != time(0) - time0) {
			printTime();
		}
		printTime();//打印时间
		if (all_time + (rem_num*add_time) - (time(0) - time0)==0) {//时间耗尽
			print_timeOver();
			return 0;//游戏结束
		}
		if (MouseHit())//是否有鼠标消息
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)//左键
			{
				//rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
				if (m.x > N*SIZE || m.y > N*SIZE)
					continue;
				m.x = m.x / SIZE;
				m.y = m.y / SIZE;
				if (now == 1) {//第一个确定状态只要不越界都可以,然后记录上一个位置
					x1 = m.x;
					y1 = m.y;//x1，y1存储上一次状态
					if (!mat[y1][x1])
						continue;//空格无效
					drawRound(x1, y1, kr, 0, 0, GREEN);
					now = 2;
				}
				else {//第二个要判定状态
					x2 = m.x;
					y2 = m.y;
					if (mat[y1][x1] != mat[y2][x2] || x1 == x2 && y1 == y2) {
						clearRound(x1, y1, kr, 0, 0);//重新选择
						now = 1;
						continue;
					}
					if (x1 > 0 && x1 <= N - 2 && y1 > 0 && y1 <= N - 2 && x2 > 0
						&& x2 <= N - 2 && y2 > 0 && y2 <= N - 2) {
						int check = 0;
						dfs(x1, y1, x1, y1, x2, y2, 0, 0, check);
						if (check) {
							deal(x1, x2, y1, y2);//标记线
							bool if_over = deal_after(x1, x2, y1, y2);//判断是否结束，同时善后工作
							if (if_over) {
								printOver();//打印结束语句
								return 1;//通过当前关卡
							}
							int check = remind(x1, y1, x2, y2);
							if (!check) {
								refresh();//没有可以消除的了,刷新
							}
							else {
								while (!st.empty())
									st.pop();
							}
						}
						else {
							clearRound(x1, y1, kr, 0, 0);//不能消除
							while (!st.empty())//清空栈
								st.pop();
						}
					}
					now = 1;
				}
			}
		}
		if (_kbhit()) {//如果有按键
			int ch = _getch();
			if (ch == KONG) {//提示按钮
				int check = remind(x1,y1,x2,y2);
				if (check) {
					deal(x1, x2, y1, y2);//标记线
					show_mat(tmp);
					Sleep(remind_price*1000);//代价
					cleardevice();
					show_mat(mat);
				}
			}
			else if (ch == ESC) {
				time0+=stop_game();//要把计时起点向后挪动
				cleardevice();
				show_mat(mat);
			}
			else if (ch == RIGHT) {//刷新
				refresh();
			}
		}
	}
}//游戏操作部分

/*总控程序*/
void game() {
	initgraph(MAXX + 401, MAXY + 1);
	while (1) {
		for (now_level = 5; now_level <= 6; now_level++) {//七关
			printNextLevel();//打印转场动画
			all_ini(now_level);//初始化所有矩阵和图片库
			create_mat(all_num);//生成矩阵
			int check = play();//开始游戏并返回游戏状态
			if (!check) {
				break;//游戏结束
			}
			else if (now_level == 6) {
				prinfAllOk();//打印全部通关
			}
		}
	}
	//closegraph();//关闭窗口
}//总控程序
