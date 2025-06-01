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
#define ADMIN_PASSWORD "admin" // ����Ա����
#define ORDER_MAX 300

IMAGE image;
IMAGE ima;
IMAGE ge;
IMAGE mmenu;
IMAGE mm;
//�洢��¼�˺�����
char Buf[22] = { 0 };
char Buff[10] = { 0 };
char password[20] = { 0 };
struct ButtonColor {
	COLORREF defaultColor;	//Ĭ����ɫ
	COLORREF hoverColor;	//��ͣ��ɫ
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
	char id[20];			// ������
	char start[20]; // ������
	char end[20];	// �յ����
	char syear[7];		// �� ��
	char smon[5];		// ��  ��
	char sday[5];		//  �� ��
	char shour[5];		//  �� ʱ
	char smin[5];		//  �� ��
	char eyear[7];		// �� ��
	char emon[5];		// �� ��
	char eday[5];		// �� ��
	char ehour[5];		// �� ʱ
	char emin[5];		// �� ��
	char price[20]; // �۸� 
	char flytime[20];//����ʱ��
	struct Flight* next;

};

struct Order
{
	char id[20];             // �������
	char flightId[50];  // ������
	char customerId[50]; // �û���ʵ����
	char detail[1024];//��¼��������յ�
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
	int screenWidth = GetSystemMetrics(SM_CXSCREEN); // ��ȡ��Ļ���
	int screenHeight = GetSystemMetrics(SM_CYSCREEN); // ��ȡ��Ļ�߶�

	int windowWidth = 700; // ���ڿ��
	int windowHeight = 560; // ���ڸ߶�

	HideCaret(GetHWnd());  // ���ؿ���̨���ڵĹ��

	ShowWindow(GetHWnd(), SW_MINIMIZE);  // ��С������̨����

	int x = (screenWidth - windowWidth) / 2; // ���㴰�����Ͻ�x����
	int y = (screenHeight - windowHeight) / 2; // ���㴰�����Ͻ�y����

	initgraph(windowWidth, windowHeight, EX_NOMINIMIZE); // ��ʼ��EasyX����
	SetWindowText(GetHWnd(), "airFlight��Ʊ����ϵͳ"); // ���ô��ڱ���
	setbkmode(TRANSPARENT); // ����͸������

	HWND hwnd = GetHWnd(); // ��ȡ���ھ��
	SetWindowPos(hwnd, HWND_TOP, x, y, windowWidth, windowHeight, SWP_SHOWWINDOW); // ���ô���λ��

	loadimage(&image, "./image.png", 700, 560);
	loadimage(&ima, "./ima.png", 700, 560);
	loadimage(&ge, "./ge.jpg", 700, 560);
	loadimage(&mmenu, "./mmenu.jpg", 700, 560);
	loadimage(&mm, "./menu.jpg", 700, 560);
	putimage(0, 0, &image);
}



//���֤�������
char funny(int n)  //zֵ��Mֵ��Ӧ
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
			MessageBox(GetHWnd(), "������Ϸ������֤�ţ������Ϸ��ĳ��������ա�", "����", MB_OK | MB_ICONERROR);
			return false;  // ��ݷ�Χ���Ϸ�
		}

		if (mm < 1 || mm > 12) {
			MessageBox(GetHWnd(), "������Ϸ������֤�ţ������Ϸ��ĳ��������ա�", "����", MB_OK | MB_ICONERROR);
			return false;  // �·ݷ�Χ���Ϸ�
		}

		int daysInMonth;
		if (mm == 2) {
			if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) {
				daysInMonth = 29;  // ����2����29��
			}
			else {
				daysInMonth = 28;  // ƽ��2����28��
			}
		}
		else if (mm == 4 || mm == 6 || mm == 9 || mm == 11) {
			daysInMonth = 30;  // 4�¡�6�¡�9�¡�11�¸���30��
		}
		else {
			daysInMonth = 31;  // �����·���31��
		}

		if (dd < 1 || dd > daysInMonth) {
			MessageBox(GetHWnd(), "������Ϸ������֤�ţ������Ϸ��ĳ��������ա�", "����", MB_OK | MB_ICONERROR);
			return false;  // ���ڷ�Χ���Ϸ�
		}

		return true;
	}
	else {
		MessageBox(GetHWnd(), "������Ϸ������֤�ţ���������ĵ����롣", "����", MB_OK | MB_ICONERROR);
		return false;
	}
}
bool isValidIDNumber(const char* idNumber) {
	if (strlen(idNumber) != 18) {
		MessageBox(GetHWnd(), "������18λ��Ч���֤���롣", "����", MB_OK | MB_ICONERROR);
		return false;
	}

	const char* firstTwoDigits = "11,12,13,14,15,21,22,23,31,32,33,34,35,36,37,41,42,43,44,45,46,50,51,52,53,54,61,62,63,64,65,81,82,83";
	char firstTwo[3];
	strncpy(firstTwo, idNumber, 2);
	firstTwo[2] = '\0';

	if (strstr(firstTwoDigits, firstTwo) == NULL) {
		MessageBox(GetHWnd(), "������Ϸ������֤�ţ���������ĵ����롣", "����", MB_OK | MB_ICONERROR);
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
		MessageBox(GetHWnd(), "������Ϸ������֤�ţ����������У��λ��", "����", MB_OK | MB_ICONERROR);
		free(year);
		free(month);
		free(day);
		return false;
	}
}
//�ֻ��������

bool isValidPhoneNumber(const char* phoneNumber) {
	if (strlen(phoneNumber) != 11) {
		MessageBox(GetHWnd(), "������11λ��Ч�ֻ����롣", "����", MB_OK | MB_ICONERROR);
		return false;
	}

	const char* firstThreeDigits = "130,131,132,133,134,135,136,137,138,139,144,145,146,147,148,149,150,151,152,153,155,156,157,158,159,170,171,172,173,174,175,176,177,178,162,163,165,166,167,180,181,182,183,184,185,186,187,188,189,198,199";
	char* firstThree = (char*)malloc(4); // ��Ϊ4���ֽڵĿռ�
	strncpy(firstThree, phoneNumber, 3);
	firstThree[3] = '\0';  // ˳����ӽ�����
	//printf("%s", firstThree);
	if (strstr(firstThreeDigits, firstThree) != NULL) {
		free(firstThree);
		return true;
	}
	else {
		MessageBox(GetHWnd(), "������Ϸ����ֻ��ţ������������Ӫ�����롣", "����", MB_OK | MB_ICONERROR);
		free(firstThree);
		return false;
	}
}

//����ڰ�ť��
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
//��ť
bool Button(COLORREF hoverColor, COLORREF defaultColor, int l, int o, int r, int u, int w, int h, const char* str) {
	// �������Ƿ��ڰ�ť������
	bool isHover = Isin(l, o, r, u);

	// �������״̬ѡ��ť��ɫ
	COLORREF cor = isHover ? hoverColor : defaultColor;

	// ���ư�ť
	setfillcolor(cor);
	fillroundrect(l, o, r, u, w, h);
	settextcolor(BLACK);
	settextstyle(30, 0, "����");
	outtextxy(l + ((r - l) - textwidth(str)) / 2, o + ((u - o) - textheight(str)) / 2, str);

	// ����ť����¼�
	if (isHover && Isclick(l, o, r, u)) {
		return true;
	}
	return false;
}

bool adminLogin(const char password[], int n) {
	if (password == NULL || strcmp(password, "") == 0) {
		MessageBox(GetHWnd(), "δ���룡", "��ʾ", MB_OK);
		return false;
	}

	// ��������Ƿ���ȷ
	if (strcmp(password, ADMIN_PASSWORD) == 0) {
		return true;
	}
	else {
		MessageBox(GetHWnd(), "�������", "��ʾ", MB_OK);
		return false;
	}
}

