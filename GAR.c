/*
旅馆信息管理 GAR.c
GAR:GuestAndHotel
该文件定义了关于GuestAndRoom链表的函数。
*/

#include"Structs.h"

extern int guestNum;

void LoadGARInfo()//从文件读取旅客与房间信息到链表GuestAndRoom
{
	FILE* fp;
	char title[20] = "";
	if ((fp = fopen("GuestAndRoom.txt.", "r")) == NULL)
	{
		printf("文件读取失败");
		exit(0);
	}
	fscanf(fp, "%s %s %s %s\n", title, title, title, title);
	while (!feof(fp))//此处的feof条件不严格，因为文件末尾留有空格，因此函数中对fscanf的返回值进行了判断。
	{
		struct GuestAndRoom* insert = (struct GuestAndRoom *)malloc(sizeof(struct GuestAndRoom));
		if (fscanf(fp, "%d %s ", &insert->guestRoom, insert->guestID) != 2)
		{
			free(insert);
			break;
		}
		fgets(insert->checkInTime, 25, fp);
		strcpy(insert->checkOutTime, " ");
		GAR_tail->next = insert;
		insert->next = NULL;
		GAR_tail = insert;
	}
	fclose(fp);
}

void SaveGARInfo()
{
	FILE* fp;
	if ((fp = fopen("GuestAndRoom.txt", "w")) == NULL)
	{
		printf("文件读取失败");
		exit(0);
	}
	fprintf(fp, "%s %s %s %s", "房间", "身份证", "入住时间", "退房时间");
	struct GuestAndRoom* cur = GAR_head->next;
	while (cur != NULL)
	{
		fprintf(fp, "\n%d %s %s %s", cur->guestRoom, cur->guestID, cur->checkInTime, cur->checkOutTime);
		cur = cur->next;
	}
	fclose(fp);
}

