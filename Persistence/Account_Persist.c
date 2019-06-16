#include"../Common/List.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //access函数
#include"Account_Persist.h"
#include "EntityKey_Persist.h"
#include"../Service/Account.h"

static const char ACCOUNT_DATA_FILE[]= "account.dat";
static const char ACCOUNT_DATA_TMP_FILE[]= "account_tmp.dat";
//判断系统用户文件是否存在
int Account_Perst_CheckAccFile()
{
    if(access(ACCOUNT_DATA_FILE,0))
    return 1;
    return 0;
}

//存储系统用户
int Account_Perst_Insert(account_t *data)
{
	FILE *fp = fopen(ACCOUNT_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(account_t), 1, fp);

	fclose(fp);
	return rtn;
}

int Account_Perst_Update(account_t *data)
{
	FILE *fp;
	account_t account_tmp;
	int flag=0;
	fp=fopen(ACCOUNT_DATA_FILE,"rb+");
	if(!fp)
	{
		printf("打开文件失败！\n");
		return 0;
	}
	while(1)
	{
		if(fread(&account_tmp,sizeof(account_t),1,fp)<1)
		break;
        if(data->id==account_tmp.id)
		{
            flag=1;
            fpos_t pos;
            fgetpos(fp,&pos);
            fseek(fp,-(int)sizeof(account_t),SEEK_CUR);
            fwrite(data,sizeof(account_t),1,fp);
            fsetpos(fp,&pos);
			break;
        }
	}
    if(!flag) printf("未找到想修改用户：\n");
    fclose(fp);
	printf("修改成功！\n");
	return flag;
}

int Account_Perst_RemByID(int id)
{
    FILE *fp_read=fopen(ACCOUNT_DATA_FILE,"rb");
    FILE *fp_write=fopen(ACCOUNT_DATA_TMP_FILE,"wb");
    if(!fp_read || !fp_write)
    {
        printf("open file for Delete is failed!\n");
        return 0;
    }
	account_t seat_tmp;
	static int flag=0;
	while(1)
	{
		if(fread(&seat_tmp,sizeof(account_t),1,fp_read)<1)
		break;
		if(id!=seat_tmp.id)
		{
			fwrite(&seat_tmp,sizeof(account_t),1,fp_write);
		}else
		{
			flag++;
		}
	}
    fclose(fp_read);
    fclose(fp_write);
	remove(ACCOUNT_DATA_FILE);
	rename(ACCOUNT_DATA_TMP_FILE,ACCOUNT_DATA_FILE);
	if(flag) return 1;
	return 0;
}

int Account_Perst_SelectAll(account_list_t list)
{
	FILE *fp=fopen(ACCOUNT_DATA_FILE,"rb");
	account_t account_tmp;
	static int flag=0;
	if(!fp)
	{
		printf("open file for read by roomID fileture!\n");
		//printf("%d, %s",errno,strerror(errno));
		return 0;
	}
	while(1)
	{
		if(fread(&account_tmp,sizeof(account_t),1,fp)<1)
		break;
		else
		{
			account_list_t account_list_tmp=(account_list_t)malloc(sizeof(account_node_t));
			//这里用了malloc 切记后面进行销毁
			account_list_tmp->data = account_tmp;
			List_AddTail(list,account_list_tmp);
			flag++;
		}
	}
	printf("载入链表的节点数量：%d\n",flag);
	if(flag) printf("载入链表成功！\n");
	fclose(fp);
	return flag;
}

//在文件中找到与usr name 相匹配的变量 找到后进行密码匹配
int Account_Perst_SelByName(account_t *usr)
{
	FILE *fp=fopen(ACCOUNT_DATA_FILE,"rb");
	account_t *account_tmp=(account_t *)malloc(sizeof(account_t));
	//容易内存泄露 智能指针真好 还没free
    static int flag=0;
	if(!fp)
	{
		printf("open file for read by roomID fileture!\n");
		return 0;
	}
	while(1)
	{
		if(fwrite(account_tmp,sizeof(account_t),1,fp)<1)
		break;
		if(!strcmp(account_tmp->username,usr->username))
		{
            if(!strcmp(!strcmp(account_tmp->password,usr->password)))
            {
                *usr=*account_tmp;
                flag=1;
                break;
            }else
            {
                flag=0;
                break;
            }
		}
	}
	fclose(fp);
	return flag; 
}