//�ж��Ƿ񶩵��ظ� 
bool isDuplicateOrder(char* orderId) {
	FILE* file = fopen("������Ϣ.txt", "r");
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
//д�������ı� 
void writeOrders(struct Order* orders, int numOrders) {
	FILE* file = fopen("������Ϣ.txt", "a+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "�޷����ļ���", "����", MB_OK);
		return;
	}

	for (int i = 0; i < numOrders; ++i) {
		if (isDuplicateOrder(orders[i].id)) {
			MessageBox(GetHWnd(), "�ظ��������޷�׷�ӣ�", "��ʾ", MB_OK);
			continue;
		}
		fprintf(file, "%s %s %s %s\n", orders[i].id, orders[i].flightId, orders[i].customerId, orders[i].detail);
	}

	fclose(file);
}


// ���ı��ļ���ȡ������Ϣ����������
struct Order* readOrdersFromFile() {
	FILE* file = fopen("������Ϣ.txt", "r+");
	if (file == NULL) {
		return NULL;
	}

	struct Order* hd = NULL;
	struct Order* tl = NULL;
	struct Order* tp;

	while (1) {
		tp = (struct Order*)malloc(sizeof(struct Order));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "�ڴ����ʧ�ܣ�", "��ʾ", MB_OK);
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
			free(tp); // �ͷ��� malloc ������ڴ�
			break;
		}
	}

	fclose(file);
	return hd;
}
//�ж��Ƿ񺽰��ظ� 
bool isDuplicateFlight(char* flightId) {
	FILE* file = fopen("������Ϣ.txt", "r");
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
//д�������ı�
void writeFlights(Flight* flights, int numFlights) {
	FILE* file = fopen("������Ϣ.txt", "a+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "�޷����ļ���", "����", MB_OK);
		return;
	}

	for (int i = 0; i < numFlights; ++i) {
		if (isDuplicateFlight(flights[i].id)) {
			MessageBox(GetHWnd(), "�ظ����࣬�޷�׷�ӣ�", "��ʾ", MB_OK);
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

//��ȡ�����ļ� 
struct Flight* readFlightsFromFile() {
	FILE* file = fopen("������Ϣ.txt", "r+");
	if (file == NULL) {
		return NULL;
	}

	struct Flight* hd = NULL;
	struct Flight* tl = NULL;
	struct Flight* tp;

	while (1) {
		tp = (struct Flight*)malloc(sizeof(struct Flight));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "�ڴ����ʧ�ܣ�", "��ʾ", MB_OK);
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

//���˱��
void drawTable()
{
	setlinecolor(GREEN);
	line(80, 100, 620, 100); // �ϱ߿�
	line(80, 460, 620, 460); // �±߿�
	line(80, 100, 80, 460); // ��߿�
	line(620, 100, 620, 460); // �ұ߿�
	line(300, 100, 300, 460); // ����
	for (int i = 100; i <= 460; i += 60) {
		line(80, i, 620, i); // ����
	}
}
// ���Ƹ�����Ϣ
void drawText(const char* text, int x, int y, int row, int column) {
	settextstyle(28, 0, "����");
	int textWidth = textwidth(text);
	if (column != 1)
	{
		outtextxy((x + 240 - textWidth) / 2 + 20, 10 + y + row * 60, text);
	}
	else {
		outtextxy((x + 240 - textWidth) / 2 + 40, 6 + y + row * 60, text);
	}
}

//����
//д
void write() {
	char m[20] = { 1 };
	m[19] = 0;
	FILE* fp = fopen("�û���Ϣ.txt", "w+");//д��
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
			fprintf(fp, "%s %s %s %s %s %s\n", temp->user, temp->code, temp->name, temp->phonenum, temp->id, temp->text);//����
			temp = temp->next;
		}
	}
	fclose(fp);
}
//��
void read() {
	int flag = 0;
	FILE* fp = fopen("�û���Ϣ.txt", "r+");//��д
	if (fp == NULL) {
		return;
	}
	Init();
	while (1) {
		temp = (struct ticket*)malloc(sizeof(struct ticket));
		if (temp != NULL) {
			flag = fscanf(fp, "%s %s %s %s %s %s", temp->user, temp->code, temp->name, temp->phonenum, temp->id, temp->text);
			if (flag == -1) {
				free(temp); // �ͷ��� malloc ������ڴ�
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
			MessageBox(GetHWnd(), "�ڴ����ʧ�ܣ�", "��ʾ", MB_OK);
			break;
		}
	}
	fclose(fp);
}

//������Ϣ
void text()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	read();
	temp = head;
	//��������
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
		strcpy(ee, "���ް���");
	}
	else {
		strcpy(ee, temp->text);
	}
	while (1) {
		FlushBatchDraw();
		setcolor(YELLOW);
		settextstyle(70, 0, "������κ");
		outtextxy((700 - textwidth("������Ϣ")) / 2, 30, "������Ϣ");

		drawTable();

		drawText("�û�����", 100, 110, 0, 0);
		drawText("�û���", 100, 110, 1, 0);
		drawText("�˺�״̬", 100, 110, 2, 0);
		drawText("�ֻ�����", 100, 110, 3, 0);
		drawText("���֤����", 100, 110, 4, 0);
		drawText("���ల��", 100, 110, 5, 0);
		drawText(aa, 600, 110, 0, 1);
		drawText(bb, 600, 110, 1, 1);
		drawText("����", 600, 110, 2, 1);
		drawText(cc, 600, 110, 3, 1);
		drawText(dd, 600, 110, 4, 1);
		drawText(ee, 600, 110, 5, 1);

		// ����
		if (Button(button.hoverColor, button.defaultColor, 15, 470, 120, 510, 80, 40, "����"))
		{
			sign();
			break;
		}
	}
}

//�޸���Ϣ
void changetext()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	setcolor(YELLOW);
	settextstyle(70, 0, "������κ");
	outtextxy((700 - textwidth("�޸ĸ�����Ϣ")) / 2, 30, "�޸ĸ�����Ϣ");
	settextstyle(50, 0, "�п�");
	outtextxy((700 - textwidth("ʵ����Ϣ��")) / 3, 155, "ʵ����Ϣ��");
	outtextxy((700 - textwidth("���û�����")) / 3, 225, "���û�����");
	outtextxy((700 - textwidth("�޸����룺")) / 3, 295, "�޸����룺");
	outtextxy((700 - textwidth("�ֻ����룺")) / 3, 365, "�ֻ����룺");

	read();
	temp = head;
	//��������
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
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			strcpy(Buf, temp->user);
			strcpy(Buff, temp->code);
			sign();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 395, 160, 555, 210, 80, 40, bufff))
		{//���name
			InputBox(bufff, 128, "��������������", "����", "", 69, 40);
			Sleep(50);
			InputBox(u, 128, "�������������֤����", "���֤����", "", 69, 40);
			if (strcmp(bufff, "") != NULL && strcmp(u, "") != NULL)
			{
				strcpy(temp->name, bufff);
				strcpy(temp->id, u);
				strcpy(Buf, temp->user);
				strcpy(Buff, temp->code);
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 230, 555, 280, 80, 40, f))
		{//����û�
			InputBox(Buf, 128, "���������µ��û���", "�û���", "", 69, 40);
			if (strcmp(Buf, "") != NULL)
			{
				strcpy(f, Buf);
				strcpy(temp->user, Buf);
				strcpy(Buff, temp->code);
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 300, 555, 350, 80, 40, ff))
		{//�������
			InputBox(Buff, 128, "���������µ�����", "����", "", 69, 40);
			if (strcmp(Buff, "") != NULL)
			{
				strcpy(ff, Buff);
				strcpy(temp->code, Buff);
				strcpy(Buf, temp->user);
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 370, 555, 420, 80, 40, b))
		{//����ֻ�����
			InputBox(b, 128, "���������µ��ֻ�����", "�ֻ�����", "", 69, 40);
			if (strcmp(b, "") != NULL)
			{
				strcpy(temp->phonenum, b);
				strcpy(Buf, temp->user);
				strcpy(Buff, temp->code);
			}
		}
		if (Button(button.hoverColor, button.defaultColor, 590, 255, 650, 300, 80, 40, "ȷ��"))
		{
			if (strcmp(bufff, temp->name) == 0 || strcmp(b, temp->phonenum) == 0 || strcmp(Buff, temp->code) == 0 || strcmp(Buf, temp->user) == 0)
			{
				write();
				MessageBox(GetHWnd(), "�޸ĳɹ���", "��ʾ", MB_OK);
				sign();
				break;
			}
			else {
				MessageBox(GetHWnd(), "�밴Ҫ�����룡", "��ʾ", MB_OK);
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
				// ����������Ϣ�ڵ������
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


// ��ѯ������Ϣ
void queryFlight() {
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	void bubbleSortByPrice(struct Flight* head);
	void drawTable33(int x, int y, int width, int height, int row, int col);
	setcolor(WHITE);
	settextstyle(40, 0, "����");
	outtextxy((700 - textwidth("��ѯ������Ϣ")) / 2, 50, "��ѯ������Ϣ");
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
			settextstyle(40, 0, "����");
			outtextxy((700 - textwidth("��ѯ������Ϣ")) / 2, 50, "��ѯ������Ϣ");
			make = true;
		}
		FlushBatchDraw();
		// ����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			Admin();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 240, 200, 460, 240, 20, 20, "�������Ų�ѯ")) {

			InputBox(ci, 128, "�����뺽����", "������", "", 69, 40);

			struct Flight* flights = readFlightsFromFile(); // ���ļ��ж�ȡ������Ϣ

			bool found = false; // ����Ƿ��ҵ�����

			struct Flight* curr = flights;
			while (curr != NULL) {
				if (strcmp(curr->id, ci) == 0) {
					cleardevice();
					putimage(0, 0, &ima);
					while (1)
					{
						FlushBatchDraw();
						setcolor(YELLOW);
						settextstyle(40, 0, "������κ");
						outtextxy((700 - textwidth("����ѯ�ĺ�����ϢΪ")) / 2, 50, "����ѯ�ĺ�����ϢΪ");
						setcolor(BLACK);
						settextstyle(30, 0, "����");
						std::stringstream s1;
						s1 << "����ţ�" << curr->id;
						std::stringstream s11;
						s11 << "�г̣�" << curr->start << " -> " << curr->end;
						std::stringstream s2;
						s2 << "����ʱ�䣺" << curr->syear << "��" << curr->smon << "��" << curr->sday << "��" << curr->shour << "ʱ" << curr->smin << "��";
						std::stringstream s21;
						s21 << "����ʱ�䣺" << curr->eyear << "��" << curr->emon << "��" << curr->eday << "��" << curr->ehour << "ʱ" << curr->emin << "��";
						std::stringstream s10;
						//ת��ʱ��
						int flyt = atoi(curr->flytime);
						int fh = flyt / 60;
						int fm = flyt % 60;
						s10 << "��" << fh << "Сʱ" << fm << "����";
						std::stringstream s22;
						s22 << "�����ѣ���" << curr->price << "Ԫ";
						//FlushBatchDraw();
						outtextxy((700 - textwidth(s1.str().c_str())) / 2, 140, s1.str().c_str());
						outtextxy((700 - textwidth(s11.str().c_str())) / 2, 190, s11.str().c_str());
						outtextxy((700 - textwidth(s2.str().c_str())) / 2, 240, s2.str().c_str());
						outtextxy((700 - textwidth(s21.str().c_str())) / 2, 290, s21.str().c_str());
						outtextxy((700 - textwidth(s10.str().c_str())) / 2, 340, s10.str().c_str());
						outtextxy((700 - textwidth(s22.str().c_str())) / 2, 390, s22.str().c_str());
						if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
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
				MessageBox(GetHWnd(), "δ�ҵ��ú��࣡", "��ʾ", MB_OK);
				make = false;
			}

			// �ͷ��ڴ�
			curr = flights;
			while (curr != NULL) {
				struct Flight* temp = curr;
				curr = curr->next;
				free(temp);
			}
		}
		if (Button(button.hoverColor, button.defaultColor, 240, 260, 460, 300, 20, 20, "ʼ�յ����")) {
			InputBox(bnum, 128, "���������ĳ�����", "Ԥ�������", "", 69, 40);
			InputBox(Enum, 128, "����������Ŀ�ĵ�", "Ԥ�������", "", 69, 40);
			if (bnum != NULL && Enum != NULL)
			{
			start:
				cleardevice();
				putimage(0, 0, &ima);
				FlushBatchDraw();
				// ���Ʊ��
				drawTable33(70, 40, 560, 400, 4, 6);

				outtextxy(100, 80, _T("�����"));
				outtextxy(190, 80, _T("������"));
				outtextxy(290, 80, _T("Ŀ�ĵ�"));
				outtextxy(370, 80, _T("����ʱ��"));
				outtextxy(460, 80, _T("����ʱ��"));
				outtextxy(550, 80, _T("��Ʊ����"));
				struct Flight* ft = readFlightsFromFile();
				struct Flight* rt = NULL;
				struct Flight* tail = NULL; // ���ڼ�¼currt�����ĩβ
				bool Find = false;
				int Num = 0;
				while (ft != NULL) {
					if (strcmp(bnum, ft->start) == 0 && strcmp(Enum, ft->end) == 0) {
						Find = true;
						Num++;
						if (rt == NULL) {
							rt = ft; // ���currt����Ϊ�գ���ֱ�ӽ�ƥ��ĺ�����Ϊ����ͷ
							tail = rt; // ��������ĩβָ��
						}
						else {
							tail->next = ft; // ��ƥ��ĺ����������ĩβ
							tail = ft; // ��������ĩβָ��
						}
					}
					ft = ft->next;
				}
				// ���������ĩβָ��ָ��NULL����ʾ�������
				if (tail != NULL) {
					tail->next = NULL;
				}
				struct Flight* currt = rt;
				if (Find == true)
				{
					//���� Ĭ��Ʊ������ ��Ĭ������ʱ��
					if (MessageBoxA(GetHWnd(), "Ĭ��Ʊ������ ѡ������Ƿ���ʱ������", "��ʾ", MB_YESNO | MB_ICONQUESTION) != IDYES) {
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
							//ת��ʱ��
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "��" << fh << "Сʱ";
							std::stringstream s22;
							s22 << fm << "����";
							std::stringstream s3;
							s3 << currt->price << "Ԫ";
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
							//ת��ʱ��
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "��" << fh << "Сʱ";
							std::stringstream s22;
							s22 << fm << "����";
							std::stringstream s3;
							s3 << currt->price << "Ԫ";
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
							//ת��ʱ��
							int flyt = atoi(currt->flytime);
							int fh = flyt / 60;
							int fm = flyt % 60;
							s2 << "��" << fh << "Сʱ";
							std::stringstream s22;
							s22 << fm << "����";
							std::stringstream s3;
							s3 << currt->price << "Ԫ";
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
					// �����һҳ��ť��Ӧ�¼�
					bool nextPage = false;
					int startRow = 1;
					while (true)
					{
						// TODO�����Ƶ�ǰҳ�ĺ�����Ϣ

						FlushBatchDraw();
						settextstyle(20, 0, _T("΢���ź�"));
						std::stringstream s3;
						s3 << "��" << startRow << "ҳ";
						outtextxy(560, 490, s3.str().c_str());
						// ����
						if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
						{
							make = false;
							strcpy(Enum, "");
							strcpy(bnum, "");
							break;
						}
						// �׽���
						if (Button(button.hoverColor, button.defaultColor, 460, 455, 560, 485, 80, 40, "��һҳ"))
						{
							goto start;
							break;
						}
						if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "��һҳ"))
						{
							startRow++;
							cleardevice(); // ��ջ�ͼ����
							putimage(0, 0, &ima); // ���»��Ʊ���ͼ��
							// ���Ʊ��
							drawTable22(70, 40, 560, 400, 4, 6);
							// �����ͷ
							outtextxy(100, 80, "�����");
							outtextxy(190, 80, "������");
							outtextxy(290, 80, "Ŀ�ĵ�");
							outtextxy(370, 80, "����ʱ��");
							outtextxy(460, 80, "����ʱ��");
							outtextxy(550, 80, "��Ʊ����");
							// ���Ƶ�ǰҳ�ĺ�����Ϣ
							// TODO����Ӻ�����Ϣ
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
									//ת��ʱ��
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "��" << fh << "Сʱ";
									std::stringstream s22;
									s22 << fm << "����";
									std::stringstream s3;
									s3 << currt->price << "Ԫ";
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
									//ת��ʱ��
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "��" << fh << "Сʱ";
									std::stringstream s22;
									s22 << fm << "����";
									std::stringstream s3;
									s3 << currt->price << "Ԫ";
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
									//ת��ʱ��
									int flyt = atoi(currt->flytime);
									int fh = flyt / 60;
									int fm = flyt % 60;
									s2 << "��" << fh << "Сʱ";
									std::stringstream s22;
									s22 << fm << "����";
									std::stringstream s3;
									s3 << currt->price << "Ԫ";
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
					MessageBox(GetHWnd(), "δ�ҵ�ָ������", "��ʾ", MB_OK);
					make = false;
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "�������������Ƿ���ȷ��", "��ʾ", MB_OK);
				make = false;
				break;
			}
		}
	}
}
// ȡ�����к��ಢ����Ӧ�������û���text����Ϊ���ް��� 
void clearAllFlights() {
	FILE* file1 = fopen("������Ϣ.txt", "w");
	FILE* file2 = fopen("������Ϣ.txt", "w");
	if (file1 == NULL || file2 == NULL) {
		MessageBox(GetHWnd(), "����ļ���ʧ�ܣ�", "��ʾ", MB_OK);
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

// ���ݸ����ĺ�����ɾ���ú��༰����������Ϣ
bool deleteFlightById(char* flightId) {
	FILE* file = fopen("������Ϣ.txt", "r+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "�ļ���ʧ�ܣ�", "��ʾ", MB_OK);
		return false;
	}

	struct Flight* hd = NULL;
	struct Flight* tl = NULL;
	struct Flight* tp;

	while (1) {
		tp = (struct Flight*)malloc(sizeof(struct Flight));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "�ڴ����ʧ�ܣ�", "��ʾ", MB_OK);
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
	file = fopen("������Ϣ.txt", "w");
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
//����غ����ŵĶ���ɾ��ͬʱҲ����ص��û���textҲ��Ϊ��
bool deleteOrderflight(char* flightId) {
	FILE* file = fopen("������Ϣ.txt", "r+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "�����ļ���ʧ�ܣ�", "��ʾ", MB_OK);
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
			MessageBox(GetHWnd(), "�ڴ����ʧ�ܣ�", "��ʾ", MB_OK);
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
	file = fopen("������Ϣ.txt", "w");
	if (file == NULL) {
		MessageBox(GetHWnd(), "�����ļ���ʧ�ܣ�", "��ʾ", MB_OK);
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
/* ȡ��������Ϣ */
void cancelFlight()
{
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(YELLOW);
	settextstyle(55, 0, "����");
	outtextxy((700 - textwidth("��ѡ��������һ��������")) / 2, 80, "��ѡ��������һ��������");
	char fid[20] = { 0 };
	while (1) {
		// ����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			//Admin();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 235, 211, 460, 260, 20, 20, "  ȡ�����к���  ")) {
			//���ı��е���Ϣ��� 
			clearAllFlights();
			MessageBox(GetHWnd(), "���к��༰���ж�����ȡ����", "��ʾ", MB_OK);
			if (MessageBoxA(GetHWnd(), "�Ƿ񷵻�", "��ʾ", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				//Admin();
				break;
			}
		}
		else if (Button(button.hoverColor, button.defaultColor, 235, 282, 460, 330, 20, 20, "ȡ���ض�����")) {
			while (1) {
				InputBox(fid, 180, "��������Ҫȡ���ĺ�����", "��ʾ", "", 69, 40);
				if (strcmp(fid, "") == 0) {
					MessageBox(GetHWnd(), "�����Ų���Ϊ�գ�", "������ʾ", MB_OK);
					continue;
				}
				break;
			}
			deleteFlightById(fid);
			if (deleteFlightById(fid) && deleteOrderflight(fid)) {
				MessageBox(GetHWnd(), "�˺��༰��Ӧ������ȡ����ȡ����", "��ʾ", MB_OK);
				if (MessageBoxA(GetHWnd(), "�Ƿ񷵻�", "��ʾ", MB_YESNO | MB_ICONQUESTION) == IDYES) {
					//Admin();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "�ú����Ŷ�Ӧ�ĺ��಻���ڣ�", "��ʾ", MB_OK);
				if (MessageBoxA(GetHWnd(), "�Ƿ񷵻�", "��ʾ", MB_YESNO | MB_ICONQUESTION) == IDYES) {
					//Admin();
					break;
				}
			}

		}
		FlushBatchDraw();
	}
	Admin();
}

/* ��ӿͻ� */
void addCustomer()
{
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(YELLOW);
	settextstyle(90, 0, "����");
	outtextxy((700 - textwidth("����û���Ϣ")) / 2, (500 - textheight("����û���Ϣ")) / 2 - 120, "����û���Ϣ");
	settextstyle(60, 0, "�п�");
	outtextxy((700 - textwidth("�û���:")) / 4 - 56, (500 - textheight("�û���:")) / 2, "�û���:");
	outtextxy((700 - textwidth("����:")) / 4 - 5, (500 - textheight("����:")) / 2 + 90, "����:");

	int rectX = (960 - textwidth("�˺�")) / 4;
	int rectY = (690 - textheight("�˺�")) / 2;

	//�洢�˺�����
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
		// �ڴ����ʧ�ܣ����д�����
	}

	while (true) {
		FlushBatchDraw();
		//����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			Admin();
			break;
		}
		//�����û���
		if (Button(button.hoverColor, button.defaultColor, 300, 235, 450, 280, 20, 20, Buf))
		{
			InputBox(Buf, 128, "�������û���", "�û���", "", 69, 40);
			if (strcmp(Buf, "") == 0) {
				MessageBox(GetHWnd(), "�û�������Ϊ�գ�", "������ʾ", MB_OK);
				continue;
			}

			// ����ظ��û���
			pd = head;
			while (pd != NULL) {
				if (strcmp(Buf, pd->user) == 0) {
					MessageBox(GetHWnd(), "�û����Ѵ��ڣ����������롣", "��ʾ", MB_OK);
					break;
				}
				pd = pd->next;
			}
			if (pd != NULL) {
				continue;
			}
		}
		//��������
		if (Button(button.hoverColor, button.defaultColor, 300, 320, 450, 365, 20, 20, Buff))
		{
			//�������
			InputBox(Buff, 128, "�������û�����", "����", "", 69, 40);
			Sleep(150);
			InputBox(bufff, 128, "���ٴ������û�����", "����", "", 69, 40);
			Sleep(50);
			if (strcmp(Buff, bufff) != 0)
			{
				MessageBox(GetHWnd(), "������������벻һ���������ԣ�", "��ʾ", MB_OKCANCEL);
				continue;
			}
		}
		//����ע��
		if (Button(button.hoverColor, button.defaultColor, 320, 400, 390, 440, 80, 40, "ע��"))
		{
			if (strcmp(Buf, "") != 0 && strcmp(Buff, "") != 0)
			{
				if (strcmp(Buff, bufff) == 0)
				{
					if (temp != NULL)
					{
						temp->next = NULL;
						MessageBox(GetHWnd(), "�û��������óɹ�", "��ʾ", MB_OKCANCEL);
						InputBox(name, 128, "�������û���ʵ����", "����", "", 69, 40);
						bool isPhoneNumberValid = false;
						while (!isPhoneNumberValid) {
							char numm[15] = { 0 }; // ��num�ĳ�ʼ���Ƶ�ѭ���ڲ�
							InputBox(numm, 128, "�������û��绰����", "�绰����", "", 69, 40);
							if (isValidPhoneNumber(numm) == true) {
								strcpy(num, numm);
								isPhoneNumberValid = true;
							}

						}
						bool inputValid = false;
						while (!inputValid) {
							InputBox(id, 128, "�������û����֤����", "���֤����", "", 69, 40);
							if (isValidIDNumber(id)) {
								inputValid = true;
							}
						}

						if (strlen(name) == 0 || strlen(num) == 0 || strlen(id) == 0)
						{
							MessageBox(GetHWnd(), "�������绰��������֤����Ϊ�գ�������", "��ʾ", MB_OKCANCEL);
							continue;
						}
						if (strlen(Buf) < sizeof(temp->user) && strlen(Buff) < sizeof(temp->code))
						{
							strcpy(temp->user, Buf);
							strcpy(temp->code, Buff);
							strcpy(temp->name, name);
							strcpy(temp->phonenum, num);
							strcpy(temp->id, id);
							strcpy(temp->text, "���ް���");
						}
						else {
							MessageBox(GetHWnd(), "������������Ϣ��", "��ʾ", MB_OKCANCEL);
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

					MessageBox(GetHWnd(), "�û��˺�ע��ɹ���", "��ʾ", MB_OKCANCEL);
					cleardevice();
					putimage(0, 0, &ima);
					FlushBatchDraw();

					read();
					temp = head;
					//��������
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
						settextstyle(70, 0, "������κ");
						outtextxy((700 - textwidth("������Ϣ")) / 2, 30, "������Ϣ");

						drawTable();

						drawText("�û�����", 100, 110, 0, 0);
						drawText("�û���", 100, 110, 1, 0);
						drawText("�˺�״̬", 100, 110, 2, 0);
						drawText("�ֻ�����", 100, 110, 3, 0);
						drawText("���֤����", 100, 110, 4, 0);
						drawText("���ల��", 100, 110, 5, 0);
						drawText(aa, 600, 110, 0, 1);
						drawText(bb, 600, 110, 1, 1);
						drawText("����", 600, 110, 2, 1);
						drawText(cc, 600, 110, 3, 1);
						drawText(dd, 600, 110, 4, 1);
						drawText(ee, 600, 110, 5, 1);

						// ����
						if (Button(button.hoverColor, button.defaultColor, 15, 470, 120, 510, 80, 40, "����"))
						{
							Admin();
							break;
						}
					}
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "���õ��û��˺Ż���������������", "��ʾ", MB_OKCANCEL);
				continue;
			}
		}
	}
}

/* ��Ӻ�����Ϣ */
void addFlight()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	setcolor(BLACK);
	settextstyle(40, 0, "����");
	outtextxy(50, 15, "������");
	outtextxy(50, 70, "������");
	outtextxy(50, 125, "�յ����");
	outtextxy(50, 180, "�������");
	outtextxy(330, 180, "��");
	outtextxy(450, 180, "��");
	outtextxy(570, 180, "��");
	outtextxy(50, 235, "���ʱ��");
	outtextxy(340, 235, "ʱ");
	outtextxy(500, 235, "��");
	outtextxy(50, 290, "��������");
	outtextxy(330, 290, "��");
	outtextxy(450, 290, "��");
	outtextxy(570, 290, "��");
	outtextxy(50, 345, "����ʱ��");
	outtextxy(340, 345, "ʱ");
	outtextxy(500, 345, "��");
	outtextxy(50, 400, "��Ʊ�۸�");
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
		// ����
		if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "����"))
		{
			Admin();
			break;
		}
		//���
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "���"))
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
					MessageBox(GetHWnd(), "��鵽��ʱ���Ƿ�����ʱ����", "��ʾ", MB_OK);
					continue;
				}
				sprintf(flights.flytime, "%d", flyt);
				writeFlights(&flights, 1);
				cleardevice();
				putimage(0, 0, &ima);
				FlushBatchDraw();
				MessageBox(GetHWnd(), "������ӳɹ���", "��ʾ", MB_OK);
				while (1) {
					FlushBatchDraw();
					setcolor(YELLOW);
					settextstyle(50, 0, "����");
					std::stringstream s1;
					s1 << "������ " << flights.id;
					std::stringstream s2;
					s2 << "ʼ: " << flights.start << " �գ�" << flights.end;
					std::stringstream s3;
					s3 << "ԭ��" << flights.price;
					std::stringstream s4;
					s4 << flights.syear << "��" << flights.smon << "��" << flights.sday << "��" << flights.shour << "ʱ" << flights.smin <<
						"����";
					std::stringstream s5;
					s5 << "��" << flights.eyear << "��" << flights.emon << "��" << flights.eday << "��" << flights.ehour << "ʱ" << flights.emin << "��";
					std::stringstream s6;
					int fh = flyt / 60;
					int fm = flyt % 60;
					s6 << "��" << fh << "Сʱ" << fm << "����";

					outtextxy((700 - textwidth(s1.str().c_str())) / 2, 40, s1.str().c_str());
					outtextxy((700 - textwidth(s2.str().c_str())) / 2, 100, s2.str().c_str());
					outtextxy((700 - textwidth(s3.str().c_str())) / 2, 170, s3.str().c_str());
					outtextxy((700 - textwidth(s4.str().c_str())) / 2, 250, s4.str().c_str());
					outtextxy((700 - textwidth(s5.str().c_str())) / 2, 320, s5.str().c_str());
					outtextxy((700 - textwidth(s6.str().c_str())) / 2, 390, s6.str().c_str());
					if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
					{
						all_values_present = 1;
						goto start;
						break;
					}
				}
			}
			else {
				MessageBox(GetHWnd(), "�������룡", "��ʾ", MB_OK);
			}
		}
		if (Button(button.hoverColor, button.defaultColor, 260, 15, 460, 55, 20, 20, sid)) {

			InputBox(sid, 128, "�����뺽����", "��ʾ", "", 69, 40);
			while (1) {
				if (isDuplicateFlight(sid)) {
					MessageBox(GetHWnd(), "�������Ѵ��ڣ�", "��ʾ", MB_OK);
					strcpy(sid, "");
					break;
				}
				else if (strcmp(sid, "") != 0) {
					break;
				}
				else {
					InputBox(sid, 128, "�����뺽����", "��ʾ", "", 69, 40);
				}
			}
		}
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 260, 70, 460, 110, 20, 20, start)) {

			InputBox(start, 128, "������������(2��10���ַ�", "��ʾ", "", 69, 40);

			while (1) {
				if (strlen(start) <= 10 && strlen(start) >= 2)
				{

					break;
				}
				else
				{
					InputBox(start, 128, "������Ϸ���������(2��10���ַ�", "��ʾ", "", 69, 40);

				}
			}
		}
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 260, 125, 460, 165, 20, 20, end)) {

			InputBox(end, 128, "�������յ����", "��ʾ", "", 69, 40);

			while (1) {
				if (strlen(end) <= 10 && strlen(end) >= 2)
				{

					break;
				}
				else
				{
					InputBox(end, 128, "������Ϸ����յ����(2-10���ַ�)", "��ʾ", "", 69, 40);
				}
			}
		}

		// ���������İ�ť
		if (Button(button.hoverColor, button.defaultColor, 240, 180, 320, 220, 20, 20, ssyear)) {
			InputBox(ssyear, 128, "������ݣ�YYYY)", "��ʾ", "", 69, 40);
			while (!isValidYear(ssyear)) {
				InputBox(ssyear, 128, "�������������Ϸ���ʽ��YYYY)", "��ʾ", "", 69, 40);
			}
		}
		// ��������µİ�ť
		if (Button(button.hoverColor, button.defaultColor, 370, 180, 440, 220, 20, 20, ssmon)) {
			InputBox(ssmon, 128, "�����·�", "��ʾ", "", 69, 40);
			while (!isValidMonth(ssmon)) {
				InputBox(ssmon, 128, "�������������Ϸ���ʽ��MM)", "��ʾ", "", 69, 40);
			}
		}
		FlushBatchDraw();
		// ��������յİ�ť
		if (Button(button.hoverColor, button.defaultColor, 500, 180, 570, 220, 20, 20, ssday)) {
			InputBox(ssday, 128, "��������", "��ʾ", "", 69, 40);
			while (!isValidDay(ssday)) {
				InputBox(ssday, 128, "�������������Ϸ���ʽ��dd)", "��ʾ", "", 69, 40);
			}
		}
		FlushBatchDraw();
		// �������Сʱ�İ�ť
		if (Button(button.hoverColor, button.defaultColor, 230, 235, 330, 275, 20, 20, sshour)) {
			InputBox(sshour, 128, "���������ʱ�䣨HH��", "��ʾ", "", 69, 40);
			while (!isValidHour(sshour)) {
				InputBox(sshour, 128, "�������������Ϸ����ʱ�䣨HH��", "��ʾ", "", 69, 40);
			}

		}
		// ����������ӵİ�ť
		if (Button(button.hoverColor, button.defaultColor, 380, 235, 480, 275, 20, 20, ssmin)) {
			InputBox(ssmin, 128, "���������ʱ�䣨MM)", "��ʾ", "", 69, 40);
			while (!isValidMinute(ssmin)) {
				InputBox(ssmin, 128, "�������������Ϸ����ʱ�䣨MM)", "��ʾ", "", 69, 40);
			}

		}
		// ���뵽����İ�ť
		if (Button(button.hoverColor, button.defaultColor, 240, 290, 320, 330, 20, 20, cty)) {
			InputBox(cty, 128, "������ݣ�YYYY)", "��ʾ", "", 69, 40);
			while (!isValidYear(cty)) {
				InputBox(cty, 128, "�������������Ϸ���ʽ��YYYY)", "��ʾ", "", 69, 40);
			}
		}
		// ���뵽���µİ�ť
		if (Button(button.hoverColor, button.defaultColor, 370, 290, 440, 330, 20, 20, ctmon)) {
			InputBox(ctmon, 128, "�����·�", "��ʾ", "", 69, 40);
			while (!isValidMonth(ctmon)) {
				InputBox(ctmon, 128, "�������������Ϸ���ʽ��MM)", "��ʾ", "", 69, 40);
			}
		}
		FlushBatchDraw();
		// ���뵽���յİ�ť
		if (Button(button.hoverColor, button.defaultColor, 500, 290, 570, 330, 20, 20, ctd)) {
			InputBox(ctd, 128, "��������", "��ʾ", "", 69, 40);
			while (!isValidDay(ctd)) {
				InputBox(ctd, 128, "�������������Ϸ���ʽ��dd)", "��ʾ", "", 69, 40);
			}
		}
		FlushBatchDraw();
		//Сʱ
		if (Button(button.hoverColor, button.defaultColor, 230, 345, 330, 385, 20, 20, cth)) {
			InputBox(cth, 128, "�����뵽��ʱ��", "��ʾ", "", 69, 40);
			while (!isValidHour(cth)) {
				InputBox(cth, 128, "�������������Ϸ����ʱ�䣨HH��", "��ʾ", "", 69, 40);
			}
		}
		//����
		if (Button(button.hoverColor, button.defaultColor, 380, 345, 480, 385, 20, 20, ctmin)) {
			InputBox(ctmin, 128, "�����뵽��ʱ��", "��ʾ", "", 69, 40);
			while (!isValidMinute(ctmin)) {
				InputBox(ctmin, 128, "�������������Ϸ����ʱ�䣨MM)", "��ʾ", "", 69, 40);
			}
		}
		//�۸�
		if (Button(button.hoverColor, button.defaultColor, 260, 400, 360, 440, 20, 20, cprice)) {
			InputBox(cprice, 128, "�������Ʊ�۸�", "��ʾ", "", 69, 40);
			while (!isValidPrice(cprice)) {
				InputBox(cprice, 128, "������淶�Ļ�Ʊ�۸�", "��ʾ", "", 69, 40);
			}
		}

		FlushBatchDraw();
	}
