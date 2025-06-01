#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<easyx.h>
#include <graphics.h>
#include<conio.h>
#include<string>
#include<cstring>
#include <cstdlib>
#include <synchapi.h>
#include <ctime>
#include <sstream>
#include< algorithm >
using namespace std;
void sign();
int user();
int administrator();
void write();
void read();
void Admin();
void drawTable22(int x, int y, int width, int height, int row, int col);
void freeOrders(struct Order* orderList);
struct Flight* findFlightsByOrder(struct Order* orderList);
struct Order* findOrdersByflightName(const char* flightName);
struct Order* findOrdersByCustomerName(const char* customerName);
#define ADMIN_PASSWORD "admin" // 管理员密码
#define ORDER_MAX 300

IMAGE image;
IMAGE ima;
IMAGE ge;
IMAGE mmenu;
IMAGE mm;
//存储登录账号密码
char Buf[22] = { 0 };
char Buff[10] = { 0 };
char password[20] = { 0 };
struct ButtonColor {
	COLORREF defaultColor;	//默认颜色
	COLORREF hoverColor;	//悬停颜色
};
struct ticket {
	char user[20];
	char code[10];
	char name[20];
	char phonenum[15];
	char id[20];
	char text[1024];
	struct ticket* next;
}TK;
struct Flight
{
	char id[20];			// 航班编号
	char start[20]; // 起点城市
	char end[20];	// 终点城市
	char syear[7];		// 起 年
	char smon[5];		// 起  月
	char sday[5];		//  起 日
	char shour[5];		//  起 时
	char smin[5];		//  起 分
	char eyear[7];		// 到 年
	char emon[5];		// 到 月
	char eday[5];		// 到 天
	char ehour[5];		// 到 时
	char emin[5];		// 到 分
	char price[20]; // 价格 
	char flytime[20];//飞行时间
	struct Flight* next;

};

struct Order
{
	char id[20];             // 订单编号
	char flightId[50];  // 航班编号
	char customerId[50]; // 用户真实名字
	char detail[1024];//记录航班起点终点
	struct Order* next;
};


ButtonColor button = { RGB(232, 232, 232), RGB(255, 0, 0) };
struct ticket* head, * tail, * temp;
void Init() {
	head = NULL;
	tail = NULL;
	temp = NULL;
}
void clrscr() {
	cleardevice();
	putimage(0, 0, &image);
}
void init()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN); // 获取屏幕宽度
	int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 获取屏幕高度

	int windowWidth = 700; // 窗口宽度
	int windowHeight = 560; // 窗口高度

	HideCaret(GetHWnd());  // 隐藏控制台窗口的光标

	ShowWindow(GetHWnd(), SW_MINIMIZE);  // 最小化控制台窗口

	int x = (screenWidth - windowWidth) / 2; // 计算窗口左上角x坐标
	int y = (screenHeight - windowHeight) / 2; // 计算窗口左上角y坐标

	initgraph(windowWidth, windowHeight, EX_NOMINIMIZE); // 初始化EasyX窗口
	SetWindowText(GetHWnd(), "airFlight机票管理系统"); // 设置窗口标题
	setbkmode(TRANSPARENT); // 设置透明背景

	HWND hwnd = GetHWnd(); // 获取窗口句柄
	SetWindowPos(hwnd, HWND_TOP, x, y, windowWidth, windowHeight, SWP_SHOWWINDOW); // 设置窗口位置

	loadimage(&image, "./image.png", 700, 560);
	loadimage(&ima, "./ima.png", 700, 560);
	loadimage(&ge, "./ge.jpg", 700, 560);
	loadimage(&mmenu, "./mmenu.jpg", 700, 560);
	loadimage(&mm, "./menu.jpg", 700, 560);
	putimage(0, 0, &image);
}



//身份证号码查验
char funny(int n)  //z值与M值对应
{
	switch (n) {
	case 0: return '1';
	case 1: return '0';
	case 2: return 'X';
	case 3: return '9';
	case 4: return '8';
	case 5: return '7';
	case 6: return '6';
	case 7: return '5';
	case 8: return '4';
	case 9: return '3';
	case 10:return '2';
	}
}
bool isValidDate(const char* year, const char* month, const char* day, const char* place) {
	int pp = atoi(place);
	int yyyy = atoi(year);
	int mm = atoi(month);
	int dd = atoi(day);

	if (pp >= 11 && pp <= 15 || pp >= 21 && pp <= 23 || pp >= 31 && pp <= 37 || pp >= 41 && pp <= 46 || pp >= 50 && pp <= 54 || pp >= 61 && pp <= 65 || pp >= 81 && pp <= 83)
	{
		if (yyyy < 1900 || yyyy > 2100) {
			MessageBox(GetHWnd(), "请输入合法的身份证号，包括合法的出生年月日。", "错误", MB_OK | MB_ICONERROR);
			return false;  // 年份范围不合法
		}

		if (mm < 1 || mm > 12) {
			MessageBox(GetHWnd(), "请输入合法的身份证号，包括合法的出生年月日。", "错误", MB_OK | MB_ICONERROR);
			return false;  // 月份范围不合法
		}

		int daysInMonth;
		if (mm == 2) {
			if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) {
				daysInMonth = 29;  // 闰年2月有29天
			}
			else {
				daysInMonth = 28;  // 平年2月有28天
			}
		}
		else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
			daysInMonth = 30;  // 4月、6月、9月、11月各有30天
		}
		else {
			daysInMonth = 31;  // 其他月份有31天
		}

		if (dd < 1 || dd > daysInMonth) {
			MessageBox(GetHWnd(), "请输入合法的身份证号，包括合法的出生年月日。", "错误", MB_OK | MB_ICONERROR);
			return false;  // 日期范围不合法
		}

		return true;
	}
	else {
		MessageBox(GetHWnd(), "请输入合法的身份证号，包括合理的地区码。", "错误", MB_OK | MB_ICONERROR);
		return false;
	}
}
bool isValidIDNumber(const char* idNumber) {
	if (strlen(idNumber) != 18) {
		MessageBox(GetHWnd(), "请输入18位有效身份证号码。", "错误", MB_OK | MB_ICONERROR);
		return false;
	}

	const char* firstTwoDigits = "11,12,13,14,15,21,22,23,31,32,33,34,35,36,37,41,42,43,44,45,46,50,51,52,53,54,61,62,63,64,65,81,82,83";
	char firstTwo[3];
	strncpy(firstTwo, idNumber, 2);
	firstTwo[2] = '\0';

	if (strstr(firstTwoDigits, firstTwo) == NULL) {
		MessageBox(GetHWnd(), "请输入合法的身份证号，包括合理的地区码。", "错误", MB_OK | MB_ICONERROR);
		return false;
	}

	char* year = (char*)malloc(5);
	strncpy(year, idNumber + 6, 4);
	year[4] = '\0';
	char* month = (char*)malloc(3);
	month[2] = '\0';
	strncpy(month, idNumber + 10, 2);
	char* day = (char*)malloc(3);
	day[2] = '\0';
	strncpy(day, idNumber + 12, 2);

	if (!isValidDate(year, month, day, firstTwo)) {
		free(year);
		free(month);
		free(day);
		return false;
	}

	int sum = (idNumber[0] - '0') * 7 + (idNumber[1] - '0') * 9 + (idNumber[2] - '0') * 10 +
		(idNumber[3] - '0') * 5 + (idNumber[4] - '0') * 8 + (idNumber[5] - '0') * 4 +
		(idNumber[6] - '0') * 2 + (idNumber[7] - '0') * 1 + (idNumber[8] - '0') * 6 +
		(idNumber[9] - '0') * 3 + (idNumber[10] - '0') * 7 + (idNumber[11] - '0') * 9 +
		(idNumber[12] - '0') * 10 + (idNumber[13] - '0') * 5 + (idNumber[14] - '0') * 8 +
		(idNumber[15] - '0') * 4 + (idNumber[16] - '0') * 2;

	int z = sum % 11;
	if (idNumber[17] == funny(z)) {
		free(year);
		free(month);
		free(day);
		return true;
	}
	else {
		MessageBox(GetHWnd(), "请输入合法的身份证号，包括合理的校验位。", "错误", MB_OK | MB_ICONERROR);
		free(year);
		free(month);
		free(day);
		return false;
	}
}
//手机号码查验

bool isValidPhoneNumber(const char* phoneNumber) {
	if (strlen(phoneNumber) != 11) {
		MessageBox(GetHWnd(), "请输入11位有效手机号码。", "错误", MB_OK | MB_ICONERROR);
		return false;
	}

	const char* firstThreeDigits = "130,131,132,133,134,135,136,137,138,139,144,145,146,147,148,149,150,151,152,153,155,156,157,158,159,170,171,172,173,174,175,176,177,178,162,163,165,166,167,180,181,182,183,184,185,186,187,188,189,198,199";
	char* firstThree = (char*)malloc(4); // 改为4个字节的空间
	strncpy(firstThree, phoneNumber, 3);
	firstThree[3] = '\0';  // 顺便添加结束符
	//printf("%s", firstThree);
	if (strstr(firstThreeDigits, firstThree) != NULL) {
		free(firstThree);
		return true;
	}
	else {
		MessageBox(GetHWnd(), "请输入合法的手机号，包括合理的运营商区码。", "错误", MB_OK | MB_ICONERROR);
		free(firstThree);
		return false;
	}
}

//鼠标在按钮上
bool Isin(int l, int o, int r, int u) {
	POINT p = { 0 };
	GetCursorPos(&p);
	ScreenToClient(GetHWnd(), &p);
	if (p.x >= l && p.x <= r && p.y >= o && p.y <= u) {
		return true;
	}
	return false;
}

bool Isclick(int l, int o, int r, int u) {
	if (MouseHit()) {
		const MOUSEMSG m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (m.x >= l && m.x <= r && m.y >= o && m.y <= u) {
				return true;
			}
		}
	}
	return false;
}
//按钮
bool Button(COLORREF hoverColor, COLORREF defaultColor, int l, int o, int r, int u, int w, int h, const char* str) {
	// 检测鼠标是否在按钮区域内
	bool isHover = Isin(l, o, r, u);

	// 根据鼠标状态选择按钮颜色
	COLORREF cor = isHover ? hoverColor : defaultColor;

	// 绘制按钮
	setfillcolor(cor);
	fillroundrect(l, o, r, u, w, h);
	settextcolor(BLACK);
	settextstyle(30, 0, "楷体");
	outtextxy(l + ((r - l) - textwidth(str)) / 2, o + ((u - o) - textheight(str)) / 2, str);

	// 处理按钮点击事件
	if (isHover && Isclick(l, o, r, u)) {
		return true;
	}
	return false;
}

bool adminLogin(const char password[], int n) {
	if (password == NULL || strcmp(password, "") == 0) {
		MessageBox(GetHWnd(), "未输入！", "提示", MB_OK);
		return false;
	}

	// 检查密码是否正确
	if (strcmp(password, ADMIN_PASSWORD) == 0) {
		return true;
	}
	else {
		MessageBox(GetHWnd(), "密码错误！", "提示", MB_OK);
		return false;
	}
}

//判断是否订单重复 
bool isDuplicateOrder(char* orderId) {
	FILE* file = fopen("订单信息.txt", "r");
	if (file == NULL) {
		return false;
	}

	struct Order currentOrder;
	while (fscanf(file, "%s %s %s %s", currentOrder.id, currentOrder.flightId, currentOrder.customerId, currentOrder.detail) == 4) {
		if (strcmp(orderId, currentOrder.id) == 0) {
			fclose(file);
			return true;
		}
	}
	fclose(file);
	return false;
}
//写进订单文本 
void writeOrders(struct Order* orders, int numOrders) {
	FILE* file = fopen("订单信息.txt", "a+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "无法打开文件！", "错误", MB_OK);
		return;
	}

	for (int i = 0; i < numOrders; ++i) {
		if (isDuplicateOrder(orders[i].id)) {
			MessageBox(GetHWnd(), "重复订单，无法追加！", "提示", MB_OK);
			continue;
		}
		fprintf(file, "%s %s %s %s\n", orders[i].id, orders[i].flightId, orders[i].customerId, orders[i].detail);
	}

	fclose(file);
}


// 从文本文件读取订单信息并构建链表
struct Order* readOrdersFromFile() {
	FILE* file = fopen("订单信息.txt", "r+");
	if (file == NULL) {
		return NULL;
	}

	struct Order* hd = NULL;
	struct Order* tl = NULL;
	struct Order* tp;

	while (1) {
		tp = (struct Order*)malloc(sizeof(struct Order));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "内存分配失败！", "提示", MB_OK);
			break;
		}
		if (fscanf(file, "%s %s %s %s", tp->id, tp->flightId, tp->customerId, tp->detail) == 4) {
			tp->next = NULL;
			if (hd == NULL) {
				hd = tp;
			}
			else {
				tl->next = tp;
			}
			tl = tp;
		}
		else {
			free(tp); // 释放由 malloc 分配的内存
			break;
		}
	}

	fclose(file);
	return hd;
}
//判断是否航班重复 
bool isDuplicateFlight(char* flightId) {
	FILE* file = fopen("航班信息.txt", "r");
	if (file == NULL) {
		return false;
	}

	Flight currentFlight;
	while (fscanf(file, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
		currentFlight.id, currentFlight.start, currentFlight.end,
		currentFlight.syear, currentFlight.smon, currentFlight.sday, currentFlight.shour, currentFlight.smin,
		currentFlight.eyear, currentFlight.emon, currentFlight.eday, currentFlight.ehour, currentFlight.emin, currentFlight.price, currentFlight.flytime) == 15) {
		if (strcmp(flightId, currentFlight.id) == 0) {
			fclose(file);
			return true;
		}
	}
	fclose(file);
	return false;
}
//写进航班文本
void writeFlights(Flight* flights, int numFlights) {
	FILE* file = fopen("航班信息.txt", "a+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "无法打开文件！", "错误", MB_OK);
		return;
	}

	for (int i = 0; i < numFlights; ++i) {
		if (isDuplicateFlight(flights[i].id)) {
			MessageBox(GetHWnd(), "重复航班，无法追加！", "提示", MB_OK);
			continue;
		}
		fprintf(file, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",
			flights[i].id, flights[i].start, flights[i].end,
			flights[i].syear, flights[i].smon, flights[i].sday, flights[i].shour, flights[i].smin,
			flights[i].eyear, flights[i].emon, flights[i].eday, flights[i].ehour, flights[i].emin,
			flights[i].price, flights[i].flytime);
	}

	fclose(file);
}

//读取航班文件 
struct Flight* readFlightsFromFile() {
	FILE* file = fopen("航班信息.txt", "r+");
	if (file == NULL) {
		return NULL;
	}

	struct Flight* hd = NULL;
	struct Flight* tl = NULL;
	struct Flight* tp;

	while (1) {
		tp = (struct Flight*)malloc(sizeof(struct Flight));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "内存分配失败！", "提示", MB_OK);
			break;
		}
		if (fscanf(file, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
			tp->id, tp->start, tp->end,
			tp->syear, tp->smon, tp->sday, tp->shour, tp->smin,
			tp->eyear, tp->emon, tp->eday, tp->ehour, tp->emin,
			tp->price, tp->flytime) == 15) {
			tp->next = NULL;
			if (hd == NULL) {
				hd = tp;
			}
			else {
				tl->next = tp;
			}
			tl = tp;
		}
		else {
			free(tp);
			break;
		}
	}

	fclose(file);
	return hd;
}

bool isValidYear(const char* year) {
	int len = strlen(year);
	if (len != 4) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (!isdigit(year[i])) {
			return false;
		}
	}
	int y = atoi(year);
	if (y < 2023 || y > 9999) {
		return false;
	}
	return true;
}
bool isValidMonth(const char* month) {
	int len = strlen(month);
	if (len < 1 || len > 2) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (!isdigit(month[i])) {
			return false;
		}
	}
	int m = atoi(month);
	if (m < 1 || m > 12) {
		return false;
	}
	return true;
}

bool isValidDay(const char* day) {
	int len = strlen(day);
	if (len < 1 || len > 2) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (!isdigit(day[i])) {
			return false;
		}
	}
	int d = atoi(day);
	if (d < 1 || d > 30) {
		return false;
	}
	return true;
}

bool isValidHour(const char* hour) {
	int len = strlen(hour);
	if (len < 1 || len > 2) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (!isdigit(hour[i])) {
			return false;
		}
	}
	int h = atoi(hour);
	if (h < 0 || h > 24) {
		return false;
	}
	return true;
}

bool isValidMinute(const char* minute) {
	int len = strlen(minute);
	if (len < 1 || len > 2) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (!isdigit(minute[i])) {
			return false;
		}
	}
	int m = atoi(minute);
	if (m < 0 || m > 60) {
		return false;
	}
	return true;
}

bool isValidPrice(const char* price) {
	int len = strlen(price);
	if (len == 0) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (!isdigit(price[i])) {
			return false;
		}
	}
	return true;
}

//个人表格
void drawTable()
{
	setlinecolor(GREEN);
	line(80, 100, 620, 100); // 上边框
	line(80, 460, 620, 460); // 下边框
	line(80, 100, 80, 460); // 左边框
	line(620, 100, 620, 460); // 右边框
	line(300, 100, 300, 460); // 竖线
	for (int i = 100; i <= 460; i += 60) {
		line(80, i, 620, i); // 横线
	}
}
// 绘制个人信息
void drawText(const char* text, int x, int y, int row, int column) {
	settextstyle(28, 0, "行书");
	int textWidth = textwidth(text);
	if (column != 1)
	{
		outtextxy((x + 240 - textWidth) / 2 + 20, 10 + y + row * 60, text);
	}
	else {
		outtextxy((x + 240 - textWidth) / 2 + 40, 6 + y + row * 60, text);
	}
}

