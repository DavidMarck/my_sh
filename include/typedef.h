#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef struct commandNode
{
	struct commandNode* mainRoot;
	struct commandNode* parentNode;
	char* value;
	struct commandNode* left;
	struct commandNode* right;
} commandNode;

#endif
