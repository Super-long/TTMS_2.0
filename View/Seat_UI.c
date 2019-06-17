/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* ������ţ�TTMS_UC_02
* File name: Seat_UI.c			  
* Description : ������λ���������	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#include "Seat_UI.h"
#include "../Service/Seat.h"

#include "../Service/Studio.h"
#include "../Common/List.h"
#include "../Persistence/EntityKey_Persist.h"
#include <stdio.h>

inline char Seat_UI_Status2Char(seat_status_t status) {
	if(status==SEAT_GOOD)
	return '#';
	if(status==SEAT_BROKEN)
	return '~';
	if(status==SEAT_NONE)
	return '-';
}

inline seat_status_t Seat_UI_Char2Status(char statusChar) {
	if(statusChar=='-')
	return SEAT_NONE;
	if(statusChar=='#')
	return SEAT_GOOD;
	if(statusChar=='~')
	return SEAT_BROKEN;
}

void Seat_UI_MgtEntry(int roomID) {
	
	studio_t *buf;
	buf=(studio_t*)malloc(sizeof(studio_t));
	Studio_Srv_FetchByID(roomID,buf);
	//printf("%d %d\n",buf->rowsCount,buf->colsCount);
	seat_list_t list=NULL;
	List_Init(list,seat_node_t);
	//传入一个已经初始化的一个链表
	if(buf!=NULL)
	{
		if(!Seat_Srv_FetchByRoomID(list,roomID))
		{
			if(list->next==list) printf("000\n");
			if(list->next==list)
			{
				printf("已初始化！\n");
				Seat_Srv_RoomInit(list,roomID,buf->rowsCount,buf->colsCount);
				//如果座位不存在 进行初始化
				printf("roominit函数运行\n");
			}
		}
	}//函数过后list一定是一个座位链表
	else
	{
		printf("%d号演出厅不存在！\n",roomID);
		return ;
	}
	//printf("%d %d %d %d\n",list->next->data.id,list->next->data.column,list->next->data.column,list->next->data.roomID);
	//Seat_Srv_SortSeatList(list);
	//链表已排好序
	int row=buf->rowsCount;
	int col=buf->colsCount;
	int i, id;
	char choice;

	seat_node_t *pos;
	Pagination_t paging;

	paging.offset = 0;
	paging.pageSize = 10;//默认每一页为10
	printf("计算总数上方\n");
	paging.totalRecords = Seat_Srv_FetchByRoom(list);
	Paging_Locate_FirstPage(list, paging);

	do {
		system("clear");
		printf( "----ID: %d---------------------row:%d-------------------col:%d-----\n",buf->id,buf->rowsCount,buf->colsCount);

		printf(
				"\n==================================================================\n");
		printf(
				"********************** Projection Room List **********************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "roomID", "Rows Count",
				"Columns Count", "Status");
		printf(
				"------------------------------------------------------------------\n");
		Paging_ViewPage_ForEach(list, paging, seat_node_t, pos, i){
			printf("%5d  %10d  %10d  %18d  %10c\n", pos->data.id,
					pos->data.roomID, pos->data.row, pos->data.column,
					Seat_UI_Status2Char(pos->data.status));
					//最后一个以符号形式输出
		}

		printf(
				"------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]revPage|[N]extPage | [A]dd|[D]elete|[U]pdate | [S]how | [R]eturn");
		printf(
				"\n==================================================================\n");
		printf("Your Choice:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
		{
			char ch='A';
			while(ch=='A'|| ch=='a')
			{
				int roww,coll;
				printf("请输入你想添加的行数和列数！");
				scanf("%d %d",&roww,&coll);
				getchar();
				if(roww>0 && roww<=buf->rowsCount && coll>0 && coll<=buf->colsCount)
				{
					if (Seat_UI_Add(list,roomID,roww,coll))//后两个参数在前面声明过 
					{
						paging.totalRecords = Seat_Srv_FetchByRoom(list);
						//printf("链表在添加后的数量 理论上来说应该加一: %d\n",paging.totalRecords);
						Paging_Locate_LastPage(list, paging, seat_node_t);
					}
					printf("想继续添加输入A 输入其他退出当前页面！");
					scanf("%c",&ch);
					getchar();
				}//else的情况在add_UI中出现过
			}
		}
			break;
		case 'd':
		case 'D':
			{
				int rrow,ccol;
				printf("Input the rows and col to Delete:");
				printf("row %d col %d\n",buf->rowsCount,buf->colsCount);
				scanf("%d %d", &rrow,&ccol);
				if(rrow>0 && rrow<=buf->rowsCount && ccol >0 && ccol<=buf->colsCount)
				{
					if (Seat_UI_Delete(list,rrow,ccol)) 
					{	
					seat_list_t temp=Seat_Srv_FindByRowCol(list,rrow,ccol);
					List_DelNode(temp);//用于修改链表 同步显示
					paging.totalRecords = Seat_Srv_FetchByRoom(list);
					List_Paging(list, paging, seat_node_t);
					}
				}else
				{
					printf("所输入的行列数不正确！\n");
				}
				break;
			}
		case 'u':
		case 'U':
			{
				int rrow,ccol;
				printf("Input the rows and col to Update:");
				scanf("%d %d", &rrow,&ccol);
				if(rrow>0 && rrow<=buf->rowsCount && ccol >0 && ccol<=buf->colsCount)
				{
					if (Seat_UI_Modify(list,rrow,ccol)) 
					{	
					paging.totalRecords = Seat_Srv_FetchByRoom(list);
					List_Paging(list, paging, seat_node_t);
					}
				}else
				{
					printf("所输入的行列数不正确！\n");
				}
				break;
			}
		case 's':
		case 'S':
		 	{
			system("clear");
			char ch[10];
			for(int i=0;i<=buf->rowsCount;i++)
			{
				for(int j=0;j<=buf->colsCount;j++)
				{
					if(i==0)
					{
						printf("%3d",j);
						continue;
					}
					else if(j==0)
					{
						printf("%3d",i);
						continue;
					}
/* 					else if(list->next->data.row==i && list->next->data.column==j)
					{
						printf("%c",Seat_UI_Status2Char(list->next->data.status));
						List_FreeNode(list->next);
					}//用于每次显示一个座位的符号 */
					else
					{
						int flag=0;
						seat_list_t temp;
						List_ForEach(list,temp)
						{
							if(temp->data.row==i && temp->data.column==j)
							{
								flag=1;
								printf("%3c",Seat_UI_Status2Char(temp->data.status));
								break;
							}
						}
						if(!flag) printf("  -");//此处没有座位
					}
					
				}
				putchar('\n');
			} //用于显示座位情况列表
/* 			printf("按任意键退出!\n");
			scanf("%s",ch); */
			printf("按回车键退出！");
			getchar();
			getchar();
			break;}
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, -1, seat_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(list, paging, 1, seat_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	List_Destroy(list, seat_node_t);
	free(buf);
	//这很
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //����һ����λ

	seat_list_t tempdd=Seat_Srv_FindByRowCol(list,row,column);
	if(tempdd==NULL)
	{
		tempdd=(seat_list_t)malloc(sizeof(seat_node_t));
		char ch[10];
		printf("请输入这个座位的状态：");
		scanf("%s",&ch);
		tempdd->data.row=row;
		tempdd->data.column=column;
		tempdd->data.status=Seat_UI_Char2Status(ch[0]);
		tempdd->data.roomID=roomID;
		tempdd->data.id=EntKey_Perst_GetNewKeys("Seat",1);
		printf("添加的座位的id：%d\n",tempdd->data.id);
		//printf("添加的座位的roomid：%d\n",tempdd->data.roomID);
		if(Seat_Srv_Add(tempdd)) //函数中分配主键 加入文件
		{
			//printf("插入排序上面:\n");
			//Seat_Srv_AddToSoftedList(list,temp);
			List_AddHead(list,tempdd);
			printf("添加座位成功！\n");
			return 1;
		}else
		{
			printf("添加座位失败！\n");
			return 0;
		}
	}else
	{
		printf("座位已存在!\n");
		//free(temp);
		return 0;
	}
}

int Seat_UI_Modify(seat_list_t list, int row, int column) {
	printf("SEAT_NONE = '-'\nSEAT_GOOD = '#'\nSEAT_BROKEN = '~'\n");
	seat_list_t temp=Seat_Srv_FindByRowCol(list,row,column);
	printf("该座位现在的状态为 %c :\n",Seat_UI_Status2Char(temp->data.status));
    char ch[20];
    printf("输入你希望的状态：");
    scanf("%s",ch);
    getchar();
    temp->data.status=Seat_UI_Char2Status(ch[0]);
	//在这里修改了链表
	if(Seat_Srv_Modify(&temp->data))
	{
		//printf("修改文件成功！\n");
		return 1;
	}
	printf("修改文件失败！\n");
	return 0;
}

int Seat_UI_Delete(seat_list_t list, int row, int column) {
	seat_list_t temp=Seat_Srv_FindByRowCol(list,row,column);
	if(!temp) return 0;
	//防止座位不存在 运行底下会出现段错误  
	printf("要删除座位的ID : %d！\n",temp->data.id);
	if(Seat_Srv_DeleteByID(temp->data.id))
	{
		printf("删除座位成功！\n");
		return 1;
	}
	List_DelNode(temp);
	printf("删除座位失败！\n");
	free(temp);
	return 0;
}