//数据
//写
void write() {
	char m[20] = { 1 };
	m[19] = 0;
	FILE* fp = fopen("用户信息.txt", "w+");//写读
	temp = head;
	while (temp != NULL) {
		if (strcmp(temp->user, m) == 0 && strcmp(temp->code, m) == 0 && strcmp(temp->name, m) == 0)
		{
			temp = temp->next;
			continue;
		}
		else if (temp->id == NULL && temp->phonenum == NULL)
		{
			temp = temp->next;
			continue;
		}
		else {
			fprintf(fp, "%s %s %s %s %s %s\n", temp->user, temp->code, temp->name, temp->phonenum, temp->id, temp->text);//输入
			temp = temp->next;
		}
	}
	fclose(fp);
}
//读
void read() {
	int flag = 0;
	FILE* fp = fopen("用户信息.txt", "r+");//读写
	if (fp == NULL) {
		return;
	}
	Init();
	while (1) {
		temp = (struct ticket*)malloc(sizeof(struct ticket));
		if (temp != NULL) {
			flag = fscanf(fp, "%s %s %s %s %s %s", temp->user, temp->code, temp->name, temp->phonenum, temp->id, temp->text);
			if (flag == -1) {
				free(temp); // 释放由 malloc 分配的内存
				break;
			}
			else {
				temp->next = NULL;
				if (head == NULL) {
					head = temp;
					tail = temp;
				}
				else {
					tail->next = temp;
					tail = tail->next;
				}
			}
		}
		else {
			MessageBox(GetHWnd(), "内存分配失败！", "提示", MB_OK);
			break;
		}
	}
	fclose(fp);
}

//个人信息
void text()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	read();
	temp = head;
	//读出数据
	while (temp != NULL)
	{
		if (strcmp(Buf, temp->user) == 0 && strcmp(Buff, temp->code) == 0)
		{
			break;
		}
		else {
			temp = temp->next;
		}
	}
	char aa[20] = { 0 };
	char bb[20] = { 0 };
	char cc[15] = { 0 };
	char dd[20] = { 0 };
	char ee[1024] = { 0 };

	strcpy(aa, temp->name);
	strcpy(bb, temp->user);
	strcpy(cc, temp->phonenum);
	strcpy(dd, temp->id);
	if (temp->text == NULL)
	{
		strcpy(ee, "暂无安排");
	}
	else {
		strcpy(ee, temp->text);
	}
	while (1) {
		FlushBatchDraw();
		setcolor(YELLOW);
		settextstyle(70, 0, "华文新魏");
		outtextxy((700 - textwidth("个人信息")) / 2, 30, "个人信息");

		drawTable();

		drawText("用户姓名", 100, 110, 0, 0);
		drawText("用户名", 100, 110, 1, 0);
		drawText("账号状态", 100, 110, 2, 0);
		drawText("手机号码", 100, 110, 3, 0);
		drawText("身份证号码", 100, 110, 4, 0);
		drawText("航班安排", 100, 110, 5, 0);
		drawText(aa, 600, 110, 0, 1);
		drawText(bb, 600, 110, 1, 1);
		drawText("正常", 600, 110, 2, 1);
		drawText(cc, 600, 110, 3, 1);
		drawText(dd, 600, 110, 4, 1);
		drawText(ee, 600, 110, 5, 1);

		// 返回
		if (Button(button.hoverColor, button.defaultColor, 15, 470, 120, 510, 80, 40, "返回"))
		{
			sign();
			break;
		}
	}
}

//修改信息
void changetext()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	setcolor(YELLOW);
	settextstyle(70, 0, "华文新魏");
	outtextxy((700 - textwidth("修改个人信息")) / 2, 30, "修改个人信息");
	settextstyle(50, 0, "行楷");
	outtextxy((700 - textwidth("实名信息：")) / 3, 155, "实名信息：");
	outtextxy((700 - textwidth("改用户名：")) / 3, 225, "改用户名：");
	outtextxy((700 - textwidth("修改密码：")) / 3, 295, "修改密码：");
	outtextxy((700 - textwidth("手机号码：")) / 3, 365, "手机号码：");

	read();
	temp = head;
	//读出数据
	while (temp != NULL)
	{
		if (strcmp(Buf, temp->user) == 0 && strcmp(Buff, temp->code) == 0)
		{
			break;
		}
		else {
			temp = temp->next;
		}
	}
	strcpy(Buf, "");
	strcpy(Buff, "");
	char f[20] = { 0 };
	char ff[10] = { 0 };
	char bufff[20] = { 0 };//name
	char b[15] = { 0 };//phnum
	char u[20] = { 0 };//id
	while (1)
	{
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			strcpy(Buf, temp->user);
			strcpy(Buff, temp->code);
			sign();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 395, 160, 555, 210, 80, 40, bufff))
		{//点击name
			InputBox(bufff, 128, "请输入您的姓名", "姓名", "", 69, 40);
			Sleep(50);
			InputBox(u, 128, "请输入您的身份证号码", "身份证号码", "", 69, 40);
			if (strcmp(bufff, "") != NULL && strcmp(u, "") != NULL)
			{
				strcpy(temp->name, bufff);
				strcpy(temp->id, u);
				strcpy(Buf, temp->user);
				strcpy(Buff, temp->code);
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 230, 555, 280, 80, 40, f))
		{//点击用户
			InputBox(Buf, 128, "请输入您新的用户名", "用户名", "", 69, 40);
			if (strcmp(Buf, "") != NULL)
			{
				strcpy(f, Buf);
				strcpy(temp->user, Buf);
				strcpy(Buff, temp->code);
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 300, 555, 350, 80, 40, ff))
		{//点击密码
			InputBox(Buff, 128, "请输入您新的密码", "密码", "", 69, 40);
			if (strcmp(Buff, "") != NULL)
			{
				strcpy(ff, Buff);
				strcpy(temp->code, Buff);
				strcpy(Buf, temp->user);
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 370, 555, 420, 80, 40, b))
		{//点击手机号码
			InputBox(b, 128, "请输入您新的手机号码", "手机号码", "", 69, 40);
			if (strcmp(b, "") != NULL)
			{
				strcpy(temp->phonenum, b);
				strcpy(Buf, temp->user);
				strcpy(Buff, temp->code);
			}
		}
		if (Button(button.hoverColor, button.defaultColor, 590, 255, 650, 300, 80, 40, "确定"))
		{
			if (strcmp(bufff, temp->name) == 0 || strcmp(b, temp->phonenum) == 0 || strcmp(Buff, temp->code) == 0 || strcmp(Buf, temp->user) == 0)
			{
				write();
				MessageBox(GetHWnd(), "修改成功！", "提示", MB_OK);
				sign();
				break;
			}
			else {
				MessageBox(GetHWnd(), "请按要求输入！", "提示", MB_OK);
				continue;
			}
		}
	}

}

void bubbleSortByFlightTime(struct Flight* head) {
	int swapped;
	struct Flight* ptr1;
	struct Flight* lptr = NULL;

	if (head == NULL)
		return;

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr) {
			float time1 = atof(ptr1->flytime);
			float time2 = atof(ptr1->next->flytime);

			if (time1 > time2) {
				// 交换航班信息节点的内容
				struct Flight tempFlight;
				strcpy(tempFlight.id, ptr1->id);
				strcpy(tempFlight.start, ptr1->start);
				strcpy(tempFlight.end, ptr1->end);
				strcpy(tempFlight.syear, ptr1->syear);
				strcpy(tempFlight.smon, ptr1->smon);
				strcpy(tempFlight.sday, ptr1->sday);
				strcpy(tempFlight.shour, ptr1->shour);
				strcpy(tempFlight.smin, ptr1->smin);
				strcpy(tempFlight.eyear, ptr1->eyear);
				strcpy(tempFlight.emon, ptr1->emon);
				strcpy(tempFlight.eday, ptr1->eday);
				strcpy(tempFlight.ehour, ptr1->ehour);
				strcpy(tempFlight.emin, ptr1->emin);
				strcpy(tempFlight.price, ptr1->price);
				strcpy(tempFlight.flytime, ptr1->flytime);

				strcpy(ptr1->id, ptr1->next->id);
				strcpy(ptr1->start, ptr1->next->start);
				strcpy(ptr1->end, ptr1->next->end);
				strcpy(ptr1->syear, ptr1->next->syear);
				strcpy(ptr1->smon, ptr1->next->smon);
				strcpy(ptr1->sday, ptr1->next->sday);
				strcpy(ptr1->shour, ptr1->next->shour);
				strcpy(ptr1->smin, ptr1->next->smin);
				strcpy(ptr1->eyear, ptr1->next->eyear);
				strcpy(ptr1->emon, ptr1->next->emon);
				strcpy(ptr1->eday, ptr1->next->eday);
				strcpy(ptr1->ehour, ptr1->next->ehour);
				strcpy(ptr1->emin, ptr1->next->emin);
				strcpy(ptr1->price, ptr1->next->price);
				strcpy(ptr1->flytime, ptr1->next->flytime);

				strcpy(ptr1->next->id, tempFlight.id);
				strcpy(ptr1->next->start, tempFlight.start);
				strcpy(ptr1->next->end, tempFlight.end);
				strcpy(ptr1->next->syear, tempFlight.syear);
				strcpy(ptr1->next->smon, tempFlight.smon);
				strcpy(ptr1->next->sday, tempFlight.sday);
				strcpy(ptr1->next->shour, tempFlight.shour);
				strcpy(ptr1->next->smin, tempFlight.smin);
				strcpy(ptr1->next->eyear, tempFlight.eyear);
				strcpy(ptr1->next->emon, tempFlight.emon);
				strcpy(ptr1->next->eday, tempFlight.eday);
				strcpy(ptr1->next->ehour, tempFlight.ehour);
				strcpy(ptr1->next->emin, tempFlight.emin);
				strcpy(ptr1->next->price, tempFlight.price);
				strcpy(ptr1->next->flytime, tempFlight.flytime);

				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);

	while (head != NULL) {
		printf("%s\n", head->flytime);
		head = head->next;
	}
}


// 查询航班信息
void queryFlight() {
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	void bubbleSortByPrice(struct Flight* head);
	void drawTable33(int x, int y, int width, int height, int row, int col);
	setcolor(WHITE);
	settextstyle(40, 0, "楷体");
	outtextxy((700 - textwidth("查询航班信息")) / 2, 50, "查询航班信息");
	char ci[20] = { 0 };
	char bnum[24] = { 0 };
	char Enum[24] = { 0 };
	bool made = false;
	bool make = true;
	while (1) {
		if (make == false) {
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(WHITE);
			settextstyle(40, 0, "楷体");
			outtextxy((700 - textwidth("查询航班信息")) / 2, 50, "查询航班信息");
			make = true;
		}
		FlushBatchDraw();
		// 返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			Admin();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 240, 200, 460, 240, 20, 20, "按航班编号查询")) {

			InputBox(ci, 128, "请输入航班编号", "航班编号", "", 69, 40);

			struct Flight* flights = readFlightsFromFile(); // 从文件中读取航班信息

			bool found = false; // 标记是否找到航班

			struct Flight* curr = flights;
			while (curr != NULL) {
				if (strcmp(curr->id, ci) == 0) {
					cleardevice();
					putimage(0, 0, &ima);
					while (1)
					{
						FlushBatchDraw();
						setcolor(YELLOW);
						settextstyle(40, 0, "华文新魏");
						outtextxy((700 - textwidth("您查询的航班信息为")) / 2, 50, "您查询的航班信息为");
						setcolor(BLACK);
						settextstyle(30, 0, "楷体");
						std::stringstream s1;
						s1 << "航班号：" << curr->id;
						std::stringstream s11;
						s11 << "行程：" << curr->start << " -> " << curr->end;
						std::stringstream s2;
						s2 << "出发时间：" << curr->syear << "年" << curr->smon << "月" << curr->sday << "日" << curr->shour << "时" << curr->smin << "分";
						std::stringstream s21;
						s21 << "到达时间：" << curr->eyear << "年" << curr->emon << "月" << curr->eday << "日" << curr->ehour << "时" << curr->emin << "分";
						std::stringstream s10;
						//转化时间
						int flyt = atoi(curr->flytime);
						int fh = flyt / 60;
						int fm = flyt % 60;
						s10 << "共" << fh << "小时" << fm << "分钟";
						std::stringstream s22;
						s22 << "共花费：￥" << curr->price << "元";
						//FlushBatchDraw();
						outtextxy((700 - textwidth(s1.str().c_str())) / 2, 140, s1.str().c_str());
						outtextxy((700 - textwidth(s11.str().c_str())) / 2, 190, s11.str().c_str());
						outtextxy((700 - textwidth(s2.str().c_str())) / 2, 240, s2.str().c_str());
						outtextxy((700 - textwidth(s21.str().c_str())) / 2, 290, s21.str().c_str());
						outtextxy((700 - textwidth(s10.str().c_str())) / 2, 340, s10.str().c_str());
						outtextxy((700 - textwidth(s22.str().c_str())) / 2, 390, s22.str().c_str());
						if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
						{
							make = false;
							break;
						}
					}

					found = true;
					break;
				}
				curr = curr->next;
			}

			if (!found) {
				MessageBox(GetHWnd(), "未找到该航班！", "提示", MB_OK);
				make = false;
			}

			// 释放内存
			curr = flights;
			while (curr != NULL) {
				struct Flight* temp = curr;
				curr = curr->next;
				free(temp);
			}
		}
		if (Button(button.hoverColor, button.defaultColor, 240, 260, 460, 300, 20, 20, "始终点查找")) {
			InputBox(bnum, 128, "请输入您的出发地", "预定航班号", "", 69, 40);
			InputBox(Enum, 128, "请输入您的目的地", "预定航班号", "", 69, 40);
			if (bnum != NULL && Enum != NULL)
			{
			start:
				cleardevice();
				putimage(0, 0, &ima);
				FlushBatchDraw();
				// 绘制表格
				drawTable33(70, 40, 560, 400, 4, 6);

				outtextxy(100, 80, _T("航班号"));
				outtextxy(190, 80, _T("出发地"));
				outtextxy(290, 80, _T("目的地"));
				outtextxy(370, 80, _T("出发时间"));
				outtextxy(460, 80, _T("飞行时间"));
				outtextxy(550, 80, _T("机票费用"));
				struct Flight* ft = readFlightsFromFile();
				struct Flight* rt = NULL;
				struct Flight* tail = NULL; // 用于记录currt链表的末尾
				bool Find = false;
				int Num = 0;
				while (ft != NULL) {
					if (strcmp(bnum, ft->start) == 0 && strcmp(Enum, ft->end) == 0) {
						Find = true;
						Num++;
						if (rt == NULL) {
							rt = ft; // 如果currt链表为空，则直接将匹配的航班作为链表头
							tail = rt; // 更新链表末尾指针
						}
						else {
							tail->next = ft; // 将匹配的航班接在链表末尾
							tail = ft; // 更新链表末尾指针
						}
					}
					ft = ft->next;
				}
				// 在最后将链表末尾指针指向NULL，表示链表结束
				if (tail != NULL) {
					tail->next = NULL;
				}
				struct Flight* currt = rt;
				if (Find == true)
				{
					//排序 默认票价排序 不默认则是时间
					if (MessageBoxA(GetHWnd(), "默认票价排序 选择否则是飞行时间排序", "提示", MB_YESNO | MB_ICONQUESTION) != IDYES) {
						bubbleSortByFlightTime(currt);
					}
					else {
						bubbleSortByPrice(currt);
					}
					for (int i = 0; i < 3; i++)
					{
						if (i == 0)
						{
							if (currt == NULL)
							{
								break;
							}
							std::stringstream s1;
							s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
							std::stringstream s11;
							s11 << currt->shour << ":" << currt->smin;
							std::stringstream s2;
							//转化时间
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "共" << fh << "小时";
							std::stringstream s22;
							s22 << fm << "分钟";
							std::stringstream s3;
							s3 << currt->price << "元";
							outtextxy(100, 180, currt->id);
							outtextxy(200, 180, currt->start);
							outtextxy(290, 180, currt->end);
							outtextxy(360, 170, s1.str().c_str());
							outtextxy(380, 190, s11.str().c_str());
							outtextxy(465, 170, s2.str().c_str());
							outtextxy(470, 190, s22.str().c_str());
							outtextxy(560, 180, s3.str().c_str());
							currt = currt->next;
						}
						if (i == 1)
						{
							if (currt == NULL)
							{
								break;
							}
							std::stringstream s1;
							s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
							std::stringstream s11;
							s11 << currt->shour << ":" << currt->smin;
							std::stringstream s2;
							//转化时间
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "共" << fh << "小时";
							std::stringstream s22;
							s22 << fm << "分钟";
							std::stringstream s3;
							s3 << currt->price << "元";
							outtextxy(100, 280, currt->id);
							outtextxy(200, 280, currt->start);
							outtextxy(290, 280, currt->end);
							outtextxy(360, 270, s1.str().c_str());
							outtextxy(380, 290, s11.str().c_str());
							outtextxy(465, 270, s2.str().c_str());
							outtextxy(470, 290, s22.str().c_str());
							outtextxy(560, 280, s3.str().c_str());
							currt = currt->next;
						}
						if (i == 2)
						{
							if (currt == NULL)
							{
								break;
							}
							std::stringstream s1;
							s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
							std::stringstream s11;
							s11 << currt->shour << ":" << currt->smin;
							std::stringstream s2;
							//转化时间
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "共" << fh << "小时";
							std::stringstream s22;
							s22 << fm << "分钟";
							std::stringstream s3;
							s3 << currt->price << "元";
							outtextxy(100, 380, currt->id);
							outtextxy(200, 380, currt->start);
							outtextxy(290, 380, currt->end);
							outtextxy(360, 370, s1.str().c_str());
							outtextxy(380, 390, s11.str().c_str());
							outtextxy(465, 370, s2.str().c_str());
							outtextxy(470, 390, s22.str().c_str());
							outtextxy(560, 380, s3.str().c_str());
							currt = currt->next;
						}
					}
					// 添加下一页按钮响应事件
					bool nextPage = false;
					int startRow = 1;
					while (true)
					{
						// TODO：绘制当前页的航班信息

						FlushBatchDraw();
						settextstyle(20, 0, _T("微软雅黑"));
						std::stringstream s3;
						s3 << "第" << startRow << "页";
						outtextxy(560, 490, s3.str().c_str());
						// 返回
						if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
						{
							make = false;
							strcpy(Enum, "");
							strcpy(bnum, "");
							break;
						}
						// 首界面
						if (Button(button.hoverColor, button.defaultColor, 460, 455, 560, 485, 80, 40, "第一页"))
						{
							goto start;
							break;
						}
						if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "下一页"))
						{
							startRow++;
							cleardevice(); // 清空绘图区域
							putimage(0, 0, &ima); // 重新绘制背景图像
							// 绘制表格
							drawTable22(70, 40, 560, 400, 4, 6);
							// 输出表头
							outtextxy(100, 80, "航班号");
							outtextxy(190, 80, "出发地");
							outtextxy(290, 80, "目的地");
							outtextxy(370, 80, "出发时间");
							outtextxy(460, 80, "飞行时间");
							outtextxy(550, 80, "机票费用");
							// 绘制当前页的航班信息
							// TODO：添加航班信息
							for (int i = 0; i < 3; i++)
							{
								if (i == 0)
								{
									if (currt == NULL)
									{
										break;
									}
									std::stringstream s1;
									s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
									std::stringstream s11;
									s11 << currt->shour << ":" << currt->smin;
									std::stringstream s2;
									//转化时间
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "共" << fh << "小时";
									std::stringstream s22;
									s22 << fm << "分钟";
									std::stringstream s3;
									s3 << currt->price << "元";
									outtextxy(100, 180, currt->id);
									outtextxy(200, 180, currt->start);
									outtextxy(290, 180, currt->end);
									outtextxy(360, 170, s1.str().c_str());
									outtextxy(380, 190, s11.str().c_str());
									outtextxy(465, 170, s2.str().c_str());
									outtextxy(470, 190, s22.str().c_str());
									outtextxy(560, 180, s3.str().c_str());
									currt = currt->next;
								}
								if (i == 1)
								{
									if (currt == NULL)
									{
										break;
									}
									std::stringstream s1;
									s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
									std::stringstream s11;
									s11 << currt->shour << ":" << currt->smin;
									std::stringstream s2;
									//转化时间
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "共" << fh << "小时";
									std::stringstream s22;
									s22 << fm << "分钟";
									std::stringstream s3;
									s3 << currt->price << "元";
									outtextxy(100, 280, currt->id);
									outtextxy(200, 280, currt->start);
									outtextxy(290, 280, currt->end);
									outtextxy(360, 270, s1.str().c_str());
									outtextxy(380, 290, s11.str().c_str());
									outtextxy(465, 270, s2.str().c_str());
									outtextxy(470, 290, s22.str().c_str());
									outtextxy(560, 280, s3.str().c_str());
									currt = currt->next;
								}
								if (i == 2)
								{
									if (currt == NULL)
									{
										break;
									}
									std::stringstream s1;
									s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
									std::stringstream s11;
									s11 << currt->shour << ":" << currt->smin;
									std::stringstream s2;
									//转化时间
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "共" << fh << "小时";
									std::stringstream s22;
									s22 << fm << "分钟";
									std::stringstream s3;
									s3 << currt->price << "元";
									outtextxy(100, 380, currt->id);
									outtextxy(200, 380, currt->start);
									outtextxy(290, 380, currt->end);
									outtextxy(360, 370, s1.str().c_str());
									outtextxy(380, 390, s11.str().c_str());
									outtextxy(465, 370, s2.str().c_str());
									outtextxy(470, 390, s22.str().c_str());
									outtextxy(560, 380, s3.str().c_str());
									currt = currt->next;
								}
							}
						}
					}
				}
				else {
					MessageBox(GetHWnd(), "未找到指定航班", "提示", MB_OK);
					make = false;
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "请检查您的输入是否正确！", "提示", MB_OK);
				make = false;
				break;
			}
		}
	}
}
// 取消所有航班并将对应订单及用户的text设置为暂无安排 
void clearAllFlights() {
	FILE* file1 = fopen("航班信息.txt", "w");
	FILE* file2 = fopen("订单信息.txt", "w");
	if (file1 == NULL || file2 == NULL) {
		MessageBox(GetHWnd(), "相关文件打开失败！", "提示", MB_OK);
		return;
	}

	fclose(file1);
	fclose(file2);

	read();
	temp = head;
	while (temp) {
		strcpy(temp->text, "");
		temp = temp->next;
	}
	write();
}