void GuestCheckIn()//旅客入住
{
	int roomChoice = 0;
	int sexchoice = 0;
	char name_input[20] = {0};
	char ID_insert[20] = {0};
	char sex[10] = {0};
	char time_insert[30] = {0};//局部数组初始化赋空值！
	time_t t = time(NULL);//用于记录入住时间

	printf("====================房间信息====================\n");
	ViewAvaRoom();//显示可用房间
	printline();
	printf("请输入旅客身份证号：\n");
	scanf("%s", ID_insert);
	//检查是否已经入住过
	struct Guests* cur = guest_head->next;
	while (cur != NULL)
	{
		if (strcmp(cur->ID, ID_insert) == 0)
		{
			printf("该旅客已登记入住.已入住的旅客可选择换房.");
			return;//结束执行当前函数
		}
		cur = cur->next;
	}
	//添加信息到Guests链表
	struct Guests* guest_insert = (struct Guests *)malloc(sizeof(struct Guests));
	strcpy(guest_insert->ID, ID_insert);
	printf("请输入旅客姓名：\n");
	scanf("%s", name_input);
	strcpy(guest_insert->name, name_input);
	printf("请选择性别：\n1 男  2 女\n");
	scanf("%d", &sexchoice);
	switch (sexchoice)
	{
	default:
		break;
	case 1:
		strcpy(guest_insert->sex, "男");
		break;
	case 2:
		strcpy(guest_insert->sex, "女");
		break;
	}
	strcpy(sex, guest_insert->sex);

	guest_tail->next = guest_insert;
	guest_insert->next = NULL;
	guest_tail = guest_insert;
	//修改Rooms链表中的信息
	printf("请输入旅客将入住的房间号：\n");
	int findChoice;//输入的房间号是否是存在的
	int checkIn;//是否成功入住
	while (1)// 判断 房间号是否有误 或 房间是否空闲
	{
		struct Rooms* room_cur = room_head->next;
		findChoice = 0;
		checkIn = 0;
		scanf("%d", &roomChoice);
		while (room_cur != NULL)
		{
			if (room_cur->roomNum == roomChoice && ((room_cur->roomtype) - (room_cur->checkinNum)) > 0)
			{
				(room_cur->checkinNum)++;//该房间入住人数加1
				findChoice = 1;
				checkIn = 1;
				break;
			}
			else if (room_cur->roomNum == roomChoice && ((room_cur->roomtype) - (room_cur->checkinNum)) <= 0)
			{
				findChoice = 1;
				checkIn = 0;
				printf("选择的房间已满，请选择可入住的房间:\n");
				break;
			}
			room_cur = room_cur->next;
		}
		if (!findChoice)//输入有误
		{
			printf("输入的房间号有误，请输入正确的房间号：\n");
			continue;
		}
		else if (findChoice && !checkIn)//房间已满
		{
			continue;
		}
		else if (checkIn && findChoice)//入住成功
			break;
	}

	//添加旅客信息到GuestAndRoom链表
	struct GuestAndRoom* GAR_insert = (struct GuestAndRoom *)malloc(sizeof(struct GuestAndRoom));
	GAR_insert->guestRoom = roomChoice;
	strcpy(GAR_insert->guestID, ID_insert);
	strncpy(GAR_insert->checkInTime, ctime(&t), strlen(ctime(&t)) - 1);//ctime()返回值末尾有换行符！
	GAR_insert->checkInTime[strlen(ctime(&t)) - 1] = '\0';//strncpy()用完后需要手动添加'\0'
	printf("入住记录成功，入住时间为：%s", GAR_insert->checkInTime);
	strcpy(GAR_insert->checkOutTime, " ");

	GAR_tail->next = GAR_insert;
	GAR_insert->next = NULL;
	GAR_tail = GAR_insert;

	guestNum++;
	//入住后在Guest.txt中添加信息
	FILE* Guest_p;
	if ((Guest_p = fopen("Guest.txt", "a")) == NULL)
	{
		printf("Guest.txt文件读取失败");
		exit(0);
	}
	fprintf(Guest_p, "\n%s %s %s", name_input, sex, ID_insert);
	fclose(Guest_p);

	//入住后在GuestAndRoom.txt中添加信息
	FILE* GAR_p;
	if ((GAR_p = fopen("GuestAndRoom.txt", "a")) == NULL)
	{
		printf("GuestAndRoom.txt文件读取失败");
		exit(0);
	}
	strncpy(time_insert, ctime(&t), strlen(ctime(&t)) - 1);//ctime()返回值末尾有换行符！
	time_insert[strlen(ctime(&t)) - 1] = '\0';//strncpy()用完后需要手动添加'\0'
	fprintf(GAR_p, "\n%d %s %s %s", roomChoice, ID_insert, time_insert, " ");
	fclose(GAR_p);
}

