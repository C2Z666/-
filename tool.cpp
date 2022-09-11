#include"head.h"
#include"conio.h"

/*ȫ�ֱ���*/
struct node {
	int x, y, dir;//������,������,����(���»�������)
};

int dirx[] = { 0,1,0,-1 };
int diry[] = { -1,0,1,0 };
stack<node>st;//��¼·��
IMAGE img[40];//�洢ͼƬ
int time0;//��Ϸ��ʼʱ��
int rem_num;//��������
int flag[N][N];//����,�����ܷ�����Ҫ��
int mat[N][N];//��Ϸ�������,��¼��ÿ��λ�õ�ͼƬ���
int tmp[N][N];//�ݴ���󣬱����
//��ʾģ�����ģ��
clock_t start, stop;//clock_tΪclock()�������صı�������
double duration;//��¼���⺯������ʱ�䣬����Ϊ��λ
bool if_time;//����Ƿ��ʱ
int max_num;//�����������
int now_num;//��ǰ��������
int now_level;//��ǰ�ؿ�
int k = (N - 2) / 2;//��С������Ϊ�˵��ڲ�ͬ��С��Ӧ�ķ������
int kr=SIZE/5;//��ʾ��ǰ��С�ķ����Ӧ��Բ�뾶ϵ��
//�ؿ�����
int all_time;//���ó�ʼʱ��
int add_time;//����һ����1��
int remind_price;//��ʾһ����Ҫһ�����
int all_num;//ˮ������Ŀ
int cag;//ˮ�����࣬1-7
//�����ǹؿ���������
int eve_num[] = {1, 30, 35, 40, 45, 50,60 };//ÿһ�ص�ˮ������ 
int eve_cag[] = { 1, 8, 10, 12, 13, 14 ,14};//ÿһ�ص�����
int eve_start_time[] = { 100,15,15,20,20,30,30 };//ÿһ�صĳ�ʼʱ��
int eve_add_time[] = { 100,2,2,1,1,1 ,1};//ÿһ�ص������ӳ�ʱ��
int eve_remind_price[] = { 1, 1, 1, 1, 2, 2 ,2 };//��ʾ����

/*��ʼ��*/
void all_ini(int gk) {//����ؿ�ֵ
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			mat[i][j] = 0;
		}
	}
	//Ĭ������
	all_num = (N - 2)*(N - 2)/2;//������ɵ�����
	if_time = 0;//һ��ʼ�Ĳ���Ҫ��ʱ
	time0 = time(0);//��ʼʱ��
	rem_num = 0;//��������
	max_num = 1;//�������
	now_num = 1;//��ǰ����
	loadimage(&img[1], "./imgs/����.png");
	loadimage(&img[2], "./imgs/ƻ��.png");
	loadimage(&img[3], "./imgs/��ݮ.png");
	loadimage(&img[4], "./imgs/��.png");
	loadimage(&img[5], "./imgs/������.png");
	loadimage(&img[6], "./imgs/����.png");
	loadimage(&img[7], "./imgs/�㽶.png");
	loadimage(&img[8], "./imgs/����.png");
	loadimage(&img[9], "./imgs/����.png");
	loadimage(&img[10], "./imgs/��֦.png");
	loadimage(&img[11], "./imgs/����.png");
	loadimage(&img[12], "./imgs/â��.png");
	loadimage(&img[13], "./imgs/����.png");
	loadimage(&img[14], "./imgs/⨺���.png");
	loadimage(&img[21], "./imgs/����.png");
	loadimage(&img[22], "./imgs/����.png");
	loadimage(&img[23], "./imgs/����.png");
	loadimage(&img[24], "./imgs/����.png");
	loadimage(&img[25], "./imgs/����.png");
	loadimage(&img[26], "./imgs/����.png");
	loadimage(&img[27], "./noChangeSizeImgs/��ͣ.png");
	//�ؿ�����
	all_time = eve_start_time[gk];
	remind_price = eve_remind_price[gk];
	add_time=eve_add_time[gk];
	cag = eve_cag[gk];
	all_num = eve_num[gk];
}//��ʼ��

