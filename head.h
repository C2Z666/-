#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<algorithm>
#include<stack>
#include <stdlib.h>
#include <time.h>
#include<graphics.h>//ͼ�λ��ƿ�
#include<string>
#include<iostream>
using namespace std;

const int N = 12 + 2; // ��Ϸ�����������һȦ�Ĵ�С,��һ����������������Ϸ�����С
const int inf = 0x3f3f;
#define SIZE 60 //ͼƬ�ߴ�
#define MAXX N*SIZE
#define MAXY N*SIZE
#define RIGHT 77
#define LEFT 75
#define UP 72
#define DOWN 80
#define ENTER 13
#define W 119
#define A 97
#define S 115
#define D 100
#define KONG 32
#define ESC 27//��ȡ��ֵ
#define EASY 130
#define NORMAL 90
#define DIFFICULT 70
#define DET 3 //�������

void game();//��Ϸ�������
