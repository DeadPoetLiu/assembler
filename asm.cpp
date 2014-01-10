#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "mymap.h"

const int operandNum[]={1,1,2,2,2,2,0,0,0,0,1,0,0,2,0,2,2,2,0,1,1};
const int CN=21;
const char * commandTable[]={"ldc","adc","ldl","stl","ldnl","stnl",
	                "add","sub","shl","shr",
				    "adj","a2sp","sp2a","call","return",
				"brz","brlz","br","HALT","data","SET"};

#define M 1000
unsigned int memory[1000];
Map map;
int counter=0;
int line=0;

typedef struct {
	int line;
	int type;
}Error;


 int getOpCode(const char * cn)
{
	for(int i=0;i<CN;i++)
	{
		if(strcmp(cn,commandTable[i])==0)
			return i;
	}
	return -1;
 }


 

 void processInstruction(const char * op,const char * v, int num){


	 int u=getOpCode(op);
	 int ins=u;
	 int val=0;
	 if(u==-1){
		 printf("error no instruction\n");
	 }
	 if(operandNum[u]==0){
		 if(num==1){


		 }else{

			 "too many operand";
		 }

	 }else{
		 if(num==2){
			 int t=getKey(&map,v);

			 if(t!=-1){
				 if(t>=13&&t<=17){
					 val=t-counter;

				 }else{
					 val=t;
				 }

			 }else{

				 val=strtol(v,NULL,0);

			 }


		 }else{

			 "missing operand";
		 }


	 }


}


void processLabel(const char * l)
{
	if(getKey(&map,l)==-1){
		putKeyValue(&map,l,counter);

	}else{
		printf("redefinition");
	}
}




void processLine( char * line,int pass)
{
	int i;
	for(i=0;i<strlen(line);i++)
	{
		if(line[i]==';')
		{
			line[i]='\0';
			break;
		}

	}

	char label[10];
	char * statement=line;
	for(i=0;i<strlen(line);i++)
	{
		if(line[i]==':')
		{
			line[i]='\0';
			sscanf(line,"%s",label);
			printf("Label---  %s  ",label);
			statement=line+(i+1);
			if(pass==1){

				processLabel(label);

			}


			break;
		}

	}
	
	
	char mne[10];

	char operand[10];

	int ns=sscanf(statement,"%s %s",mne,operand);
	if(ns>0){

		counter++;
	}

	if(pass==2){

		processInstruction(mne,operand,ns);

	}


	if(ns==1){
		printf("%s",mne);
	}else if(ns==2){
		printf("%s %s",mne,operand);
	}
	printf("\n");
}









void input(const char * code)
{
	FILE * file=fopen(code,"r");
	if(!file)
	{printf("error");
	 return;
	}
    char line[256];
	
    while( fgets(line,sizeof(line),file)){

		processLine(line);

    }

}
void main()
{
	input("input.txt");
		  
	system("pause");
}