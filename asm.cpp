#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#include "mymap.h"

enum{
    nosuchlabel,
    boguslabel,
    bogusmnemonic,
    
    missingoperand,
    unexpectedoperand,
    duplicatedlabel,
    notanumber
    
};

const char * errorinfor[]={"no such label","bogus label name","bogus mnemonic","missing operand","unexpected operand","duplicate label definition","not a number"};


const int operandNum[]={1,1,2,2,2,
                        2,0,0,0,0,
                        1,0,0,2,0,
                        2,2,2,0,1,1};
const int CN=21;
const char * commandTable[]={"ldc","adc","ldl","stl","ldnl","stnl",
	                "add","sub","shl","shr",
				    "adj","a2sp","sp2a","call","return",
"brz","brlz","br","HALT","data","SET"};

#define setnum 20

#define M 1000
int hasLabel[1000];

#define M 1000
unsigned int memory[1000];

typedef struct{
    int line;
    int err;
} Error;


void outputError(FILE * f,Error e){
    fprintf(f,"line %d: %s\n", e.line,errorinfor[e.err]);
}

Error errors[1000];

int ei=0;

int isSet;

int setValue;


Map map;
int counter=0;
int line=0;


void changeRecent(int nl)
{
    int x=map.v[map.num-1];
    hasLabel[map.v[map.num-1]]=0;
    map.v[map.num-1]=nl;
    
    hasLabel[nl]=1;
    
    printf("&&&&& %d %d \n",x,nl);
    
}
void printNumber(FILE * file,unsigned int num)
{
    static char temp[10];
    sprintf(temp, "%08x",num);
    
    for(int i=0;i<strlen(temp);i++)
    {
        temp[i]=toupper(temp[i]);
    }
    
    fprintf(file, "%s",temp);
    
}

void outputObj(FILE * file)
{
    for(int i=0;i<counter;i++){
        printNumber(file, memory[i]);
        fprintf(file, "\n");
    }
}

void outputlabelline(FILE * f,int l)
{
    printNumber(f, l);
    fprintf(f,"\n");
}

void outputmemoryline(FILE * f,int l)
{
    printNumber(f, l);
    fprintf(f," ");
    printNumber(f, memory[l]);
     fprintf(f,"\n");
}

void outputList(FILE * f)
{
    for(int i=0;i<counter;i++){
        if(hasLabel[i]){
            outputlabelline(f, i);
            
            
            
        }
        
        outputmemoryline(f, i);
        
        
    }
    
}


void addError( int e){
    
    errors[ei].line=line;
    errors[ei].err=e;
    ei++;
}


 int getOpCode(const char * cn)
{
	for(int i=0;i<CN;i++)
	{
		if(strcmp(cn,commandTable[i])==0)
			return i;
	}
	return -1;
 }

 unsigned int complement(int x)
 {
	 // to do
     static const unsigned int u=1<<24;
     if(x>=0)
         return x;
     
     else
         return u+x;
}


void printError(FILE * file)
{
    int i;
    for(i=0;i<ei;i++)
    {
        outputError(file, errors[i]);
    }
    
    
}

 void processInstruction(const char * op,const char * v, int num,int pass){
     if(num==0)
         return;
     
	 int u=getOpCode(op);
	 unsigned int ins=u;
	 int val=0;
     if(pass==1&&u!=setnum){
         isSet=0;
         return;
     }
     
	 if(u==-1){
		 //printf("error no instruction\n");
         addError(bogusmnemonic);
         
	 }
	 if(operandNum[u]==0){
		 if(num==1){
            

		 }else{

			 addError(unexpectedoperand);
		 }

	 }else{
		 if(num==2){
			 int t=getKey(&map,v);
             
             printf("___ %d ___",t);
             
             

			 if(t!=-1){
				 if(u>=13&&u<=17){
                     
                     printf("\n*****%d %d***\n",t,counter);
					 val=t-counter-1;

				 }else{
					 val=t;
				 }

			 }else{
                 
                 if(strlen(v)!=0&&isalpha(v[0])){
                     addError(nosuchlabel);
                     return;
                 }
                
                 char * end;
                 

				 val=(int)strtol(v,&end,0);
                 
                 printf("\nnnnn__ %d nnnn__\n",val);
                 if(end==v||*end != '\0'){
                     
                     addError(notanumber);
                     
                     printf("Eeeeeee");
                 }
                 
                 printf("success %s %s %d",op,v,val);
                 
                 if(u==19){
                     memory[counter]=val;
                     
                     printf("%d))))",val);
                     return;
                 }

			 }


		 }else{

			 //"missing operand";
             
             addError(missingoperand);
		 }


	 }
     
     

     
   
         
         if(u==setnum){
         isSet=1;
         
         setValue=val;
             
             printf("***********");
             return;
        
         }else{
             isSet=0;
         }
         
     if(pass==1)
         return;
     
     
     
	 memory[counter]=(complement(val)<<8)+ins;
}


void processLabel(const char * l)
{
	if(getKey(&map,l)==-1){
		putKeyValue(&map,l,counter);
        
        hasLabel[counter]=1;

	}else{
		//printf("redefinition");
        
        addError(duplicatedlabel);
	}
}


void testLabel(const char * l)
{
    if(strlen(l)==0||! isalpha(l[0])){
        addError(boguslabel);
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

	char label[50];
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
            if(pass==2)
            {
                testLabel(label);
            }
            


			break;
		}

	}
	char mne[50];
    
	char operand[50];
    
	int ns=sscanf(statement,"%s %s",mne,operand);
	

	
	   
	if(ns>0){

		processInstruction(mne,operand,ns,pass);

	}
    if(pass==1&&isSet){
        changeRecent(setValue);
        printf("((((((");
        
    }
    
    
    if(ns>0&&isSet==0){
        
		counter++;
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
    counter=0;
	FILE * file=fopen(code,"r");
	if(!file)
	{printf("error");
	 return;
	}
    char linestr[256];
	
    line=0;
    while( fgets(linestr,sizeof(linestr),file)){

		processLine(linestr,1);
        
        line++;

    }
    
    fclose(file);
    line=0;
    counter=0;
    file=fopen(code,"r");
    
    while( fgets(linestr,sizeof(linestr),file)){
        
		processLine(linestr,2);
        line++;
    }

    fclose(file);

}

void start(){
    initMap(&map);
    for(int i=0;i<M;i++){
        hasLabel[i]=0;
    }
   
    input("/Users/liumeng/assembler/input.txt");
    
}
void test()
{
    const char * statement="g -556882451";
    char mne[10];
    
	char operand[12];
    
	int ns=sscanf(statement," %s %s",mne,operand);
    
    printf("%s %s",mne,operand);
}



int main()
{
   // test();
    
   start();
 //   printError(stdout);
    outputObj(stdout);
   // outputList(stdout);
  //  int x=1358610133;
 //   int y=-556882451;
  //  int z=-1061097459;
 //   printNumber(stdout, x);
 //   printf("\n");
//    printNumber(stdout, y);
  //   printf("\n");
 //   printNumber(stdout, z);
    return 0;
}