// 根据给定的航班编号删除该航班及后续航班信息
bool deleteFlightById(char* flightId) {
	FILE* file = fopen("航班信息.txt", "r+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "文件打开失败！", "提示", MB_OK);
		return false;
	}

	struct Flight* hd = NULL;
	struct Flight* tl = NULL;
	struct Flight* tp;

	while (1) {
		tp = (struct Flight*)malloc(sizeof(struct Flight));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "内存分配失败！", "提示", MB_OK);
			break;
		}
		if (fscanf(file, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
			tp->id, tp->start, tp->end,
			tp->syear, tp->smon, tp->sday, tp->shour, tp->smin,
			tp->eyear, tp->emon, tp->eday, tp->ehour, tp->emin,
			tp->price, tp->flytime) == 15) {
			if (strcmp(flightId, tp->id) == 0) {
				free(tp);
				continue;
			}
			tp->next = NULL;
			if (hd == NULL) {
				hd = tp;
				tl = tp;
			}
			else {
				tl->next = tp;
				tl = tp;
			}
		}
		else {
			free(tp);
			break;
		}
	}

	fclose(file);
	file = fopen("航班信息.txt", "w");
	while (hd != NULL) {
		fprintf(file, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",
			hd->id, hd->start, hd->end,
			hd->syear, hd->smon, hd->sday, hd->shour, hd->smin,
			hd->eyear, hd->emon, hd->eday, hd->ehour, hd->emin,
			hd->price, hd->flytime);
		tp = hd;
		hd = hd->next;
		free(tp);
	}
	fclose(file);
	return true;
}
//将相关航班编号的订单删除同时也让相关的用户的text也置为空
bool deleteOrderflight(char* flightId) {
	FILE* file = fopen("订单信息.txt", "r+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "订单文件打开失败！", "提示", MB_OK);
		return false;
	}

	struct Order* hd = NULL;
	struct Order* tl = NULL;
	struct Order* prev = NULL;
	struct Order* tp;

	read();
	temp = head;

	while (1) {
		tp = (struct Order*)malloc(sizeof(struct Order));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "内存分配失败！", "提示", MB_OK);
			break;
		}

		if (fscanf(file, "%s %s %s %s", tp->id, tp->flightId, tp->customerId, tp->detail) == 4) {
			if (strcmp(flightId, tp->flightId) == 0) {
				free(tp);
				while (temp) {
					if (strcmp(temp->name, tp->customerId) == 0) {
						struct Order* filteredOrders = findOrdersByCustomerName(tp->customerId);
						if (filteredOrders) {
							while (filteredOrders->next) {
								filteredOrders = filteredOrders->next;
							}
							strcpy(temp->text, filteredOrders->detail);
						}
						else {
							strcpy(temp->text, "");
						}
						strcpy(temp->text, "");
					}
					temp = temp->next;
				}

				continue;
			}

			tp->next = NULL;
			if (hd == NULL) {
				hd = tp;
			}
			else {
				tl->next = tp;
			}
			tl = tp;
		}
		else {
			free(tp);
			break;
		}
	}

	fclose(file);
	write();
	file = fopen("订单信息.txt", "w");
	if (file == NULL) {
		MessageBox(GetHWnd(), "订单文件打开失败！", "提示", MB_OK);
		return false;
	}

	struct Order* current = hd;
	while (current != NULL) {
		fprintf(file, "%s %s %s %s\n", current->id, current->flightId, current->customerId, current->detail);
		struct Order* t = current;
		current = current->next;
		free(t);
	}
	fclose(file);
	return true;
}
/* 取消航班信息 */
void cancelFlight()
{
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(YELLOW);
	settextstyle(55, 0, "行书");
	outtextxy((700 - textwidth("请选择您的下一步操作：")) / 2, 80, "请选择您的下一步操作：");
	char fid[20] = { 0 };
	while (1) {
		// 返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			//Admin();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 235, 211, 460, 260, 20, 20, "  取消所有航班  ")) {
			//将文本中的信息清空 
			clearAllFlights();
			MessageBox(GetHWnd(), "所有航班及所有订单已取消！", "提示", MB_OK);
			if (MessageBoxA(GetHWnd(), "是否返回", "提示", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				//Admin();
				break;
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 235, 282, 460, 330, 20, 20, "取消特定航班")) {
			while (1) {
				InputBox(fid, 180, "请输入您要取消的航班编号", "提示", "", 69, 40);
				if (strcmp(fid, "") == 0) {
					MessageBox(GetHWnd(), "航班编号不能为空！", "错误提示", MB_OK);
					continue;
				}
				break;
			}
			deleteFlightById(fid);
			if (deleteFlightById(fid) && deleteOrderflight(fid)) {
				MessageBox(GetHWnd(), "此航班及对应订单已取消已取消！", "提示", MB_OK);
				if (MessageBoxA(GetHWnd(), "是否返回", "提示", MB_YESNO | MB_ICONQUESTION) == IDYES) {
					//Admin();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "该航班编号对应的航班不存在！", "提示", MB_OK);
				if (MessageBoxA(GetHWnd(), "是否返回", "提示", MB_YESNO | MB_ICONQUESTION) == IDYES) {
					//Admin();
					break;
				}
			}

		}
		FlushBatchDraw();
	}
	Admin();
}

/* 添加客户 */
void addCustomer()
{
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(YELLOW);
	settextstyle(90, 0, "楷体");
	outtextxy((700 - textwidth("添加用户信息")) / 2, (500 - textheight("添加用户信息")) / 2 - 120, "添加用户信息");
	settextstyle(60, 0, "行楷");
	outtextxy((700 - textwidth("用户名:")) / 4 - 56, (500 - textheight("用户名:")) / 2, "用户名:");
	outtextxy((700 - textwidth("密码:")) / 4 - 5, (500 - textheight("密码:")) / 2 + 90, "密码:");

	int rectX = (960 - textwidth("账号")) / 4;
	int rectY = (690 - textheight("账号")) / 2;

	//存储账号密码
	strcpy(Buf, "");
	strcpy(Buff, "");
	char bufff[10] = { 0 };
	char name[20] = { 0 };
	char num[15] = { 0 };
	char id[20] = { 0 };

	read();
	struct ticket* pd;
	pd = head;
	temp = (struct ticket*)malloc(sizeof(struct ticket));
	if (temp == NULL) {
		// 内存分配失败，进行错误处理
	}

	while (true) {
		FlushBatchDraw();
		//返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			Admin();
			break;
		}
		//输入用户名
		if (Button(button.hoverColor, button.defaultColor, 300, 235, 450, 280, 20, 20, Buf))
		{
			InputBox(Buf, 128, "请设置用户名", "用户名", "", 69, 40);
			if (strcmp(Buf, "") == 0) {
				MessageBox(GetHWnd(), "用户名不能为空！", "错误提示", MB_OK);
				continue;
			}

			// 检查重复用户名
			pd = head;
			while (pd != NULL) {
				if (strcmp(Buf, pd->user) == 0) {
					MessageBox(GetHWnd(), "用户名已存在！请重新输入。", "提示", MB_OK);
					break;
				}
				pd = pd->next;
			}
			if (pd != NULL) {
				continue;
			}
		}
		//输入密码
		if (Button(button.hoverColor, button.defaultColor, 300, 320, 450, 365, 20, 20, Buff))
		{
			//点击密码
			InputBox(Buff, 128, "请设置用户密码", "密码", "", 69, 40);
			Sleep(150);
			InputBox(bufff, 128, "请再次输入用户密码", "密码", "", 69, 40);
			Sleep(50);
			if (strcmp(Buff, bufff) != 0)
			{
				MessageBox(GetHWnd(), "两次输入的密码不一样，请重试！", "提示", MB_OKCANCEL);
				continue;
			}
		}
		//进行注册
		if (Button(button.hoverColor, button.defaultColor, 320, 400, 390, 440, 80, 40, "注册"))
		{
			if (strcmp(Buf, "") != 0 && strcmp(Buff, "") != 0)
			{
				if (strcmp(Buff, bufff) == 0)
				{
					if (temp != NULL)
					{
						temp->next = NULL;
						MessageBox(GetHWnd(), "用户密码设置成功", "提示", MB_OKCANCEL);
						InputBox(name, 128, "请输入用户真实姓名", "姓名", "", 69, 40);
						bool isPhoneNumberValid = false;
						while (!isPhoneNumberValid) {
							char numm[15] = { 0 }; // 将num的初始化移到循环内部
							InputBox(numm, 128, "请输入用户电话号码", "电话号码", "", 69, 40);
							if (isValidPhoneNumber(numm) == true) {
								strcpy(num, numm);
								isPhoneNumberValid = true;
							}

						}
						bool inputValid = false;
						while (!inputValid) {
							InputBox(id, 128, "请输入用户身份证号码", "身份证号码", "", 69, 40);
							if (isValidIDNumber(id)) {
								inputValid = true;
							}
						}

						if (strlen(name) == 0 || strlen(num) == 0 || strlen(id) == 0)
						{
							MessageBox(GetHWnd(), "姓名、电话号码或身份证号码为空！请重试", "提示", MB_OKCANCEL);
							continue;
						}
						if (strlen(Buf) < sizeof(temp->user) && strlen(Buff) < sizeof(temp->code))
						{
							strcpy(temp->user, Buf);
							strcpy(temp->code, Buff);
							strcpy(temp->name, name);
							strcpy(temp->phonenum, num);
							strcpy(temp->id, id);
							strcpy(temp->text, "暂无安排");
						}
						else {
							MessageBox(GetHWnd(), "请输入合理的信息！", "提示", MB_OKCANCEL);
							continue;
						}

						if (head == NULL) {
							head = temp;
							tail = temp;
						}
						else {
							tail->next = temp;
							tail = tail->next;
						}
						write();
					}

					MessageBox(GetHWnd(), "用户账号注册成功！", "提示", MB_OKCANCEL);
					cleardevice();
					putimage(0, 0, &ima);
					FlushBatchDraw();

					read();
					temp = head;
					//读出数据
					while (temp != NULL)
					{
						if (strcmp(Buf, temp->user) == 0 && strcmp(Buff, temp->code) == 0)
						{
							break;
						}
						else {
							temp = temp->next;
						}
					}

					char aa[20] = { 0 };
					char bb[20] = { 0 };
					char cc[15] = { 0 };
					char dd[20] = { 0 };
					char ee[1024] = { 0 };

					strcpy(aa, temp->name);
					strcpy(bb, temp->user);
					strcpy(cc, temp->phonenum);
					strcpy(dd, temp->id);
					strcpy(ee, temp->text);

					while (1) {
						FlushBatchDraw();
						setcolor(YELLOW);
						settextstyle(70, 0, "华文新魏");
						outtextxy((700 - textwidth("个人信息")) / 2, 30, "个人信息");

						drawTable();

						drawText("用户姓名", 100, 110, 0, 0);
						drawText("用户名", 100, 110, 1, 0);
						drawText("账号状态", 100, 110, 2, 0);
						drawText("手机号码", 100, 110, 3, 0);
						drawText("身份证号码", 100, 110, 4, 0);
						drawText("航班安排", 100, 110, 5, 0);
						drawText(aa, 600, 110, 0, 1);
						drawText(bb, 600, 110, 1, 1);
						drawText("正常", 600, 110, 2, 1);
						drawText(cc, 600, 110, 3, 1);
						drawText(dd, 600, 110, 4, 1);
						drawText(ee, 600, 110, 5, 1);

						// 返回
						if (Button(button.hoverColor, button.defaultColor, 15, 470, 120, 510, 80, 40, "返回"))
						{
							Admin();
							break;
						}
					}
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "设置的用户账号或密码有误！请重试", "提示", MB_OKCANCEL);
				continue;
			}
		}
	}
}

