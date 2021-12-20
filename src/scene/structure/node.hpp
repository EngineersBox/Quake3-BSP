#pragma once

#ifndef NODE_H
#define NODE_H

class Node {
public:
	Node* parent;
	Node* child;
	Node* prev;
	Node* next;

	[[nodiscard]] bool hasParent() const { return this->parent != nullptr; }
	[[nodiscard]] bool hasChild() const { return this->child != nullptr; }
	bool isFirstChild();
	bool isLastChild();

	void attachTo(Node* newParent);
	void attach(Node* newChild);
	void detach();

	[[nodiscard]] int countNodes() const;

	Node();
	explicit Node(Node* node);
	virtual ~Node();
};

#endif // NODE_H