start:
	if (all_values_present == true) {
		Admin();
	}

}

//��ʼ����
int menu()
{
	cleardevice();
	putimage(0, 0, &image);
	FlushBatchDraw();
	settextstyle(66, 0, "�п�");
	outtextxy((700 - textwidth("��Ʊ����ϵͳ")) / 2, 110, "��Ʊ����ϵͳ");
	int pp = 0;
	while (true)
	{
		FlushBatchDraw();

		if (Button(button.hoverColor, button.defaultColor, 300, 240, 400, 280, 80, 40, "����Ա"))
		{
			pp = 1;
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 300, 300, 400, 340, 80, 40, "�û�"))
		{
			pp = 2;
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 300, 360, 400, 400, 80, 40, "�˳�"))
		{
			pp = 3;
			break;
		}
	}
	return pp;
}

//ʱ��
void displayTime()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	// ���ڵ�ǰϵͳ�ĵ�ǰ����/ʱ��
	time_t now = time(0);
	// �� now ת��Ϊ�ַ�����ʽ
	char* dt = ctime(&now);
	//�� now ת��Ϊ tm �ṹ
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
		s << "�簲������ð���";
	}
	else if (hour > 12 && hour <= 18)
	{
		s << "Having Good Afternoon��";
	}
	else {
		s << "ף���и��������ϣ�";
	}
	std::stringstream ss;
	ss << year << "��" << month << "��" << day << "��";
	std::stringstream Ss;
	Ss << hour << "ʱ" << minute << "��" << second << "��";
	//cout << "�������ں�ʱ�䣺" << dt << endl;
	while (1)
	{
		FlushBatchDraw();

		settextstyle(40, 0, "������κ");
		settextcolor(GREEN);
		outtextxy((700 - textwidth(s.str().c_str())) / 2, 40, s.str().c_str());
		outtextxy((700 - textwidth("----ף���и�������;��")) * 2 / 3 + 66, 440, "----ף���и�������;��");
		settextstyle(50, 0, "�п�");
		settextcolor(BLUE);
		outtextxy(0, (500 - textheight("�����Ǳ���ʱ�䣺")) / 4, "�����Ǳ���ʱ�䣺");
		settextstyle(44, 0, "����");
		settextcolor(YELLOW);
		setlinecolor(BLACK);
		line(180, 200, 520, 200);
		line(180, 200, 180, 400);
		line(180, 400, 520, 400);
		line(520, 200, 520, 400);
		outtextxy((700 - textwidth(ss.str().c_str())) / 2, 230, ss.str().c_str());
		outtextxy((700 - textwidth(Ss.str().c_str())) / 2, 320, Ss.str().c_str());

		//����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			sign();
			break;
		}
	}
}


