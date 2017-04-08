/*编写一个菜单驱动的程序，用来创建某班学生的链表，并执行以下操作
a.显示出该链表的内容
b.编辑指定学生的详细信息
c.计算给定年龄和体重学生的数量*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

struct linked_list//创建链表
{
	char name[50];
	int age;
	float weight;
	struct linked_list *next;//指向下一链的指针
};
typedef struct linked_list Student;
Student *head,*tail;//声明链表首尾

void initialize(void);
void print(Student *stu);
void printAll(void);
void insert(void);
void showMenu(void);
int getChoice(void);
void modify(void);
Student *searchByname(char *);
void countWeight(void);
void countAge(void);

main()
{
	int choice;
	initialize();
	while(1)
	{
		showMenu();
		choice=getChoice();
		switch(choice)
		{
			case 1:
				insert();break;
			case 2:
				printAll();break;
			case 3:
				modify();break;
			case 4:
				countAge();break;
			case 5:
				countWeight();break;
			case 0:
				exit(0);break;
			default:
                break;
		}
	}
}

void showMenu()
{
	printf("\n*******MENU*******\n");
	printf("1.Insert a student\n");
	printf("2.List all students\n");
	printf("3.Modify a student's info\n");
	printf("4.Age statistics\n");
	printf("5.Weight statistics\n");
	printf("0.Quit\n");
	printf("******************\n");
}

int getChoice()
{
	int c;
	printf("Your choice:");
	scanf("%d",&c);
	return c;
}

void initialize()//初始化链表
{
	head=(Student *)malloc(sizeof(Student));//动态分配内存
	head->next=NULL;
	tail=head;
}

void print(Student *stu)
{
	printf("\nName:\t%s\nAge:\t%d\nWeight:\t%.2f\n",stu->name,stu->age,stu->weight);
}

void printAll()
{
	Student *cur=head->next;//head后第一个元素
	while(cur!=NULL)
	{
		print(cur);
		cur=cur->next;
	}
}

void insert()
{
	Student *stu=(Student *)malloc(sizeof(Student));//分配一块新的内存区域给当前这个stu
	printf("Insert a student.\n");
	printf("Please input info of the student:\n");
	printf("Name:");
	scanf("%s",stu->name);
	printf("Age:");
	scanf("%d",&(stu->age));
	printf("Weight:");
	scanf("%f",&(stu->weight));
	tail->next=stu;//将该stu插入到链表最后
	stu->next=NULL;
	tail=stu;
}

void modify()
{
	char name[50];
	Student *stu;
	printf("Please input the name of the student you want to modify:");
	scanf("%s",name);
	stu=searchByname(name);//按姓名查找学生
	if(stu==NULL)//未找到学生信息
		printf("%s is not found.\n",name);
	else//找到学生信息，修改
	{
		printf("Please input info of the student:\n");
		printf("Name:");
		scanf("%s",stu->name);
		printf("Age:");
		scanf("%d",&(stu->age));
		printf("Weight:");
		scanf("%f",&(stu->weight));
		printf("Modification succeed.\n");
	}
}

Student *searchByname(char *name)
{
	Student *cur=head->next;
	while(cur!=NULL)
	{
		if(strcmp(cur->name,name)==0)//找到这个姓名的学生
			break;
		cur=cur->next;//当前不匹配，继续查找
	}
	return cur;
}

void countWeight()
{
	Student *cur=head->next;
	int count=0;
	float w;
	printf("Input the weight:");
	scanf("%f",&w);
	while(cur!=NULL)
	{
		if(abs(cur->weight-w)<0.00001)
			count++;
		cur=cur->next;
	}
	printf("Number of students with weight %.2f:%d\n",w,count);
}

void countAge()
{
	Student *cur=head->next;
	int count=0;
	int a;
	printf("Input the age:");
	scanf("%d",&a);
	while(cur!=NULL)
	{
		if(cur->age==a)
			count++;
		cur=cur->next;
	}
	printf("Number of students with age %d:%d\n:",a,count);
}
