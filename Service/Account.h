#ifndef ACCOUNT_H_
#define ACCOUNT_H_

typedef enum {
    USR_ANOMY = 0, //匿名类型
    USR_CLERK = 1, //销售员
    USR_MANG = 2,  //经理
    USR_ADMIN = 9  //系统管理员
}account_type_t;

typedef struct {
    int id;
    account_type_t type;
    char username[30];
    char password[30];
}account_t;

typedef struct account_node {
    account_t data;
    struct account_node *next;
    struct account_node *prev;
}account_node_t,*account_list_t;

void Account_Srv_InitSys();
//系统文件不存在 则创建匿名系统用户
 
int Account_Srv_Verify(char usrName[],char pwd[]);
//验证用户名和密码

int Account_Srv_Add(const account_t *data);
//添加新系统用户

int Account_Srv_Modify(const account_t *data);
//修改用户密码

int Account_Srv_DeleteByID(int usrID);
//根据ID删除用户


//获取所有的系统用户 返回获取个数
int Account_Srv_FetchAll(account_list_t list);

//返回链表中的节点数
int Account_Srv_Listnumber(account_list_t list);

//在已获取链表上查找用户 无则返回NULL
account_node_t * Account_Srv_FindByUsrname(account_list_t list,char usrname[]);
//在获取销售额时可以使用这个函数代替 因为刚开始为了载入ID一定要载入系统用户文件



#endif ACCOUNT_H_