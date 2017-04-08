/*
计算机类二 1613班 20165248 舒意恒
旅馆信息管理 Structs.h
该文件用于列举整个程序的头文件、结构体定义和函数声明等信息。
*/

//程序包含的头文件、宏定义
#include <stdio.h>//标准输入输出
#include <stdlib.h>//标准库
#include <conio.h>//控制台输入输出
#include <string.h>
#include <time.h>
#include <locale.h>

//结构体定义
typedef enum roomType{Single = 1, Double = 2, Triangle = 3}roomType;//枚举定义房间类型

struct Rooms
{
	int roomNum;//房间号
	int checkinNum;//入住人数
	int roomPrice;//房间价格
	roomType roomtype;//房间类型
	struct Rooms *next;
};//储存房间信息的链表
struct Rooms *room_head, *room_tail;

struct Guests
{
	char name[20];//旅客姓名
	char ID[25];//旅客身份证号
	char sex[10];//旅客性别
	struct Guests *next;
};//储存旅客信息的链表
struct Guests  *guest_head, *guest_tail;

struct GuestAndRoom
{
	char guestID[20];//旅客身份证号
	int guestRoom;//旅客入住的房间
	char checkInTime[30];//入住时间
	char checkOutTime[30];//退房时间
	struct GuestAndRoom *next;
};
struct GuestAndRoom *GAR_head, *GAR_tail;

//mainbody.c函数声明
void PrintMain();//打印菜单函数
void HandleMain();//处理菜单函数
void printline();//换两行
void InitializeLinkedList();//初始化链表
void ChangeTheme();//更换命令行的主题
void LoadTheme();//加载主题

//Hotel.c函数声明
void LoadHotelInfo();//从文件中读取房间信息到Rooms链表中
void SaveHotelInfo();//将现有的Rooms链表中的信息保存到文件
void ViewAllHotel();//查看所有房间入住情况
void ViewOneRoom();//查看某房间旅客信息
int ViewAvaRoom();//显示可用房间
void CntCheckinNum();//统计各房间人数

//Guest.c函数声明
void LoadGuestInfo();//从文件中读取旅客信息到Guests链表中
void SaveGuestInfo();//将现有的Guests链表中的信息保存到文件
void ViewOneGuest();//查看某旅客信息
void ViewAllGuest();//查看所有旅客信息
int IsOldGuest(char ID[]);//判定是否为老顾客
int UnitPrice(int roomChoice);//查找房间单价

//GAR.c函数声明
void GuestCheckIn();//旅客入住
void GuestChangeRoom();//旅客换房
void GuestCheckOut();//旅客退房
void LoadGARInfo();//从文件读取旅客与房间信息到链表GuestAndRoom
void SaveGARInfo();//从当前GuestAndRoom链表保存旅客与房间信息到文件
