/*
* Copyright(C), 2007-2008, XUPT Univ.
* ������ţ�TTMS_UC_02	 
* File name: Seat.h	  
* Description : ������λ����ҵ���߼���ͷ�ļ�	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#ifndef SEAT_H_
#define SEAT_H_

//��λ״̬ 
typedef enum{
	SEAT_NONE=0,			//��λ
	SEAT_GOOD=1,			//����λ
	SEAT_BROKEN=9			//�𻵵���λ
}seat_status_t;

//��λ���ݽṹ 
typedef struct {
	int id;					//��λid
	int roomID;				//�����ݳ���id
	int row;           		//��λ�к�
    int column;        		//��λ�к�
    seat_status_t status;	//��λ�ڸ��е�״̬��0��ʾû����λ��1��ʾ����λ����չ2�ɱ�ʾ��λ����
} seat_t;


//˫������
typedef struct seat_node {
	seat_t data;
	struct seat_node *next, *prev;
} seat_node_t, *seat_list_t;

/*
��ʶ����TTMS_SCU_Seat_Srv_RoomInit
���ܣ������С�������ʼ���ݳ�������λ
*/ 
int Seat_Srv_RoomInit(seat_list_t list, int roomID,int rowsCount,int colsCount);

void Seat_Srv_SortSeatList(seat_list_t list );

void Seat_Srv_AddToSoftedList(seat_list_t list , seat_node_t *node);

int Seat_Srv_Add(const seat_t *data);

int Seat_Srv_AddBatch(seat_list_t list);

int Seat_Srv_Modify(const seat_t *data);


int Seat_Srv_DeleteByID(int ID);


int Seat_Srv_DeleteAllByRoomID(int roomID);

int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID);


int Seat_Srv_FetchByID(int ID, seat_t *buf);


int Seat_Srv_FetchByRoomID(seat_list_t list, int roomID);


int Seat_Srv_FetchValidByRoomID(seat_list_t list, int roomID);


seat_node_t * Seat_Srv_FindByRowCol(seat_list_t list, int row, int column);


seat_node_t * Seat_Srv_FindByID(seat_list_t list, int seatID);

int Seat_Srv_FetchByRoom(seat_list_t list);

#endif SEAT_H_





