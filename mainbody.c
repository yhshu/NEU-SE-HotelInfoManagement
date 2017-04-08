/*
旅馆信息管理 mainbody.c
1.	要求实现旅客的入住，换房和退房，
	查看旅客和房间信息，
	以及从文件读取相关信息，将相关信息储存到文件的功能，从而实现旅馆信息管理的基本功能。
2.	要求用两个结构体分别存储旅客和房间的信息。用一个枚举类型定义房间的类型。
3.	使用结构数组来实现旅客入住，换房，退房三种行为。
4.	能将文件中的信息读到结构体中，能将结构体的信息保存到文件中。
*/

#include "Structs.h"

extern int guestNum = 0;//入住旅客数 全局整型变量声明

void main()
{
	InitializeLinkedList();//初始化链表
	LoadTheme();//加载主题
	LoadGuestInfo();
	LoadGARInfo();
	LoadHotelInfo();//从文件中读取房间信息到Rooms链表中
	HandleMain();
}

//函数定义
void PrintMain()
{
	printf("********************\n");
	printf("1 查看旅店信息\n");
	printf("2 查看某一房间信息\n");
	printf("3 查看旅客信息\n");
	printf("4 查找某一旅客信息\n");
	printf("5 旅客入住\n");
	printf("6 旅客换房\n");
	printf("7 旅客退房\n");
	printf("8 保存信息并退出\n");
	printf("9 更换主题\n");
	printf("********************\n");
}

void printline()
{
	printf("\n\n");
}

void HandleMain()
{
	while (1)
	{
		PrintMain();
		int choice = 0;
		scanf("%d", &choice);
		fflush(stdin);//清除读写缓存区
		if (choice <= 9 && choice >= 1)
		{
			switch (choice)
			{
			case 1: //查看旅店信息
				printline();
				ViewAllHotel();
				printline();
				break;
			case 2: //查看某一房间信息
				printline();
				ViewOneRoom();
				printline();
				break;
			case 3: //查看旅客信息
				ViewAllGuest();
				printline();
				break;
			case 4: //查找某一旅客信息
				printline();
				ViewOneGuest();
				printline();
				break;
			case 5: //旅客入住
				printline();
				GuestCheckIn();
				printline();
				break;
			case 6: //旅客换房
				GuestChangeRoom();
				printline();
				break;
			case 7: //旅客退房
				GuestCheckOut();
				printline();
				break;
			case 8: //保存信息并退出
				SaveGARInfo();
				SaveGuestInfo();
				SaveHotelInfo();
				exit(0);
			case 9:
				ChangeTheme();
				break;
			default:
				break;
			}
		}
		else
		{
			printline();
			continue;
		}
	}
}

void InitializeLinkedList()//初始化链表
{
	guest_head = (struct Guests *)malloc(sizeof(struct Guests));
	guest_head->next = NULL;
	guest_tail = guest_head;

	room_head = (struct Rooms *)malloc(sizeof(struct Rooms));
	room_head->next = NULL;
	room_tail = room_head;

	GAR_head = (struct GuestAndRoom *)malloc(sizeof(struct GuestAndRoom));
	GAR_head->next = NULL;
	GAR_tail = GAR_head;
}

void ChangeTheme()
{
	int choice = 0;

	printf("********************\n");
	printf("1.黑色背景\n");
	printf("2.蓝色背景\n");
	printf("3.绿色背景\n");
	printf("4.湖蓝色背景\n");
	printf("5.红色背景\n");
	printf("6.紫色背景\n");
	printf("7.棕色背景\n");
	printf("8.灰色背景\n");
	printf("9.白色背景\n");
	printf("********************\n");

	while (1)
	{
		scanf("%d", &choice);
		if (choice >= 1 && choice <= 9)
			break;
	}

	FILE* fp;
	if ((fp = fopen("Theme.txt", "w")) == NULL)
	{
		printf("主题信息获取失败\n");
		exit(0);
	}

	switch (choice)
	{
	default: break;
	case 1:
		system("color 0F");
		fprintf(fp, "%s", "0F");
		break;
	case 2:
		system("color 1F");
		fprintf(fp, "%s", "1F");
		break;
	case 3:
		system("color 2F");
		fprintf(fp, "%s", "2F");
		break;;
	case 4:
		system("color 3F");
		fprintf(fp, "%s", "3F");
		break;
	case 5:
		system("color 4F");
		fprintf(fp, "%s", "4F");
		break;
	case 6:
		system("color 5F");
		fprintf(fp, "%s", "5F");
		break;
	case 7:
		system("color 6F");
		fprintf(fp, "%s", "6F");
		break;
	case 8:
		system("color 70");
		fprintf(fp, "%s", "70");
		break;
	case 9:
		system("color F0");
		fprintf(fp, "%s", "F0");
	}

	fclose(fp);
}

void LoadTheme()
{
	FILE* fp;
	if ((fp = fopen("Theme.txt", "r")) == NULL)
	{
		printf("主题信息读取失败.\n");
		exit(0);
	}
	char color[10] = {0};
	fscanf(fp, "%s", color);
	if (strcmp(color, "0F") == 0)
	{
		system("color 0F");
	}
	else if (strcmp(color, "1F") == 0)
	{
		system("color 1F");
	}
	else if (strcmp(color, "2F") == 0)
	{
		system("color 2F");
	}
	else if (strcmp(color, "3F") == 0)
	{
		system("color 3F");
	}
	else if (strcmp(color, "4F") == 0)
	{
		system("color 4F");
	}
	else if (strcmp(color, "5F") == 0)
	{
		system("color 5F");
	}
	else if (strcmp(color, "6F") == 0)
	{
		system("color 6F");
	}
	else if (strcmp(color, "70") == 0)
	{
		system("color 70");
	}
	else if (strcmp(color, "F0") == 0)
	{
		system("color F0");
	}
	fclose(fp);
}