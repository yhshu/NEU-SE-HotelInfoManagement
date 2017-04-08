/*
旅馆信息管理 Guests.c
*/

#include"Structs.h"

extern int guestNum;

void LoadGuestInfo()//将文件中的旅客信息载入到Guests结构体数组中
{
	FILE* fp;
	char title[20] = "";
	if ((fp = fopen("Guest.txt", "r")) == NULL)
	{
		printf("文件读取失败");
		exit(0);
	}
	fscanf(fp, "%s %s %s", title, title, title);
	while (!feof(fp))//EOF之前返回假，直到EOF返回真
	{
		struct Guests* insert = (struct Guests *)malloc(sizeof(struct Guests));//每次插入信息到链表，都需要申请新的内存。
		fscanf(fp, "%s %s %s", insert->name, insert->sex, insert->ID);
		guest_tail->next = insert;
		insert->next = NULL;
		guest_tail = insert;
	}
	//计算文件中存储的旅客总数
	struct Guests* cur = guest_head->next;
	while (cur != NULL)
	{
		guestNum++;
		cur = cur->next;
	}
	fclose(fp);
}

void SaveGuestInfo(int LocalGuestNum)//Guests结构体数组中的信息保存到文件
{
	FILE* fp;
	if ((fp = fopen("Guest.txt", "w")) == NULL)
	{
		printf("文件读取失败");
		exit(0);
	}
	fprintf(fp, "%s %s %s", "姓名", "性别", "身份证");
	struct Guests* cur = guest_head->next;
	while (cur != NULL)
	{
		fprintf(fp, "\n%s %s %s", cur->name, cur->sex, cur->ID);
		cur = cur->next;
	}
	fclose(fp);
}

void ViewOneGuest()//查看某旅客信息
{
	char IDchoice[25];
	int flag = 0;//找到为1，未找到为0
	struct Guests* cur = guest_head->next;
	printf("请输入旅客身份证号:");
	scanf("%s", &IDchoice);
	while (cur != NULL)
	{
		if (strcmp(cur->ID, IDchoice) == 0)//通过身份证号在Guests链表中找到旅客姓名、性别
		{
			struct GuestAndRoom* GAR_cur = GAR_head->next;
			while (GAR_cur != NULL)
			{
				if (strcmp(GAR_cur->guestID, IDchoice) == 0)//通过身份证号在GuestAndRoom链表中找到房间号、入住时间
				{
					flag = 1;
					printf("姓名\t性别\t身份证\t\t\t房间\t入住时间");
					printf("\n%s\t%s\t%s\t%d\t%s\t", cur->name, cur->sex, IDchoice, GAR_cur->guestRoom, GAR_cur->checkInTime);
					break;
				}
				GAR_cur = GAR_cur->next;
			}
			break;
		}
		cur = cur->next;
	}
	if (!flag)
	{
		printline();
		printf("未找到该旅客.");
	}
}

void ViewAllGuest()//查看所有旅客信息
{
	printline();
	printf("-----------------------------共有%d名旅客----------------------------\n", guestNum);
	printf("----------------------------所有旅客信息----------------------------\n");
	printf("姓名\t性别\t身份证\t\t\t房间\t入住时间");
	struct Guests* cur = guest_head->next;//遍历Guest链表，输出旅客信息。
	while (cur != NULL)
	{
		struct GuestAndRoom* GAR_cur = GAR_head->next;
		int guestRoom = 0;
		char checkInTime[30] = {0};
		while (GAR_cur != NULL)//根据旅客信息，通过链表遍历，在GuestAndRoom链表中查询房间号和入住时间
		{
			if (strcmp(GAR_cur->guestID, cur->ID) == 0)//字符串不能直接用==比较，必须使用strcmp函数
			{
				guestRoom = GAR_cur->guestRoom;
				strcpy(checkInTime, GAR_cur->checkInTime);
			}
			GAR_cur = GAR_cur->next;
		}
		printf("\n%s\t%s\t%s\t%d\t%s", cur->name, cur->sex, cur->ID, guestRoom, checkInTime);
		cur = cur->next;
	}
	printf("\n--------------------------------完----------------------------------");
}

int IsOldGuest(char IDtar[])//判定是否为老顾客
{
	//形参char IDtar[]即要查找的身份证号
	FILE* fp;
	char title[30] = {0};
	char name[20] = {0};
	char ID[25] = {0};
	int roomNum;
	char checkTime[60] = {0};
	int historyTimes = 0;//历史入住次数
	if ((fp = fopen("GuestHistoryInfo.txt", "r")) == NULL)
	{
		printf("旅客历史信息无法读取.");
		exit(0);
	}
	fscanf(fp, "%s%s%s%s%s", title, title, title, title, title);//姓名 身份证 房间号 入住时间 退房时间
	while (!feof(fp))
	{
		fscanf(fp, "%s %s %d ", name, ID, &roomNum);//注意整型的取地址
		fgets(checkTime, 51, fp);
		if (strcmp(IDtar, ID) == 0)//比对身份证号
			historyTimes++;
	}
	fclose(fp);
	return historyTimes;
}

int UnitPrice(int roomChoice)//查找房间单价
{
	FILE* fp;
	if ((fp = fopen("Hotel.txt", "r")) == NULL)
	{
		printf("房间信息读取失败.");
		exit(0);
	}
	char title[30] = {0};
	int roomNum = 0;
	int roomtype = 0;
	int roomPrice = 0;

	fscanf(fp, "%s%s%s", title, title, title);
	while (!feof(fp))
	{
		fscanf(fp, "%d %d %d", &roomNum, &roomtype, &roomPrice);
		if (roomChoice == roomNum)
			return roomPrice;
	}
	fclose(fp);
	return 0;
}