/* 添加航班信息 */
void addFlight()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	setcolor(BLACK);
	settextstyle(40, 0, "楷体");
	outtextxy(50, 15, "航班编号");
	outtextxy(50, 70, "起点城市");
	outtextxy(50, 125, "终点城市");
	outtextxy(50, 180, "起飞日期");
	outtextxy(330, 180, "年");
	outtextxy(450, 180, "月");
	outtextxy(570, 180, "日");
	outtextxy(50, 235, "起飞时间");
	outtextxy(340, 235, "时");
	outtextxy(500, 235, "分");
	outtextxy(50, 290, "到达日期");
	outtextxy(330, 290, "年");
	outtextxy(450, 290, "月");
	outtextxy(570, 290, "日");
	outtextxy(50, 345, "到达时间");
	outtextxy(340, 345, "时");
	outtextxy(500, 345, "分");
	outtextxy(50, 400, "机票价格");
	FlushBatchDraw();
	Flight flights;
	char sid[20] = { 0 };
	char start[20] = { 0 }, end[20] = { 0 };
	char ssyear[7] = { 0 };
	char ssmon[5] = { 0 };
	char ssday[5] = { 0 };
	char sshour[5] = { 0 };
	char ssmin[5] = { 0 };
	char cty[7] = { 0 }, ctmon[5] = { 0 }, ctd[5] = { 0 };
	char cth[5] = { 0 }, ctmin[5] = { 0 };
	char cprice[20] = { 0 };

	int all_values_present = 0;
	while (all_values_present == 0) {
		// 返回
		if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "返回"))
		{
			Admin();
			break;
		}
		//添加
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "添加"))
		{
			if (strcmp(sid, "") != 0 && strcmp(start, "") != 0 && strcmp(end, "") != 0
				&& strcmp(ssyear, "") != 0 && strcmp(ssmon, "") != 0 && strcmp(ssday, "") != 0 && strcmp(sshour, "") != 0
				&& strcmp(ssmin, "") != 0 && strcmp(cty, "") != 0 && strcmp(ctmon, "") != 0 && strcmp(ctd, "") != 0 && strcmp(cth, "") != 0 && strcmp(ctmin, "") != 0 && strcmp(cprice, "") != 0)
			{
				strcpy(flights.id, sid);
				strcpy(flights.start, start);
				strcpy(flights.end, end);
				strcpy(flights.syear, ssyear);
				strcpy(flights.smon, ssmon);
				strcpy(flights.sday, ssday);
				strcpy(flights.shour, sshour);
				strcpy(flights.smin, ssmin);
				strcpy(flights.eyear, cty);
				strcpy(flights.emon, ctmon);
				strcpy(flights.eday, ctd);
				strcpy(flights.ehour, cth);
				strcpy(flights.emin, ctmin);
				strcpy(flights.price, cprice);
				int startYear = atoi(flights.syear);
				int startMonth = atoi(flights.smon);
				int startDay = atoi(flights.sday);
				int startHour = atoi(flights.shour);
				int startMin = atoi(flights.smin);

				int endYear = atoi(flights.eyear);
				int endMonth = atoi(flights.emon);
				int endDay = atoi(flights.eday);
				int endHour = atoi(flights.ehour);
				int endMin = atoi(flights.emin);
				int startTime = startYear * 365 * 24 * 60 + startMonth * 30 * 24 * 60 + startDay * 24 * 60 + startHour * 60 + startMin;
				int endTime = endYear * 365 * 24 * 60 + endMonth * 30 * 24 * 60 + endDay * 24 * 60 + endHour * 60 + endMin;

				int flyt = endTime - startTime;
				if (flyt <= 0) {
					MessageBox(GetHWnd(), "检查到达时间是否比起飞时间晚！", "提示", MB_OK);
					continue;
				}
				sprintf(flights.flytime, "%d", flyt);
				writeFlights(&flights, 1);
				cleardevice();
				putimage(0, 0, &ima);
				FlushBatchDraw();
				MessageBox(GetHWnd(), "航班添加成功！", "提示", MB_OK);
				while (1) {
					FlushBatchDraw();
					setcolor(YELLOW);
					settextstyle(50, 0, "楷体");
					std::stringstream s1;
					s1 << "航班编号 " << flights.id;
					std::stringstream s2;
					s2 << "始: " << flights.start << " 终：" << flights.end;
					std::stringstream s3;
					s3 << "原价" << flights.price;
					std::stringstream s4;
					s4 << flights.syear << "年" << flights.smon << "月" << flights.sday << "日" << flights.shour << "时" << flights.smin <<
						"分起";
					std::stringstream s5;
					s5 << "到" << flights.eyear << "年" << flights.emon << "月" << flights.eday << "日" << flights.ehour << "时" << flights.emin << "分";
					std::stringstream s6;
					int fh = flyt / 60;
					int fm = flyt % 60;
					s6 << "共" << fh << "小时" << fm << "分钟";

					outtextxy((700 - textwidth(s1.str().c_str())) / 2, 40, s1.str().c_str());
					outtextxy((700 - textwidth(s2.str().c_str())) / 2, 100, s2.str().c_str());
					outtextxy((700 - textwidth(s3.str().c_str())) / 2, 170, s3.str().c_str());
					outtextxy((700 - textwidth(s4.str().c_str())) / 2, 250, s4.str().c_str());
					outtextxy((700 - textwidth(s5.str().c_str())) / 2, 320, s5.str().c_str());
					outtextxy((700 - textwidth(s6.str().c_str())) / 2, 390, s6.str().c_str());
					if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
					{
						all_values_present = 1;
						goto start;
						break;
					}
				}
			}
			else {
				MessageBox(GetHWnd(), "请检查输入！", "提示", MB_OK);
			}
		}
		if (Button(button.hoverColor, button.defaultColor, 260, 15, 460, 55, 20, 20, sid)) {

			InputBox(sid, 128, "请输入航班编号", "提示", "", 69, 40);
			while (1) {
				if (isDuplicateFlight(sid)) {
					MessageBox(GetHWnd(), "航班编号已存在！", "提示", MB_OK);
					strcpy(sid, "");
					break;
				}
				else if (strcmp(sid, "") != 0) {
					break;
				}
				else {
					InputBox(sid, 128, "请输入航班编号", "提示", "", 69, 40);
				}
			}
		}
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 260, 70, 460, 110, 20, 20, start)) {

			InputBox(start, 128, "请输入起点城市(2到10个字符", "提示", "", 69, 40);

			while (1) {
				if (strlen(start) <= 10 && strlen(start) >= 2)
				{

					break;
				}
				else
				{
					InputBox(start, 128, "请输入合法的起点城市(2到10个字符", "提示", "", 69, 40);

				}
			}
		}
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 260, 125, 460, 165, 20, 20, end)) {

			InputBox(end, 128, "请输入终点城市", "提示", "", 69, 40);

			while (1) {
				if (strlen(end) <= 10 && strlen(end) >= 2)
				{

					break;
				}
				else
				{
					InputBox(end, 128, "请输入合法的终点城市(2-10个字符)", "提示", "", 69, 40);
				}
			}
		}

		// 输入出发年的按钮
		if (Button(button.hoverColor, button.defaultColor, 240, 180, 320, 220, 20, 20, ssyear)) {
			InputBox(ssyear, 128, "输入年份（YYYY)", "提示", "", 69, 40);
			while (!isValidYear(ssyear)) {
				InputBox(ssyear, 128, "输入错误。请输入合法格式（YYYY)", "提示", "", 69, 40);
			}
		}
		// 输入出发月的按钮
		if (Button(button.hoverColor, button.defaultColor, 370, 180, 440, 220, 20, 20, ssmon)) {
			InputBox(ssmon, 128, "输入月份", "提示", "", 69, 40);
			while (!isValidMonth(ssmon)) {
				InputBox(ssmon, 128, "输入错误。请输入合法格式（MM)", "提示", "", 69, 40);
			}
		}
		FlushBatchDraw();
		// 输入出发日的按钮
		if (Button(button.hoverColor, button.defaultColor, 500, 180, 570, 220, 20, 20, ssday)) {
			InputBox(ssday, 128, "输入日期", "提示", "", 69, 40);
			while (!isValidDay(ssday)) {
				InputBox(ssday, 128, "输入错误。请输入合法格式（dd)", "提示", "", 69, 40);
			}
		}
		FlushBatchDraw();
		// 输入出发小时的按钮
		if (Button(button.hoverColor, button.defaultColor, 230, 235, 330, 275, 20, 20, sshour)) {
			InputBox(sshour, 128, "请输入起飞时间（HH）", "提示", "", 69, 40);
			while (!isValidHour(sshour)) {
				InputBox(sshour, 128, "输入错误，请输入合法起飞时间（HH）", "提示", "", 69, 40);
			}

		}
		// 输入出发分钟的按钮
		if (Button(button.hoverColor, button.defaultColor, 380, 235, 480, 275, 20, 20, ssmin)) {
			InputBox(ssmin, 128, "请输入起飞时间（MM)", "提示", "", 69, 40);
			while (!isValidMinute(ssmin)) {
				InputBox(ssmin, 128, "输入错误，请输入合法起飞时间（MM)", "提示", "", 69, 40);
			}

		}
		// 输入到达年的按钮
		if (Button(button.hoverColor, button.defaultColor, 240, 290, 320, 330, 20, 20, cty)) {
			InputBox(cty, 128, "输入年份（YYYY)", "提示", "", 69, 40);
			while (!isValidYear(cty)) {
				InputBox(cty, 128, "输入错误。请输入合法格式（YYYY)", "提示", "", 69, 40);
			}
		}
		// 输入到达月的按钮
		if (Button(button.hoverColor, button.defaultColor, 370, 290, 440, 330, 20, 20, ctmon)) {
			InputBox(ctmon, 128, "输入月份", "提示", "", 69, 40);
			while (!isValidMonth(ctmon)) {
				InputBox(ctmon, 128, "输入错误。请输入合法格式（MM)", "提示", "", 69, 40);
			}
		}
		FlushBatchDraw();
		// 输入到达日的按钮
		if (Button(button.hoverColor, button.defaultColor, 500, 290, 570, 330, 20, 20, ctd)) {
			InputBox(ctd, 128, "输入日期", "提示", "", 69, 40);
			while (!isValidDay(ctd)) {
				InputBox(ctd, 128, "输入错误。请输入合法格式（dd)", "提示", "", 69, 40);
			}
		}
		FlushBatchDraw();
		//小时
		if (Button(button.hoverColor, button.defaultColor, 230, 345, 330, 385, 20, 20, cth)) {
			InputBox(cth, 128, "请输入到达时间", "提示", "", 69, 40);
			while (!isValidHour(cth)) {
				InputBox(cth, 128, "输入错误，请输入合法起飞时间（HH）", "提示", "", 69, 40);
			}
		}
		//分钟
		if (Button(button.hoverColor, button.defaultColor, 380, 345, 480, 385, 20, 20, ctmin)) {
			InputBox(ctmin, 128, "请输入到达时间", "提示", "", 69, 40);
			while (!isValidMinute(ctmin)) {
				InputBox(ctmin, 128, "输入错误，请输入合法起飞时间（MM)", "提示", "", 69, 40);
			}
		}
		//价格
		if (Button(button.hoverColor, button.defaultColor, 260, 400, 360, 440, 20, 20, cprice)) {
			InputBox(cprice, 128, "请输入机票价格", "提示", "", 69, 40);
			while (!isValidPrice(cprice)) {
				InputBox(cprice, 128, "请输入规范的机票价格", "提示", "", 69, 40);
			}
		}

		FlushBatchDraw();
	}
start:
	if (all_values_present == true) {
		Admin();
	}

}

//初始界面
int menu()
{
	cleardevice();
	putimage(0, 0, &image);
	FlushBatchDraw();
	settextstyle(66, 0, "行楷");
	outtextxy((700 - textwidth("机票管理系统")) / 2, 110, "机票管理系统");
	int pp = 0;
	while (true)
	{
		FlushBatchDraw();

		if (Button(button.hoverColor, button.defaultColor, 300, 240, 400, 280, 80, 40, "管理员"))
		{
			pp = 1;
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 300, 300, 400, 340, 80, 40, "用户"))
		{
			pp = 2;
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 300, 360, 400, 400, 80, 40, "退出"))
		{
			pp = 3;
			break;
		}
	}
	return pp;
}

//时间
void displayTime()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	// 基于当前系统的当前日期/时间
	time_t now = time(0);
	// 把 now 转换为字符串形式
	char* dt = ctime(&now);
	//把 now 转换为 tm 结构
	tm* gmtm = gmtime(&now);
	dt = asctime(gmtm);
	int year = gmtm->tm_year + 1900;
	int month = gmtm->tm_mon + 1;
	int day = gmtm->tm_mday;
	int hour = gmtm->tm_hour + 8;
	int minute = gmtm->tm_min;
	int second = gmtm->tm_sec;
	std::stringstream s;
	if (hour >= 0 && hour <= 12)
	{
		s << "早安，上午好啊！";
	}
	else if (hour > 12 && hour <= 18)
	{
		s << "Having Good Afternoon！";
	}
	else {
		s << "祝您有个愉快的晚上！";
	}
	std::stringstream ss;
	ss << year << "年" << month << "月" << day << "日";
	std::stringstream Ss;
	Ss << hour << "时" << minute << "分" << second << "秒";
	//cout << "本地日期和时间：" << dt << endl;
	while (1)
	{
		FlushBatchDraw();

		settextstyle(40, 0, "华文新魏");
		settextcolor(GREEN);
		outtextxy((700 - textwidth(s.str().c_str())) / 2, 40, s.str().c_str());
		outtextxy((700 - textwidth("----祝您有个愉快的旅途！")) * 2 / 3 + 66, 440, "----祝您有个愉快的旅途！");
		settextstyle(50, 0, "行楷");
		settextcolor(BLUE);
		outtextxy(0, (500 - textheight("现在是北京时间：")) / 4, "现在是北京时间：");
		settextstyle(44, 0, "隶书");
		settextcolor(YELLOW);
		setlinecolor(BLACK);
		line(180, 200, 520, 200);
		line(180, 200, 180, 400);
		line(180, 400, 520, 400);
		line(520, 200, 520, 400);
		outtextxy((700 - textwidth(ss.str().c_str())) / 2, 230, ss.str().c_str());
		outtextxy((700 - textwidth(Ss.str().c_str())) / 2, 320, Ss.str().c_str());

		//返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			sign();
			break;
		}
	}
}


//用户界面
void sign()
{
	cleardevice();
	putimage(0, 0, &ge);
	FlushBatchDraw();
	read();
	temp = head;
	//读出数据
	while (temp != NULL)
	{
		if (strcmp(Buf, temp->user) == 0 && strcmp(Buff, temp->code) == 0)
		{
			break;
		}
		else {
			temp = temp->next;
		}
	}
	void Mflight();
	void Qflight();
	void fcheck();
	void myflight();
	while (true)
	{
		FlushBatchDraw();
		setcolor(BLACK);
		settextstyle(30, 0, "行楷");
		outtextxy((700 - textwidth(" 请选择您的需求：")) * 2 / 3, 120, "------请选择您的需求：");
		setcolor(YELLOW);
		settextstyle(60, 0, "华文新魏");
		std::stringstream ss;
		char sex = temp->id[16];
		int sexn = sex - '0';
		if (sexn % 2 == 1)
		{
			ss << "尊敬的" << temp->name << "先生" << "欢迎您！";
		}
		else {
			ss << "尊敬的" << temp->name << "女士" << "欢迎您！";
		}

		outtextxy((700 - textwidth(ss.str().c_str())) / 2, 30, ss.str().c_str());
		if (Button(button.hoverColor, button.defaultColor, 160, 190, 280, 230, 20, 20, "预定订单")) {
			//1. 预定订单
			Mflight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 190, 560, 230, 20, 20, "取消订单")) {
			//2. 取消订单
			Qflight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 250, 280, 290, 20, 20, "查询航班")) {
			//3. 查询航班
			fcheck();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 250, 560, 290, 20, 20, "我的订单")) {
			//4. 我的订单
			myflight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 310, 280, 350, 20, 20, "个人信息")) {
			//5. 个人信息
			text();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 310, 560, 350, 20, 20, "修改信息")) {
			//6. 修改信息
			changetext();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 370, 280, 410, 20, 20, "当前时间")) {
			//7. 当前时间
			displayTime();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 370, 560, 410, 20, 20, "退出使用")) {
			//8. 退出管理员模式
			MessageBox(GetHWnd(), "感谢您的本次使用，尊敬的用户！", "提示", MB_OK);
			menu();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 300, 420, 420, 460, 20, 20, "退出系统")) {
			//9. 退出airFlight机票管理系统
			MessageBox(GetHWnd(), "感谢您的使用，下次再见！", "提示", MB_OK);
			return exit(66);
		}
	}

}

