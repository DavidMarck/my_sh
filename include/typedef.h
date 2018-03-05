#ifndef TYPEDEF_H
#define TYPEDEF_H

#define TRUE 1
#define FALSE 0

typedef struct commandNode
{
	char* value;
	struct commandNode* left;
	struct commandNode* right;
} commandNode;

#endif
