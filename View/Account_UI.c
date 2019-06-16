#include"../Common/List.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../Persistence/Account_Persist.h"
#include "../Persistence/EntityKey_Persist.h"
#include"../Service/Account.h"
#include"Account_UI.h"
#include <termios.h>
#include <fcntl.h>
//系统用户进行对用户的操作 其他用户有其自己的功能
int SysLogin();

static const ACCOUNT_PAGE_SIZE = 5;

int inputpsword(char *pass)
{
	int i,j;
	char inletter;
	struct termios term, termsave;  //专门的结构体termios
	int fd=-1;
 
	fd=open("/dev/tty",O_RDWR|O_NOCTTY);    //open一个tty句柄
	if(fd<0) return -1;
 
	tcgetattr(fd, &term);          //将tty句柄属性get成结构体termios
	tcgetattr(fd, &termsave);
	term.c_lflag &= ~(ICANON|ECHO|ISIG);
 
	tcsetattr(fd, TCSANOW, &termsave); 
	//向tty句柄输出任何字符前，必须先执行tcsetattr()
	for(i=0;i<31;i++)
	{
		tcsetattr(fd, TCSANOW, &term);
		read(fd, &inletter, 1);//读入口令
		if(inletter!='\n')
		{
			pass[i]=inletter;
			tcsetattr(fd, TCSANOW, &termsave);
			write(fd,"*",1);  //回显口令，显示*
		} 
		else 
		{
			pass[i]='\0';
			tcsetattr(fd, TCSANOW, &termsave);
			write(fd,"\n",1);
			break;
		}
	}
 
	if(i==31)
	{ 
		//到长度限制，手工对字符串变量结尾（’\0’),同时回显”\n”
		pass[i]='\0';
		tcsetattr(fd, TCSANOW, &termsave);
		write(fd,"\n",1);
	}
	close(fd);
	return 0;
}//网上copy大佬的代码 原因是用不了getch

inline char Account_UI_Status2Char(account_type_t status) {
	if(status==USR_ANOMY)
	return '~';
	if(status==USR_CLERK)
	return '#';
	if(status==USR_MANG)
	return '-';
    if(status==USR_ADMIN)
    return '*';
}

inline account_type_t Account_UI_Char2Status(char statusChar) {
	if(statusChar=='~')
	return USR_ANOMY;
	if(statusChar=='#')
	return USR_CLERK;
	if(statusChar=='-')
	return USR_MANG;
    if(statusChar=='*')
    return USR_ADMIN;
}