void GuestChangeRoom()//旅客换房
{
	char IDchoice[25] = {0};
	int oldRoomNum = 0;//原房间号
	int roomChoice = 0;
	int findChoice;//输入的房间号是否是存在的
	int ifCheckIn;//是否成功入住
	printf("请输入身份证号：\n");
	scanf("%s", IDchoice);
	//检查是否已入住
	struct Guests* guest_cur = guest_head->next;
	int isCheckedIn = 0;
	while (guest_cur != NULL)
	{
		if (strcmp(guest_cur->ID, IDchoice) == 0)
		{
			isCheckedIn = 1;
			struct GuestAndRoom* GAR_cur = GAR_head->next;
			while (GAR_cur != NULL)
			{
				if (strcmp(GAR_cur->guestID, IDchoice) == 0)//通过身份证号在GuestAndRoom链表中找到房间号、入住时间
				{
					printline();
					printf("姓名\t性别\t身份证\t\t\t房间\t入住时间");
					printf("\n%s\t%s\t%s\t%d\t%s\t", guest_cur->name, guest_cur->sex, IDchoice, GAR_cur->guestRoom, GAR_cur->checkInTime);
					printline();
					break;
				}
				GAR_cur = GAR_cur->next;
			}
			break;
		}
		guest_cur = guest_cur->next;
	}
	if (!isCheckedIn)//没有入住过
	{
		printf("该旅客尚未办理入住.已返回主菜单.\n");
		return;
	}

	if (ViewAvaRoom() == 0)//如果无空房 //函数会打印可入住房间信息
		return;
	printf("请输入将换入的房间号：\n");

	//判断 是否换入原房间、房间号是否有误、房间是否空闲
	while (1)
	{
		scanf("%d", &roomChoice);
		int oldRoomFlag = 0;//是否选择了原房间
		struct GuestAndRoom* GAR_cur = GAR_head->next;
		while (GAR_cur != NULL)
		{
			if (GAR_cur->guestRoom == roomChoice && strcmp(GAR_cur->guestID, guest_cur->ID) == 0)
			{
				printf("不能换入原房间，请选择新房间：\n");
				oldRoomFlag = 1;
				break;
			}
			GAR_cur = GAR_cur->next;
		}
		if (oldRoomFlag == 1)//如果是原房间
		{
			continue;//重新输入房间号
		}
		//选择的是新房间，往下执行
		struct Rooms* room_cur = room_head->next;
		findChoice = 0;
		ifCheckIn = 0;
		while (room_cur != NULL)
		{
			if (room_cur->roomNum == roomChoice && ((room_cur->roomtype) - (room_cur->checkinNum)) > 0)
			{
				(room_cur->checkinNum)++;//新房间入住人数加1
				//原房间的入住人数减1
				GAR_cur = GAR_head->next;
				while (GAR_cur != NULL)
				{
					if (strcmp(GAR_cur->guestID, IDchoice) == 0)
					{
						oldRoomNum = GAR_cur->guestRoom;//原房间人数减少将在后面操作
						GAR_cur->guestRoom = roomChoice;//更换GAR链表中的房间号
						break;
					}
					GAR_cur = GAR_cur->next;
				}
				if (!oldRoomNum)
				{
					printf("无法找到原房间.已返回主菜单.");
					return;
				}
				findChoice = 1;
				ifCheckIn = 1;
				break;
			}
			else if (room_cur->roomNum == roomChoice && ((room_cur->roomtype) - (room_cur->checkinNum)) <= 0)
			{
				findChoice = 1;
				ifCheckIn = 0;
				printf("选择的房间已满，请选择可入住的房间:\n");
				break;
			}
			room_cur = room_cur->next;
		}
		if (!findChoice)//输入有误
		{
			printf("输入的房间号有误，请输入正确的房间号：\n");
			continue;
		}
		else if (findChoice && !ifCheckIn)//房间已满
		{
			continue;
		}
		else if (ifCheckIn && findChoice)//入住成功
			break;
	}

	//结账
	char checkInTime[30] = {0};
	struct GuestAndRoom* GAR_cur = GAR_head->next;
	while (GAR_cur != NULL)
	{
		if (strcmp(GAR_cur->guestID, IDchoice) == 0)
		{
			strcpy(checkInTime, GAR_cur->checkInTime);
			break;
		}
		GAR_cur = GAR_cur->next;
	}
	struct tm checkIn = {0};//星期 月份 日期 时：分：秒 年
	checkIn.tm_isdst = -1;
	char week[10] = {0};
	char month[10] = {0};
	sscanf(checkInTime, "%s %s %d %d:%d:%d %d", week, month, &checkIn.tm_mday, &checkIn.tm_hour, &checkIn.tm_min, &checkIn.tm_sec, &checkIn.tm_year);
	checkIn.tm_year -= 1900;//tm_year是从1900年开始计算的！
	if (strcmp(week, "Sun") == 0)//C语言中switch的表达式只能是整数，C++中可以是字符串
		checkIn.tm_wday = 0;
	else if (strcmp(week, "Mon") == 0)
		checkIn.tm_wday = 1;
	else if (strcmp(week, "Tue") == 0)
		checkIn.tm_wday = 2;
	else if (strcmp(week, "Wed") == 0)
		checkIn.tm_wday = 3;
	else if (strcmp(week, "Thu") == 0)
		checkIn.tm_wday = 4;
	else if (strcmp(week, "Fri") == 0)
		checkIn.tm_wday = 5;
	else if (strcmp(week, "Sat") == 0)
		checkIn.tm_wday = 6;
	if (strcmp(month, "Jan") == 0)
		checkIn.tm_mon = 0;
	else if (strcmp(month, "Feb") == 0)
		checkIn.tm_mon = 1;
	else if (strcmp(month, "Mar") == 0)
		checkIn.tm_mon = 2;
	else if (strcmp(month, "Apr") == 0)
		checkIn.tm_mon = 3;
	else if (strcmp(month, "May") == 0)
		checkIn.tm_mon = 4;
	else if (strcmp(month, "Jun") == 0)
		checkIn.tm_mon = 5;
	else if (strcmp(month, "Jul") == 0)
		checkIn.tm_mon = 6;
	else if (strcmp(month, "Aug") == 0)
		checkIn.tm_mon = 7;
	else if (strcmp(month, "Sep") == 0)
		checkIn.tm_mon = 8;
	else if (strcmp(month, "Oct") == 0)
		checkIn.tm_mon = 9;
	else if (strcmp(month, "Nov") == 0)
		checkIn.tm_mon = 10;
	else if (strcmp(month, "Dec") == 0)
		checkIn.tm_mon = 11;
	time_t checkOutTime = time(NULL);
	double money;//应付款金额
	double discount = 1;//折扣率
	double duration = difftime(checkOutTime, mktime(&checkIn));//入住时长，按秒计算
	duration = (int)duration / 86400 + 1;//换算为天数
	money = duration * (double)UnitPrice(oldRoomNum);
	printf("换房前请支付已产生的房款.\n");
	printf("请输入折扣率：(0-1)\n");
	while (1)
	{
		scanf("%lf", &discount);
		if (discount <= 1 && discount >= 0)
		{
			printf("换房前 应付款%.2lf元\n", discount * money);
			break;
		}
	}

	//修改Rooms链表中原房间的人数（新房间的人数增多已在上面实现）
	struct Rooms* room_cur = room_head->next;
	while (room_cur != NULL)
	{
		if (room_cur->roomNum == oldRoomNum)
			room_cur->checkinNum--;
		room_cur = room_cur->next;
	}

	//修改GAR文件中的房间号
	SaveGARInfo();
	//不用添加退房时间！
}

