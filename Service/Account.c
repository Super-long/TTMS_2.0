#include"Account.h"
#include"../View/Account_UI.h"
//为了使用密码回显
#include"../Persistence/Account_Persist.h"
#include"../Common/List.h"
#include "../Persistence/EntityKey_Persist.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h> //access函数

account_t gl_CurUser;

static const char ACCOUNT_DATA_FILE[]= "account.dat" ;

//全局变量 用来存储目前登录用户的信息

void Account_Srv_InitSys()
{
    if(Account_Perst_CheckAccFile())
    {
        int flag=1;
        printf("文件不存在，生成Admin匿名系统管理员！\n");
        account_t temp;
        strcpy(temp.username,"Admin");
        temp.id=EntKey_Perst_GetNewKeys("account",1);
        temp.type=Account_UI_Char2Status('~');
        while(flag)
        {
            printf("请输入密码(<30位)：");//注意边界问题.如越界了没有报错，那只是“犯罪了未被抓”而已
            inputpsword(temp.password);
            char pwd[35];
            printf("请确认密码：");
            inputpsword(pwd);
            if(strcmp(pwd,temp.password))
            printf("第二遍输入错误！");
            else flag=0;
        }
        if(Account_Srv_Add(&temp))
        //直接放入文件 因为从文件里读取在这一步之后
        {
            printf("加入匿名用户Admin成功！\n");
        }else
        {
            printf("添加匿名用户失败！\n");
        }      
        printf("按回车键退出！\n");
        getchar();
    }
}


int Account_Srv_Verify(char usrName[],char pwd[])
{
    account_t usr;
    strcpy(usr.password,pwd);
    strcpy(usr.username,usrName);
    if(Account_Perst_SelByName(&usr))
    {
        printf("欢迎回来！%s\n",usr.username);
        gl_CurUser=usr;
        //对信息进行存储
        return 1;
    }
    printf("密码或用户名不存在!\n");
    return 0;
}

int Account_Srv_Add(const account_t *data)
{
    if(Account_Perst_Insert(data))
    {
        printf("存入新用户成功!\n");
        return 1;
    }else
    {
        printf("存入新用户失败！\n");
        return 0;
    }
}

//修改密码
int Account_Srv_Modify(const account_t *data)
{
    if(Account_Perst_Update(data))
    {
        printf("修改文件成功！\n");
        return 1;
    }else
    {
        printf("修改文件失败！\n");
        return 0;
    }
}


int Account_Srv_DeleteByID(int usrID)
{
    if(Account_Perst_RemByID(usrID))
    {
        //printf("删除用户成功！\n");
        return 1;
    }else
    {
        //printf("删除用户失败！\n");
        return 0;
    }
}


int Account_Srv_FetchAll(account_list_t list)
{
    return Account_Perst_SelectAll(list);
}

int Account_Srv_Listnumber(account_list_t list)
{
    static flag=0;
    account_list_t temp;
    List_ForEach(list,temp)
    {
        flag++;
    }
    return flag;
}

account_node_t * Account_Srv_FindByUsrname(account_list_t list,char usrname[])
{
    account_list_t temp=(account_list_t)malloc(sizeof(account_node_t));
    List_ForEach(list,temp)
    {
        if(strcmp(temp->data.username,usrname)==0)
        {
            printf("找到用户！\n");
            return temp;
        }
    }
    return NULL;
}