//�û�����
void sign()
{
	cleardevice();
	putimage(0, 0, &ge);
	FlushBatchDraw();
	read();
	temp = head;
	//��������
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
		settextstyle(30, 0, "�п�");
		outtextxy((700 - textwidth(" ��ѡ����������")) * 2 / 3, 120, "------��ѡ����������");
		setcolor(YELLOW);
		settextstyle(60, 0, "������κ");
		std::stringstream ss;
		char sex = temp->id[16];
		int sexn = sex - '0';
		if (sexn % 2 == 1)
		{
			ss << "�𾴵�" << temp->name << "����" << "��ӭ����";
		}
		else {
			ss << "�𾴵�" << temp->name << "Ůʿ" << "��ӭ����";
		}

		outtextxy((700 - textwidth(ss.str().c_str())) / 2, 30, ss.str().c_str());
		if (Button(button.hoverColor, button.defaultColor, 160, 190, 280, 230, 20, 20, "Ԥ������")) {
			//1. Ԥ������
			Mflight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 190, 560, 230, 20, 20, "ȡ������")) {
			//2. ȡ������
			Qflight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 250, 280, 290, 20, 20, "��ѯ����")) {
			//3. ��ѯ����
			fcheck();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 250, 560, 290, 20, 20, "�ҵĶ���")) {
			//4. �ҵĶ���
			myflight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 310, 280, 350, 20, 20, "������Ϣ")) {
			//5. ������Ϣ
			text();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 310, 560, 350, 20, 20, "�޸���Ϣ")) {
			//6. �޸���Ϣ
			changetext();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 370, 280, 410, 20, 20, "��ǰʱ��")) {
			//7. ��ǰʱ��
			displayTime();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 370, 560, 410, 20, 20, "�˳�ʹ��")) {
			//8. �˳�����Աģʽ
			MessageBox(GetHWnd(), "��л���ı���ʹ�ã��𾴵��û���", "��ʾ", MB_OK);
			menu();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 300, 420, 420, 460, 20, 20, "�˳�ϵͳ")) {
			//9. �˳�airFlight��Ʊ����ϵͳ
			MessageBox(GetHWnd(), "��л����ʹ�ã��´��ټ���", "��ʾ", MB_OK);
			return exit(66);
		}
	}

}

