/*
* Copyright(C), 2007-2008, XUPT Univ.
* ������ţ�TTMS_UC_02	 
* File name: Seat.h	  
* Description : ������λ�����־û���	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../Common/List.h"
#include "EntityKey_Persist.h"//用于批量存入组座位
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

static const char SEAT_KEY_NAME[] = "Seat";

int Seat_Perst_Insert(seat_t *data) {   
	assert(NULL!=data);
/* 	FILE *fp;
	fp=fopen(SEAT_DATA_FILE,"ab");
	if(!fp)
	{
		printf("打开文件失败！\n");
		return 0;
	}else
	{
		if(fwrite(data,sizeof(seat_t),1,fp))
		{
			//printf("%d ,%s\n",errno,strerror(errno));
			printf("存储座位信息成功！\n");
			return 1;
		}else
		{
			printf("存储座位信息失败！\n");
			return 0;
		}
	}
	fclose(fp); */
	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("Cannot open file %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(seat_t), 1, fp);

	fclose(fp);
	return rtn;
}

//批量存入文件
int Seat_Perst_InsertBatch(seat_list_t list) {
	seat_node_t *p;
	assert(NULL!=list);
	FILE *fp;
	fp=fopen(SEAT_DATA_FILE,"ab+");
	if(!fp)
	{
		printf("打开文件失败！\n");
		return 0;
	}else
	{
		seat_list_t seat_tmp;
		List_ForEach(list,seat_tmp)
		{
			fwrite(&seat_tmp->data,sizeof(seat_t),1,fp);
		}
	}
	fclose(fp);
	return 0;
}

int Seat_Perst_Update(const seat_t *seatdata) {
	//assert(NULL!=seatdata);
	FILE *fp;
	seat_t seat_tmp;
	int flag=0;
	fp=fopen(SEAT_DATA_FILE,"rb+");
	if(!fp)
	{
		printf("打开文件失败！\n");
		return 0;
	}
	while(1)
	{
		if(fread(&seat_tmp,sizeof(seat_t),1,fp)<1)
		break;
        if(seatdata->column==seat_tmp.column &&seatdata->row==seat_tmp.row)
        //if(seatdata->id==seat_tmp.id)
		{
            flag=1;
            fpos_t pos;
            fgetpos(fp,&pos);
            fseek(fp,-(int)sizeof(seat_t),SEEK_CUR);
            fwrite(seatdata,sizeof(seat_t),1,fp);
            fsetpos(fp,&pos);
			break;
        }
	}
    if(!flag) printf("未找到想修改的座位：\n");
    fclose(fp);
	printf("修改文件成功！\n");
	return flag;

}

int Seat_Perst_RemoveByID(int ID) {
    FILE *fp_read=fopen(SEAT_DATA_FILE,"rb");
    FILE *fp_write=fopen(SEAT_DATA_TEMP_FILE,"wb");
    if(!fp_read || !fp_write)
    {
        printf("open file for Delete is failed!\n");
        return 0;
    }
	seat_t seat_tmp;
	static int flag=0;
	while(1)
	{
		if(fread(&seat_tmp,sizeof(seat_t),1,fp_read)<1)
		break;
		if(ID!=seat_tmp.id)
		{
			fwrite(&seat_tmp,sizeof(seat_t),1,fp_write);
		}else
		{
			flag++;
		}
	}
    fclose(fp_read);
    fclose(fp_write);
	remove(SEAT_DATA_FILE);
	rename(SEAT_DATA_TEMP_FILE,SEAT_DATA_FILE);
	if(flag) return 1;
	return 0;
}
 
int Seat_Perst_RemoveAllByRoomID(int roomID) {
    FILE *fp_read=fopen(SEAT_DATA_FILE,"rb");
    FILE *fp_write=fopen(SEAT_DATA_TEMP_FILE,"wb");
    if(!fp_read || !fp_write)
    {
        printf("open file for Delete is failed!\n");
        return 0;
    }
	seat_t seat_tmp;
	static int flag=0;
	while(1)
	{
		if(fread(&seat_tmp,sizeof(seat_t),1,fp_read)<1)
		break;
		if(roomID!=seat_tmp.roomID)
		{
			fwrite(&seat_tmp,sizeof(seat_t),1,fp_write);
		}else
		{
			flag++;
		}
	}
    fclose(fp_read);
    fclose(fp_write);
	remove(SEAT_DATA_FILE);
	rename(SEAT_DATA_TEMP_FILE,SEAT_DATA_FILE);
	return flag;
}


int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID) {
	FILE *fp=fopen(SEAT_DATA_FILE,"rb");
	seat_t seat_tmp;
	static int flag=0;
	if(!fp)
	{
		printf("open file for read by roomID fileture!\n");
		//printf("%d, %s",errno,strerror(errno));
		return 0;
	}
	while(1)
	{
		if(fread(&seat_tmp,sizeof(seat_t),1,fp)<1)
		break;
		else
		{
			if(roomID==seat_tmp.roomID)
			{
				//printf("%d %d\n",seat_tmp.roomID,roomID);
				seat_list_t seat_list_tmp=(seat_list_t)malloc(sizeof(seat_node_t));
				//这里用了malloc 切记后面进行销毁
				seat_list_tmp->data = seat_tmp;
				List_AddTail(list,seat_list_tmp);
				flag++;
			}
		}
	}
	printf("载入链表的节点数量：%d\n",flag);
	if(flag) printf("载入链表成功！\n");
	fclose(fp);
	return flag;
}

int Seat_Perst_SelectByID(int ID, seat_t *buf) {
	FILE *fp=fopen(SEAT_DATA_FILE,"rb");
	seat_t seat_tmp;
	static int flag=0;
	if(!fp)
	{
		printf("open file for read by roomID fileture!\n");
		return 0;
	}
	while(1)
	{
		if(fwrite(&seat_tmp,sizeof(seat_t),1,fp)<1)
		break;
		if(seat_tmp.id==ID)
		{
			*buf=seat_tmp;
			flag=1;
			break;
		}
	}
	fclose(fp);
	return flag;
}

/* int Seat_Perst_SelectAll(seat_list_t list) {
	
	return 0;
} */
