#ifndef TYPEDEF_H
#define TYPEDEF_H

#define STDIN  0  
#define STDOUT 1
#define STDERR 2

#define TRUE 1
#define FALSE 0

typedef struct commandNode
{
	struct commandNode* mainRoot;
	char* value;
	struct commandNode* left;
	struct commandNode* right;
} commandNode;

#endif
