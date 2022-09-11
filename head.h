#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<algorithm>
#include<stack>
#include <stdlib.h>
#include <time.h>
#include<graphics.h>//图形绘制库
#include<string>
#include<iostream>
using namespace std;

const int N = 12 + 2; // 游戏界面加上隐藏一圈的大小,第一个数字是真正的游戏界面大小
const int inf = 0x3f3f;
#define SIZE 60 //图片尺寸
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
#define ESC 27//获取键值
#define EASY 130
#define NORMAL 90
#define DIFFICULT 70
#define DET 3 //连击间隔

void game();//游戏函数入口
