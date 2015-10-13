#include"map.h"
#include"mystring.h"
#include<stdlib.h>
#include<string.h>

void init(map *maps)
{
	node *newnode = malloc(sizeof(node));
	strclear(newnode->name);
	strclear(newnode->value);
	newnode->next = NULL;
	maps->head = newnode;
	maps->rear = newnode;
}

void insert(map *maps,char *name, char *value)
{
	node *newnode = malloc(sizeof(node));
	strcpy(newnode->name,name);
	strcpy(newnode->value,value);
	newnode->next = NULL;
	maps->rear->next = newnode;
	maps->rear = newnode;
	maps->count++;	
}

node *find(map *maps, char *name)
{
	node *temp;
	for(temp = maps->head;temp!=NULL;temp = temp->next)
	{
		if(!strcmp(name,temp->name))
			return temp;
	}
	
	return NULL;
}