void GuestCheckOut()//旅客退房
{
	time_t t = time(NULL);
	char IDchoice[25] = {0};
	struct Guests** guest_delete = NULL;
	struct GuestAndRoom** GAR_delete = NULL;
	int flag = 0;//找到旅客为1，未找到旅客为0

	printf("请输入身份证号：\n");
	scanf("%s", IDchoice);

	struct Guests* guest_cur = guest_head->next;
	while (guest_cur != NULL)
	{
		if (strcmp(guest_cur->ID, IDchoice) == 0)//找到身份证匹配的旅客 //字符串比较需要使用strcmp函数
		{
			guest_delete = &guest_cur;//用二级指针记录要删除的Guests链表的地址
			flag = 1;//找到旅客为1，未找到旅客为0
			struct GuestAndRoom* GAR_cur = GAR_head->next;
			while (GAR_cur != NULL)
			{
				if (strcmp(GAR_cur->guestID, IDchoice) == 0)//字符串比较需要使用strcmp函数
				{
					GAR_delete = &GAR_cur;
					break;
				}
				GAR_cur = GAR_cur->next;
			}
			break;
		}
		guest_cur = guest_cur->next;
	}

	if (!flag)
	{
		printf("没有找到该旅客\n");
		return;
	}

	//文件输出记录旅客信息
	FILE* GH_p;//GuestHistoryInfo_pointer
	if ((GH_p = fopen("GuestHistoryInfo.txt", "a")) == NULL)
	{
		printf("文件读取失败");
		exit(0);
	}
	strncpy((*GAR_delete)->checkOutTime, ctime(&t), strlen(ctime(&t)) - 1);
	(*GAR_delete)->checkOutTime[strlen(ctime(&t)) - 1] = '\0';//记录退房时间
	fprintf(GH_p, "\n%s %s %d %s %s", (*guest_delete)->name, (*guest_delete)->ID, (*GAR_delete)->guestRoom, (*GAR_delete)->checkInTime, (*GAR_delete)->checkOutTime);//姓名 身份证 房间号 入住时间 退房时间
	fclose(GH_p);

	//屏幕输出退房信息
	printline();
	printf("此次是该旅客第%d次住店.\n", IsOldGuest(IDchoice) + 1);
	printf("该旅客入住时间为%s\n", (*GAR_delete)->checkInTime);
	printf("退房时间为%s\n", (*GAR_delete)->checkOutTime);
	struct tm checkIn = {0};//星期 月份 日期 时：分：秒 年
	checkIn.tm_isdst = -1;
	char week[10] = {0};
	char month[10] = {0};
	sscanf((*GAR_delete)->checkInTime, "%s %s %d %d:%d:%d %d", week, month, &checkIn.tm_mday, &checkIn.tm_hour, &checkIn.tm_min, &checkIn.tm_sec, &checkIn.tm_year);
	checkIn.tm_year -= 1900;//tm_year是从1900年开始计算的！
	if (strcmp(week, "Sun") == 0)//C语言中switch的表达式只能是整数，C++中可以是字符串
		checkIn.tm_wday = 0;
	else if (strcmp(week, "Mon") == 0)
		checkIn.tm_wday = 1;
	else if (strcmp(week, "Tue") == 0)
		checkIn.tm_wday = 2;
	else if (strcmp(week, "Wed") == 0)
		checkIn.tm_wday = 3;
	else if (strcmp(week, "Thu") == 0)
		checkIn.tm_wday = 4;
	else if (strcmp(week, "Fri") == 0)
		checkIn.tm_wday = 5;
	else if (strcmp(week, "Sat") == 0)
		checkIn.tm_wday = 6;
	if (strcmp(month, "Jan") == 0)
		checkIn.tm_mon = 0;
	else if (strcmp(month, "Feb") == 0)
		checkIn.tm_mon = 1;
	else if (strcmp(month, "Mar") == 0)
		checkIn.tm_mon = 2;
	else if (strcmp(month, "Apr") == 0)
		checkIn.tm_mon = 3;
	else if (strcmp(month, "May") == 0)
		checkIn.tm_mon = 4;
	else if (strcmp(month, "Jun") == 0)
		checkIn.tm_mon = 5;
	else if (strcmp(month, "Jul") == 0)
		checkIn.tm_mon = 6;
	else if (strcmp(month, "Aug") == 0)
		checkIn.tm_mon = 7;
	else if (strcmp(month, "Sep") == 0)
		checkIn.tm_mon = 8;
	else if (strcmp(month, "Oct") == 0)
		checkIn.tm_mon = 9;
	else if (strcmp(month, "Nov") == 0)
		checkIn.tm_mon = 10;
	else if (strcmp(month, "Dec") == 0)
		checkIn.tm_mon = 11;
	struct tm checkOut = {0};//星期 月份 日期 时：分：秒 年
	checkOut.tm_isdst = -1;
	sscanf((*GAR_delete)->checkOutTime, "%s %s %d %d:%d:%d %d", week, month, &checkOut.tm_mday, &checkOut.tm_hour, &checkOut.tm_min, &checkOut.tm_sec, &checkOut.tm_year);
	checkOut.tm_year -= 1900;//tm_year是从1900年开始计算的！
	if (strcmp(week, "Sun") == 0)//C语言中switch的表达式只能是整数，C++中可以是字符串
		checkOut.tm_wday = 0;
	else if (strcmp(week, "Mon") == 0)
		checkOut.tm_wday = 1;
	else if (strcmp(week, "Tue") == 0)
		checkOut.tm_wday = 2;
	else if (strcmp(week, "Wed") == 0)
		checkOut.tm_wday = 3;
	else if (strcmp(week, "Thu") == 0)
		checkOut.tm_wday = 4;
	else if (strcmp(week, "Fri") == 0)
		checkOut.tm_wday = 5;
	else if (strcmp(week, "Sat") == 0)
		checkOut.tm_wday = 6;
	if (strcmp(month, "Jan") == 0)
		checkOut.tm_mon = 0;
	else if (strcmp(month, "Feb") == 0)
		checkOut.tm_mon = 1;
	else if (strcmp(month, "Mar") == 0)
		checkOut.tm_mon = 2;
	else if (strcmp(month, "Apr") == 0)
		checkOut.tm_mon = 3;
	else if (strcmp(month, "May") == 0)
		checkOut.tm_mon = 4;
	else if (strcmp(month, "Jun") == 0)
		checkOut.tm_mon = 5;
	else if (strcmp(month, "Jul") == 0)
		checkOut.tm_mon = 6;
	else if (strcmp(month, "Aug") == 0)
		checkOut.tm_mon = 7;
	else if (strcmp(month, "Sep") == 0)
		checkOut.tm_mon = 8;
	else if (strcmp(month, "Oct") == 0)
		checkOut.tm_mon = 9;
	else if (strcmp(month, "Nov") == 0)
		checkOut.tm_mon = 10;
	else if (strcmp(month, "Dec") == 0)
		checkOut.tm_mon = 11;
	double money;//应付款金额
	double discount = 1;//折扣率
	double duration = difftime(mktime(&checkOut), mktime(&checkIn));//入住时长，按秒计算
	duration = (int)duration / 86400 + 1;//换算为天数
	money = duration * (double)UnitPrice((*GAR_delete)->guestRoom);
	printf("请输入折扣率：(0-1)\n");
	while (1)
	{
		scanf("%lf", &discount);
		if (discount <= 1 && discount >= 0)
		{
			printf("应付款%.2lf元\n", discount * money);
			break;
		}
	}

	//删除Guests链表中的信息
	if ((*guest_delete)->next != NULL)//如果要删除的节点不是在链表最后
	{
		//注意指针本身的地址&p、指针指向的内容*p和指针存储的地址p三者不同 //注意此处使用了二级指针
		struct Guests* temp = (*guest_delete)->next;//记录将删除的一级指针的地址
		**guest_delete = *(*guest_delete)->next;//是将一级指针指向的内容赋值
		free(temp);//free()用于释放动态分配的内存
	}
	else if ((*guest_delete)->next == NULL)//如果要删除的节点是在链表最后
	{
		struct Guests* end = guest_head->next;
		while (end != NULL)
		{
			if (end->next == NULL)//链表中只有一个
			{
				free(end);
				guest_head->next = NULL;
				guest_tail = guest_head;
				break;
			}
			else if (end->next->next == NULL)//end此时是倒数第二个
			{
				free(end->next);
				end->next = NULL;
				guest_tail = end;
				*guest_delete = NULL;
				break;
			}
			end = end->next;
		}
	}

	//删除GAR链表中的信息
	if ((*GAR_delete)->next != NULL)//要删除的节点不是最后一个
	{
		struct GuestAndRoom* temp = (*GAR_delete)->next;
		**GAR_delete = *(*GAR_delete)->next;
		free(temp);
	}
	else if ((*GAR_delete)->next == NULL)//要删除的节点是最后一个
	{
		struct GuestAndRoom* end = GAR_head->next;
		while (end != NULL)
		{
			if (end->next == NULL)//链表中只有一个
			{
				free(end);
				GAR_head->next = NULL;
				GAR_tail = GAR_head;
				break;
			}
			else if (end->next->next == NULL)//end此时是倒数第二个
			{
				free(end->next);
				end->next = NULL;
				GAR_tail = end;
				*GAR_delete = NULL;
				break;
			}
			end = end->next;
		}
	}
	guestNum--;
	//覆盖链表到文件
	SaveGARInfo();
	SaveGuestInfo();
	SaveHotelInfo();
}