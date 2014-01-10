#include<stdio.h>
#include<string.h>
#include<stdlib.h>




void processLine( char * line)
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
			break;
		}

	}
	
	
	char mne[10];

	char operand[10];

	int ns=sscanf(statement,"%s %s",mne,operand);
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