//找回密码
void findcode()
{
	void forget();
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(WHITE);
	settextstyle(90, 0, "微软雅黑");
	outtextxy((700 - textwidth("找回密码")) / 2, (500 - textheight("找回密码")) / 2 - 170, "找回密码");
	setcolor(BLACK);
	settextstyle(50, 0, "行楷");
	outtextxy((700 - textwidth("真实姓名：")) / 3, 155, "真实姓名：");
	outtextxy((700 - textwidth("身份证号：")) / 3, 225, "身份证号：");
	outtextxy((700 - textwidth("手机号码：")) / 3, 295, "手机号码：");
	temp = head;
	char bufff[20] = { 0 };
	char f[20] = { 0 };
	char b[15] = { 0 };
	bool wW = false;
	while (1)
	{
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			forget();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 395, 160, 600, 210, 80, 40, bufff))
		{//点击name
			InputBox(bufff, 128, "请输入您的姓名", "姓名", "", 69, 40);
		}
		else if (Button(button.hoverColor, button.defaultColor, 375, 230, 660, 280, 80, 40, f))
		{//点击用户
			InputBox(f, 128, "请输入您的身份证号码", "身份证号码", "", 73, 40);
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 300, 600, 350, 80, 40, b))
		{//点击密码
			InputBox(b, 128, "请输入您新的手机号码", "手机号码", "", 69, 40);
		}
		if (Button(button.hoverColor, button.defaultColor, 310, 385, 385, 430, 80, 40, "确定"))
		{
			while (temp != NULL)
			{
				if (strcmp(bufff, temp->name) == 0 && strcmp(b, temp->phonenum) == 0 && strcmp(f, temp->id) == 0)
				{
					std::stringstream s;
					s << "验证成功！您的用户名为“ " << temp->user << "”";
					strcpy(Buff, "");
					MessageBox(GetHWnd(), s.str().c_str(), "提示", MB_OK);
					InputBox(Buff, 128, "请输入您新的密码", "密码", "", 69, 40);
					strcpy(temp->code, Buff);
					strcpy(Buf, temp->user);
					write();
					MessageBox(GetHWnd(), "修改成功！", "提示", MB_OK);
					wW = true;
					break;
				}
				else {
					temp = temp->next;
				}
			}
			if (strcmp(bufff, "") == 0 || strcmp(b, "") == 0 || strcmp(f, "") == 0)
			{
				MessageBox(GetHWnd(), "请按要求输入！", "提示", MB_OK);
				continue;
			}
			if (wW == true)
			{
				sign();
				break;
			}
		}
	}
}
//根据航班编号查找到的一个航班
struct Flight* findFlightById(struct Flight* flights, char* flightId) {
	struct Flight* current = flights;
	while (current != NULL) {
		if (strcmp(current->id, flightId) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
// 生成随机数字字符数组，并指定长度
char* generateRandomArray(int length) {
	char* arr = new char[length + 1];  // 需要额外一个位置存储字符串结束符 '\0'
	srand(time(NULL));  // 使用时间作为随机种子

	for (int i = 0; i < length; i++) {
		arr[i] = '0' + rand() % 10;  // 生成 0 到 9 之间的随机数字字符
	}

	arr[length] = '\0';  // 添加字符串结束符
	return arr;
}
// 给定指定的用户名返回用户的真实姓名
char* getRealName(char* username) {
	struct ticket* current = head;
	while (current != NULL) {
		if (strcmp(current->user, username) == 0) {
			printf("%s", current->name);
			return current->name;
		}
		current = current->next;
	}
	return NULL; // 如果找不到匹配的用户名，则返回NULL
}
//指定地点航班表格
void drawTable33(int x, int y, int width, int height, int row, int col)
{
	// 设置表格边框宽度
	int borderWidth = 2;
	// 计算每个单元格宽度和高度
	int cellWidth = (width - (col + 1) * borderWidth) / col;
	int cellHeight = (height - (row + 2) * borderWidth) / row;

	// 绘制表格的纵向和横向线条
	setlinecolor(BLACK);
	for (int i = 0; i <= row + 1; i++)
	{
		int posY = y + i * cellHeight + (i + 1) * borderWidth;
		line(x, posY, x + width, posY);
	}
	for (int i = 0; i <= col; i++)
	{
		int posX = x + i * cellWidth + (i + 1) * borderWidth;
		line(posX, y, posX, y + height);
	}

	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("微软雅黑"));

}
// 对航班信息按照价格进行冒泡排序
void bubbleSortByPrice(struct Flight* head) {
	int swapped;
	struct Flight* ptr1;
	struct Flight* lptr = NULL;

	if (head == NULL)
		return;

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr) {
			// 将价格字符串转换为浮点数进行比较
			float price1 = atof(ptr1->price);
			float price2 = atof(ptr1->next->price);
			if (price1 > price2) {

				struct Flight tempFlight;
				strcpy(tempFlight.id, ptr1->id);
				strcpy(tempFlight.start, ptr1->start);
				strcpy(tempFlight.end, ptr1->end);
				strcpy(tempFlight.syear, ptr1->syear);
				strcpy(tempFlight.smon, ptr1->smon);
				strcpy(tempFlight.sday, ptr1->sday);
				strcpy(tempFlight.shour, ptr1->shour);
				strcpy(tempFlight.smin, ptr1->smin);
				strcpy(tempFlight.eyear, ptr1->eyear);
				strcpy(tempFlight.emon, ptr1->emon);
				strcpy(tempFlight.eday, ptr1->eday);
				strcpy(tempFlight.ehour, ptr1->ehour);
				strcpy(tempFlight.emin, ptr1->emin);
				strcpy(tempFlight.price, ptr1->price);
				strcpy(tempFlight.flytime, ptr1->flytime);

				strcpy(ptr1->id, ptr1->next->id);
				strcpy(ptr1->start, ptr1->next->start);
				strcpy(ptr1->end, ptr1->next->end);
				strcpy(ptr1->syear, ptr1->next->syear);
				strcpy(ptr1->smon, ptr1->next->smon);
				strcpy(ptr1->sday, ptr1->next->sday);
				strcpy(ptr1->shour, ptr1->next->shour);
				strcpy(ptr1->smin, ptr1->next->smin);
				strcpy(ptr1->eyear, ptr1->next->eyear);
				strcpy(ptr1->emon, ptr1->next->emon);
				strcpy(ptr1->eday, ptr1->next->eday);
				strcpy(ptr1->ehour, ptr1->next->ehour);
				strcpy(ptr1->emin, ptr1->next->emin);
				strcpy(ptr1->price, ptr1->next->price);
				strcpy(ptr1->flytime, ptr1->next->flytime);

				strcpy(ptr1->next->id, tempFlight.id);
				strcpy(ptr1->next->start, tempFlight.start);
				strcpy(ptr1->next->end, tempFlight.end);
				strcpy(ptr1->next->syear, tempFlight.syear);
				strcpy(ptr1->next->smon, tempFlight.smon);
				strcpy(ptr1->next->sday, tempFlight.sday);
				strcpy(ptr1->next->shour, tempFlight.shour);
				strcpy(ptr1->next->smin, tempFlight.smin);
				strcpy(ptr1->next->eyear, tempFlight.eyear);
				strcpy(ptr1->next->emon, tempFlight.emon);
				strcpy(ptr1->next->eday, tempFlight.eday);
				strcpy(ptr1->next->ehour, tempFlight.ehour);
				strcpy(ptr1->next->emin, tempFlight.emin);
				strcpy(ptr1->next->price, tempFlight.price);
				strcpy(ptr1->next->flytime, tempFlight.flytime);

				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);

	// 打印排序后的航班价格
	while (head != NULL) {
		printf("%s\n", head->price);
		head = head->next;
	}
}
//预定航班
void Mflight()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	read();
	temp = head;
	//读出数据
	while (temp != NULL)
	{
		if (strcmp(Buf, temp->user) == 0 && strcmp(Buff, temp->code) == 0)
		{
			break;
		}
		else {
			temp = temp->next;
		}
	}
	setcolor(WHITE);
	settextstyle(60, 0, "隶书");
	outtextxy((700 - textwidth("欢迎您选择我们航班！")) / 2, (500 - textheight("欢迎您选择我们航班！")) / 2 - 170, "欢迎您选择我们航班！");
	setcolor(BLACK);
	settextstyle(36, 0, "行楷");
	outtextxy((700 - textwidth("请在下方输入您要预定的航班号:")) / 4 - 44, (500 - textheight("请在下方输入您要预定的航班号:")) / 2 - 60, "请在下方输入您要预定的航班号:");
	char fnum[20] = { 0 };
	char bnum[24] = { 0 };
	char Enum[24] = { 0 };
	int length = 4;  // 数组长度
	char* randomArray = generateRandomArray(4);

	bool made = false;
	bool make = true;
	while (true) {
		if (make == false)
		{
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(WHITE);
			settextstyle(60, 0, "隶书");
			outtextxy((700 - textwidth("欢迎您选择我们航班！")) / 2, (500 - textheight("欢迎您选择我们航班！")) / 2 - 170, "欢迎您选择我们航班！");
			setcolor(BLACK);
			settextstyle(36, 0, "行楷");
			outtextxy((700 - textwidth("请在下方输入您要预定的航班号:")) / 4 - 44, (500 - textheight("请在下方输入您要预定的航班号:")) / 2 - 60, "请在下方输入您要预定的航班号:");
			make = true;
		}
		FlushBatchDraw();
		//返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			sign();
			break;
		}
		//输入航班号
		if (Button(button.hoverColor, button.defaultColor, 260, 235, 480, 280, 80, 40, fnum))
		{
			InputBox(fnum, 128, "请输入您要预定的航班号", "预定航班号", "", 69, 40);
			struct Flight* ft = readFlightsFromFile();
			struct Flight* flight = findFlightById(ft, fnum);
			//获取用户名 设置全局变量获取登录的用户名 Buf[22]
			struct Order* od = (struct Order*)malloc(sizeof(struct Order));
			char* n = getRealName(Buf);

			if (flight != NULL) {
				//time_t startTime = time(0);
				cleardevice();
				putimage(0, 0, &ima);
				int flyt = atoi(flight->flytime);
				int fh = flyt / 60;
				int fm = flyt % 60;
				while (1) {
					FlushBatchDraw();
					setcolor(WHITE);
					settextstyle(40, 0, "行书");
					outtextxy((700 - textwidth("预定的航班信息")) / 2, (500 - textheight("预定的航班信息")) / 2 - 200, "预定的航班信息");
					std::stringstream s1;
					s1 << "航班编号 " << flight->id;
					std::stringstream s2;
					s2 << "始: " << flight->start << " 终：" << flight->end;
					std::stringstream s3;
					s3 << "原价：￥" << flight->price << "元";
					std::stringstream s4;
					s4 << flight->syear << "年" << flight->smon << "月" << flight->sday << "日" << flight->shour << "时" << flight->smin <<
						"分起";
					std::stringstream s5;
					s5 << "到" << flight->eyear << "年" << flight->emon << "月" << flight->eday << "日" << flight->ehour << "时" << flight->smin <<
						"分";
					std::stringstream s6;
					s6 << "共" << fh << "小时" << fm << "分钟";
					settextstyle(36, 0, "隶书");
					setcolor(BLACK);
					outtextxy((700 - textwidth(s1.str().c_str())) / 2, 140, s1.str().c_str());
					outtextxy((700 - textwidth(s2.str().c_str())) / 2, 190, s2.str().c_str());
					outtextxy((700 - textwidth(s3.str().c_str())) / 2, 240, s3.str().c_str());
					outtextxy((700 - textwidth(s4.str().c_str())) / 2, 290, s4.str().c_str());
					outtextxy((700 - textwidth(s5.str().c_str())) / 2, 340, s5.str().c_str());
					outtextxy((700 - textwidth(s6.str().c_str())) / 2, 390, s6.str().c_str());
					//返回
					if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
					{
						strcpy(fnum, "");
						make = false;
						break;
					}
					//预定
					if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "预定"))
					{
						made = true;
						// 初始化结构体成员变量
						strcpy(od->flightId, "");
						strcpy(od->id, "");
						strcpy(od->customerId, "");
						strcpy(od->flightId, flight->id);
						strcpy(od->id, randomArray);
						strcpy(od->customerId, n);
						std::stringstream sSs;
						sSs << flight->start << "->" << flight->end;
						strcpy(temp->text, sSs.str().c_str());
						strcpy(od->detail, sSs.str().c_str());
						writeOrders(od, 1);
						write();
						MessageBox(GetHWnd(), "预定该航班成功！", "提示", MB_OK);
						break;
					}

				}

			}
			else {
				MessageBox(GetHWnd(), "未找到指定航班", "提示", MB_OK);
			}
			if (made == true)
			{
				sign();
				break;
			}
		}
		//输入目的地
		if (Button(button.hoverColor, button.defaultColor, 260, 300, 480, 350, 80, 40, "始终点查找"))
		{
			InputBox(bnum, 128, "请输入您的出发地", "预定航班号", "", 69, 40);
			InputBox(Enum, 128, "请输入您的目的地", "预定航班号", "", 69, 40);
			if (bnum != NULL && Enum != NULL)
			{
			start:
				cleardevice();
				putimage(0, 0, &ima);
				FlushBatchDraw();
				// 绘制表格
				drawTable33(70, 40, 560, 400, 4, 6);

				outtextxy(100, 80, _T("航班号"));
				outtextxy(190, 80, _T("出发地"));
				outtextxy(290, 80, _T("目的地"));
				outtextxy(370, 80, _T("出发时间"));
				outtextxy(460, 80, _T("飞行时间"));
				outtextxy(550, 80, _T("机票费用"));
				struct Flight* ft = readFlightsFromFile();
				struct Flight* rt = NULL;
				struct Flight* tail = NULL; // 用于记录currt链表的末尾
				bool Find = false;
				int Num = 0;
				while (ft != NULL) {
					if (strcmp(bnum, ft->start) == 0 && strcmp(Enum, ft->end) == 0) {
						Find = true;
						Num++;
						if (rt == NULL) {
							rt = ft; // 如果currt链表为空，则直接将匹配的航班作为链表头
							tail = rt; // 更新链表末尾指针
						}
						else {
							tail->next = ft; // 将匹配的航班接在链表末尾
							tail = ft; // 更新链表末尾指针
						}
					}
					ft = ft->next;
				}
				// 在最后将链表末尾指针指向NULL，表示链表结束
				if (tail != NULL) {
					tail->next = NULL;
				}
				struct Flight* currt = rt;
				if (Find == true)
				{
					//排序 默认票价排序 不默认则是时间
					if (MessageBoxA(GetHWnd(), "默认票价排序 选择否则是飞行时间排序", "提示", MB_YESNO | MB_ICONQUESTION) != IDYES) {
						bubbleSortByFlightTime(currt);
					}
					else {
						bubbleSortByPrice(currt);
					}
					for (int i = 0; i < 3; i++)
					{
						if (i == 0)
						{
							if (currt == NULL)
							{
								break;
							}
							std::stringstream s1;
							s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
							std::stringstream s11;
							s11 << currt->shour << ":" << currt->smin;
							std::stringstream s2;
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "共" << fh << "小时";
							std::stringstream s22;
							s22 << fm << "分钟";
							std::stringstream s3;
							s3 << currt->price << "元";
							outtextxy(100, 180, currt->id);
							outtextxy(200, 180, currt->start);
							outtextxy(290, 180, currt->end);
							outtextxy(360, 170, s1.str().c_str());
							outtextxy(380, 190, s11.str().c_str());
							outtextxy(465, 170, s2.str().c_str());
							outtextxy(470, 190, s22.str().c_str());
							outtextxy(560, 180, s3.str().c_str());
							currt = currt->next;
						}
						if (i == 1)
						{
							if (currt == NULL)
							{
								break;
							}
							std::stringstream s1;
							s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
							std::stringstream s11;
							s11 << currt->shour << ":" << currt->smin;
							std::stringstream s2;
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "共" << fh << "小时";
							std::stringstream s22;
							s22 << fm << "分钟";
							std::stringstream s3;
							s3 << currt->price << "元";
							outtextxy(100, 280, currt->id);
							outtextxy(200, 280, currt->start);
							outtextxy(290, 280, currt->end);
							outtextxy(360, 270, s1.str().c_str());
							outtextxy(380, 290, s11.str().c_str());
							outtextxy(465, 270, s2.str().c_str());
							outtextxy(470, 290, s22.str().c_str());
							outtextxy(560, 280, s3.str().c_str());
							currt = currt->next;
						}
						if (i == 2)
						{
							if (currt == NULL)
							{
								break;
							}
							std::stringstream s1;
							s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
							std::stringstream s11;
							s11 << currt->shour << ":" << currt->smin;
							std::stringstream s2;
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "共" << fh << "小时";
							std::stringstream s22;
							s22 << fm << "分钟";
							std::stringstream s3;
							s3 << currt->price << "元";
							outtextxy(100, 380, currt->id);
							outtextxy(200, 380, currt->start);
							outtextxy(290, 380, currt->end);
							outtextxy(360, 370, s1.str().c_str());
							outtextxy(380, 390, s11.str().c_str());
							outtextxy(465, 370, s2.str().c_str());
							outtextxy(460, 390, s22.str().c_str());
							outtextxy(560, 380, s3.str().c_str());
							currt = currt->next;
						}
					}
					// 添加下一页按钮响应事件
					bool nextPage = false;
					int startRow = 1;
					while (true)
					{
						// TODO：绘制当前页的航班信息

						FlushBatchDraw();
						settextstyle(20, 0, _T("微软雅黑"));
						std::stringstream s3;
						s3 << "第" << startRow << "页";
						outtextxy(560, 490, s3.str().c_str());
						// 返回
						if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
						{
							make = false;
							strcpy(Enum, "");
							strcpy(bnum, "");
							break;
						}
						// 首界面
						if (Button(button.hoverColor, button.defaultColor, 460, 455, 560, 485, 80, 40, "第一页"))
						{
							goto start;
							break;
						}
						if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "下一页"))
						{
							startRow++;
							cleardevice(); // 清空绘图区域
							putimage(0, 0, &ima); // 重新绘制背景图像
							// 绘制表格
							drawTable22(70, 40, 560, 400, 4, 6);
							// 输出表头
							outtextxy(100, 80, "航班号");
							outtextxy(190, 80, "出发地");
							outtextxy(290, 80, "目的地");
							outtextxy(370, 80, "出发时间");
							outtextxy(460, 80, "飞行时间");
							outtextxy(550, 80, "机票费用");
							// 绘制当前页的航班信息
							// TODO：添加航班信息
							for (int i = 0; i < 3; i++)
							{
								if (i == 0)
								{
									if (currt == NULL)
									{
										break;
									}
									std::stringstream s1;
									s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
									std::stringstream s11;
									s11 << currt->shour << ":" << currt->smin;
									std::stringstream s2;
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "共" << fh << "小时";
									std::stringstream s22;
									s22 << fm << "分钟";
									std::stringstream s3;
									s3 << currt->price << "元";
									outtextxy(100, 180, currt->id);
									outtextxy(200, 180, currt->start);
									outtextxy(290, 180, currt->end);
									outtextxy(360, 170, s1.str().c_str());
									outtextxy(380, 190, s11.str().c_str());
									outtextxy(465, 170, s2.str().c_str());
									outtextxy(470, 190, s22.str().c_str());
									outtextxy(560, 180, s3.str().c_str());
									currt = currt->next;
								}
								if (i == 1)
								{
									if (currt == NULL)
									{
										break;
									}
									std::stringstream s1;
									s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
									std::stringstream s11;
									s11 << currt->shour << ":" << currt->smin;
									std::stringstream s2;
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "共" << fh << "小时";
									std::stringstream s22;
									s22 << fm << "分钟";
									std::stringstream s3;
									s3 << currt->price << "元";
									outtextxy(100, 280, currt->id);
									outtextxy(200, 280, currt->start);
									outtextxy(290, 280, currt->end);
									outtextxy(360, 270, s1.str().c_str());
									outtextxy(380, 290, s11.str().c_str());
									outtextxy(465, 270, s2.str().c_str());
									outtextxy(470, 290, s22.str().c_str());
									outtextxy(560, 280, s3.str().c_str());
									currt = currt->next;
								}
								if (i == 2)
								{
									if (currt == NULL)
									{
										break;
									}
									std::stringstream s1;
									s1 << currt->syear << "-" << currt->smon << "-" << currt->sday;
									std::stringstream s11;
									s11 << currt->shour << ":" << currt->smin;
									std::stringstream s2;
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "共" << fh << "小时";
									std::stringstream s22;
									s22 << fm << "分钟";
									std::stringstream s3;
									s3 << currt->price << "元";
									outtextxy(100, 380, currt->id);
									outtextxy(200, 380, currt->start);
									outtextxy(290, 380, currt->end);
									outtextxy(360, 370, s1.str().c_str());
									outtextxy(380, 390, s11.str().c_str());
									outtextxy(465, 370, s2.str().c_str());
									outtextxy(470, 390, s22.str().c_str());
									outtextxy(560, 380, s3.str().c_str());
									currt = currt->next;
								}
							}
						}
					}
				}
				else {
					MessageBox(GetHWnd(), "未找到指定航班", "提示", MB_OK);
					Mflight();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "请检查您的输入是否正确！", "提示", MB_OK);
				Mflight();
				break;
			}
		}
	}
}
//取消订单
bool deleteOrderById(char* flightId, char* customerName) {
	FILE* file = fopen("订单信息.txt", "r+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "文件打开失败！", "提示", MB_OK);
		return false;
	}

	struct Order* hd = NULL;
	struct Order* tl = NULL;
	struct Order* prev = NULL;
	struct Order* tp;
	bool found = false; // 添加一个变量用于跟踪是否找到匹配的订单

	while (1) {
		tp = (struct Order*)malloc(sizeof(struct Order));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "内存分配失败！", "提示", MB_OK);
			break;
		}

		if (fscanf(file, "%s %s %s %s", tp->id, tp->flightId, tp->customerId, tp->detail) == 4) {
			// 判断航班号和姓名是否匹配
			if (strcmp(flightId, tp->flightId) == 0 && strcmp(customerName, tp->customerId) == 0)
			{
				found = true; // 找到了匹配的订单
				free(tp);
				continue;
			}

			tp->next = NULL;
			if (hd == NULL) {
				hd = tp;
			}
			else {
				tl->next = tp;
			}
			tl = tp;
		}
		else {
			free(tp);
			break;
		}
	}

	fclose(file);
	file = fopen("订单信息.txt", "w");
	if (file == NULL) {
		MessageBox(GetHWnd(), "文件打开失败！", "提示", MB_OK);
		return false;
	}

	struct Order* current = hd;
	while (current != NULL) {
		fprintf(file, "%s %s %s %s\n", current->id, current->flightId, current->customerId, current->detail);
		struct Order* temp = current;
		current = current->next;
		free(temp);
	}
	fclose(file);

	if (found) {
		return true;
	}
	else {
		return false; // 如果没有找到匹配的订单则返回false
	}
}