//�һ�����
void findcode()
{
	void forget();
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(WHITE);
	settextstyle(90, 0, "΢���ź�");
	outtextxy((700 - textwidth("�һ�����")) / 2, (500 - textheight("�һ�����")) / 2 - 170, "�һ�����");
	setcolor(BLACK);
	settextstyle(50, 0, "�п�");
	outtextxy((700 - textwidth("��ʵ������")) / 3, 155, "��ʵ������");
	outtextxy((700 - textwidth("���֤�ţ�")) / 3, 225, "���֤�ţ�");
	outtextxy((700 - textwidth("�ֻ����룺")) / 3, 295, "�ֻ����룺");
	temp = head;
	char bufff[20] = { 0 };
	char f[20] = { 0 };
	char b[15] = { 0 };
	bool wW = false;
	while (1)
	{
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			forget();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 395, 160, 600, 210, 80, 40, bufff))
		{//���name
			InputBox(bufff, 128, "��������������", "����", "", 69, 40);
		}
		else if (Button(button.hoverColor, button.defaultColor, 375, 230, 660, 280, 80, 40, f))
		{//����û�
			InputBox(f, 128, "�������������֤����", "���֤����", "", 73, 40);
		}
		else if (Button(button.hoverColor, button.defaultColor, 395, 300, 600, 350, 80, 40, b))
		{//�������
			InputBox(b, 128, "���������µ��ֻ�����", "�ֻ�����", "", 69, 40);
		}
		if (Button(button.hoverColor, button.defaultColor, 310, 385, 385, 430, 80, 40, "ȷ��"))
		{
			while (temp != NULL)
			{
				if (strcmp(bufff, temp->name) == 0 && strcmp(b, temp->phonenum) == 0 && strcmp(f, temp->id) == 0)
				{
					std::stringstream s;
					s << "��֤�ɹ��������û���Ϊ�� " << temp->user << "��";
					strcpy(Buff, "");
					MessageBox(GetHWnd(), s.str().c_str(), "��ʾ", MB_OK);
					InputBox(Buff, 128, "���������µ�����", "����", "", 69, 40);
					strcpy(temp->code, Buff);
					strcpy(Buf, temp->user);
					write();
					MessageBox(GetHWnd(), "�޸ĳɹ���", "��ʾ", MB_OK);
					wW = true;
					break;
				}
				else {
					temp = temp->next;
				}
			}
			if (strcmp(bufff, "") == 0 || strcmp(b, "") == 0 || strcmp(f, "") == 0)
			{
				MessageBox(GetHWnd(), "�밴Ҫ�����룡", "��ʾ", MB_OK);
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
//���ݺ����Ų��ҵ���һ������
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
// ������������ַ����飬��ָ������
char* generateRandomArray(int length) {
	char* arr = new char[length + 1];  // ��Ҫ����һ��λ�ô洢�ַ��������� '\0'
	srand(time(NULL));  // ʹ��ʱ����Ϊ�������

	for (int i = 0; i < length; i++) {
		arr[i] = '0' + rand() % 10;  // ���� 0 �� 9 ֮�����������ַ�
	}

	arr[length] = '\0';  // ����ַ���������
	return arr;
}
// ����ָ�����û��������û�����ʵ����
char* getRealName(char* username) {
	struct ticket* current = head;
	while (current != NULL) {
		if (strcmp(current->user, username) == 0) {
			printf("%s", current->name);
			return current->name;
		}
		current = current->next;
	}
	return NULL; // ����Ҳ���ƥ����û������򷵻�NULL
}
//ָ���ص㺽����
void drawTable33(int x, int y, int width, int height, int row, int col)
{
	// ���ñ��߿���
	int borderWidth = 2;
	// ����ÿ����Ԫ���Ⱥ͸߶�
	int cellWidth = (width - (col + 1) * borderWidth) / col;
	int cellHeight = (height - (row + 2) * borderWidth) / row;

	// ���Ʊ�������ͺ�������
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
	settextstyle(20, 0, _T("΢���ź�"));

}
// �Ժ�����Ϣ���ռ۸����ð������
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
			// ���۸��ַ���ת��Ϊ���������бȽ�
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

	// ��ӡ�����ĺ���۸�
	while (head != NULL) {
		printf("%s\n", head->price);
		head = head->next;
	}
}
//Ԥ������
void Mflight()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	read();
	temp = head;
	//��������
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
	settextstyle(60, 0, "����");
	outtextxy((700 - textwidth("��ӭ��ѡ�����Ǻ��࣡")) / 2, (500 - textheight("��ӭ��ѡ�����Ǻ��࣡")) / 2 - 170, "��ӭ��ѡ�����Ǻ��࣡");
	setcolor(BLACK);
	settextstyle(36, 0, "�п�");
	outtextxy((700 - textwidth("�����·�������ҪԤ���ĺ����:")) / 4 - 44, (500 - textheight("�����·�������ҪԤ���ĺ����:")) / 2 - 60, "�����·�������ҪԤ���ĺ����:");
	char fnum[20] = { 0 };
	char bnum[24] = { 0 };
	char Enum[24] = { 0 };
	int length = 4;  // ���鳤��
	char* randomArray = generateRandomArray(4);

	bool made = false;
	bool make = true;
	while (true) {
		if (make == false)
		{
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(WHITE);
			settextstyle(60, 0, "����");
			outtextxy((700 - textwidth("��ӭ��ѡ�����Ǻ��࣡")) / 2, (500 - textheight("��ӭ��ѡ�����Ǻ��࣡")) / 2 - 170, "��ӭ��ѡ�����Ǻ��࣡");
			setcolor(BLACK);
			settextstyle(36, 0, "�п�");
			outtextxy((700 - textwidth("�����·�������ҪԤ���ĺ����:")) / 4 - 44, (500 - textheight("�����·�������ҪԤ���ĺ����:")) / 2 - 60, "�����·�������ҪԤ���ĺ����:");
			make = true;
		}
		FlushBatchDraw();
		//����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			sign();
			break;
		}
		//���뺽���
		if (Button(button.hoverColor, button.defaultColor, 260, 235, 480, 280, 80, 40, fnum))
		{
			InputBox(fnum, 128, "��������ҪԤ���ĺ����", "Ԥ�������", "", 69, 40);
			struct Flight* ft = readFlightsFromFile();
			struct Flight* flight = findFlightById(ft, fnum);
			//��ȡ�û��� ����ȫ�ֱ�����ȡ��¼���û��� Buf[22]
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
					settextstyle(40, 0, "����");
					outtextxy((700 - textwidth("Ԥ���ĺ�����Ϣ")) / 2, (500 - textheight("Ԥ���ĺ�����Ϣ")) / 2 - 200, "Ԥ���ĺ�����Ϣ");
					std::stringstream s1;
					s1 << "������ " << flight->id;
					std::stringstream s2;
					s2 << "ʼ: " << flight->start << " �գ�" << flight->end;
					std::stringstream s3;
					s3 << "ԭ�ۣ���" << flight->price << "Ԫ";
					std::stringstream s4;
					s4 << flight->syear << "��" << flight->smon << "��" << flight->sday << "��" << flight->shour << "ʱ" << flight->smin <<
						"����";
					std::stringstream s5;
					s5 << "��" << flight->eyear << "��" << flight->emon << "��" << flight->eday << "��" << flight->ehour << "ʱ" << flight->smin <<
						"��";
					std::stringstream s6;
					s6 << "��" << fh << "Сʱ" << fm << "����";
					settextstyle(36, 0, "����");
					setcolor(BLACK);
					outtextxy((700 - textwidth(s1.str().c_str())) / 2, 140, s1.str().c_str());
					outtextxy((700 - textwidth(s2.str().c_str())) / 2, 190, s2.str().c_str());
					outtextxy((700 - textwidth(s3.str().c_str())) / 2, 240, s3.str().c_str());
					outtextxy((700 - textwidth(s4.str().c_str())) / 2, 290, s4.str().c_str());
					outtextxy((700 - textwidth(s5.str().c_str())) / 2, 340, s5.str().c_str());
					outtextxy((700 - textwidth(s6.str().c_str())) / 2, 390, s6.str().c_str());
					//����
					if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
					{
						strcpy(fnum, "");
						make = false;
						break;
					}
					//Ԥ��
					if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "Ԥ��"))
					{
						made = true;
						// ��ʼ���ṹ���Ա����
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
						MessageBox(GetHWnd(), "Ԥ���ú���ɹ���", "��ʾ", MB_OK);
						break;
					}

				}

			}
			else {
				MessageBox(GetHWnd(), "δ�ҵ�ָ������", "��ʾ", MB_OK);
			}
			if (made == true)
			{
				sign();
				break;
			}
		}
		//����Ŀ�ĵ�
		if (Button(button.hoverColor, button.defaultColor, 260, 300, 480, 350, 80, 40, "ʼ�յ����"))
		{
			InputBox(bnum, 128, "���������ĳ�����", "Ԥ�������", "", 69, 40);
			InputBox(Enum, 128, "����������Ŀ�ĵ�", "Ԥ�������", "", 69, 40);
			if (bnum != NULL && Enum != NULL)
			{
			start:
				cleardevice();
				putimage(0, 0, &ima);
				FlushBatchDraw();
				// ���Ʊ��
				drawTable33(70, 40, 560, 400, 4, 6);

				outtextxy(100, 80, _T("�����"));
				outtextxy(190, 80, _T("������"));
				outtextxy(290, 80, _T("Ŀ�ĵ�"));
				outtextxy(370, 80, _T("����ʱ��"));
				outtextxy(460, 80, _T("����ʱ��"));
				outtextxy(550, 80, _T("��Ʊ����"));
				struct Flight* ft = readFlightsFromFile();
				struct Flight* rt = NULL;
				struct Flight* tail = NULL; // ���ڼ�¼currt�����ĩβ
				bool Find = false;
				int Num = 0;
				while (ft != NULL) {
					if (strcmp(bnum, ft->start) == 0 && strcmp(Enum, ft->end) == 0) {
						Find = true;
						Num++;
						if (rt == NULL) {
							rt = ft; // ���currt����Ϊ�գ���ֱ�ӽ�ƥ��ĺ�����Ϊ����ͷ
							tail = rt; // ��������ĩβָ��
						}
						else {
							tail->next = ft; // ��ƥ��ĺ����������ĩβ
							tail = ft; // ��������ĩβָ��
						}
					}
					ft = ft->next;
				}
				// ���������ĩβָ��ָ��NULL����ʾ�������
				if (tail != NULL) {
					tail->next = NULL;
				}
				struct Flight* currt = rt;
				if (Find == true)
				{
					//���� Ĭ��Ʊ������ ��Ĭ������ʱ��
					if (MessageBoxA(GetHWnd(), "Ĭ��Ʊ������ ѡ������Ƿ���ʱ������", "��ʾ", MB_YESNO | MB_ICONQUESTION) != IDYES) {
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
							s2 << "��" << fh << "Сʱ";
							std::stringstream s22;
							s22 << fm << "����";
							std::stringstream s3;
							s3 << currt->price << "Ԫ";
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
							s2 << "��" << fh << "Сʱ";
							std::stringstream s22;
							s22 << fm << "����";
							std::stringstream s3;
							s3 << currt->price << "Ԫ";
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
							s2 << "��" << fh << "Сʱ";
							std::stringstream s22;
							s22 << fm << "����";
							std::stringstream s3;
							s3 << currt->price << "Ԫ";
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
					// �����һҳ��ť��Ӧ�¼�
					bool nextPage = false;
					int startRow = 1;
					while (true)
					{
						// TODO�����Ƶ�ǰҳ�ĺ�����Ϣ

						FlushBatchDraw();
						settextstyle(20, 0, _T("΢���ź�"));
						std::stringstream s3;
						s3 << "��" << startRow << "ҳ";
						outtextxy(560, 490, s3.str().c_str());
						// ����
						if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
						{
							make = false;
							strcpy(Enum, "");
							strcpy(bnum, "");
							break;
						}
						// �׽���
						if (Button(button.hoverColor, button.defaultColor, 460, 455, 560, 485, 80, 40, "��һҳ"))
						{
							goto start;
							break;
						}
						if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "��һҳ"))
						{
							startRow++;
							cleardevice(); // ��ջ�ͼ����
							putimage(0, 0, &ima); // ���»��Ʊ���ͼ��
							// ���Ʊ��
							drawTable22(70, 40, 560, 400, 4, 6);
							// �����ͷ
							outtextxy(100, 80, "�����");
							outtextxy(190, 80, "������");
							outtextxy(290, 80, "Ŀ�ĵ�");
							outtextxy(370, 80, "����ʱ��");
							outtextxy(460, 80, "����ʱ��");
							outtextxy(550, 80, "��Ʊ����");
							// ���Ƶ�ǰҳ�ĺ�����Ϣ
							// TODO����Ӻ�����Ϣ
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
									s2 << "��" << fh << "Сʱ";
									std::stringstream s22;
									s22 << fm << "����";
									std::stringstream s3;
									s3 << currt->price << "Ԫ";
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
									s2 << "��" << fh << "Сʱ";
									std::stringstream s22;
									s22 << fm << "����";
									std::stringstream s3;
									s3 << currt->price << "Ԫ";
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
									s2 << "��" << fh << "Сʱ";
									std::stringstream s22;
									s22 << fm << "����";
									std::stringstream s3;
									s3 << currt->price << "Ԫ";
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
					MessageBox(GetHWnd(), "δ�ҵ�ָ������", "��ʾ", MB_OK);
					Mflight();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "�������������Ƿ���ȷ��", "��ʾ", MB_OK);
				Mflight();
				break;
			}
		}
	}
}
//ȡ������
bool deleteOrderById(char* flightId, char* customerName) {
	FILE* file = fopen("������Ϣ.txt", "r+");
	if (file == NULL) {
		MessageBox(GetHWnd(), "�ļ���ʧ�ܣ�", "��ʾ", MB_OK);
		return false;
	}

	struct Order* hd = NULL;
	struct Order* tl = NULL;
	struct Order* prev = NULL;
	struct Order* tp;
	bool found = false; // ���һ���������ڸ����Ƿ��ҵ�ƥ��Ķ���

	while (1) {
		tp = (struct Order*)malloc(sizeof(struct Order));
		if (tp == NULL) {
			MessageBox(GetHWnd(), "�ڴ����ʧ�ܣ�", "��ʾ", MB_OK);
			break;
		}

		if (fscanf(file, "%s %s %s %s", tp->id, tp->flightId, tp->customerId, tp->detail) == 4) {
			// �жϺ���ź������Ƿ�ƥ��
			if (strcmp(flightId, tp->flightId) == 0 && strcmp(customerName, tp->customerId) == 0)
			{
				found = true; // �ҵ���ƥ��Ķ���
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
	file = fopen("������Ϣ.txt", "w");
	if (file == NULL) {
		MessageBox(GetHWnd(), "�ļ���ʧ�ܣ�", "��ʾ", MB_OK);
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
		return false; // ���û���ҵ�ƥ��Ķ����򷵻�false
	}
}


//ȡ������
void Qflight()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	read();
	temp = head;
	//��������
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
	settextstyle(60, 0, "����");
	outtextxy((700 - textwidth("��ӭ��ѡ�����Ǻ��࣡")) / 2, (500 - textheight("��ӭ��ѡ�����Ǻ��࣡")) / 2 - 170, "��ӭ��ѡ�����Ǻ��࣡");
	setcolor(BLACK);
	settextstyle(36, 0, "�п�");
	outtextxy((700 - textwidth("�����·�������Ҫȡ���ĺ����:")) / 4 - 44, (500 - textheight("�����·�������Ҫȡ���ĺ����:")) / 2 - 60, "�����·�������Ҫȡ���ĺ����:");
	char fnum[20] = { 0 };
	char* n = getRealName(Buf);
	while (true) {
		FlushBatchDraw();
		//����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			sign();
			break;
		}
		//���뺽���
		if (Button(button.hoverColor, button.defaultColor, 260, 235, 480, 280, 80, 40, fnum))
		{
			InputBox(fnum, 180, "��������Ҫȡ���ĺ�����", "��ʾ", "", 69, 40);
			if (deleteOrderById(fnum, n)) {
				MessageBox(GetHWnd(), "����ȡ���ɹ���", "��ʾ", MB_OK);
				struct Order* filteredOrders = findOrdersByCustomerName(n);
				if (filteredOrders) {
					while (filteredOrders->next) {
						filteredOrders = filteredOrders->next;
					}
					strcpy(temp->text, filteredOrders->detail);
				}
				else {
					strcpy(temp->text, "���ް���");
				}

				write();
				sign();
				break;
			}
			else {
				MessageBox(GetHWnd(), "�ú����Ŷ�Ӧ�Ķ��������ڣ�", "��ʾ", MB_OK);
				sign();
				break;

			}

		}
	}
}
//�鿴����
void drawTable22(int x, int y, int width, int height, int row, int col)
{
	// ���ñ��߿���
	int borderWidth = 2;
	// ����ÿ����Ԫ���Ⱥ͸߶�
	int cellWidth = (width - (col + 1) * borderWidth) / col;
	int cellHeight = (height - (row + 2) * borderWidth) / row;

	// ���Ʊ�������ͺ�������
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
	settextstyle(20, 0, _T("΢���ź�"));

}
void fcheck()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	// ���Ʊ��
	drawTable22(70, 40, 560, 400, 4, 6);

	outtextxy(100, 80, _T("�����"));
	outtextxy(190, 80, _T("������"));
	outtextxy(290, 80, _T("Ŀ�ĵ�"));
	outtextxy(370, 80, _T("����ʱ��"));
	outtextxy(460, 80, _T("����ʱ��"));
	outtextxy(550, 80, _T("��Ʊ����"));

	// TODO����Ӻ�����Ϣ
	struct Flight* flights = readFlightsFromFile(); // ���ļ��ж�ȡ������Ϣ
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
			s2 << "��" << fh << "Сʱ";
			std::stringstream s22;
			s22 << fm << "����";
			std::stringstream s3;
			s3 << curr->price << "Ԫ";
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
			s2 << "��" << fh << "Сʱ";
			std::stringstream s22;
			s22 << fm << "����";
			std::stringstream s3;
			s3 << curr->price << "Ԫ";
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
			s2 << "��" << fh << "Сʱ";
			std::stringstream s22;
			s22 << fm << "����";
			std::stringstream s3;
			s3 << curr->price << "Ԫ";
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
	// �����һҳ��ť��Ӧ�¼�
	bool nextPage = false;
	int startRow = 1;
	while (true)
	{
		// TODO�����Ƶ�ǰҳ�ĺ�����Ϣ

		FlushBatchDraw();
		settextstyle(20, 0, _T("΢���ź�"));
		std::stringstream s3;
		s3 << "��" << startRow << "ҳ";
		outtextxy(560, 490, s3.str().c_str());
		// ����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			sign();
			break;
		}
		// �׽���
		if (Button(button.hoverColor, button.defaultColor, 460, 455, 560, 485, 80, 40, "��һҳ"))
		{
			fcheck();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 40, "��һҳ"))
		{
			startRow++;
			cleardevice(); // ��ջ�ͼ����
			putimage(0, 0, &ima); // ���»��Ʊ���ͼ��
			// ���Ʊ��
			drawTable22(70, 40, 560, 400, 4, 6);
			// �����ͷ
			outtextxy(100, 80, "�����");
			outtextxy(190, 80, "������");
			outtextxy(290, 80, "Ŀ�ĵ�");
			outtextxy(370, 80, "����ʱ��");
			outtextxy(460, 80, "����ʱ��");
			outtextxy(550, 80, "��Ʊ����");
			// ���Ƶ�ǰҳ�ĺ�����Ϣ
			// TODO����Ӻ�����Ϣ
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
					s2 << "��" << fh << "Сʱ";
					std::stringstream s22;
					s22 << fm << "����";
					std::stringstream s3;
					s3 << curr->price << "Ԫ";
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
					s2 << "��" << fh << "Сʱ";
					std::stringstream s22;
					s22 << fm << "����";
					std::stringstream s3;
					s3 << curr->price << "Ԫ";
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
					s2 << "��" << fh << "Сʱ";
					std::stringstream s22;
					s22 << fm << "����";
					std::stringstream s3;
					s3 << curr->price << "Ԫ";
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

//��ȡ����������������Щ�����г��ֵĺ���id���Ӧ���ı��ļ�������������Ϣ��
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
					MessageBox(GetHWnd(), "�ڴ����ʧ�ܣ�", "��ʾ", MB_OK);
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
	// ���Ʊ��
	drawTable22(100, 40, 500, 400, 4, 5);

	// ���ñ�ͷ����
	settextstyle(20, 0, _T("΢���ź�"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(130, 80, _T("������"));
	outtextxy(230, 80, _T("Ŀ�ĵ�"));
	outtextxy(320, 80, _T("����ʱ��"));
	outtextxy(420, 80, _T("����ʱ��"));
	outtextxy(520, 80, _T("��Ʊ����"));

	outtextxy(130, 280, _T("������"));
	outtextxy(230, 280, _T("Ŀ�ĵ�"));
	outtextxy(320, 280, _T("����ʱ��"));
	outtextxy(420, 280, _T("����ʱ��"));
	outtextxy(520, 280, _T("��Ʊ����"));
	//��ȡ������ͨ��Buf�ҵ����� 
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
			s2 << "��" << fh << "Сʱ";
			std::stringstream s22;
			s22 << fm << "����";
			std::stringstream s3;
			s3 << curr->price << "Ԫ";
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
			s2 << "��" << fh << "Сʱ";
			std::stringstream s22;
			s22 << fm << "����";
			std::stringstream s3;
			s3 << curr->price << "Ԫ";
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
	// �����һҳ��ť��Ӧ�¼�
	bool nextPage = false;
	int startRow = 1;
	while (true)
	{
		// TODO�����Ƶ�ǰҳ�ĺ�����Ϣ

		FlushBatchDraw();
		settextstyle(20, 0, _T("΢���ź�"));
		std::stringstream s3;
		s3 << "��" << startRow << "ҳ";
		outtextxy(510, 450, s3.str().c_str());
		// ����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			sign();
			break;
		}

		if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "��һҳ"))
		{
			startRow++;
			cleardevice(); // ��ջ�ͼ����
			putimage(0, 0, &ima); // ���»��Ʊ���ͼ��
			// ���Ʊ��
			drawTable22(100, 40, 500, 400, 4, 5);
			// �����ͷ
			// ���ñ�ͷ����
			settextstyle(20, 0, _T("΢���ź�"));
			setbkmode(TRANSPARENT);
			settextcolor(BLACK);
			outtextxy(130, 80, _T("������"));
			outtextxy(230, 80, _T("Ŀ�ĵ�"));
			outtextxy(320, 80, _T("����ʱ��"));
			outtextxy(420, 80, _T("����ʱ��"));
			outtextxy(520, 80, _T("��Ʊ����"));

			outtextxy(130, 280, _T("������"));
			outtextxy(230, 280, _T("Ŀ�ĵ�"));
			outtextxy(320, 280, _T("����ʱ��"));
			outtextxy(420, 280, _T("����ʱ��"));
			outtextxy(520, 280, _T("��Ʊ����"));
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
					s2 << "��" << fh << "Сʱ";
					std::stringstream s22;
					s22 << fm << "����";
					std::stringstream s3;
					s3 << curr->price << "Ԫ";
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
					s2 << "��" << fh << "Сʱ";
					std::stringstream s22;
					s22 << fm << "����";
					std::stringstream s3;
					s3 << curr->price << "Ԫ";
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
//ע��������
void forget()
{
	cleardevice();
	putimage(0, 0, &ima);
	FlushBatchDraw();
	setcolor(WHITE);
	settextstyle(90, 0, "����");
	outtextxy((700 - textwidth("�û��˺�ע��")) / 2, (500 - textheight("�û��˺�ע��")) / 2 - 120, "�û��˺�ע��");
	settextstyle(60, 0, "�п�");
	outtextxy((700 - textwidth("�û���:")) / 4 - 56, (500 - textheight("�û���:")) / 2, "�û���:");
	outtextxy((700 - textwidth("����:")) / 4 - 5, (500 - textheight("����:")) / 2 + 90, "����:");

	int rectX = (960 - textwidth("�˺�")) / 4;
	int rectY = (690 - textheight("�˺�")) / 2;

	//�洢�˺�����
	strcpy(Buf, "");
	strcpy(Buff, "");
	char bufff[10] = { 0 };
	char name[20] = { 0 };
	char num[15] = { 0 };
	char id[20] = { 0 };

	setfillcolor(WHITE);
	fillrectangle((960 - textwidth("�������룿")) * 2 / 3 + 105, (690 - textheight("�������룿")) / 2 + 200, (960 - textwidth("�������룿")) / 2 + textwidth("�������룿") + 35, (690 - textheight("�������룿ע��")) / 2 + 180 + textheight("�������룿ע��"));
	settextcolor(BLUE);
	settextstyle(34, 0, "�п�");
	outtextxy((960 - textwidth("�������룿")) * 2 / 3, (690 - textheight("�������룿")) / 2 + 190, "�������룿");

	struct ticket* pd;
	pd = head;
	temp = (struct ticket*)malloc(sizeof(struct ticket));
	if (temp == NULL) {
		// �ڴ����ʧ�ܣ����д�����
	}

	while (true) {
		FlushBatchDraw();
		//����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			user();
			break;
		}
		//�����û���
		if (Button(button.hoverColor, button.defaultColor, 300, 235, 450, 280, 80, 40, Buf))
		{
			InputBox(Buf, 128, "�����������û���", "�û���", "", 69, 40);
			if (strcmp(Buf, "") == 0) {
				MessageBox(GetHWnd(), "�û�������Ϊ�գ�", "������ʾ", MB_OK);
				continue;
			}

			// ����ظ��û���
			pd = head;
			while (pd != NULL) {
				if (strcmp(Buf, pd->user) == 0) {
					MessageBox(GetHWnd(), "�����û����Ѵ��ڣ����������롣", "��ʾ", MB_OK);
					break;
				}
				pd = pd->next;
			}
			if (pd != NULL) {
				continue;
			}
		}
		//��������
		if (Button(button.hoverColor, button.defaultColor, 300, 320, 450, 365, 80, 40, Buff))
		{
			//�������
			InputBox(Buff, 128, "��������������", "����", "", 69, 40);
			Sleep(150);
			InputBox(bufff, 128, "���ٴ�������������", "����", "", 69, 40);
			Sleep(50);
			if (strcmp(Buff, bufff) != 0)
			{
				MessageBox(GetHWnd(), "��������������벻һ���������ԣ�", "��ʾ", MB_OKCANCEL);
				continue;
			}
		}
		//����ע��
		if (Button(button.hoverColor, button.defaultColor, 320, 400, 390, 440, 80, 40, "ע��"))
		{
			if (strcmp(Buf, "") != 0 && strcmp(Buff, "") != 0)
			{
				if (strcmp(Buff, bufff) == 0)
				{
					if (temp != NULL)
					{
						temp->next = NULL;
						MessageBox(GetHWnd(), "�����������óɹ�", "��ʾ", MB_OKCANCEL);
						InputBox(name, 128, "������������ʵ����", "����", "", 69, 40);
						int tryCount = 0;
						bool isPhoneNumberValid = false;
						while (tryCount < 3 && !isPhoneNumberValid) {
							char numm[15] = { 0 }; // ��num�ĳ�ʼ���Ƶ�ѭ���ڲ�
							InputBox(numm, 128, "���������ĵ绰����", "�绰����", "", 69, 40);
							//printf("%s", numm);
							if (isValidPhoneNumber(numm) == true) {
								strcpy(num, numm);
								isPhoneNumberValid = true;
							}
							else {
								//MessageBox(GetHWnd(), "���ĵ绰������������������", "��ʾ", MB_OKCANCEL);
								tryCount++;
							}
						}

						if (!isPhoneNumberValid) {
							MessageBox(GetHWnd(), "������ĵ绰���������ѳ�������Դ�����", "��ʾ", MB_OK);
							user();
							break;
						}

						int attempts = 0;
						bool inputValid = false;
						while (attempts < 3) {
							InputBox(id, 128, "�������������֤����", "���֤����", "", 69, 40);
							if (isValidIDNumber(id)) {
								inputValid = true;
								break;
							}
							else {
								//MessageBox(NULL, "��������Ч�����֤����", "����", MB_OK | MB_ICONERROR);
								attempts++;
							}
						}

						if (!inputValid) {
							MessageBox(GetHWnd(), "���Դ����þ���δ��������ȷ�����֤����", "����", MB_OK | MB_ICONWARNING);
							user();
							break;
						}
						if (strlen(name) == 0 || strlen(num) == 0 || strlen(id) == 0)
						{
							MessageBox(GetHWnd(), "�������绰��������֤����Ϊ�գ�������", "��ʾ", MB_OKCANCEL);
							continue;
						}
						if (strlen(Buf) < sizeof(temp->user) && strlen(Buff) < sizeof(temp->code))
						{
							strcpy(temp->user, Buf);
							strcpy(temp->code, Buff);
							strcpy(temp->name, name);
							strcpy(temp->phonenum, num);
							strcpy(temp->id, id);
							strcpy(temp->text, "���ް���");
						}
						else {
							MessageBox(GetHWnd(), "������������Ϣ��", "��ʾ", MB_OKCANCEL);
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

					MessageBox(GetHWnd(), "�����˺�ע��ɹ���", "��ʾ", MB_OKCANCEL);
					sign();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "��������˺Ż���������������", "��ʾ", MB_OKCANCEL);
				continue;
			}
		}
		//��������
		else if (Button(button.hoverColor, button.defaultColor, 505, 285, 640, 315, 80, 40, "����\n����"))
		{
			//���������Ϣ���˶�
			findcode();
			break;
		}
	}
}

// �ͷŶ���������ڴ�
void freeOrders(struct Order* orderList) {
	struct Order* currOrder = orderList;
	struct Order* nextOrder;

	while (currOrder != NULL) {
		nextOrder = currOrder->next;
		free(currOrder);
		currOrder = nextOrder;
	}
}

// �����������Ҷ��������ذ����������Ķ�������
struct Order* findOrdersByCustomerName(const char* customerName) {
	FILE* file = fopen("������Ϣ.txt", "r");
	if (file == NULL) {
		printf("�޷����ļ�\n");
		return NULL;
	}

	struct Order* resultHead = NULL;
	struct Order* resultTail = NULL;
	struct Order* temp;

	while (1) {
		temp = (struct Order*)malloc(sizeof(struct Order));
		if (temp == NULL) {
			printf("�ڴ����ʧ�ܣ�\n");
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
// ���ݺ����Ų��Ҷ��������ذ����ú����ŵĶ�������
struct Order* findOrdersByflightName(const char* flightName) {
	FILE* file = fopen("������Ϣ.txt", "r");
	if (file == NULL) {
		return NULL;
	}

	struct Order* hd = NULL;
	struct Order* tl = NULL;
	struct Order* tp;

	while (1) {
		tp = (struct Order*)malloc(sizeof(struct Order));
		if (tp == NULL) {
			printf("�ڴ����ʧ�ܣ�\n");
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
//����Ա ��ѯ������Ϣ
void queryOrder()
{
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(BLUE);
	settextstyle(20, 0, "����");
	FlushBatchDraw();
	while (1) {
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			Admin();
			break;
		}
		//1.��ѯ���еĶ���
		else if (Button(button.hoverColor, button.defaultColor, 235, 140, 460, 190, 20, 20, "��ѯȫ������"))
		{
			struct Order* filteredOrders = readOrdersFromFile();
			struct Order* curr = filteredOrders;

			int startRow = 1;
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(BLUE);
			FlushBatchDraw();
			settextstyle(20, 0, "����");

			// ���Ʊ��
			drawTable22(70, 40, 560, 400, 4, 4);

			outtextxy(109, 80, _T("�������"));
			outtextxy(248, 80, _T("������"));
			outtextxy(387, 80, _T("�û�����"));
			outtextxy(526, 80, _T("��������"));
			settextstyle(20, 0, _T("΢���ź�"));
			std::stringstream s3;
			s3 << "��" << startRow << "ҳ";
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
				if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����")) {
					cleardevice();
					putimage(0, 0, &mm);
					break;
				}

				if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "��һҳ")) {
					startRow++;
					cleardevice();
					putimage(0, 0, &ima);
					setcolor(BLUE);
					settextstyle(20, 0, "����");
					FlushBatchDraw();
					drawTable22(70, 40, 560, 400, 4, 4);
					outtextxy(109, 80, _T("�������"));
					outtextxy(248, 80, _T("������"));
					outtextxy(387, 80, _T("�û�����"));
					outtextxy(526, 80, _T("��������"));
					settextstyle(20, 0, _T("΢���ź�"));
					std::stringstream s3;
					s3 << "��" << startRow << "ҳ";
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

		//2.����������ѯ����
		else if (Button(button.hoverColor, button.defaultColor, 235, 210, 460, 260, 20, 20, "������ѯ����")) {
			char cid[23] = { 0 };
			InputBox(cid, 23, "����������", "����", "", 69, 40);

			struct Order* filteredOrders = findOrdersByCustomerName(cid);
			struct Order* curr = filteredOrders;

			int startRow = 1;
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(BLUE);
			FlushBatchDraw();
			settextstyle(20, 0, "����");

			drawTable22(70, 40, 560, 400, 4, 4);
			outtextxy(109, 80, _T("�������"));
			outtextxy(248, 80, _T("������"));
			outtextxy(387, 80, _T("�û�����"));
			outtextxy(526, 80, _T("��������"));
			settextstyle(20, 0, _T("΢���ź�"));
			std::stringstream s3;
			s3 << "��" << startRow << "ҳ";
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
				if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����")) {
					cleardevice();
					putimage(0, 0, &mm);
					break;
				}

				if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "��һҳ")) {
					startRow++;
					cleardevice();
					putimage(0, 0, &ima);
					setcolor(BLUE);
					FlushBatchDraw();
					settextstyle(20, 0, "����");

					drawTable22(70, 40, 560, 400, 4, 4);
					outtextxy(109, 80, _T("�������"));
					outtextxy(248, 80, _T("������"));
					outtextxy(387, 80, _T("�û�����"));
					outtextxy(526, 80, _T("��������"));
					settextstyle(20, 0, _T("΢���ź�"));
					std::stringstream s3;
					s3 << "��" << startRow << "ҳ";
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
		//3.���ݺ����Ų�ѯ����
		else if (Button(button.hoverColor, button.defaultColor, 235, 280, 460, 330, 20, 20, "�����ѯ����")) {
			char fid[23] = { 0 };
			InputBox(fid, 23, "�����뺽����", "������", "", 69, 40);

			struct Order* filteredOrders = findOrdersByflightName(fid);
			struct Order* curr = filteredOrders;

			int startRow = 1;
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(BLUE);
			FlushBatchDraw();
			settextstyle(20, 0, "����");

			drawTable22(70, 40, 560, 400, 4, 4);
			outtextxy(109, 80, _T("�������"));
			outtextxy(248, 80, _T("������"));
			outtextxy(387, 80, _T("�û�����"));
			outtextxy(526, 80, _T("��������"));
			settextstyle(20, 0, _T("΢���ź�"));
			std::stringstream s3;
			s3 << "��" << startRow << "ҳ";
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
				if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����")) {
					cleardevice();
					putimage(0, 0, &mm);
					break;
				}

				if (Button(button.hoverColor, button.defaultColor, 580, 455, 680, 485, 80, 30, "��һҳ")) {
					startRow++;
					cleardevice();
					putimage(0, 0, &ima);
					setcolor(BLUE);
					settextstyle(20, 0, "����");
					FlushBatchDraw();
					drawTable22(70, 40, 560, 400, 4, 4);
					outtextxy(109, 80, _T("�������"));
					outtextxy(248, 80, _T("������"));
					outtextxy(387, 80, _T("�û�����"));
					outtextxy(526, 80, _T("��������"));
					settextstyle(20, 0, _T("΢���ź�"));
					std::stringstream s3;
					s3 << "��" << startRow << "ҳ";
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

//��ѯ�ͻ���Ϣ 
void queryCustomer() {
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(YELLOW);
	settextstyle(80, 0, "����");
	outtextxy((700 - textwidth("�������û�����")) / 2, 100, "�������û�����");
	FlushBatchDraw();
	char name[20] = { 0 }, id[20] = { 0 };
	bool make = true;
	while (true) {
		if (make == false)
		{
			cleardevice();
			putimage(0, 0, &ima);
			setcolor(YELLOW);
			settextstyle(80, 0, "����");
			outtextxy((700 - textwidth("�������û�����")) / 2, 100, "�������û�����");
			make = true;
		}
		FlushBatchDraw();
		//����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			Admin();
			break;
		}
		//�����û���
		if (Button(button.hoverColor, button.defaultColor, 300, 235, 450, 280, 20, 20, name))
		{
			InputBox(name, 128, "�������û���", "�û���", "", 69, 40);
			if (strcmp(name, "") == 0) {
				MessageBox(GetHWnd(), "�û�������Ϊ�գ�", "������ʾ", MB_OK);
				continue;
			}
			bool inputValid = false;
			while (!inputValid) {
				InputBox(id, 128, "�������û����֤����", "���֤����", "", 69, 40);
				if (isValidIDNumber(id)) {
					inputValid = true;
					break;
				}
			}
		}
		//��ѯ 
		if (Button(button.hoverColor, button.defaultColor, 320, 320, 425, 360, 80, 40, "��ѯ"))
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
						if (strcmp(temp->text, "���ް���") == 0)
						{
							strcpy(ee, "���ް���");
						}
						else {
							strcpy(ee, temp->text);
						}
						cleardevice();
						putimage(0, 0, &ima);
						while (1) {
							FlushBatchDraw();
							setcolor(WHITE);
							settextstyle(70, 0, "������κ");
							outtextxy((700 - textwidth("������Ϣ")) / 2, 30, "������Ϣ");
							drawTable();
							drawText("�û�����", 100, 110, 0, 0);
							drawText("�û���", 100, 110, 1, 0);
							drawText("�˺�״̬", 100, 110, 2, 0);
							drawText("�ֻ�����", 100, 110, 3, 0);
							drawText("���֤����", 100, 110, 4, 0);
							drawText("���ల��", 100, 110, 5, 0);
							drawText(aa, 600, 110, 0, 1);
							drawText(bb, 600, 110, 1, 1);
							drawText("����", 600, 110, 2, 1);
							drawText(cc, 600, 110, 3, 1);
							drawText(dd, 600, 110, 4, 1);
							drawText(ee, 600, 110, 5, 1);

							// ����
							if (Button(button.hoverColor, button.defaultColor, 15, 470, 120, 510, 80, 40, "����"))
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
					MessageBox(GetHWnd(), "δ�ҵ��ÿͻ���", "��ʾ", MB_OK);
					Admin();
					break;
				}
			}
			else {
				MessageBox(GetHWnd(), "�������������Ƿ���ȷ��", "��ʾ", MB_OK);
				continue;
			}
		}
	}

}

//ȡ������     
void cancelOrder() {
	cleardevice();
	putimage(0, 0, &ima);
	setcolor(BLUE);
	settextstyle(80, 0, "����");
	outtextxy((700 - textwidth("ȡ������")) / 2, 100, "ȡ������");
	FlushBatchDraw();
	read();
	temp = head;
	char fnum[20] = { 0 };
	char cn[20] = { 0 };
	char ccode[10] = { 0 };
	while (true) {
		FlushBatchDraw();
		//����
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			Admin();
			break;
		}
		if (Button(button.hoverColor, button.defaultColor, 235, 235, 460, 280, 20, 20, "ȡ�����ඩ��"))
		{
			InputBox(cn, 180, "��������Ҫȡ�����ඩ���Ŀͻ�������", "��ʾ", "", 69, 40);
			InputBox(ccode, 180, "��������Ҫȡ�����ඩ���Ŀͻ������֤", "��ʾ", "", 69, 40);
			InputBox(fnum, 180, "��������Ҫȡ�������ĺ�����", "��ʾ", "", 69, 40);
			if (deleteOrderById(fnum, cn)) {
				MessageBox(GetHWnd(), "����ȡ���ɹ���", "��ʾ", MB_OK);
				//��������
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
				MessageBox(GetHWnd(), "�ú����Ŷ�Ӧ�ĺ��಻���ڣ�", "��ʾ", MB_OK);
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
	settextstyle(90, 0, "����");
	outtextxy((700 - textwidth("�û�")) / 2, 60, "�û�");

	settextstyle(50, 0, "�п�");
	outtextxy((700 - textwidth("�˺�:")) / 3, (500 - textheight("�˺�:")) / 2 - 30, "�˺�:");
	outtextxy((700 - textwidth("����:")) / 3, (500 - textheight("����:")) / 2 + 50, "����:");

	settextcolor(BLACK);
	settextstyle(30, 0, "������κ");
	outtextxy((700 - textwidth("�������룿ע��")) * 2 / 3 + 105, (500 - textheight("�������룿ע��")) / 2 + 205, "�������룿ע��");
	settextcolor(WHITE);
	line((700 - textwidth("�������룿ע��")) * 2 / 3 + 105, (500 - textheight("�������룿ע��")) / 2 + 205 + textheight("�������룿ע��"), (700 - textwidth("�������룿ע��")) * 2 / 3 + 105 + textwidth("�������룿ע��"), (500 - textheight("�������룿ע��")) / 2 + 205 + textheight("�������룿ע��"));

	read();
	//��������
	strcpy(Buf, "");
	strcpy(Buff, "");
	char bug[10] = { 0 };

	bool loggedIn = false;
	while (!loggedIn) {
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			return 0;
		}
		//�ж�
		if (Button(button.hoverColor, button.defaultColor, 330, 205, 500, 245, 80, 40, Buf))
		{//����˺�
			InputBox(Buf, 128, "�����������˺�", "����", "", 69, 40);
		}
		else if (Button(button.hoverColor, button.defaultColor, 330, 285, 500, 325, 80, 40, bug))
		{//�������
			InputBox(Buff, 128, "��������������", "����", "", 69, 40);
			if (strcmp(Buff, "") != 0)
			{
				strcpy(bug, "******");
			}
		}
		else if (Isin((700 - textwidth("�������룿ע��")) * 2 / 3 + 105, (500 - textheight("�������룿ע��")) / 2 + 185 + textheight("�������룿ע��"), (700 - textwidth("�������룿ע��")) * 2 / 3 + 105 + textwidth("�������룿ע��"), (500 - textheight("�������룿ע��")) / 2 + 205 + textheight("�������룿ע��")))
		{//�����������
			settextcolor(RED);
			settextstyle(30, 0, "������κ");
			outtextxy((700 - textwidth("�������룿ע��")) * 2 / 3 + 105, (500 - textheight("�������룿ע��")) / 2 + 205, "�������룿ע��");
			if (Isclick((700 - textwidth("�������룿ע��")) * 2 / 3 + 105, (500 - textheight("�������룿ע��")) / 2 + 185 + textheight("�������룿ע��"), (700 - textwidth("�������룿ע��")) * 2 / 3 + 105 + textwidth("�������룿ע��"), (500 - textheight("�������룿ע��")) / 2 + 205 + textheight("�������룿ע��")))
			{
				forget();
				break;
			}
		}
		else if (!Isin((700 - textwidth("�������룿ע��")) * 2 / 3 + 105, (500 - textheight("�������룿ע��")) / 2 + 185 + textheight("�������룿ע��"), (700 - textwidth("�������룿ע��")) * 2 / 3 + 105 + textwidth("�������룿ע��"), (500 - textheight("�������룿ע��")) / 2 + 205 + textheight("�������룿ע��")))
		{
			settextcolor(BLACK);
			settextstyle(30, 0, "������κ");
			outtextxy((700 - textwidth("�������룿ע��")) * 2 / 3 + 105, (500 - textheight("�������룿ע��")) / 2 + 205, "�������룿ע��");
		}
		if (Button(button.hoverColor, button.defaultColor, 310, 370, 390, 410, 80, 40, "��¼"))
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
					MessageBox(GetHWnd(), "�����˺�������������", "��ʾ", MB_OK);
					strcpy(Buf, "");
					strcpy(Buff, "");
					strcpy(bug, "");
					continue;
				}
			}
		}

	}
}
//����Ա����
void Admin()
{
	cleardevice();
	putimage(0, 0, &ge);
	//FlushBatchDraw();
	int ff = 0;
	while (true) {
		FlushBatchDraw();
		setcolor(BLACK);
		settextstyle(90, 0, "������κ");
		outtextxy((700 - textwidth("��ӭ��������Ա")) / 2, 60, "��ӭ��������Ա");
		if (Button(button.hoverColor, button.defaultColor, 160, 180, 280, 220, 20, 20, "���Ӻ���")) {
			//1. ���Ӻ�����Ϣ������Ա��Ȩ��
			addFlight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 180, 560, 220, 20, 20, "ȡ������")) {
			//2. ȡ�����ࣨ����Ա��Ȩ��
			cancelFlight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 230, 280, 270, 20, 20, "ȡ������")) {
			//3. ȡ������
			cancelOrder();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 230, 560, 270, 20, 20, "��ѯ�ͻ�")) {
			//4. ��ѯ�ͻ���Ϣ
			queryCustomer();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 280, 280, 320, 20, 20, "��ѯ����")) {
			//5. ��ѯ������Ϣ
			queryFlight();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 280, 560, 320, 20, 20, "��ѯ����")) {
			//6. ��ѯ������Ϣ
			queryOrder();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 160, 330, 280, 370, 20, 20, "��ӿͻ�")) {
			//7. ��ӿͻ���Ϣ
			addCustomer();
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 440, 330, 560, 370, 20, 20, "�˳�����")) {
			//8. �˳�����Աģʽ
			MessageBox(GetHWnd(), "��лʹ�ù���Աģʽ���ټ���", "��ʾ", MB_OK);
			ff = 1;
			break;
		}
		else if (Button(button.hoverColor, button.defaultColor, 300, 390, 420, 430, 20, 20, "�˳�ϵͳ")) {
			//9. �˳�airFlight��Ʊ����ϵͳ
			MessageBox(GetHWnd(), "��л����ʹ�ã��´��ټ���", "��ʾ", MB_OK);
			return exit(66);
		}
	}
	if (ff == 1)
	{
		menu();
	}
}
//����Ա
int administrator() {

	cleardevice();
	putimage(0, 0, &mm);
	FlushBatchDraw();
	setcolor(BLACK);
	settextstyle(90, 0, "����");
	outtextxy((700 - textwidth("����Ա")) / 2, 60, "����Ա");
	settextstyle(50, 0, "����");
	outtextxy((700 - textwidth("����")) / 3 + 20, 270, "���룺");
	FlushBatchDraw();
	strcpy(password, "");
	char hidden[10] = { 0 };
	int ff = 0;
	bool loggedIn = false;
	while (!loggedIn) {
		FlushBatchDraw();
		if (Button(button.hoverColor, button.defaultColor, 15, 450, 120, 490, 80, 40, "����"))
		{
			return 0;
		}
		if (Button(button.hoverColor, button.defaultColor, 345, 280, 500, 315, 80, 40, hidden)) {
			// �������Ա����
			InputBox(password, 20, "���������Ա���룺", "����", "", 69, 40);
			if (strcmp(password, "") != 0)
			{
				strcpy(hidden, "******");
			}

		}
		if (Button(button.hoverColor, button.defaultColor, 305, 360, 390, 400, 80, 40, "��¼"))
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
			// ����Ա����
			pp = administrator();
			pp = 0;
			break;
		case 2:
			// �û�����
			pp = user();
			pp = 0;
			break;
		case 3:

			clrscr();
			FlushBatchDraw();
			MessageBox(GetHWnd(), "��лʹ��airFlight��Ʊ����ϵͳ���ټ���", "��ʾ", MB_OK);
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