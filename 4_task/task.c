#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1024

typedef struct list
{
	char *buff;
	struct list *next;
} node;

int main()
{
	char *buffer = (char*)malloc(sizeof(char)*MAX_SIZE);
	node *p, *head;
	head = (node*)malloc(sizeof(node));
	
	if (head == NULL)
	{
		perror("Can't allocate memory.\n");
		return 1;
	}
	head->next = NULL;
	p = head;
	while(1)
	{
		if (fgets(buffer, MAX_SIZE - 1, stdin) == NULL)
		{
			perror("Can't read the line.\n");
			return 1;
		}
		if(buffer[0] == '.')
		{
			break;
		}
		//push back
		node *tmp = (node*)malloc(sizeof(node));
		if (tmp == NULL)
		{
			perror("Can't allocate memory.\n");
			return 1;
		}	

		tmp->buff = (char*)malloc(sizeof(char)*strlen(buffer) + 1);
		if (tmp->buff == NULL)
		{
			perror("Can't allocate memory.\n");
			return 1;
		}	
		strcpy(tmp->buff, buffer);
		tmp->next = NULL;
		p->next = tmp;
		p = p->next;
	}

	p = head;
	node *tmp;
	for(p = head->next; p != NULL; p = p->next)
	{
		printf("%s", p->buff);
	}


	p = head;
	while(p != NULL)
	{
		head = head->next;
		free(p->buff);
		free(p);
		p = head;
	}
	return 0;
}

