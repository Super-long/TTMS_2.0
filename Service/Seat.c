#include <stdlib.h>
#include "../Common/List.h"
#include "Seat.h"
#include "../Persistence/Seat_Persist.h"
#include "../Persistence/EntityKey_Persist.h"
int Seat_Srv_Add(const seat_t *data){
       if(Seat_Perst_Insert(data))
       {
              //printf("持久化层ok\n");
              return 1;
       }
       return 0;
}


int Seat_Srv_Modify(const seat_t *data){
	if(Seat_Perst_Update(data));
       return 1;
       return 0;
}


int Seat_Srv_DeleteByID(int ID){
       if(Seat_Perst_RemoveByID(ID)) 
       return 1;
       return 0;
}


inline int Seat_Srv_DeleteAllByRoomID(int roomID){
	return Seat_Perst_RemoveAllByRoomID(roomID);
       //返回值为删除的座位个数
}


int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID){
       if(Seat_Perst_SelectByRoomID(list,roomID))
       return 1;
       return 0;
}


int Seat_Srv_RoomInit(seat_list_t list, int roomID, int rowsCount,int colsCount) 
{
       printf("%d %d\n",rowsCount,colsCount);
	for(int i=1;i<=rowsCount;i++)
       {
              for(int j=1;j<=colsCount;j++)
              {
                     seat_list_t temp=(seat_list_t)malloc(sizeof(seat_node_t));
                     temp->data.roomID=roomID;
                     temp->data.column=j;
                     temp->data.row=i;
                     temp->data.status=Seat_UI_Char2Status('#');
                     temp->data.id=EntKey_Perst_GetNewKeys("seat",1);
                     List_AddTail(list,temp);
              }
       }
       Seat_Perst_InsertBatch(list);
       return 0;
}


void Seat_Srv_SortSeatList(seat_list_t list) {
       
       if(List_IsEmpty(list))
       {
              printf("链表为空 / 电影院座位行列均为0才出现\n");
              return ;
       }//assert(NULL!=list);
       seat_list_t listLift=list->next;
       list->prev->next=NULL;
       list->next=list->prev=list;
       printf("进入链表\n");
       //把环变成一条链
       Seat_Srv_AddToSoftedList(list,listLift);
       printf("插入没问题\n");
       while(listLift->next!=NULL)
       {
              printf("1\n");
              listLift=listLift->next;
              printf("1\n");
              free(listLift->prev);//用于删除老链表
              printf("1\n");
              //printf("进入循环\n");
              //int node_row = listLift->data.row;
              Seat_Srv_AddToSoftedList(list,listLift);
              printf("1\n");
       }
       list->prev=listLift;
       listLift->next=list;
       //把断开的链连成一个环
       return 1;
}


void Seat_Srv_AddToSoftedList(seat_list_t list, seat_node_t *node) {
       seat_list_t temp=(seat_list_t)malloc(sizeof(seat_node_t));
       temp=node;
       temp->next=NULL;
       temp->prev=NULL;
       printf("1\n");
       if(list->next==list)
       {
              printf("进行了一个元素的插入\n");
              List_AddTail(list,temp);
              return;
       }//特判当node为第一个元素时
       int node_row = node->data.row;
       int node_col = node->data.column;
       seat_list_t p=(seat_list_t)malloc(sizeof(seat_node_t));
       p=list;
       static int flag=0;
       printf("11\n");
       while(p->next!=list)
       {
              printf("6\n");
              int row=p->data.row;
              int col=p->data.column;
              printf("4\n");
              if((row>node_row || row==node_row) && (col>node_col || col==node_col))
              {
                     printf("2\n");
                     seat_list_t tmp=Seat_Srv_FindByRowCol(list,row,col);
                     printf("10\n");
                     List_InsertAfter(tmp,temp);
                     printf("3\n");
                     flag=1;
                     break;
                     printf("插入元素成功！\n");
              }
              p=p->next;
       }
       free(p);
       if(!flag) List_AddTail(list,temp);//证明这个按照排序是最小的 所以直接排在最后面
       return;
}


int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID)
{
       //这里我默认你使用这个函数的时候给我传递的是一个初始化过的list 
       //用完这个list 记得在代码最后面加一个list_Destory
       seat_list_t temp;
       if(Seat_Perst_SelectByRoomID(list,roomID))
       {
              List_ForEach(list,temp)
              {
                     if(Seat_UI_Status2Char(temp->data.status)!='#')
                     List_DelNode(temp);
              }
              return 1;
       }else
       {
              printf("数据链表载入失败！\n");
              return 0;
       }
}


//根据座位id从文件获取座位信息
//用于从链表还没有加载时获取
int Seat_Srv_FetchByID(int ID, seat_t *buf){
       if(Seat_Perst_SelectByID(ID,buf))
       {
              printf("找到ID为 %d 的座位！\n");
              return 1;
       }else
       {
              printf("未找到ID为 %d 的座位！\n");
              return 0;
       }
}


/* int Seat_Srv_AddBatch(seat_list_t list){
	
       return 0;
} */


//根据演出厅id获取有效座位
seat_node_t * Seat_Srv_FindByRowCol(seat_list_t list, int row, int column) {
       seat_list_t seat_tmp=(seat_list_t)malloc(sizeof(seat_node_t));
       List_ForEach(list,seat_tmp)
       {
              if(seat_tmp->data.row==row && seat_tmp->data.column==column)
              return seat_tmp;
       }//这里记得后面要free掉 
       //用一个智能指针最好
       printf("没有找到%d行 %d列的座位！(这就对了)\n",row,column);
       return NULL;
}

//用于从链表还没有加载时获取
seat_node_t * Seat_Srv_FindByID(seat_list_t list, int rowID) {
       seat_list_t seat_tmp=(seat_list_t)malloc(sizeof(seat_node_t));
       List_ForEach(list,seat_tmp)
       {
              if(seat_tmp->data.id==rowID)
              return seat_tmp;
              //这里记得后面要free掉 
              //用一个智能指针最好
       }
       return NULL;
}

//给予一个链表 返回链表中有几个元素
int Seat_Srv_FetchByRoom(seat_list_t list)
{
       seat_list_t tmp;
       int flag=0;
       List_ForEach(list,tmp)
       {
              flag++;
       }
       return flag;
}