//取消航班
void Qflight()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	read();
	temp = head;
	//读出数据
	while (temp != NULL)
	{
		if (strcmp(Buf, temp->user) == 0 && strcmp(Buff, temp->code) == 0)
		{
			break;
		}
		else {
			temp = temp->next;
		}
	}
	setcolor(WHITE);
	settextstyle(60, 0, "隶书");
	outtextxy((700 - textwidth("欢迎您选择我们航班！")) / 2, (500 - textheight("欢迎您选择我们航班！")) / 2 - 170, "欢迎您选择我们航班！");
	setcolor(BLACK);
	settextstyle(36, 0, "行楷");
	outtextxy((700 - textwidth("请在下方输入您要取消的航班号:")) / 4 - 44, (500 - textheight("请在下方输入您要取消的航班号:")) / 2 - 60, "请在下方输入您要取消的航班号:");
	char fnum[20] = { 0 };
	char* n = getRealName(Buf);
	while (true) {
		FlushBatchDraw();
		//返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			sign();
			break;
		}
		//输入航班号
		if (Button(button.hoverColor, button.defaultColor, 260, 235, 480, 280, 80, 40, fnum))
		{
			InputBox(fnum, 180, "请输入您要取消的航班编号", "提示", "", 69, 40);
			if (deleteOrderById(fnum, n)) {
				MessageBox(GetHWnd(), "订单取消成功！", "提示", MB_OK);
				struct Order* filteredOrders = findOrdersByCustomerName(n);
				if (filteredOrders) {
					while (filteredOrders->next) {
						filteredOrders = filteredOrders->next;
					}
					strcpy(temp->text, filteredOrders->detail);
				}
				else {
					strcpy(temp->text, "暂无安排");
				}

				write();
				sign();
				break;
			}
			else {
				MessageBox(GetHWnd(), "该航班编号对应的订单不存在！", "提示", MB_OK);
				sign();
				break;

			}

		}
	}
}
//查看航班
void drawTable22(int x, int y, int width, int height, int row, int col)
{
	// 设置表格边框宽度
	int borderWidth = 2;
	// 计算每个单元格宽度和高度
	int cellWidth = (width - (col + 1) * borderWidth) / col;
	int cellHeight = (height - (row + 2) * borderWidth) / row;

	// 绘制表格的纵向和横向线条
	setlinecolor(BLACK);
	for (int i = 0; i <= row + 1; i++)
	{
		int posY = y + i * cellHeight + (i + 1) * borderWidth;
		line(x, posY, x + width, posY);
	}
	for (int i = 0; i <= col; i++)
	{
		int posX = x + i * cellWidth + (i + 1) * borderWidth;
		line(posX, y, posX, y + height);
	}

	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("微软雅黑"));

}
void fcheck()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	// 绘制表格
	drawTable22(70, 40, 560, 400, 4, 6);

	outtextxy(100, 80, _T("航班号"));
	outtextxy(190, 80, _T("出发地"));
	outtextxy(290, 80, _T("目的地"));
	outtextxy(370, 80, _T("出发时间"));
	outtextxy(460, 80, _T("飞行时间"));
	outtextxy(550, 80, _T("机票费用"));

	// TODO：添加航班信息
	struct Flight* flights = readFlightsFromFile(); // 从文件中读取航班信息
	struct Flight* curr = flights;
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			if (curr == NULL)
			{
				break;
			}
			std::stringstream s1;
			s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
			std::stringstream s11;
			s11 << curr->shour << ":" << curr->smin;
			std::stringstream s2;
			int flyt = atoi(curr->flytime);
			int fh = flyt / 60;
			int fm = flyt % 60;
			s2 << "共" << fh << "小时";
			std::stringstream s22;
			s22 << fm << "分钟";
			std::stringstream s3;
			s3 << curr->price << "元";
			outtextxy(100, 180, curr->id);
			outtextxy(200, 180, curr->start);
			outtextxy(290, 180, curr->end);
			outtextxy(360, 170, s1.str().c_str());
			outtextxy(380, 190, s11.str().c_str());
			outtextxy(465, 170, s2.str().c_str());
			outtextxy(470, 190, s22.str().c_str());
			outtextxy(560, 180, s3.str().c_str());
			curr = curr->next;
		}
		if (i == 1)
		{
			if (curr == NULL)
			{
				break;
			}
			std::stringstream s1;
			s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
			std::stringstream s11;
			s11 << curr->shour << ":" << curr->smin;
			std::stringstream s2;
			int flyt = atoi(curr->flytime);
			int fh = flyt / 60;
			int fm = flyt % 60;
			s2 << "共" << fh << "小时";
			std::stringstream s22;
			s22 << fm << "分钟";
			std::stringstream s3;
			s3 << curr->price << "元";
			outtextxy(100, 280, curr->id);
			outtextxy(200, 280, curr->start);
			outtextxy(290, 280, curr->end);
			outtextxy(360, 270, s1.str().c_str());
			outtextxy(380, 290, s11.str().c_str());
			outtextxy(465, 270, s2.str().c_str());
			outtextxy(470, 290, s22.str().c_str());
			outtextxy(560, 280, s3.str().c_str());
			curr = curr->next;
		}
		if (i == 2)
		{
			if (curr == NULL)
			{
				break;
			}
			std::stringstream s1;
			s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
			std::stringstream s11;
			s11 << curr->shour << ":" << curr->smin;
			std::stringstream s2;
			int flyt = atoi(curr->flytime);
			int fh = flyt / 60;
			int fm = flyt % 60;
			s2 << "共" << fh << "小时";
			std::stringstream s22;
			s22 << fm << "分钟";
			std::stringstream s3;
			s3 << curr->price << "元";
			outtextxy(100, 380, curr->id);
			outtextxy(200, 380, curr->start);
			outtextxy(290, 380, curr->end);
			outtextxy(360, 370, s1.str().c_str());
			outtextxy(380, 390, s11.str().c_str());
			outtextxy(465, 370, s2.str().c_str());
			outtextxy(470, 390, s22.str().c_str());
			outtextxy(560, 380, s3.str().c_str());
			curr = curr->next;
		}
	}
	// 添加下一页按钮响应事件
	bool nextPage = false;
	int startRow = 1;
	while (true)
	{
		// TODO：绘制当前页的航班信息

		FlushBatchDraw();
		settextstyle(20, 0, _T("微软雅黑"));
		std::stringstream s3;
		s3 << "第" << startRow << "页";
		outtextxy(560, 490, s3.str().c_str());
		// 返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			sign();
			break;
		}
		// 首界面
		if (Button(button.hoverColor, button.defaultColor, 460, 455, 560, 485, 80, 40, "第一页"))
		{
			fcheck();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 40, "下一页"))
		{
			startRow++;
			cleardevice(); // 清空绘图区域
			putimage(0, 0, &ima); // 重新绘制背景图像
			// 绘制表格
			drawTable22(70, 40, 560, 400, 4, 6);
			// 输出表头
			outtextxy(100, 80, "航班号");
			outtextxy(190, 80, "出发地");
			outtextxy(290, 80, "目的地");
			outtextxy(370, 80, "出发时间");
			outtextxy(460, 80, "飞行时间");
			outtextxy(550, 80, "机票费用");
			// 绘制当前页的航班信息
			// TODO：添加航班信息
			for (int i = 0; i < 3; i++)
			{
				if (i == 0)
				{
					if (curr == NULL)
					{
						break;
					}
					std::stringstream s1;
					s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
					std::stringstream s11;
					s11 << curr->shour << ":" << curr->smin;
					std::stringstream s2;
					int flyt = atoi(curr->flytime);
					int fh = flyt / 60;
					int fm = flyt % 60;
					s2 << "共" << fh << "小时";
					std::stringstream s22;
					s22 << fm << "分钟";
					std::stringstream s3;
					s3 << curr->price << "元";
					outtextxy(100, 180, curr->id);
					outtextxy(200, 180, curr->start);
					outtextxy(290, 180, curr->end);
					outtextxy(360, 170, s1.str().c_str());
					outtextxy(380, 190, s11.str().c_str());
					outtextxy(465, 170, s2.str().c_str());
					outtextxy(470, 190, s22.str().c_str());
					outtextxy(560, 180, s3.str().c_str());
					curr = curr->next;
				}
				if (i == 1)
				{
					if (curr == NULL)
					{
						break;
					}
					std::stringstream s1;
					s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
					std::stringstream s11;
					s11 << curr->shour << ":" << curr->smin;
					std::stringstream s2;
					int flyt = atoi(curr->flytime);
					int fh = flyt / 60;
					int fm = flyt % 60;
					s2 << "共" << fh << "小时";
					std::stringstream s22;
					s22 << fm << "分钟";
					std::stringstream s3;
					s3 << curr->price << "元";
					outtextxy(100, 280, curr->id);
					outtextxy(200, 280, curr->start);
					outtextxy(290, 280, curr->end);
					outtextxy(360, 270, s1.str().c_str());
					outtextxy(380, 290, s11.str().c_str());
					outtextxy(465, 270, s2.str().c_str());
					outtextxy(470, 290, s22.str().c_str());
					outtextxy(560, 280, s3.str().c_str());
					curr = curr->next;
				}
				if (i == 2)
				{
					if (curr == NULL)
					{
						break;
					}
					std::stringstream s1;
					s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
					std::stringstream s11;
					s11 << curr->shour << ":" << curr->smin;
					std::stringstream s2;
					int flyt = atoi(curr->flytime);
					int fh = flyt / 60;
					int fm = flyt % 60;
					s2 << "共" << fh << "小时";
					std::stringstream s22;
					s22 << fm << "分钟";
					std::stringstream s3;
					s3 << curr->price << "元";
					outtextxy(100, 380, curr->id);
					outtextxy(200, 380, curr->start);
					outtextxy(290, 380, curr->end);
					outtextxy(360, 370, s1.str().c_str());
					outtextxy(380, 390, s11.str().c_str());
					outtextxy(465, 370, s2.str().c_str());
					outtextxy(470, 390, s22.str().c_str());
					outtextxy(560, 380, s3.str().c_str());
					curr = curr->next;
				}
			}
		}
	}
}

//获取订单链表，并从与这些订单中出现的航班id相对应的文本文件中搜索航班信息。
struct Flight* findFlightsByOrder(struct Order* orderList) {
	struct Flight* flightsHead = readFlightsFromFile();
	struct Flight* resultHead = NULL;
	struct Flight* resultTail = NULL;

	struct Order* currentOrder = orderList;

	while (currentOrder != NULL) {
		struct Flight* currentFlight = flightsHead;

		while (currentFlight != NULL) {
			if (strcmp(currentFlight->id, currentOrder->flightId) == 0) {
				struct Flight* newFlight = (struct Flight*)malloc(sizeof(struct Flight));
				if (newFlight == NULL) {
					MessageBox(GetHWnd(), "内存分配失败！", "提示", MB_OK);
					return NULL;
				}
				memcpy(newFlight, currentFlight, sizeof(struct Flight));
				newFlight->next = NULL;

				if (resultHead == NULL) {
					resultHead = newFlight;
				}
				else {
					resultTail->next = newFlight;
				}

				resultTail = newFlight;
				break;
			}
			currentFlight = currentFlight->next;
		}

		currentOrder = currentOrder->next;
	}

