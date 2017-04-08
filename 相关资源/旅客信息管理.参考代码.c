/*MainBody.c*/
#include"Structs.h"
extern int GuestNumber;
int GuestNumber=0;//现有的旅客数
void main(void)
{
    LoadHotelInfo(rooms);
    LoadGuestInfo();
    LoadGARInfo();
    handlemenu();
}
void printMenu()
{
    printf(
        "1 查看旅店信息\n"
        "2 查看某一房间信息\n"
        "3 查看旅客信息\n"
        "4 查找某一旅客信息\n"
        "5 旅客入住\n"
        "6 旅客换房\n"
        "7 旅客退房\n"
        "8 退出\n"
    );
}
void handlemenu()
{
    int i=0;
    do
    {
        printMenu();
        printf("请选择:");
        scanf("%d",&i);
        fflush(stdin);
        switch(i)
        {
        case 1:
            ViewAllHotel();
            printf("\n\n");
            break;
        case 2:
            ViewOneRoom();
            printf("\n\n");
            break;
        case 3:
            ViewAllGuest();
            printf("\n\n");
            break;
        case 4:
            ViewOneGuest();
            printf("\n\n");
            break;
        case 5:
            GuestCheckIn();
            printf("\n\n");
            break;
        case 6:
            GuestChangeRoom();
            printf("\n\n");
            break;
        case 7:
            GuestCheckOut();
            printf("\n\n");
            break;
        case 8:
        {
            SaveGARInfo();
            SaveGuestInfo();
            SaveHotelInfo(rooms);
            exit(0);
        }
        }
    }
    while(1);
}
/*Structs.c*/
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#define FLOORNUMBER 4
#define ROOMNUMBER 5
typedef enum RoomType {Single=1,Double=2,Triangle=3} RoomType; //房间类型
typedef struct room
{
    int RoomNumber;//房间号
    RoomType roomtype;//房间类型
    int CheckInNumber;//入住几人
    int RoomPrice;//房间价格
} Room; //房间相关的成员的定义
typedef struct guestandroom
{
    char GuestID[20];//旅客身份证号
    int GuestInRoom;//旅客入住的房间
    char CheckInTime[30];//住入的时间
    char CheckOutTime[30];//退房的时间
} GuestAndRoom; //房间相关的成员的定义
typedef struct guest
{
    char Name[10];//姓名
    char sex[3];//性别
    char ID[20];//身份证号码
} Guest; //旅客的相关属性的定义
void menu();
void LoadHotelInfo(Room pararoom[FLOORNUMBER][]);
void ViewAllHotel();
void SaveHotelInfo(Room pararoom[FLOORNUMBER][]);
void LoadGuestInfo();
void SaveGuestInfo();
void ViewOneGuest();
void ViewAllGuest();
void AppendGuestInfo();
void GuestCheckIn();
void GuestChangeRoom();
void GuestCheckOut();
void ChangeBookDay();
void AppendGARInfo();
void LoadGARInfo();
void SaveGARInfo();
void AppendGuestHistoryInfo(int Guesti,int GARj);
void handlemenu();
void printMenu();
void ViewOneRoom();
int IsOldGuest();
int DelGuest(char ID[]);
int CountGuest(Room pararoom[FLOORNUMBER][]);
int CheckRAGIndex(char ID[]);
int CheckFreeRoom(char Sex[]);
int FindOneGuest();
char *CheckRoomSex(int RoomNumber);
Room rooms[FLOORNUMBER][ROOMNUMBER];//存房间信息的结构数组
Guest ptoguest[55];//存旅客信息的数组
GuestAndRoom GAR[55];//存旅客入住房间的数组
/*GAR.c*/
#include"Structs.h"
extern int GuestNumber;
//向gusetandroom文件中添加信息
void AppendGARInfo()
{
    FILE *fp;
    if((fp=fopen("guestandroom.txt","a"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fprintf(fp,"%d\t%s\t\t%s\n",GAR[GuestNumber-1].GuestInRoom,GAR[GuestNumber-1].GuestID,GAR[GuestNumber-1].CheckInTime);
    fclose(fp);
}
//初始化GAR数组
void LoadGARInfo()
{
    /*动态建立存储旅客信息的数组*/
    FILE *fp;
    int iCount=0;
    char temp[10]="";
    if((fp=fopen("guestandroom.txt","r"))==NULL)
    {
        printf("文件读取失败!\n");
        exit(0);
    }
    fscanf(fp,"%*s %*s %*s %*s",temp,temp,temp,temp);
    for(iCount=0; iCount<GuestNumber; iCount++)
    {
        fscanf(fp,"%d %s\t\t\t",&GAR[iCount].GuestInRoom,GAR[iCount].GuestID);
        fgets(GAR[iCount].CheckInTime,25,fp);
        strcpy(GAR[iCount].CheckOutTime," ");
    }
    fclose(fp);
}
void SaveGARInfo()
{
    FILE *fp;
    int iCount=0;
    if((fp=fopen("guestandroom.txt","w"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fprintf(fp,"%s\t%s\t%s\t%s\n","房间","身份证","入住时间","退房时间");
    for(iCount=0; iCount<GuestNumber; iCount++)
        fprintf(fp,"%d\t\t%s\t\t\t%s\t\t\t%s\n",GAR[iCount].GuestInRoom,GAR[iCount].GuestID,GAR[iCount].CheckInTime,GAR[iCount].CheckOutTime);
    fclose(fp);
}
//检验满足ID的RAG数组的位置
int CheckRAGIndex(char ID[])
{
    int gindex=-1;
    int garindex=-1;
    int icount;
    for(icount=0; icount<GuestNumber; icount++)
    {
        if(!strcmp(ID,ptoguest[icount].ID))
        {
            gindex=icount;
            break;
        }
    }
    if(gindex!=-1) //如果有
    {
        for(icount=0; icount<GuestNumber; icount++)
        {
            if(!strcmp(ptoguest[gindex].ID,GAR[icount].GuestID))
            {
                garindex=gindex;
                break;
            }
        }
    }
    return garindex;
}
//旅客入住
void GuestCheckIn()
{
    int NewRoomNumber;
    int NewGuestSexN;
    int i;
    int j;
    int CheckSex=0;
    time_t t;
    t=time(NULL);
    printf("请输入旅客姓名:\n");
    scanf("%s",ptoguest[GuestNumber].Name);
    do
    {
        printf("请选择旅客性别:\n");
        printf("1 男\n2 女\n");
        scanf("%d",&NewGuestSexN);
        switch(NewGuestSexN)
        {
        case 1:
            strcpy(ptoguest[GuestNumber].sex,"男");
            break;
        case 2:
            strcpy(ptoguest[GuestNumber].sex,"女");
            break;
        }
    }
    while(NewGuestSexN!=1&&NewGuestSexN!=2);
    printf("请输入旅客身份证号码:\n");
    scanf("%s",ptoguest[GuestNumber].ID);
    CheckSex=CheckFreeRoom(ptoguest[GuestNumber].sex);
    if(!CheckSex)
    {
//处理数组
        strcpy(ptoguest[GuestNumber].ID,"");
        strcpy(ptoguest[GuestNumber].Name,"");
        strcpy(ptoguest[GuestNumber].sex,"");
        return;
    }
    printf("请输入旅客选择入住房间号码:\n");
    scanf("%d",&NewRoomNumber);//如何保证选得是正确的呢
//更改房间状态,新房间
    for(i=0; i<4; i++)
    {
        for(j=0; j<5; j++)
        {
            if(rooms[i][j].RoomNumber==NewRoomNumber)
            {
                rooms[i][j].CheckInNumber=rooms[i][j].CheckInNumber+1;
            }
        }
    }
    GuestNumber+=1;//人数增加
    strcpy(GAR[GuestNumber-1].CheckInTime,ctime(&t));
    printf("入住时间为%s\n",GAR[GuestNumber-1].CheckInTime);
    strcpy(GAR[GuestNumber-1].GuestID,ptoguest[GuestNumber-1].ID);
    GAR[GuestNumber-1].GuestInRoom=NewRoomNumber;
//将信息添加到文件中
    AppendGuestInfo();
    AppendGARInfo();
    SaveHotelInfo(rooms);
}
//旅客退房
void GuestCheckOut()
{
    float discount=0,money=0;
    int Days=0,i;
    char ID[20]="";
    do
    {
        printf("请输入旅客身份证:");
        scanf("%s",ID);
        for(i=0; i<=GuestNumber-1; i++)
        {
            if(!strcmp(ID,ptoguest[i].ID))
            {
                break;
            }
        }
        if(i==GuestNumber)
        {
            printf("您输入的用户不存在！");
            i=-1;
        }
    }
    while(i==-1);
    discount=(IsOldGuest(ID)<=2)?1:(IsOldGuest(ID)<=4)?0.9:(IsOldGuest(ID)<=6)?0.8:0.7;
    printf("此位顾客此次为第%d次住店,打折率为:%f\n",IsOldGuest(ID)+1,discount);
    money=(float)DelGuest(ID);
    printf("请输入入住天数:\n");
    scanf("%d",&Days);
    money=money*Days*discount;
    printf("应该付%f元.\n",money);
    SaveGARInfo();
    SaveGuestInfo();
    SaveHotelInfo(rooms);
}
//旅客换房
//旅客换房，只能换到空房（结账）
void GuestChangeRoom()
{
    char name[10]="";
    char ID[19]="";
    int RAGindex=-1;
    int GuestIndex=-1;
    char choice='\0';
    int day=0;
    int i=0,j=0;
    int sum=0;
    int roomnumber=0;
//输入旅客信息
    printf("请输入旅客的姓名:\n");
    gets(name);
//找到旅客在旅客结构体中的下标
    printf("请输入需要换房的旅客的身份证：\n");
    gets(ID);
    RAGindex=CheckRAGIndex(ID);
    for(i=0; i<=GuestNumber; i++)
    {
        if(!strcmp(ID,ptoguest[i].ID))   //要是有同名的就麻烦了
        {
            printf("该旅客的信息如下\n"
                   "姓名\t性别\t入住的房间\t入住的时间\n");
            printf("%s\t%s\t%d\t%s\n",ptoguest[i].Name,ptoguest[i].sex,GAR[RAGindex].GuestInRoom,GAR[RAGindex].CheckInTime);
            GuestIndex=i;
            break;
        }
    }
    printf("确认以上信息？(y/n)\n");
    choice=getchar();
    if(choice=='N'||choice=='n')
    {
        return;
    }
//结账
    printf("请输入该旅客的入住天数：\n");
    scanf("%d",&day);
    for(i=0; i<FLOORNUMBER; i++)
    {
        for(j=0; j<ROOMNUMBER; j++)
        {
            if(rooms[i][j].RoomNumber==GAR[RAGindex].GuestInRoom)
            {
                sum=day*rooms[i][j].RoomPrice;
                break;
            }
        }
        if(sum)
        {
            break;
        }
    }
    printf("请旅客交纳前面的房款%d.\n",sum);
//打印能换入的房间
    CheckRAGIndex(ptoguest[GuestIndex].sex);
    printf("请输入要换入的房间号：\n");
    scanf("%d",&roomnumber);
//放入
    if(roomnumber==GAR[RAGindex].GuestInRoom)
    {
        printf("不能换入原房间");
        return;
    }
//状态更改
//原房间
    GAR[RAGindex].GuestInRoom=roomnumber;
    rooms[i][j].CheckInNumber--;
    for(i=0; i<FLOORNUMBER; i++)
        for(j=0; j<ROOMNUMBER; j++)
        {
            if(rooms[i][j].RoomNumber==roomnumber)
            {
                rooms[i][j].CheckInNumber++;
                break;
            }
        }
    SaveGARInfo();
    SaveGuestInfo();
    SaveHotelInfo(rooms);
}
Guests.c:
#include"Structs.h"
extern int GuestNumber;
//添加旅客信息
void AppendGuestInfo()
{
    FILE *fp;
    if((fp=fopen("guest.txt","a"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fprintf(fp,"%s\t%s\t%s\t\n",ptoguest[GuestNumber-1].Name,ptoguest[GuestNumber-1].sex,ptoguest[GuestNumber-1].ID);
    fclose(fp);
}
int CountGuest(Room pararoom[FLOORNUMBER][ROOMNUMBER])
{
    int iFloor=0;
    int jRoom=0;
    int sum=0;
    for(iFloor=0; iFloor<FLOORNUMBER; iFloor++)
        for(jRoom=0; jRoom<ROOMNUMBER; jRoom++)
        {
            if(pararoom[iFloor][jRoom].CheckInNumber>0)
            {
                sum+=pararoom[iFloor][jRoom].CheckInNumber;
            }
        }
    return sum;
}
//载入客人信息
void LoadGuestInfo()
{
    FILE *fp;
    int iCount=0;
    char temp[10]="";
    if((fp=fopen("guest.txt","r"))==NULL)
    {
        printf("文件读取失败!\n");
        exit(0);
    }
    GuestNumber=CountGuest(rooms);

    fscanf(fp,"%*s %*s %*s",temp,temp,temp);
    for(iCount=0; iCount<GuestNumber; iCount++)
    {
        fscanf(fp,"%s\t%s\t%s",ptoguest[iCount].Name,ptoguest[iCount].sex,ptoguest[iCount].ID);
//strcpy(GAR[iCount].GuestID,ptoguest[iCount].ID);
    }
    fclose(fp);
}
//储存旅客信息
void SaveGuestInfo()
{
    FILE *fp;
    int iCount=0;
    if((fp=fopen("guest.txt","w"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fprintf(fp,"%s\t%s\t%s\n","姓名","性别","身份证");
    for(iCount=0; iCount<GuestNumber; iCount++)
        fprintf(fp,"%s\t%s\t%s\n",ptoguest[iCount].Name,ptoguest[iCount].sex,ptoguest[iCount].ID);
    fclose(fp);
}
//查找旅客信息
void ViewOneGuest()
{
    int i=FindOneGuest();
    int GARNUM;
    if(i!=-1)
    {
        GARNUM=CheckRAGIndex(ptoguest[i].ID);
        printf("姓名\t性别\t身份证\t\t房间\t入住时间\n");
        printf("%s\t%s\t%s\t\t%d\t%s\t\n",ptoguest[i].Name,ptoguest[i].sex,ptoguest[i].ID,GAR[GARNUM].GuestInRoom,GAR[GARNUM].CheckInTime);
        return;
    }
    else
    {
        printf("没有此用户信息！\n");
        return;
    }
}
//查找旅客信息可以做成一个函数
int FindOneGuest()
{
    int i;
    char GuestID[25];
    printf("\n请输入旅客的身份证:\n");
    scanf("%s",GuestID);
    fflush(stdin);
    for(i=0; i<GuestNumber; i++)
    {
        if(!strcmp(GuestID,ptoguest[i].ID))
        {
            return i;
        }
    }
    printf("没有此名顾客！");
    return -1;
}
//查看所有旅客的信息
void ViewAllGuest()
{
    int i=0;
    int GARNUM;
    printf("---------所有旅客信息------------\n");
    printf("姓名\t性别\t身份证\t房间\t入住时间\n");
    for(i=0; i<GuestNumber; i++)
    {
        GARNUM=CheckRAGIndex(ptoguest[i].ID);//找下标
        printf("%s\t%s\t%s\t%d\t%s\t\n",ptoguest[i].Name,ptoguest[i].sex,ptoguest[i].ID,GAR[GARNUM].GuestInRoom,GAR[GARNUM].CheckInTime);
    }
    printf("打印结束!\n");
}
//判断是否是以前的旅客
int IsOldGuest(char NewID[])
{
    FILE *fp;
    char temp[20]="";
    char name[10]="";
    char ID[20]="";
    int Room=0;
    char CheckInTime[30]="";
    char CheckOutTime[30]="";
    int a=0;
    if((fp=fopen("guesthistory.txt","r"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fscanf(fp,"%*s %*s %*s %*s %*s",temp,temp,temp,temp,temp);
    while(!feof(fp))
    {
        fscanf(fp,"%s %s %d %s %s",name,ID,&Room,CheckInTime,CheckOutTime);
        if(!strcmp(NewID,ID))
        {
            a++;
        }
    }
    return a;
}
//添加新的历史记录
void AppendGuestHistoryInfo(int Guesti,int GARj)
{
    FILE *fp;
    if((fp=fopen("guesthistory.txt","a"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fprintf(fp,"%s\t%s\t%d\t\t\t%s\t\t\t%s\n",ptoguest[Guesti].Name,ptoguest[Guesti].ID,GAR[GARj].GuestInRoom,GAR[GARj].CheckInTime,GAR[GARj].CheckOutTime);
}
//打印入住记录
void ViewOldGuest()
{
    FILE *fp;
    char name[10];
    int ID;
    int Room;
    char CheckInTime[30];
    char CheckOutTime[30];
    if((fp=fopen("guesthistory.txt","r"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    printf("姓名\t\t 身份证\t\t 房间号\t\t 入住时间\t\t 退房时间\t\t\n");
    do
    {
        fscanf(fp,"%s %d %d %s %s\n",name,&ID,&Room,CheckInTime,CheckOutTime);
        printf("%s\t\t %d\t\t %d\t\t %s\t\t %s\t\t\n",name,ID,Room,CheckInTime,CheckOutTime);
    }
    while(!feof(fp));
}
//删除Guest数组中的一个信息,同时删除对应的GAR
int DelGuest(char ID[])
{
    int i=0,j=0,GARj=0,roomi=0,roomj=0;
    int money=0;
    int out=1;
    time_t t;
    t=time(NULL);
    for(i=0; i<GuestNumber; i++)
    {
        if(!strcmp(ID,ptoguest[i].ID))
        {
            GARj=CheckRAGIndex(ptoguest[i].ID);
            printf("入店时间为%s\n",GAR[GARj].CheckInTime);
            strcpy(GAR[GARj].CheckOutTime,ctime(&t));
            printf("退店时间为%s\n",GAR[GARj].CheckOutTime);
            for(roomi=0; roomi<4&&out; roomi++)
            {
                for(roomj=0; roomj<5; roomj++)
                {
                    if(GAR[GARj].GuestInRoom==rooms[roomi][roomj].RoomNumber)
                    {
                        money=rooms[roomi][roomj].RoomPrice;
                        rooms[roomi][roomj].CheckInNumber=rooms[roomi][roomj].CheckInNumber-1;
                        SaveHotelInfo(rooms);
                        out=0;
                        break;
                    }
                }
            }
            AppendGuestHistoryInfo(i,GARj);
            GuestNumber=GuestNumber-1;
            for(j=i; j<GuestNumber; j++)
            {
                ptoguest[j]=ptoguest[j+1];
            }
            for(j=GARj; j<GuestNumber; j++)
            {
                GAR[j]=GAR[j+1];
            }
            break;
        }
    }
    return money;
}
Hotels.c:
#include"Structs.h"
extern int GuestNumber;
//检查房间应住的性别
char *CheckRoomSex(int RoomNumber)
{
    int i,j;
    for(i=0; i<GuestNumber; i++) //添加那有问题GuestNumber{
        if(RoomNumber==GAR[i].GuestInRoom) //得到房间客人所在数组的下标
        {
            for(j=0; j<GuestNumber; j++)
            {
                if(!strcmp(GAR[i].GuestID,ptoguest[j].ID))//找到对应的客人
                    return ptoguest[j].sex;
            }
            break;
        }
}
return NULL;
}
//检查空房
int CheckFreeRoom(char Sex[])
{
    int i,j,find=0;
    char roomsex[3];
    printf("能入住的房间:\n");
    for(i=0; i<4; i++)
    {
        for(j=0; j<5; j++)
        {
            if(!rooms[i][j].CheckInNumber) //房间为空
            {
                printf("%d\t",rooms[i][j].RoomNumber);
                find=1;
            }
            else //房间有人
            {
                strcpy(roomsex,CheckRoomSex(rooms[i][j].RoomNumber));
                if((
                            rooms[i][j].roomtype>rooms[i][j].CheckInNumber
//入住的人数少于房间类型
                            && (strcmp(Sex,roomsex)==0)//入住的是相同性别人
                        )
                  )
                {
                    printf("%d\t",rooms[i][j].RoomNumber);
                    find=1;
                }
                else
                {
                    printf("\t");
                }
            }
        }
        printf("\n");
    }
    if(!find)
    {
        printf("没有合适房间!\n房间已满.\n");
    }
    return find;
}
//载入所有房间入住情况信息
void LoadHotelInfo(Room innerroom[FLOORNUMBER][ROOMNUMBER])
{
    FILE *fp;
    int iFloor=0;
    int jRoom=0;
    char temp[9];
    if((fp=fopen("hotel.txt","r"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fscanf(fp,"%*s %*s %*s %*s\n",temp,temp,temp,temp);
    for(iFloor=0; iFloor<FLOORNUMBER; iFloor++)
        for(jRoom=0; jRoom<ROOMNUMBER; jRoom++)
        {
            fscanf(fp,"%d %d %d
                   %d\n",&innerroom[iFloor][jRoom].RoomNumber,&innerroom[iFloor][jRoom].roomtype,&innerroom[iFloor][jRoom].CheckInNumber,&innerroom[iFloor][jRoom].RoomPrice);
        }
    fclose(fp);
}
//打印所有房间入住情况
void ViewAllHotel()
{
    int i,j;
    printf("房间号码\t房间类型\t入住人数\t 房间价格\t\n");
    for(i=0; i<4; i++)
    {
        for(j=0; j<5; j++)
        {
            printf(" %d\t\t",rooms[i][j].RoomNumber);
            switch(rooms[i][j].roomtype)
            {
            case 1:
                printf(" 单人房\t\t ");
                break;
            case 2:
                printf(" 双人房\t\t ");
                break;
            case 3:
                printf(" 三人房\t\t ");
                break;
            }
            printf("%d\t\t%d\t\t\n",rooms[i][j].CheckInNumber,rooms[i][j].RoomPrice);
        }
        printf("\n");
    }
}
//储存所有房间信息
void SaveHotelInfo(Room pararoom[FLOORNUMBER][ROOMNUMBER])
{
    FILE *fp;
    int iFloor=0;
    int jRoom=0;
    if((fp=fopen("hotel.txt","w"))==NULL)
    {
        printf("文件读取失败");
        exit(0);
    }
    fprintf(fp,"房间号\t房间类型\t入住人数\t房间价格\t\n");
    for(iFloor=0; iFloor<FLOORNUMBER; iFloor++)
    {
        for(jRoom=0; jRoom<ROOMNUMBER; jRoom++)
        {
            fprintf(fp," %d\t\t%d\t\t%d\t\t\t%d\t\n",pararoom[iFloor][jRoom].RoomNumber,pararoom[iFloor][jRoom].roomtype,pararoom[iFloor][jRoom].CheckInNumber,pararoom[iFloor][jRoom].RoomPrice);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}
//打印某一房间情况
void ViewOneRoom()
{
    int RoomNum,i,j,IsNull=0;
    printf("请输入房间号码：");
    scanf("%d",&RoomNum);
    for(i=0; i<GuestNumber; i++)
    {
        if(RoomNum==GAR[i].GuestInRoom)
        {
            for(j=0; j<GuestNumber; j++)
            {
                if(strcmp(ptoguest[j].ID,GAR[i].GuestID)==0)
                {
                    printf("姓名:%s\t身份证:%s\t性别:%s\t入住时间:%s\n",ptoguest[j].Name,ptoguest[j].ID,ptoguest[j].sex,GAR[i].CheckInTime);
                    IsNull=1;
                }
            }
        }
    }
    if(!IsNull)
    {
        printf("房间为空！\n");
    }
}
