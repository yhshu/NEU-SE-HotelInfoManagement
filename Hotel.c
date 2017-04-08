/*
旅馆信息管理 Hotel.c
*/

#include"Structs.h"

void LoadHotelInfo()//从文件中读取房间信息到Rooms链表中
{
	FILE* fp;
	char title[30] = "";
	if ((fp = fopen("Hotel.txt", "r")) == NULL)
	{
		printf("文件读取失败");
		exit(0);
	}
	fscanf(fp, "%s %s %s\n", title, title, title);
	while (!feof(fp))
	{
		struct Rooms* insert = (struct Rooms *)malloc(sizeof(struct Rooms));
		fscanf(fp, "%d%d%d\n", &insert->roomNum, &insert->roomtype, &insert->roomPrice);
		//通过遍历GuestAndRoom链表获取每个房间的人数
		insert->checkinNum = 0;
		struct GuestAndRoom* GAR_cur = GAR_head->next;;
		while (GAR_cur != NULL)
		{
			if (GAR_cur->guestRoom == insert->roomNum)
				insert->checkinNum++;
			GAR_cur = GAR_cur->next;
		}
		room_tail->next = insert;
		insert->next = NULL;
		room_tail = insert;
	}
	fclose(fp);
}

void SaveHotelInfo()//将现有的Rooms链表中的信息保存到文件
{
	FILE* fp;
	if ((fp = fopen("Hotel.txt", "w")) == NULL)
	{
		printf("文件读取失败");
		exit(0);
	}
	fprintf(fp, "房间号 房间类型 房间价格");
	struct Rooms* cur = room_head->next;
	while (cur != NULL)
	{
		fprintf(fp, "\n%d %d %d", cur->roomNum, cur->roomtype, cur->roomPrice);
		cur = cur->next;
	}
	fclose(fp);
}

void ViewAllHotel()//查看所有房间入住情况
{
	printf("房间号\t房间类型\t入住人数\t房间价格\t\n");
	struct Rooms* cur = room_head->next;//遍历Rooms链表
	while (cur != NULL)
	{
		struct GuestAndRoom* GAR_cur = GAR_head->next;
		cur->checkinNum = 0;
		while (GAR_cur != NULL)
		{
			if (GAR_cur->guestRoom == cur->roomNum)
				cur->checkinNum++;
			GAR_cur = GAR_cur->next;
		}
		printf("%d\t", cur->roomNum);
		switch (cur->roomtype)
		{
		case 1:
			printf("单人房\t");
			break;
		case 2:
			printf("双人房\t");
			break;
		case 3:
			printf("三人房\t");
			break;
		}
		printf("\t%d\t\t%d\n", cur->checkinNum, cur->roomPrice);
		cur = cur->next;
	}
	printf("\n");
}

void ViewOneRoom()//查看某房间旅客信息
{
	int roomchoice;
	int findRoom = 0;//判断房间是否存在
	int flag = 0;//旅客信息输出则改为1，无旅客信息输出为0
	printf("请输入房间号：");
	scanf("%d", &roomchoice);
	struct GuestAndRoom* GAR_cur = GAR_head->next;
	while (GAR_cur != NULL)
	{
		if (GAR_cur->guestRoom == roomchoice)//通过房间号在GAR链表中找到入住时间和身份证
		{
			struct Guests* guest_cur = guest_head->next;
			while (guest_cur != NULL)
			{
				if (strcmp(guest_cur->ID, GAR_cur->guestID) == 0)//通过身份证找到Guests链表中的姓名、性别
				{
					printf("姓名:%s 身份证:%s 性别:%s 入住时间:%s\n", guest_cur->name, guest_cur->ID, guest_cur->sex, GAR_cur->checkInTime);
					flag = 1; 
					findRoom = 1;
				}
				guest_cur = guest_cur->next;
			}
		}
		GAR_cur = GAR_cur->next;
	}
	if (!flag)
	{
		struct Rooms* room_cur = room_head->next;
		while(room_cur!=NULL)
		{
			if (room_cur->roomNum == roomchoice && room_cur->checkinNum == 0)
			{
				printline();
				printf("该房间空闲.\n");
				findRoom = 1;
				break;
			}
			room_cur = room_cur->next;
		}
	}
	if(!findRoom)
	{
		printf("请输入正确的房间号：\n");
		ViewOneRoom();
	}
}

void CntCheckinNum()//统计各房间人数
{
	struct Rooms* cur = room_head->next;//遍历Rooms链表
	while (cur != NULL)
	{
		struct GuestAndRoom* GAR_cur = GAR_head->next;
		cur->checkinNum = 0;
		while (GAR_cur != NULL)
		{
			if (GAR_cur->guestRoom == cur->roomNum)
				cur->checkinNum++;
			GAR_cur = GAR_cur->next;
		}
		cur = cur->next;
	}
}

int ViewAvaRoom()//显示可用房间
{
	CntCheckinNum();
	FILE* fp;
	char title[10] = {0};
	int roomNum = 0;
	int roomtype = 0;
	int roomPrice = 0;
	int flag = 0;
	if ((fp = fopen("Hotel.txt", "r")) == NULL)
	{
		printf("房间信息读取失败.");
		exit(0);
	}
	fscanf(fp, "%s %s %s", title, title, title);
	printf("可入住的房间：\n");
	while (!feof(fp))
	{
		fscanf(fp, "%d %d %d", &roomNum, &roomtype, &roomPrice);
		struct Rooms* cur = room_head->next;//遍历Rooms链表
		while (cur != NULL)
		{
			if (roomNum == cur->roomNum && roomtype > (cur->checkinNum))
			{
				printf("房间号：%d 房间类型：", roomNum);
				switch (cur->roomtype)
				{
				case 1:
					printf("单人房 ");
					break;
				case 2:
					printf("双人房 ");
					break;
				case 3:
					printf("三人房 ");
					break;
				}
				printf("房间价格：%d\t", cur->roomPrice);
				if (cur->checkinNum != 0)
					printf("已有人入住\n");
				else
					printf("空闲\n");
				flag = 1;
			}
			cur = cur->next;
		}
	}
	if (!flag)
	{
		printf("抱歉，旅馆暂无可用房间，欢迎下次光临！\n");
		return 0;//无可用房间
	}
	else return 1;//有可用房间
}