	return resultHead;
}
void myflight()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	// 绘制表格
	drawTable22(100, 40, 500, 400, 4, 5);

	// 设置表头文字
	settextstyle(20, 0, _T("微软雅黑"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(130, 80, _T("出发地"));
	outtextxy(230, 80, _T("目的地"));
	outtextxy(320, 80, _T("出发时间"));
	outtextxy(420, 80, _T("飞行时间"));
	outtextxy(520, 80, _T("机票费用"));

	outtextxy(130, 280, _T("出发地"));
	outtextxy(230, 280, _T("目的地"));
	outtextxy(320, 280, _T("出发时间"));
	outtextxy(420, 280, _T("飞行时间"));
	outtextxy(520, 280, _T("机票费用"));
	//获取到姓名通过Buf找到姓名 
	char* n = getRealName(Buf);
	struct Order* filteredOrders = findOrdersByCustomerName(n);
	struct Flight* curr = findFlightsByOrder(filteredOrders);
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			if (curr == NULL)
			{
				break;
			}
			std::stringstream s1;
			s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
			std::stringstream s11;
			s11 << curr->shour << ":" << curr->smin;
			std::stringstream s2;
			int flyt = atoi(curr->flytime);
			int fh = flyt / 60;
			int fm = flyt % 60;
			s2 << "共" << fh << "小时";
			std::stringstream s22;
			s22 << fm << "分钟";
			std::stringstream s3;
			s3 << curr->price << "元";
			outtextxy(140, 180, curr->start);
			outtextxy(240, 180, curr->end);
			outtextxy(310, 170, s1.str().c_str());
			outtextxy(330, 190, s11.str().c_str());
			outtextxy(415, 170, s2.str().c_str());
			outtextxy(430, 190, s22.str().c_str());
			outtextxy(520, 180, s3.str().c_str());
			curr = curr->next;
		}
		if (i == 1)
		{
			if (curr == NULL)
			{
				break;
			}
			std::stringstream s1;
			s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
			std::stringstream s11;
			s11 << curr->shour << ":" << curr->smin;
			std::stringstream s2;
			int flyt = atoi(curr->flytime);
			int fh = flyt / 60;
			int fm = flyt % 60;
			s2 << "共" << fh << "小时";
			std::stringstream s22;
			s22 << fm << "分钟";
			std::stringstream s3;
			s3 << curr->price << "元";
			outtextxy(140, 380, curr->start);
			outtextxy(240, 380, curr->end);
			outtextxy(310, 370, s1.str().c_str());
			outtextxy(330, 390, s11.str().c_str());
			outtextxy(415, 370, s2.str().c_str());
			outtextxy(430, 390, s22.str().c_str());
			outtextxy(520, 380, s3.str().c_str());
			curr = curr->next;
		}
	}
	// 添加下一页按钮响应事件
	bool nextPage = false;
	int startRow = 1;
	while (true)
	{
		// TODO：绘制当前页的航班信息

		FlushBatchDraw();
		settextstyle(20, 0, _T("微软雅黑"));
		std::stringstream s3;
		s3 << "第" << startRow << "页";
		outtextxy(510, 450, s3.str().c_str());
		// 返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			sign();
			break;
		}

		if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "下一页"))
		{
			startRow++;
			cleardevice(); // 清空绘图区域
			putimage(0, 0, &ima); // 重新绘制背景图像
			// 绘制表格
			drawTable22(100, 40, 500, 400, 4, 5);
			// 输出表头
			// 设置表头文字
			settextstyle(20, 0, _T("微软雅黑"));
			setbkmode(TRANSPARENT);
			settextcolor(BLACK);
			outtextxy(130, 80, _T("出发地"));
			outtextxy(230, 80, _T("目的地"));
			outtextxy(320, 80, _T("出发时间"));
			outtextxy(420, 80, _T("飞行时间"));
			outtextxy(520, 80, _T("机票费用"));

			outtextxy(130, 280, _T("出发地"));
			outtextxy(230, 280, _T("目的地"));
			outtextxy(320, 280, _T("出发时间"));
			outtextxy(420, 280, _T("飞行时间"));
			outtextxy(520, 280, _T("机票费用"));
			for (int i = 0; i < 2; i++)
			{
				if (i == 0)
				{
					if (curr == NULL)
					{
						break;
					}
					std::stringstream s1;
					s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
					std::stringstream s11;
					s11 << curr->shour << ":" << curr->smin;
					std::stringstream s2;
					int flyt = atoi(curr->flytime);
					int fh = flyt / 60;
					int fm = flyt % 60;
					s2 << "共" << fh << "小时";
					std::stringstream s22;
					s22 << fm << "分钟";
					std::stringstream s3;
					s3 << curr->price << "元";
					outtextxy(140, 180, curr->start);
					outtextxy(240, 180, curr->end);
					outtextxy(310, 170, s1.str().c_str());
					outtextxy(330, 190, s11.str().c_str());
					outtextxy(415, 170, s2.str().c_str());
					outtextxy(430, 190, s22.str().c_str());
					outtextxy(520, 180, s3.str().c_str());
					curr = curr->next;
				}
				if (i == 1)
				{
					if (curr == NULL)
					{
						break;
					}
					std::stringstream s1;
					s1 << curr->syear << "-" << curr->smon << "-" << curr->sday;
					std::stringstream s11;
					s11 << curr->shour << ":" << curr->smin;
					std::stringstream s2;
					int flyt = atoi(curr->flytime);
					int fh = flyt / 60;
					int fm = flyt % 60;
					s2 << "共" << fh << "小时";
					std::stringstream s22;
					s22 << fm << "分钟";
					std::stringstream s3;
					s3 << curr->price << "元";
					outtextxy(140, 380, curr->start);
					outtextxy(240, 380, curr->end);
					outtextxy(310, 370, s1.str().c_str());
					outtextxy(330, 390, s11.str().c_str());
					outtextxy(415, 370, s2.str().c_str());
					outtextxy(430, 390, s22.str().c_str());
					outtextxy(520, 380, s3.str().c_str());
					curr = curr->next;
				}
			}
		}
	}
	freeOrders(filteredOrders);
}
//注册与忘记
void forget()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	setcolor(WHITE);
	settextstyle(90, 0, "隶书");
	outtextxy((700 - textwidth("用户账号注册")) / 2, (500 - textheight("用户账号注册")) / 2 - 120, "用户账号注册");
	settextstyle(60, 0, "行楷");
	outtextxy((700 - textwidth("用户名:")) / 4 - 56, (500 - textheight("用户名:")) / 2, "用户名:");
	outtextxy((700 - textwidth("密码:")) / 4 - 5, (500 - textheight("密码:")) / 2 + 90, "密码:");

	int rectX = (960 - textwidth("账号")) / 4;
	int rectY = (690 - textheight("账号")) / 2;

	//存储账号密码
	strcpy(Buf, "");
	strcpy(Buff, "");
	char bufff[10] = { 0 };
	char name[20] = { 0 };
	char num[15] = { 0 };
	char id[20] = { 0 };

	setfillcolor(WHITE);
	fillrectangle((960 - textwidth("忘记密码？")) * 2 / 3 + 105, (690 - textheight("忘记密码？")) / 2 + 200, (960 - textwidth("忘记密码？")) / 2 + textwidth("忘记密码？") + 35, (690 - textheight("忘记密码？注册")) / 2 + 180 + textheight("忘记密码？注册"));
	settextcolor(BLUE);
	settextstyle(34, 0, "行楷");
	outtextxy((960 - textwidth("忘记密码？")) * 2 / 3, (690 - textheight("忘记密码？")) / 2 + 190, "忘记密码？");

	struct ticket* pd;
	pd = head;
	temp = (struct ticket*)malloc(sizeof(struct ticket));
	if (temp == NULL) {
		// 内存分配失败，进行错误处理
	}

	while (true) {
		FlushBatchDraw();
		//返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			user();
			break;
		}
		//输入用户名
		if (Button(button.hoverColor, button.defaultColor, 300, 235, 450, 280, 80, 40, Buf))
		{
			InputBox(Buf, 128, "请输入您的用户名", "用户名", "", 69, 40);
			if (strcmp(Buf, "") == 0) {
				MessageBox(GetHWnd(), "用户名不能为空！", "错误提示", MB_OK);
				continue;
			}

			// 检查重复用户名
			pd = head;
			while (pd != NULL) {
				if (strcmp(Buf, pd->user) == 0) {
					MessageBox(GetHWnd(), "您的用户名已存在！请重新输入。", "提示", MB_OK);
					break;
				}
				pd = pd->next;
			}
			if (pd != NULL) {
				continue;
			}
		}
		//输入密码
		if (Button(button.hoverColor, button.defaultColor, 300, 320, 450, 365, 80, 40, Buff))
		{
			//点击密码
			InputBox(Buff, 128, "请输入您的密码", "密码", "", 69, 40);
			Sleep(150);
			InputBox(bufff, 128, "请再次输入您的密码", "密码", "", 69, 40);
			Sleep(50);
			if (strcmp(Buff, bufff) != 0)
			{
				MessageBox(GetHWnd(), "您两次输入的密码不一样，请重试！", "提示", MB_OKCANCEL);
				continue;
			}
		}
		//进行注册
		if (Button(button.hoverColor, button.defaultColor, 320, 400, 390, 440, 80, 40, "注册"))
		{
			if (strcmp(Buf, "") != 0 && strcmp(Buff, "") != 0)
			{
				if (strcmp(Buff, bufff) == 0)
				{
					if (temp != NULL)
					{
						temp->next = NULL;
						MessageBox(GetHWnd(), "您的密码设置成功", "提示", MB_OKCANCEL);
						InputBox(name, 128, "请输入您的真实姓名", "姓名", "", 69, 40);
						int tryCount = 0;
						bool isPhoneNumberValid = false;
						while (tryCount < 3 && !isPhoneNumberValid) {
							char numm[15] = { 0 }; // 将num的初始化移到循环内部
							InputBox(numm, 128, "请输入您的电话号码", "电话号码", "", 69, 40);
							//printf("%s", numm);
							if (isValidPhoneNumber(numm) == true) {
								strcpy(num, numm);
								isPhoneNumberValid = true;
							}
							else {
								//MessageBox(GetHWnd(), "您的电话号码输入有误！请重试", "提示", MB_OKCANCEL);
								tryCount++;
							}
						}

						if (!isPhoneNumberValid) {
							MessageBox(GetHWnd(), "您输入的电话号码有误，已超过最大尝试次数！", "提示", MB_OK);
							user();
							break;
						}

						int attempts = 0;
						bool inputValid = false;
						while (attempts < 3) {
							InputBox(id, 128, "请输入您的身份证号码", "身份证号码", "", 69, 40);
							if (isValidIDNumber(id)) {
								inputValid = true;
								break;
							}
							else {
								//MessageBox(NULL, "请输入有效的身份证号码", "错误", MB_OK | MB_ICONERROR);
								attempts++;
							}
						}

						if (!inputValid) {
							MessageBox(GetHWnd(), "尝试次数用尽，未能输入正确的身份证号码", "警告", MB_OK | MB_ICONWARNING);
							user();
							break;
						}
						if (strlen(name) == 0 || strlen(num) == 0 || strlen(id) == 0)
						{
							MessageBox(GetHWnd(), "姓名、电话号码或身份证号码为空！请重试", "提示", MB_OKCANCEL);
							continue;
						}
						if (strlen(Buf) < sizeof(temp->user) && strlen(Buff) < sizeof(temp->code))
						{
							strcpy(temp->user, Buf);
							strcpy(temp->code, Buff);
							strcpy(temp->name, name);
							strcpy(temp->phonenum, num);
							strcpy(temp->id, id);
							strcpy(temp->text, "暂无安排");
						}
						else {
							MessageBox(GetHWnd(), "请输入合理的信息！", "提示", MB_OKCANCEL);
							continue;
						}

						if (head == NULL) {
							head = temp;
							tail = temp;
						}
						else {
							tail->next = temp;
							tail = tail->next;
						}
						write();
					}

					MessageBox(GetHWnd(), "您的账号注册成功！", "提示", MB_OKCANCEL);
					sign();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "您输入的账号或密码有误！请重试", "提示", MB_OKCANCEL);
				continue;
			}
		}
		//忘记密码
		else if (Button(button.hoverColor, button.defaultColor, 505, 285, 640, 315, 80, 40, "忘记\n密码"))
		{
			//输入个人信息并核对
			findcode();
			break;
		}
	}
}

// 释放订单链表的内存
void freeOrders(struct Order* orderList) {
	struct Order* currOrder = orderList;
	struct Order* nextOrder;

	while (currOrder != NULL) {
		nextOrder = currOrder->next;
		free(currOrder);
		currOrder = nextOrder;
	}
}

// 根据姓名查找订单并返回包含该姓名的订单链表
struct Order* findOrdersByCustomerName(const char* customerName) {
	FILE* file = fopen("订单信息.txt", "r");
	if (file == NULL) {
		printf("无法打开文件\n");
		return NULL;
	}

	struct Order* resultHead = NULL;
	struct Order* resultTail = NULL;
	struct Order* temp;

	while (1) {
		temp = (struct Order*)malloc(sizeof(struct Order));
		if (temp == NULL) {
			printf("内存分配失败！\n");
			break;
		}

		if (fscanf(file, "%s %s %s %s", temp->id, temp->flightId, temp->customerId, temp->detail) == 4) {
			if (strcmp(temp->customerId, customerName) == 0) {
				temp->next = NULL;

				if (resultHead == NULL) {
					resultHead = temp;
				}
				else {
					resultTail->next = temp;
				}

				resultTail = temp;
			}
			else {
				free(temp);
			}
		}
		else {
			free(temp);
			break;
		}
	}

	fclose(file);
	return resultHead;
}
// 根据航班编号查找订单并返回包含该航班编号的订单链表
struct Order* findOrdersByflightName(const char* flightName) {
	FILE* file = fopen("订单信息.txt", "r");
	if (file == NULL) {
		return NULL;
	}

	struct Order* hd = NULL;
	struct Order* tl = NULL;
	struct Order* tp;

	while (1) {
		tp = (struct Order*)malloc(sizeof(struct Order));
		if (tp == NULL) {
			printf("内存分配失败！\n");
			break;
		}
		if (fscanf(file, "%s %s %s %s", tp->id, tp->flightId, tp->customerId, tp->detail) == 4) {
			if (strcmp(tp->flightId, flightName) == 0) {
				tp->next = NULL;

				if (hd == NULL) {
					hd = tp;
				}
				else {
					tl->next = tp;
				}

				tl = tp;
			}
			else {
				free(tp);
			}
		}
		else {
			free(tp);
			break;
		}

	}

	fclose(file);
	return hd;
}
//管理员 查询订单信息
void queryOrder()
{
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(BLUE);
	settextstyle(20, 0, "楷体");
	FlushBatchDraw();
	while (1) {
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			Admin();
			break;
		}
		//1.查询所有的订单
		else if (Button(button.hoverColor, button.defaultColor, 235, 140, 460, 190, 20, 20, "查询全部订单"))
		{
			struct Order* filteredOrders = readOrdersFromFile();
			struct Order* curr = filteredOrders;

			int startRow = 1;
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(BLUE);
			FlushBatchDraw();
			settextstyle(20, 0, "楷体");

			// 绘制表格
			drawTable22(70, 40, 560, 400, 4, 4);

			outtextxy(109, 80, _T("订单编号"));
			outtextxy(248, 80, _T("航班编号"));
			outtextxy(387, 80, _T("用户姓名"));
			outtextxy(526, 80, _T("航班详情"));
			settextstyle(20, 0, _T("微软雅黑"));
			std::stringstream s3;
			s3 << "第" << startRow << "页";
			outtextxy(510, 450, s3.str().c_str());
			for (int i = 0; i < 3; i++) {
				if (curr == NULL) {
					break;
				}
				outtextxy(109, 179 + i * 99, curr->id);
				outtextxy(248, 179 + i * 99, curr->flightId);
				outtextxy(387, 179 + i * 99, curr->customerId);
				outtextxy(526, 179 + i * 99, curr->detail);
				curr = curr->next;
			}
			while (1) {
				FlushBatchDraw();
				if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回")) {
					cleardevice();
					putimage(0, 0, &mm);
					break;
				}

				if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "下一页")) {
					startRow++;
					cleardevice();
					putimage(0, 0, &ima);
					setcolor(BLUE);
					settextstyle(20, 0, "楷体");
					FlushBatchDraw();
					drawTable22(70, 40, 560, 400, 4, 4);
					outtextxy(109, 80, _T("订单编号"));
					outtextxy(248, 80, _T("航班编号"));
					outtextxy(387, 80, _T("用户姓名"));
					outtextxy(526, 80, _T("航班详情"));
					settextstyle(20, 0, _T("微软雅黑"));
					std::stringstream s3;
					s3 << "第" << startRow << "页";
					outtextxy(510, 450, s3.str().c_str());
					for (int i = 0; i < 3; i++) {
						if (curr == NULL) {
							break;
						}
						outtextxy(109, 179 + i * 99, curr->id);
						outtextxy(248, 179 + i * 99, curr->flightId);
						outtextxy(387, 179 + i * 99, curr->customerId);
						outtextxy(526, 179 + i * 99, curr->detail);
						curr = curr->next;
					}
				}
			}
			freeOrders(filteredOrders);
			continue;
		}

		//2.根据姓名查询订单
		else if (Button(button.hoverColor, button.defaultColor, 235, 210, 460, 260, 20, 20, "姓名查询订单")) {
			char cid[23] = { 0 };
			InputBox(cid, 23, "请输入姓名", "姓名", "", 69, 40);

			struct Order* filteredOrders = findOrdersByCustomerName(cid);
			struct Order* curr = filteredOrders;

			int startRow = 1;
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(BLUE);
			FlushBatchDraw();
			settextstyle(20, 0, "楷体");

			drawTable22(70, 40, 560, 400, 4, 4);
			outtextxy(109, 80, _T("订单编号"));
			outtextxy(248, 80, _T("航班编号"));
			outtextxy(387, 80, _T("用户姓名"));
			outtextxy(526, 80, _T("航班详情"));
			settextstyle(20, 0, _T("微软雅黑"));
			std::stringstream s3;
			s3 << "第" << startRow << "页";
			outtextxy(510, 450, s3.str().c_str());

			for (int i = 0; i < 3; i++) {
				if (curr == NULL) {
					break;
				}

				outtextxy(109, 179 + i * 99, curr->id);
				outtextxy(248, 179 + i * 99, curr->flightId);
				outtextxy(387, 179 + i * 99, curr->customerId);
				outtextxy(526, 179 + i * 99, curr->detail);

				curr = curr->next;
			}
			while (1) {
				FlushBatchDraw();
				if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回")) {
					cleardevice();
					putimage(0, 0, &mm);
					break;
				}

				if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "下一页")) {
					startRow++;
					cleardevice();
					putimage(0, 0, &ima);
					setcolor(BLUE);
					FlushBatchDraw();
					settextstyle(20, 0, "楷体");

					drawTable22(70, 40, 560, 400, 4, 4);
					outtextxy(109, 80, _T("订单编号"));
					outtextxy(248, 80, _T("航班编号"));
					outtextxy(387, 80, _T("用户姓名"));
					outtextxy(526, 80, _T("航班详情"));
					settextstyle(20, 0, _T("微软雅黑"));
					std::stringstream s3;
					s3 << "第" << startRow << "页";
					outtextxy(510, 450, s3.str().c_str());

					for (int i = 0; i < 3; i++) {
						if (curr == NULL) {
							break;
						}

						outtextxy(109, 179 + i * 99, curr->id);
						outtextxy(248, 179 + i * 99, curr->flightId);
						outtextxy(387, 179 + i * 99, curr->customerId);
						outtextxy(526, 179 + i * 99, curr->detail);

						curr = curr->next;
					}
				}
			}

			freeOrders(filteredOrders);
			continue;
		}
		//3.根据航班编号查询订单
		else if (Button(button.hoverColor, button.defaultColor, 235, 280, 460, 330, 20, 20, "航班查询订单")) {
			char fid[23] = { 0 };
			InputBox(fid, 23, "请输入航班编号", "航班编号", "", 69, 40);

			struct Order* filteredOrders = findOrdersByflightName(fid);
			struct Order* curr = filteredOrders;

			int startRow = 1;
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(BLUE);
			FlushBatchDraw();
			settextstyle(20, 0, "楷体");

			drawTable22(70, 40, 560, 400, 4, 4);
			outtextxy(109, 80, _T("订单编号"));
			outtextxy(248, 80, _T("航班编号"));
			outtextxy(387, 80, _T("用户姓名"));
			outtextxy(526, 80, _T("航班详情"));
			settextstyle(20, 0, _T("微软雅黑"));
			std::stringstream s3;
			s3 << "第" << startRow << "页";
			outtextxy(510, 450, s3.str().c_str());

			for (int i = 0; i < 3; i++) {
				if (curr == NULL) {
					break;
				}

				outtextxy(109, 179 + i * 99, curr->id);
				outtextxy(248, 179 + i * 99, curr->flightId);
				outtextxy(387, 179 + i * 99, curr->customerId);
				outtextxy(526, 179 + i * 99, curr->detail);

				curr = curr->next;
			}
			while (1) {
				FlushBatchDraw();
				if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回")) {
					cleardevice();
					putimage(0, 0, &mm);
					break;
				}

				if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "下一页")) {
					startRow++;
					cleardevice();
					putimage(0, 0, &ima);
					setcolor(BLUE);
					settextstyle(20, 0, "楷体");
					FlushBatchDraw();
					drawTable22(70, 40, 560, 400, 4, 4);
					outtextxy(109, 80, _T("订单编号"));
					outtextxy(248, 80, _T("航班编号"));
					outtextxy(387, 80, _T("用户姓名"));
					outtextxy(526, 80, _T("航班详情"));
					settextstyle(20, 0, _T("微软雅黑"));
					std::stringstream s3;
					s3 << "第" << startRow << "页";
					outtextxy(510, 450, s3.str().c_str());

					for (int i = 0; i < 3; i++) {
						if (curr == NULL) {
							break;
						}

						outtextxy(109, 179 + i * 99, curr->id);
						outtextxy(248, 179 + i * 99, curr->flightId);
						outtextxy(387, 179 + i * 99, curr->customerId);
						outtextxy(526, 179 + i * 99, curr->detail);

						curr = curr->next;
					}
				}
			}

			freeOrders(filteredOrders);
			continue;
		}

		FlushBatchDraw();
	}
}

//查询客户信息 
void queryCustomer() {
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(YELLOW);
	settextstyle(80, 0, "楷体");
	outtextxy((700 - textwidth("请输入用户姓名")) / 2, 100, "请输入用户姓名");
	FlushBatchDraw();
	char name[20] = { 0 }, id[20] = { 0 };
	bool make = true;
	while (true) {
		if (make == false)
		{
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(YELLOW);
			settextstyle(80, 0, "楷体");
			outtextxy((700 - textwidth("请输入用户姓名")) / 2, 100, "请输入用户姓名");
			make = true;
		}
		FlushBatchDraw();
		//返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			Admin();
			break;
		}
		//输入用户名
		if (Button(button.hoverColor, button.defaultColor, 300, 235, 450, 280, 20, 20, name))
		{
			InputBox(name, 128, "请设置用户名", "用户名", "", 69, 40);
			if (strcmp(name, "") == 0) {
				MessageBox(GetHWnd(), "用户名不能为空！", "错误提示", MB_OK);
				continue;
			}
			bool inputValid = false;
			while (!inputValid) {
				InputBox(id, 128, "请输入用户身份证号码", "身份证号码", "", 69, 40);
				if (isValidIDNumber(id)) {
					inputValid = true;
					break;
				}
			}
		}
		//查询 
		if (Button(button.hoverColor, button.defaultColor, 320, 320, 425, 360, 80, 40, "查询"))
		{
			if (strcmp(name, "") != 0 && strcmp(id, "") != 0)
			{
				read();

				temp = head;
				bool found = false;
				while (temp != NULL) {
					if (strcmp(name, temp->name) == 0 && strcmp(id, temp->id) == 0) {
						found = true;
						char aa[20] = { 0 };
						char bb[20] = { 0 };
						char cc[15] = { 0 };
						char dd[20] = { 0 };
						char ee[1024] = { 0 };

						strcpy(aa, temp->name);
						strcpy(bb, temp->user);
						strcpy(cc, temp->phonenum);
						strcpy(dd, temp->id);
						if (strcmp(temp->text, "暂无安排") == 0)
						{
							strcpy(ee, "暂无安排");
						}
						else {
							strcpy(ee, temp->text);
						}
						cleardevice();
						putimage(0, 0, &ima);
						while (1) {
							FlushBatchDraw();
							setcolor(WHITE);
							settextstyle(70, 0, "华文新魏");
							outtextxy((700 - textwidth("个人信息")) / 2, 30, "个人信息");
							drawTable();
							drawText("用户姓名", 100, 110, 0, 0);
							drawText("用户名", 100, 110, 1, 0);
							drawText("账号状态", 100, 110, 2, 0);
							drawText("手机号码", 100, 110, 3, 0);
							drawText("身份证号码", 100, 110, 4, 0);
							drawText("航班安排", 100, 110, 5, 0);
							drawText(aa, 600, 110, 0, 1);
							drawText(bb, 600, 110, 1, 1);
							drawText("正常", 600, 110, 2, 1);
							drawText(cc, 600, 110, 3, 1);
							drawText(dd, 600, 110, 4, 1);
							drawText(ee, 600, 110, 5, 1);

							// 返回
							if (Button(button.hoverColor, button.defaultColor, 15, 470, 120, 510, 80, 40, "返回"))
							{
								make = false;
								strcpy(name, "");
								strcpy(id, "");
								break;
							}


						}
						break;
					}
					temp = temp->next;
				}
				if (!found) {
					MessageBox(GetHWnd(), "未找到该客户！", "提示", MB_OK);
					Admin();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "请检查您的输入是否正确！", "提示", MB_OK);
				continue;
			}
		}
	}

}

//取消订单     
void cancelOrder() {
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(BLUE);
	settextstyle(80, 0, "楷体");
	outtextxy((700 - textwidth("取消订单")) / 2, 100, "取消订单");
	FlushBatchDraw();
	read();
	temp = head;
	char fnum[20] = { 0 };
	char cn[20] = { 0 };
	char ccode[10] = { 0 };
	while (true) {
		FlushBatchDraw();
		//返回
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			Admin();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 235, 235, 460, 280, 20, 20, "取消航班订单"))
		{
			InputBox(cn, 180, "请输入您要取消航班订单的客户的姓名", "提示", "", 69, 40);
			InputBox(ccode, 180, "请输入您要取消航班订单的客户的身份证", "提示", "", 69, 40);
			InputBox(fnum, 180, "请输入您要取消订单的航班编号", "提示", "", 69, 40);
			if (deleteOrderById(fnum, cn)) {
				MessageBox(GetHWnd(), "航班取消成功！", "提示", MB_OK);
				//读出数据
				while (temp != NULL)
				{
					if (strcmp(cn, temp->name) == 0 && strcmp(ccode, temp->id) == 0)
					{
						break;
					}
					else {
						temp = temp->next;
					}
				}
				strcpy(temp->text, "");
				write();
				Admin();
				break;
			}
			else {
				MessageBox(GetHWnd(), "该航班编号对应的航班不存在！", "提示", MB_OK);
				Admin();
				break;

			}
		}
	}
}
int user()
{
	cleardevice();
	putimage(0, 0, &mmenu);
	FlushBatchDraw();
	setcolor(BLACK);
	settextstyle(90, 0, "楷体");
	outtextxy((700 - textwidth("用户")) / 2, 60, "用户");

	settextstyle(50, 0, "行楷");
	outtextxy((700 - textwidth("账号:")) / 3, (500 - textheight("账号:")) / 2 - 30, "账号:");
	outtextxy((700 - textwidth("密码:")) / 3, (500 - textheight("密码:")) / 2 + 50, "密码:");

	settextcolor(BLACK);
	settextstyle(30, 0, "华文新魏");
	outtextxy((700 - textwidth("忘记密码？注册")) * 2 / 3 + 105, (500 - textheight("忘记密码？注册")) / 2 + 205, "忘记密码？注册");
	settextcolor(WHITE);
	line((700 - textwidth("忘记密码？注册")) * 2 / 3 + 105, (500 - textheight("忘记密码？注册")) / 2 + 205 + textheight("忘记密码？注册"), (700 - textwidth("忘记密码？注册")) * 2 / 3 + 105 + textwidth("忘记密码？注册"), (500 - textheight("忘记密码？注册")) / 2 + 205 + textheight("忘记密码？注册"));

	read();
	//读出数据
	strcpy(Buf, "");
	strcpy(Buff, "");
	char bug[10] = { 0 };

	bool loggedIn = false;
	while (!loggedIn) {
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			return 0;
		}
		//判断
		if (Button(button.hoverColor, button.defaultColor, 330, 205, 500, 245, 80, 40, Buf))
		{//点击账号
			InputBox(Buf, 128, "请输入您的账号", "登入", "", 69, 40);
		}
		else if (Button(button.hoverColor, button.defaultColor, 330, 285, 500, 325, 80, 40, bug))
		{//点击密码
			InputBox(Buff, 128, "请输入您的密码", "密码", "", 69, 40);
			if (strcmp(Buff, "") != 0)
			{
				strcpy(bug, "******");
			}
		}
		else if (Isin((700 - textwidth("忘记密码？注册")) * 2 / 3 + 105, (500 - textheight("忘记密码？注册")) / 2 + 185 + textheight("忘记密码？注册"), (700 - textwidth("忘记密码？注册")) * 2 / 3 + 105 + textwidth("忘记密码？注册"), (500 - textheight("忘记密码？注册")) / 2 + 205 + textheight("忘记密码？注册")))
		{//点击忘记密码
			settextcolor(RED);
			settextstyle(30, 0, "华文新魏");
			outtextxy((700 - textwidth("忘记密码？注册")) * 2 / 3 + 105, (500 - textheight("忘记密码？注册")) / 2 + 205, "忘记密码？注册");
			if (Isclick((700 - textwidth("忘记密码？注册")) * 2 / 3 + 105, (500 - textheight("忘记密码？注册")) / 2 + 185 + textheight("忘记密码？注册"), (700 - textwidth("忘记密码？注册")) * 2 / 3 + 105 + textwidth("忘记密码？注册"), (500 - textheight("忘记密码？注册")) / 2 + 205 + textheight("忘记密码？注册")))
			{
				forget();
				break;
			}
		}
		else if (!Isin((700 - textwidth("忘记密码？注册")) * 2 / 3 + 105, (500 - textheight("忘记密码？注册")) / 2 + 185 + textheight("忘记密码？注册"), (700 - textwidth("忘记密码？注册")) * 2 / 3 + 105 + textwidth("忘记密码？注册"), (500 - textheight("忘记密码？注册")) / 2 + 205 + textheight("忘记密码？注册")))
		{
			settextcolor(BLACK);
			settextstyle(30, 0, "华文新魏");
			outtextxy((700 - textwidth("忘记密码？注册")) * 2 / 3 + 105, (500 - textheight("忘记密码？注册")) / 2 + 205, "忘记密码？注册");
		}
		if (Button(button.hoverColor, button.defaultColor, 310, 370, 390, 410, 80, 40, "登录"))
		{
			if (Buf != NULL && Buff != NULL)
			{
				//struct ticket* pd = head;
				temp = head;
				while (temp != NULL)
				{
					if (strcmp(Buf, temp->user) == 0 && strcmp(Buff, temp->code) == 0)
					{
						loggedIn = true;
						sign();
						break;
					}
					temp = temp->next;
				}
				if (!loggedIn)
				{
					MessageBox(GetHWnd(), "您的账号密码输入有误", "提示", MB_OK);
					strcpy(Buf, "");
					strcpy(Buff, "");
					strcpy(bug, "");
					continue;
				}
			}
		}

	}
}
//管理员界面
void Admin()
{
	cleardevice();
	putimage(0, 0, &ge);
	//FlushBatchDraw();
	int ff = 0;
	while (true) {
		FlushBatchDraw();
		setcolor(BLACK);
		settextstyle(90, 0, "华文新魏");
		outtextxy((700 - textwidth("欢迎您！管理员")) / 2, 60, "欢迎您！管理员");
		if (Button(button.hoverColor, button.defaultColor, 160, 180, 280, 220, 20, 20, "增加航班")) {
			//1. 增加航班信息（管理员特权）
			addFlight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 180, 560, 220, 20, 20, "取消航班")) {
			//2. 取消航班（管理员特权）
			cancelFlight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 230, 280, 270, 20, 20, "取消订单")) {
			//3. 取消订单
			cancelOrder();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 230, 560, 270, 20, 20, "查询客户")) {
			//4. 查询客户信息
			queryCustomer();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 280, 280, 320, 20, 20, "查询航班")) {
			//5. 查询航班信息
			queryFlight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 280, 560, 320, 20, 20, "查询订单")) {
			//6. 查询订单信息
			queryOrder();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 330, 280, 370, 20, 20, "添加客户")) {
			//7. 添加客户信息
			addCustomer();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 330, 560, 370, 20, 20, "退出管理")) {
			//8. 退出管理员模式
			MessageBox(GetHWnd(), "感谢使用管理员模式，再见！", "提示", MB_OK);
			ff = 1;
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 300, 390, 420, 430, 20, 20, "退出系统")) {
			//9. 退出airFlight机票管理系统
			MessageBox(GetHWnd(), "感谢您的使用，下次再见！", "提示", MB_OK);
			return exit(66);
		}
	}
	if (ff == 1)
	{
		menu();
	}
}
//管理员
int administrator() {

	cleardevice();
	putimage(0, 0, &mm);
	FlushBatchDraw();
	setcolor(BLACK);
	settextstyle(90, 0, "楷体");
	outtextxy((700 - textwidth("管理员")) / 2, 60, "管理员");
	settextstyle(50, 0, "楷体");
	outtextxy((700 - textwidth("密码")) / 3 + 20, 270, "密码：");
	FlushBatchDraw();
	strcpy(password, "");
	char hidden[10] = { 0 };
	int ff = 0;
	bool loggedIn = false;
	while (!loggedIn) {
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "返回"))
		{
			return 0;
		}
		if (Button(button.hoverColor, button.defaultColor, 345, 280, 500, 315, 80, 40, hidden)) {
			// 输入管理员密码
			InputBox(password, 20, "请输入管理员密码：", "登入", "", 69, 40);
			if (strcmp(password, "") != 0)
			{
				strcpy(hidden, "******");
			}

		}
		if (Button(button.hoverColor, button.defaultColor, 305, 360, 390, 400, 80, 40, "登录"))
		{
			if (!adminLogin(password, 20)) {
				memset(hidden, 0, sizeof(hidden));
				strcpy(password, "");
			}
			else {
				loggedIn = true;
				ff = 1;
				break;
			}
		}
	}
	if (ff == 1)
	{
		Admin();
	}
}
int main()
{
	init();
	BeginBatchDraw();
	int pp = 0;
	FlushBatchDraw();
	bool running = true;
	while (running)
	{
		switch (pp)
		{
		case 0:
			pp = menu();
			break;
		case 1:
			// 管理员功能
			pp = administrator();
			pp = 0;
			break;
		case 2:
			// 用户功能
			pp = user();
			pp = 0;
			break;
		case 3:

			clrscr();
			FlushBatchDraw();
			MessageBox(GetHWnd(), "感谢使用airFlight机票管理系统，再见！", "提示", MB_OK);
			//Sleep(500);
			running = false;
			break;
		default:
			break;
		}

		FlushBatchDraw();
		Sleep(100);
	}
	clrscr();
	EndBatchDraw();
	return 0;
}