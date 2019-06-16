#ifndef Account_Persist_H_
#define Account_Persist_H_

#include"../Service/Account.h"
//判断系统用户文件是否存在
int Account_Perst_CheckAccFile();


//存储系统用户
int Account_Perst_Insert(account_t *data);

int Account_Perst_Update(account_t *data);

int Account_Perst_RemByID(int id);

int Account_Perst_SelectAll(account_list_t list);

#endif Account_Persist_H_