/*��ʼ��flag*/
void ini_flag(int x, int y,int ini_num) {//�൱��x-��,y-��,���һ���ǳ�ʼ����ֵ
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			flag[i][j] = ini_num;
		}
	flag[x][y] = 0;
}//��ʼ��flag

/*���Բ�����뻭��*/
void clearRound(int x,int y,int r,int detx,int dety) {//�ֱ���xy�����꣬�뾶��ƫ����
	int real_x, real_y;
	real_x = x * SIZE + SIZE / 2+detx;
	real_y = y * SIZE + SIZE / 2+dety;
	clearcircle(real_x, real_y, r);
	if(mat[y][x])
		putimage(x*SIZE, y*SIZE, &img[mat[y][x]]);//����ͼƬ
}//���Բ�����뻭��

/*��һ��Բ*/
void drawRound(int x,int y,int r,int detx,int dety,int color){
	int real_x = x * SIZE + SIZE / 2;//Բ������
	int real_y = y * SIZE + SIZE / 2;
	circle(real_x + detx, real_y + dety, r);
	setfillcolor(color);  //���ɫΪ��ɫ
	fillcircle(real_x+detx, real_y +dety, r);
}//��һ��Բ

/*ת������Ϊ�ַ���*/
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
}//ת������Ϊ�ַ���

/*�����ͳ��ģ��*/
void printCal() {
	char a[100];

	//��������
	settextcolor(WHITE);
	settextstyle(40, 40, 0);
	outtextxy(N*SIZE+40, 10, "��ǰ�ؿ�");
	settextstyle(100, 100, 0);
	deal(now_level, a);
	outtextxy(N*SIZE+170, 100, a);

	//ͳ����Ŀ
	settextcolor(WHITE);
	settextstyle(30, 30, 0);
	outtextxy(N*SIZE+40, N*SIZE-250, "ʣ��ˮ����");
	settextstyle(100, 100, 0);
	deal(all_num, a);
	outtextxy(N*SIZE + 150, N*SIZE - 170, a);
}//����������ͳ��ģ��

/*��ӡʱ��ľ�*/
void print_timeOver() {
	//��ӡʱ��ľ�
	settextcolor(BROWN);
	int size = k * 150 / (N - 2);//�����С
	settextstyle(size, size, 0);
	outtextxy(N*SIZE / 2 - 4 * size, N*SIZE / 2 - 2 * size, "ʱ��ľ�");
	system("pause");
}//��ӡʱ��ľ�

/*��ӡʱ��*/
void printTime() {
	char a[100];
	//ʣ��ʱ��
	settextcolor(WHITE);
	settextstyle(40, 40, 0);
	outtextxy(N*SIZE + 40, 250, "ʣ��ʱ��");
	settextstyle(100, 100, 0);
	int t = all_time + (rem_num*add_time) - (time(0) - time0);
	deal(t, a);
	if (t % 10 == 9) {
		clearrectangle(N*SIZE + 170, 350, N*SIZE + 400, 450);
	}
	outtextxy(N*SIZE + 170, 350, a);
}//��ӡʱ��

/*����*/
void printOver() {
	//��ӡ�������
	settextcolor(BROWN);
	int size = k*150 / (N - 2);//�����С
	settextstyle(size, size, 0);
	outtextxy(N*SIZE/2-4*size, N*SIZE / 2 - 2*size, "ȫ������");
	line(size, N*SIZE / 2 - size/2, N*SIZE - size, N*SIZE / 2 -  size/2);
	//��ӡ�������
	char a[100];
	settextcolor(WHITE);
	size = k*90 / (N - 2);
	settextstyle(size, size, 0);
	outtextxy(N*SIZE / 2 -6 * size, N*SIZE / 2 , "�����������");
	size = k*150 / (N - 2);
	settextstyle(size, size, 0);
	deal(max_num, a);
	outtextxy(N*SIZE / 2 -size/2, N*SIZE / 2+size, a);
	//��ӡʱ��
	printTime();
	//��ӡ�������
	settextcolor(WHITE);
	size = k * 60 / (N - 2);//�����С
	settextstyle(size, size, 0);
	outtextxy(N*SIZE / 2 - 7 * size, N*SIZE / 2 + 6 * size, "������������...");
	system("pause");
}//����

/*��ӡ�ؿ����ɶ���*/
void printNextLevel() {
	cleardevice();
	char a[100];
	char info[100] = "��";//ƴ������
	//��ӡʱ��ľ�
	settextcolor(BROWN);
	int size = k * 150 / (N - 2);//�����С
	settextstyle(size, size, 0);
	deal(now_level, a);
	strcat(info, a);
	strcat(info, "��");
	outtextxy(MAXX / 2+200-size*2, MAXY / 2-size, info);
	Sleep(1000);
	cleardevice();
}//��ӡ�ؿ����ɶ���

/*��ӡȫ��ͨ��*/
void prinfAllOk() {
	settextcolor(WHITE);
	int size = k * 150 / (N - 2);//�����С
	settextstyle(size, size, 0);
	cleardevice();
	outtextxy(N*SIZE / 2 - 4 * size, N*SIZE / 2 - 2 * size, "��ϲ��ȫ��ͨ��");
	system("pause");
}//��ӡȫ��ͨ��

/*��ӡ��ǰ״̬*/
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
}//��ӡ��ǰ״̬

/*������*/
void dfs(int x, int y, int qdx, int qdy, int zdx, int zdy, int step, int now, int &stats) {
	if (stats)return;//�ҵ��˾Ͳ�����
	//�ж�,�����ֱ���x,y,(x����,y����),���xy���յ�xy,��ǰת�����,��ǰ���������0123�ֱ�����������,��ǰѰ�ҵ�״̬
	if (step == 3) {//��������
		if (!st.empty()) {//�ó�ȥ
			st.pop();
		}
		return;
	}
	if (zdx == x && zdy == y && step <= 2) {//�յ�
		stats = 1;
		return;
	}
	for (int i = now; i < now + 4; i++) {//�ӵ�ǰ����ʼ��
		int index = i % 4;
		int xx = x + dirx[index];
		int yy = y + diry[index];
		if (i == now + 2 && (x != qdx || y != qdy) || xx < 0 || yy < 0 || xx == N || yy == N || // ��һ�����߼��ǲ��������ߣ����������
			mat[yy][xx] && !(xx == zdx && yy == zdy))//���Ϸ�,���һ�������Ѿ���ռ�ݲ��Ҳ����յ�
			continue;
		if ((x != qdx || y != qdy) && (i % 2 == 0 && (zdx - qdx)*(xx - x) < 0 || i % 2 == 1 && (zdy - qdy)*(yy - y) > 0))//�����ߺͺ����߲�����ƫ���յ�
			continue;
		if (now == index || x == qdx && y == qdy) {//������ͬ���������
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
	if (!st.empty()) {//�ĸ�����û��
		st.pop();
	}
	return;
}//������

/*���ɾ���*/
void create_mat(int max_num) {//����������ɷ�ʽ,����max_num��
	srand((unsigned)time(NULL));
	bool t[5000];//�������,�����û������
	for (int i = 0; i <= (N-2) * (N-2) + 1; i++)//����һά��
		t[i] = 0;
	for (int i = 0; ; i++) {
		for (int now = 1; now <= cag; now++) {
			int z1, z2;//�ҵ�����û���ֹ���λ��
			while (1) {
				z1 = rand() % ((N-2)*(N-2)) + 1;//ȡģ�ͳ˵����ȼ�һ��...
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
			int y2 = (z2 - 1) / (N - 2) + 1;//��������
			mat[y1][x1] = now;
			mat[y2][x2] = now;//ȫ���������
			if (i * cag + now == max_num) {
				show_mat(mat);
				return;
			}
		}
	}
}//���ɾ���

/*�����*/
void deal(int x1,int x2,int y1,int y2) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			tmp[i][j] = mat[i][j];//���ƾ���
	int bef_dir = st.top().dir;//��ʼ����
	int bef_x = st.top().x;
	int bef_y = st.top().y;
	st.pop();//�յ㲻��Ҫ�����
	while (!st.empty()) {
		int xx = st.top().x;
		int yy = st.top().y;
		int dir = st.top().dir;
		st.pop();
		if (dir == 0) {//����
			//tmp[st.top().y][st.top().x] = -1;//��ʾ����
			if (bef_dir==dir) {
				//����һ��һ��
				tmp[yy][xx] = -1;//����
			}
			else {         //����
				if (!st.empty() && st.top().y < yy || st.empty() && y1 < yy) {
					//����һ���ȶ�����,��ʱ����һ��������
					if (xx < bef_x)//������
						tmp[yy][xx] = -5;//����
					else
						tmp[yy][xx] = -6;//����
				}
				else {
					if (xx < bef_x)//������
						tmp[yy][xx] = -3;//����
					else
						tmp[yy][xx] = -4;//����
				}
			}
		}
		else {
			//tmp[st.top().y][st.top().x] = -2;//��ʾ����
			if (bef_dir == dir) {//Ҳ������
				tmp[yy][xx] = -2;
			}
			// x1 y1 ���������
			else {
				if (!st.empty() && st.top().x < xx || st.empty() && x1 < xx) {//��һ�������
					if (yy < bef_y) //������
						tmp[yy][xx] = -4;//����
					else 
						tmp[yy][xx] = -6;//����
				}
				else {
					if (yy < bef_y)//������
						tmp[yy][xx] = -3;//����
					else
						tmp[yy][xx] = -5;//����
				}
			}
		}
		bef_x = xx;
		bef_y = yy;
		bef_dir = dir;//���±�ʾ��һ��״̬����
	}
}//�����

/*��������֮��������Լ���ʱģ��*/
bool deal_after(int x1,int x2,int y1,int y2) {
	//��ʱģ��
	rem_num++;
	all_num--;
	if (if_time) {
		stop = clock();//ֹͣ��ʱ
		duration = (double)(stop - start) / CLOCKS_PER_SEC;//��������ʱ��
		if (duration <= DET) {
			now_num += 1;
			max_num = max(max_num, now_num);
		}
		else {
			now_num = 1;//������������
		}
	}
	//������ʾ������������
	show_mat(tmp);
	Sleep(300);//������ͣ
	mat[y1][x1] = 0;
	mat[y2][x2] = 0;
	cleardevice();
	show_mat(mat);
	if (!all_num) {
		return 1;//������Խ�����Ϸ��
	}
	start = clock();//��ʼ��ʱ
	if_time = 1;//��ʱ���
	return 0;
}

/*��ʾģ��*/
bool remind(int &x1, int &y1, int &x2, int &y2) {//�����Ƿ��������,����ֵ�ڰ�������յ�����
	for (int i1 = 1; i1 < N - 1; i1++) {
		for (int j1 = 1; j1 < N - 1; j1++) {
			if (!mat[i1][j1])
				continue;//�ո�
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
						while (!st.empty())//����Ҫ���ջ
							st.pop();
					}
				}
			}
		}
	}
	return 0;
}//��ʾģ��

/*ˢ��ģ��*/
void refresh() {
	int x, y;
	srand((unsigned)time(NULL));
	ini_flag(0, 0, 0);//��ʼ��flag������flag������
	for (int i = 1; i <= N - 1; i++) {
		for (int j = 1; j <= N - 1; j++) {
			if (mat[i][j]&&!flag[i][j]) {//��������������ֵĲ��Ҳ���ǰ����ƹ�����
				while (1) {
					x = rand() % (N - 2) + 1;
					y = rand() % (N - 2) + 1;//�ҵ�����û���ֹ���λ��
					if (!mat[y][x]) {//�յ�
						mat[y][x] = mat[i][j];
						mat[i][j] = 0;
						flag[y][x] = 1;//��ǵ�
						break;
					}
				}
			}
		}
	}
	cleardevice();
	show_mat(mat);
}//ˢ��ģ��

/*��Ϸ��ͣ*/
int stop_game() {
	int stop_time = time(0);
	cleardevice();
	putimage(MAXX / 2 + 100, MAXY / 2 - 100, &img[27]);
	system("pause");//ֹͣ
	return time(0)-stop_time;//������ͣʱ��
}//��Ϸ��ͣ

/*��Ϸ��������*/
bool play() {
	int x1=1, y1=1, x2=1, y2=1,now=1;//��¼��ǰλ��(x2,y2)����һ��λ��(x1,y1),now��¼��ǰ״̬��1�ǵ�һ����2�ǵڶ���
	int bef_t = time(0)-time0;
	while (all_num) {//ֻҪ��ʣ��
		MOUSEMSG m;
		if (bef_t != time(0) - time0) {
			printTime();
		}
		printTime();//��ӡʱ��
		if (all_time + (rem_num*add_time) - (time(0) - time0)==0) {//ʱ��ľ�
			print_timeOver();
			return 0;//��Ϸ����
		}
		if (MouseHit())//�Ƿ��������Ϣ
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)//���
			{
				//rectangle(m.x - 5, m.y - 5, m.x + 5, m.y + 5);
				if (m.x > N*SIZE || m.y > N*SIZE)
					continue;
				m.x = m.x / SIZE;
				m.y = m.y / SIZE;
				if (now == 1) {//��һ��ȷ��״ֻ̬Ҫ��Խ�綼����,Ȼ���¼��һ��λ��
					x1 = m.x;
					y1 = m.y;//x1��y1�洢��һ��״̬
					if (!mat[y1][x1])
						continue;//�ո���Ч
					drawRound(x1, y1, kr, 0, 0, GREEN);
					now = 2;
				}
				else {//�ڶ���Ҫ�ж�״̬
					x2 = m.x;
					y2 = m.y;
					if (mat[y1][x1] != mat[y2][x2] || x1 == x2 && y1 == y2) {
						clearRound(x1, y1, kr, 0, 0);//����ѡ��
						now = 1;
						continue;
					}
					if (x1 > 0 && x1 <= N - 2 && y1 > 0 && y1 <= N - 2 && x2 > 0
						&& x2 <= N - 2 && y2 > 0 && y2 <= N - 2) {
						int check = 0;
						dfs(x1, y1, x1, y1, x2, y2, 0, 0, check);
						if (check) {
							deal(x1, x2, y1, y2);//�����
							bool if_over = deal_after(x1, x2, y1, y2);//�ж��Ƿ������ͬʱ�ƺ���
							if (if_over) {
								printOver();//��ӡ�������
								return 1;//ͨ����ǰ�ؿ�
							}
							int check = remind(x1, y1, x2, y2);
							if (!check) {
								refresh();//û�п�����������,ˢ��
							}
							else {
								while (!st.empty())
									st.pop();
							}
						}
						else {
							clearRound(x1, y1, kr, 0, 0);//��������
							while (!st.empty())//���ջ
								st.pop();
						}
					}
					now = 1;
				}
			}
		}
		if (_kbhit()) {//����а���
			int ch = _getch();
			if (ch == KONG) {//��ʾ��ť
				int check = remind(x1,y1,x2,y2);
				if (check) {
					deal(x1, x2, y1, y2);//�����
					show_mat(tmp);
					Sleep(remind_price*1000);//����
					cleardevice();
					show_mat(mat);
				}
			}
			else if (ch == ESC) {
				time0+=stop_game();//Ҫ�Ѽ�ʱ������Ų��
				cleardevice();
				show_mat(mat);
			}
			else if (ch == RIGHT) {//ˢ��
				refresh();
			}
		}
	}
}//��Ϸ��������

/*�ܿس���*/
void game() {
	initgraph(MAXX + 401, MAXY + 1);
	while (1) {
		for (now_level = 5; now_level <= 6; now_level++) {//�߹�
			printNextLevel();//��ӡת������
			all_ini(now_level);//��ʼ�����о����ͼƬ��
			create_mat(all_num);//���ɾ���
			int check = play();//��ʼ��Ϸ��������Ϸ״̬
			if (!check) {
				break;//��Ϸ����
			}
			else if (now_level == 6) {
				prinfAllOk();//��ӡȫ��ͨ��
			}
		}
	}
	//closegraph();//�رմ���
}//�ܿس���
