#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef struct commandNode
{
	struct commandNode* mainRoot;
	char* value;
	struct commandNode* left;
	struct commandNode* right;
} commandNode;

#endif
