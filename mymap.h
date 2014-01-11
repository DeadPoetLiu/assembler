#include <stdio.h>
#include <string.h>

typedef struct
{
    int v[1000];
    char symbols[1000][100];

	int num;
} Map;

void initMap(Map * map)
{
	map->num=0;
}
void putKeyValue(Map * map,const char * str,int value){
	strcpy(map->symbols[map->num],str);

	map->v[map->num]=value;

	map->num++;

}

int getKey(Map * map,const char * str){
	for(int i=0;i<map->num;i++)
	{
		if(strcmp(map->symbols[i],str)==0){
			return map->v[i];
		}
	}

	return -1;
}