//登录完成之后 进入系统管理员界面才是这个界面
void Account_UI_MgtEntry()
{
	int i;
    char name[35];
	char choice;

	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;
    Account_Srv_InitSys();
	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;
    //定义分页器的页数
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("clear");
		printf(
				"\n==================================================\n");
		printf(
				"********************** Account  List **********************\n");
		printf("%5s  %30s %15s\n", "ID", "Name","type of account");
		printf(
				"------------------------------------------------------------------\n");
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("%5d  %30s  %15c\n",pos->data.id,pos->data.username,Account_UI_Status2Char(pos->data.type));
		}

		printf(
				"------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]revPage | [N]extPage | [A]dd | [D]elete | [U]pdate | [Q]uery | [R]eturn");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(head)) 
			{
				//paging.totalRecords = Account_Srv_Listnumber(head);
				//paging.totalRecords = Account_Srv_FetchAll(head);
                paging.totalRecords+=1;
                Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("Input the name:");
			scanf("%s", name);
            //要同步删除链表
			if (Account_UI_Delete(head,name)) {	
				//paging.totalRecords = Account_Srv_Listnumber(head);
                //paging.totalRecords = Account_Srv_FetchAll(head);
				paging.totalRecords-=1;
                List_Paging(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("Input the name:");
			scanf("%s", name);
			if (Account_UI_Modify(head,name)) {
				//paging.totalRecords = Account_Srv_Listnumber(head);
				//paging.totalRecords = Account_Srv_FetchAll(head);
                List_Paging(head, paging, account_node_t);
			}
			break;
		case 'q':
		case 'Q':
			printf("Input the name:");
			scanf("%s", name);
			Account_UI_Query(head,name);
			//paging.totalRecords = Account_Srv_FetchAll(head);
            //paging.totalRecords = Account_Srv_Listnumber(head);
            //查询不会改变总数目
			List_Paging(head, paging, account_node_t);
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
    List_Destroy(head, account_node_t);
}

int Account_UI_Add(account_list_t list)
{
    system("clear");
    account_t data;
    account_list_t temp;
    char ch[10];
    char pwd[35];
    int flag=1;
    printf("请输入你想添加的用户姓名：");
    scanf("%s",data.username);
    temp=Account_Srv_FindByUsrname(list,data.username);
    if(temp)
    {
        printf("%s 名称已重复！\n",data.username);
        printf("按回车键退出!");
        getchar();
        getchar();
        return 0;
    }
    printf("USR_ANOMY 匿名类型 ~\nUSR_CLERK 销售员 #\nUSR_MANG 经理 -\nUSR_ADMIN 系统管理员 *\n");
    printf("请输入此用户的类型：");
    scanf("%s",ch);
    data.type=Account_UI_Char2Status(ch[0]);
    while(flag)
    {
	    printf("请输入密码(<30位)：");//注意边界问题.如越界了没有报错，那只是“犯罪了未被抓”而已
	    inputpsword(data.password);
        char pwd[35];
        printf("请确认密码：");
        inputpsword(pwd);
        if(strcmp(pwd,data.password))
        printf("第二遍输入错误！");
        else flag=0;
    }
    data.id=EntKey_Perst_GetNewKeys("account",1);
    if(Account_Srv_Add(&data))
    {
        printf("添加用户成功！\n");
        account_list_t temp=(account_list_t)malloc(sizeof(account_node_t));
        temp->data=data;
        List_AddTail(list,temp);
        //同步修改链表
        return 1;
    }
    else
    {
        printf("添加用户失败！\n");
        return 0;
    }
}

int Account_UI_Modify(account_list_t list,char usrName[])
{
    system("clear");
    account_list_t temp=Account_Srv_FindByUsrname(list,usrName);
    if(temp==NULL)
    {
        printf("未找到名称为 %s 的用户！\n",usrName);
        return 0;
    }
    char ch[10];
    printf("修改名称请按 [R]ename\n修改密码请按[U]pdate\n");
    scanf("%s",ch);
    switch (ch[0])
    {
        system("clear");
        case 'r':
        case 'R':
        {
            char initname[35];
            char nowname[35];
            strcpy(initname,temp->data.username);
            printf("目前名称为：%s\n请输入你希望的名称：");
            scanf("%s",temp->data.username);
            //文件中修改
            if(Account_Srv_Modify(&temp->data))
            {
                printf("修改成功！\n原名:%s\n现名:%s\n",initname,nowname);
                //strcpy(temp->data.username,nowname);
                //链表中修改
                return 1;
            }else
            {
                printf("修改 %s 为 %s 失败！\n",initname,temp->data.username);
                return 0;
            }
            break;
        }
        case 'u':
        case 'U':
        {
            system("clear");
            char pwd[40];
            char newpwd[40];
            char newnewpwd[40];
            system("clear");
            printf("请输入原密码：");
            inputpsword(pwd);
            if(strcmp(pwd,temp->data.password))
            {
                printf("密码错误！\n按回车键退出！");
                //fflush();
                getchar();
                getchar();
                break;
            }
            printf("请输入新密码：");
            inputpsword(newpwd);
            printf("请确认新密码：");
            inputpsword(newnewpwd);
            if(strcmp(newnewpwd,newpwd)!=0)
            {
                printf("第二遍输入错误！\n按回车键退出\n");
                getchar();
                getchar();
                break;
            }
            strcpy(temp->data.password,newnewpwd);
            //文件中修改
            if(Account_Srv_Modify(&temp->data))
            {
                printf("修改成功!\n");
                //strcpy(temp->data.password,nowpwd);
                //链表中修改
                return 1;
            }else
            {
                printf("修改失败！\n");
                return 0;
            }
            break;
        }
        default:
            printf("输入错误！\n");
            break;
    }
}

int Account_UI_Delete(account_list_t list,char usrName[])
{
    system("clear");
    account_list_t temp=Account_Srv_FindByUsrname(list,usrName);
    if(temp==NULL)
    {
        printf("此用户不存在！\n");
        return 0;
    }
    if(Account_Srv_DeleteByID(temp->data.id))
    {
        printf("删除 %s 用户成功！\n",usrName);
        printf("按回车键退出\n");
        getchar();
        List_DelNode(temp);
        //链表同步删除
        return 1;
    }else
    {
        printf("删除 %s 用户失败！\n",usrName);
        return 0;
    }
}

int Account_UI_Query(account_list_t list,char usrname[])
{
    system("clear");
    account_list_t temp=Account_Srv_FindByUsrname(list,usrname);
    if(temp==NULL)
    {
        printf("此用户不存在！\n");
        return 0;
    }
	printf("%5s  %30s %15s\n", "ID", "Name","type of account");
	printf(
			"------------------------------------------------------------------\n");
    printf("%5d  %30s  %15c\n",temp->data.id,temp->data.username,Account_UI_Status2Char(temp->data.type));
    printf("按回车退出!");
    getchar();
    getchar();
}