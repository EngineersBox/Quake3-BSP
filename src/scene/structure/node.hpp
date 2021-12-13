#pragma once

#ifndef NODE_H
#define NODE_H

class Node {
public:
	Node* parent;
	Node* child;
	Node* prev;
	Node* next;

	bool hasParent() { return this->parent != nullptr; }
	bool hasChild() { return this->child != nullptr; }
	bool isFirstChild();
	bool isLastChild();

	void attachTo(Node* newParent);
	void attach(Node* newChild);
	void detach();

	int countNodes();

	Node();
	Node(Node* node);
	virtual ~Node();
};

#endif // NODE_H