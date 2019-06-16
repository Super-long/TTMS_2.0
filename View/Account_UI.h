#ifndef Account_UI_H_
#define Account_UI_H_

#include"../Service/Account.h"

int inputpsword(char *pass);
//输入密码回显 * 号

char Account_UI_Status2Char(account_type_t status);

account_type_t Account_UI_Char2Status(char statusChar);

int SysLogin();

void Account_UI_MgtEntry();

int Account_UI_Add(account_list_t list);

int Account_UI_Modify(account_list_t list,char usrName[]);

int Account_UI_Delete(account_list_t list,char usrName[]);

int Account_UI_Query(account_list_t list,char usrname[]);

#endif Account_UI_H_