#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<unistd.h>
/**************************************************/
//globle variable and preprocessive elements
#define   pageSize   32
#define   noOfBlockReq   5
#define   pageLength   4
#define   reserve   28
#define   noOfCommIbfo   32
#define   noOfFuncCall   3
#define   comdQsize      8
#define   noOfComQ       32
#define   deletNo        10
#define   blankPage      "0xFFFFFFF"
int x=0;
/***************************************************/
//structure memory space
typedef struct sNandMemory
{
       unsigned int metaData;
	char reserveByts[reserve];
	
        char page[pageLength][pageSize];
        struct sNandMemory *sNextPtr;

}sNandMemory ;


typedef struct commandQ
{
	char cmd[comdQsize];
	char blockNo[comdQsize];
	char pageNo[comdQsize];
	char bytes[comdQsize];
	char payLoad[comdQsize];
	struct commandQ *link;
}commandQ;
/*****************************************************************************/
void creatSnandMemory( sNandMemory **) ;
void writeMemory(sNandMemory *, int,int, int,char*);
void readMemory(sNandMemory *, int,int,int ,char *);// To read specific no of block
void erraseData(sNandMemory *,int ,int,int,char *); // erasse block according to block.
void openDataBaseFile(commandQ *);  //To read data from file




/*****************************************************************/
typedef void (*funcTestSnand)(sNandMemory *, int,int,int,char *);// function ponter for all
funcTestSnand allp[noOfFuncCall]={readMemory,writeMemory,erraseData};

/************************************************************/

void creatSnandMemory( sNandMemory **head) //Allocating memory as you want 
{
	sNandMemory *newNode=NULL;
	newNode=(sNandMemory*)malloc(sizeof(sNandMemory));
	if(!newNode)
	{
		printf("Memory has not been created\n");
		return ;
	}
	

	newNode->sNextPtr=*head;
	*head=newNode;
	return;
	


}
void creatSnandMemoryForCmdQ( commandQ **head) //Allocating memory as you want 
{
        commandQ *newNode=NULL;
	
        newNode=(commandQ*)malloc(sizeof(commandQ));
        if(!newNode)
        {
                printf("Memory has not been created\n");
                return ;
       }

        newNode->link=*head;
        *head=newNode;
	x++;
        return;
        


}
/*************************************************************************************/
void writeMemory(sNandMemory *head, int blockNo, int pageNo, int bytesNo,char *strInfo)// To write the specific no of block
{
        sNandMemory *temp =head;
        unsigned int movPos=0;
        char str[ pageSize];
        for(movPos=0;movPos<blockNo-1;movPos++)
                temp=temp->sNextPtr;
/*        for(movPos=0;movPos<bytesNo-1;movPos++)
                str[movPos++]=strInfo[movPos++];
                
        str[movPos]='\0';*/
        strncpy(temp->page[pageNo],strInfo,bytesNo);
	printf("-------------------------------w-%s\n",temp->page[pageNo]);
        sleep(1);
	return;
        
}
/*************************************************************************/


void readMemory(sNandMemory *head,int blockNo,int pageNo,int bytesNo,char *strInfo )// To read specific no of block
{
	sNandMemory *temp=head;
	unsigned int movPos=0;
	char str[pageSize];
	for(movPos=0;movPos<blockNo-1;movPos++)
        	temp=temp->sNextPtr;
	strncpy(str,temp->page[pageNo],bytesNo);

	//for(movPos=0;movPos<bytesNo-1;movPos++)
	//{
		printf("I want to read Block=%d page=%d  data=%s\n",blockNo,pageNo,str);
	//}	 
	sleep(0.5);
	return;
}
/**************************************************************************/


void erraseData(sNandMemory *head,int blockNo,int pageNo,int bytesNo,char *strInfo) // erasse block according to block.
{
	
	unsigned int iRow=0;
	if(head==NULL)
	{
		perror("Node has not been created\n");
		return;
	}
	else if(blockNo>=1&&blockNo<=5&&head->metaData<deletNo)
	{
		for(iRow=1;iRow<blockNo;iRow++)
                        head=head->sNextPtr;
		for(iRow=0;iRow<  noOfBlockReq ;iRow++)
                	strcpy(head->page[iRow],blankPage);
		
                head->metaData++;
		return;
		
	}
	
/*	else if(blockNo>1&&blockNo<=noOfBlockReq&&head->metaData<deletNo)

	{
		for(iRow=0;iRow<blockNo-1;iRow++)
			   head=head->sNextPtr;

		
		for(iRow=0;iRow<  noOfBlockReq ;iRow++)
		strcpy(head->page[iRow],blankPage);
		head->metaData++;
		return;
	}*/
	else
	
	{	
		printf("no Block slections \n");
		return;
	}

	return;
}

/**********************************************************************/
void openDataBaseFile(commandQ *head) //open corresponding file and read the command from data base and impliment into the operations.
{
	FILE *fPtr=NULL;
	 char temp=0;
	unsigned int iTempCmd=0,iTempBlock=0,iTempPage=0;
	fPtr=fopen("/home/epl-lab/Desktop/BusIT/backup/barunTest/firstLDD/nand/dataBase.txt","r");
	if(fPtr==NULL)
	{
		perror("file has not been opened\n");
		return ;
	}
	
	do
	{
		temp=fscanf(fPtr,"%s%s%s%s%s",head->cmd,head->blockNo,head->pageNo,head->bytes,head->payLoad);
		sleep(0.1);
		iTempCmd=atoi(head->cmd);
		iTempBlock=atoi(head->blockNo);
		iTempPage=atoi(head->pageNo);
		if(head==NULL&&iTempCmd>=0&&iTempCmd<3&&iTempBlock>=0&&iTempBlock<noOfBlockReq&&iTempPage>=0&&iTempPage<pageLength)
		{
			perror("check head page block size corresdponding to define\n");
			return;
		}
		 printf("%d\n",atoi(head->payLoad));
		head=head->link;
		//printf("%d\n",atoi(head->payLoad));
	

	}while(temp!=EOF&&head!=NULL);

	fclose(fPtr);
	return;
}
int main(int argc ,char *argv[])
{

		sNandMemory *passA=NULL;
		commandQ *passB=NULL;
		unsigned int blockNo=0;
	        for(blockNo=0;blockNo<noOfBlockReq;blockNo++)
	                 creatSnandMemory(&passA);
	        for(blockNo=0;blockNo<32 ;blockNo++)
                creatSnandMemoryForCmdQ(&passB);
		printf("my val++++++%d\n",x);
			openDataBaseFile(passB);
		printf("**********************************************************************\n");
		while(passB!=NULL)
	        {
	
        	       	allp[atoi(passB->cmd)](passA,atoi(passB->blockNo),atoi(passB->pageNo),atoi(passB->bytes),passB->payLoad);
			 passB=passB->link;
	        }
	

	return 1;
}

















