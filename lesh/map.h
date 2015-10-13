#ifndef _MAP_H
#define _MAP_H
#define MAX_LEN 40

typedef struct node 
{
	char name[MAX_LEN];
	char value[MAX_LEN];
	struct node *next;
}node;

typedef struct
{
	node *head;
	node *rear;
	int count;
}map;

extern void init(map *maps);
extern void insert(map *maps,char *name, char *value);
extern node *find(map *maps, char *name);
#endif
