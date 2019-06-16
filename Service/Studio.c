/*
* Copyright(C), 2007-2008, XUPT Univ.
* ������ţ�TTMS_UC_01		 
* File name: Studio_c		  
* Description : �ݳ�������ҵ���߼���	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/

#include "Studio.h"
#include "../Persistence/Studio_Persist.h"
#include <stdlib.h>
#include <assert.h>

/*
��ʶ����TTMS_SCU_Studio_Srv_Add 
�������ܣ��������һ�����ݳ������ݡ�
����˵����dataΪstudio_t����ָ�룬����Ҫ��ӵ��ݳ������ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�������ݳ����ı�־��
*/
int Studio_Srv_Add(studio_t *data) {
	return Studio_Perst_Insert(data);
}

/*
��ʶ����TTMS_SCU_Studio_ Srv _Mod 
�������ܣ������޸�һ���ݳ������ݡ�
����˵����dataΪstudio_t����ָ�룬����Ҫ�޸ĵ��ݳ������ݽ�㡣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ��޸����ݳ����ı�־��
*/
int Studio_Srv_Modify(const studio_t *data) {
	return Studio_Perst_Update(data);
}

/*
��ʶ����TTMS_SCU_Studio_Srv_DelByID 
����������int Studio_Srv_DeleteByID(int id)��
�������ܣ�����ɾ��һ���ݳ��������ݡ�
����˵����idΪ���ͣ�����Ҫɾ�����ݳ���ID��
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ�ɾ�����ݳ����ı�־��
*/
int Studio_Srv_DeleteByID(int ID) {
	return Studio_Perst_DeleteByID(ID);
}

/*
��ʶ����TTMS_SCU_Studio_Srv_FetchByID
�������ܣ������ݳ���ID��ȡһ���ݳ��������ݡ�
����˵������һ������IDΪ���ͣ�����Ҫ��ȡ���ݵ��ݳ���ID���ڶ�������bufΪstudio_t����ָ�룬ָ���ȡ���ݳ������ݡ�
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ���ȡ���ݳ������ݵı�־��
*/
int Studio_Srv_FetchByID(int ID, studio_t *buf) {

	int tmp=Studio_Perst_SelectByID(ID,buf);
	return tmp;

}

/*
��ʶ����TTMS_SCU_Studio_Srv_FetchAll 
�������ܣ���ȡ�����ݳ��������ݣ��γ���listΪͷָ����ݳ�������
����˵����list��studio_list_t����ָ�룬ָ���ݳ��������ͷָ�롣
�� �� ֵ�����ͣ���ʾ�Ƿ�ɹ���ȡ�������ݳ����ı�־��
*/
int Studio_Srv_FetchAll(studio_list_t list) {
	return Studio_Perst_SelectAll(list);
}

/*
��ʶ����TTMS_SCU_Studio_Srv_FindByID
�������ܣ������ݳ���ID������ͷָ���ȡ����������Ӧ�ݳ��������ݡ�
����˵������һ������listΪstudio_list_t����ָ�룬ָ���ݳ��������ͷָ�룻�ڶ�������IDΪ���ͣ���ʾ��Ҫ��ȡ���ݵ��ݳ���ID��
�� �� ֵ��studio_node_tָ�룬��ʾ��ȡ��ӦID���ݳ������ݡ�
*/
studio_node_t *Studio_Srv_FindByID(studio_list_t list, int ID){
	assert(NULL!=list);
	studio_node_t *ptr=list->next;
	while(ptr!=list){
		if(ptr->data.id==ID)
			return ptr;
		else
			ptr=ptr->next;
	}

	return NULL;
}
