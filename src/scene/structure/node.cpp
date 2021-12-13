#include "node.hpp"

bool Node::isFirstChild() {
	return this->parent ? this->parent->child == this : false;
}

bool Node::isLastChild() {
	return this->parent ? this->parent->child->prev == this : false;
}

void Node::attachTo(Node* newParent) {
	if (this->parent) detach();
	this->parent = newParent;
	if (this->parent->child) {
		this->prev = this->parent->child->prev;
		this->next = this->parent->child;
		this->parent->child->prev->next = this;
		this->parent->child->prev = this;
		return;
	}
	this->parent->child = this;
}

void Node::attach(Node* newChild) {
	if (this->child->hasParent()) this->child->detach();
	newChild->parent = this;
	if (this->child) {
		newChild->prev = this->child->prev;
		newChild->next = this->child;
		this->child->prev->next = newChild;
		this->child->prev = newChild;
		return;
	}
	this->child = newChild;
}

void Node::detach() {
	if (this->parent && this->parent->child == this) {
		this->parent->child = this->next != this ? this->next : nullptr;
	}
	this->prev->next = this->next;
	this->next->prev = this->prev;
	this->prev = this;
	this->next = this;
}

int Node::countNodes() {
	return this->child ? this->child->countNodes() + 1 : 1;
}

Node::Node() {
	this->parent = this->child = nullptr;
	this->prev = this->next = nullptr;
}

Node::Node(Node* node) {
	this->parent = this->child = nullptr;
	this->prev = this->next = this;
	attach(node);
}

Node::~Node() {
	detach();
	while (this->child) {
		delete this->child;
	}
}