#pragma once

#include <map>
#include <algorithm>
#include "DepthFirstIterator.h"


template<typename T>
class Tree
{
public:	
	typedef T value_type;
	typedef PreorderIterator<T> preorder_iterator;
private:
	typedef PreorderIterator<T> Iterator;
	typedef TreeNode<T> Node;
private:
	Node *head_m;
	Node *tail_m;
	std::map<T, Node *> keyNodeMap_m;
public:
	Tree();
	Tree(Tree const &t);
	Tree(Tree &&t);
	Tree(T const &k);
	~Tree();
	Tree &operator=(Tree const &t);
	Tree &operator=(Tree &&t);
public:
	void clear();
	bool empty() const;
	size_t size() const;
	Iterator begin() const;
	Iterator end() const;
	Iterator find(T const &key);
	void erase(Iterator &pos);
	Iterator set_root(T const &key);
	Iterator insert_parent(Iterator &child, T const &key);
	Iterator insert_sibling(Iterator &sibling, T const &key);
	Iterator insert_child(Iterator &parent, T const &key);
	Tree subTree(Iterator const &pos);
protected:
	void initialize();
	void finalize();
	void copy(Iterator const &begin);
	void disconect(Iterator &pos);
/*
	enum MoveDirection
	{
		TO_PREV_SIBLING,
		TO_NEXT_SIBLING,
		TO_FIRST_CHILD,
		TO_LAST_CHILD
	};
	public:
	size_t height() const;
	Iterator rbegin() const;
	Iterator rend() const;
	Iterator transfer(
		Iterator &dst, Iterator &src, MoveDirection dir);
protected:
	void connectToNextSibling(Iterator &dst, Iterator &src);
	void connectToPrevSibling(Iterator &dst, Iterator &src);
	void connectToFirstChild(Iterator &dst, Iterator &src);
	void connectToLastChild(Iterator &dst, Iterator &src);
*/
};


template<typename T>
inline Tree<T>::Tree()
{
	initialize();
}

template<typename T>
inline Tree<T>::Tree(Tree const &t)
{
	initialize();
	copy(t.begin());
}

template<typename T>
inline Tree<T>::Tree(Tree &&t)
{
	head_m = t.head_m;
	tail_m = t.tail_m;
	keyNodeMap_m = std::move(t.keyNodeMap_m);
	t.initialize();
}

template<typename T>
inline Tree<T>::Tree(T const &root)
{
	initialize();
	set_root(root);
}

template<typename T>
inline Tree<T>::~Tree()
{
	finalize();
}

template<typename T>
inline Tree<T> &Tree<T>::operator=(Tree const &t)
{
	if (this != &t) {
		this->~Tree();
		new (this) Tree(t);
	}
	return *this;
}

template<typename T>
inline Tree<T> &Tree<T>::operator=(Tree &&t)
{
	if(this != &t) {
		this->~Tree();
		new (this) Tree(std::move(t));
	}
	return *this;
}

template<typename T>
inline void Tree<T>::clear()
{
	erase(begin());
}

template<typename T>
inline bool Tree<T>::empty() const { return keyNodeMap_m.empty(); }

template<typename T>
inline size_t Tree<T>::size() const { return keyNodeMap_m.size(); }

template<typename T>
inline PreorderIterator<T> Tree<T>::begin() const
{
	return Iterator(head_m->nextSibling_m);
}

template<typename T>
inline PreorderIterator<T> Tree<T>::end() const { return Iterator(tail_m); }

template<typename T>
void Tree<T>::erase(Iterator &pos)
{
	if(pos.isHead() || pos.isTail()) { return; }
	while(pos.hasChild()) {
		erase(Iterator(pos.myNode_m->firstChild_m));
	}

	disconect(pos);

	keyNodeMap_m.erase(*pos);
	delete pos.myNode_m;
	pos.myNode_m = nullptr;
}

template<typename T>
Tree<T> Tree<T>::subTree(Iterator const &pos)
{
	Tree t;
	t.copy(pos);
	return t;
}

template<typename T>
inline PreorderIterator<T> Tree<T>::set_root(T const &key)
{
	clear();
	return insert_sibling(Iterator(head_m), key);
}

template<typename T>
PreorderIterator<T> Tree<T>::insert_parent(
	Iterator &child, T const &key)
{
	if (find(key) != end()){ return end(); }
	if (empty()) {
		return set_root(key);
	}
	else if (child == end()){ return end(); }

	Node* newNode = new Node(key);
	keyNodeMap_m.insert(std::make_pair(key, newNode));
	newNode->parent_m = child.myNode_m->parent_m;
	newNode->prevSibling_m = child.myNode_m->prevSibling_m;
	newNode->nextSibling_m = child.myNode_m->nextSibling_m;
	newNode->firstChild_m = child.myNode_m;
	newNode->lastChild_m = child.myNode_m;
	if (child.hasNext()) {
		child.myNode_m->nextSibling_m->prevSibling_m = newNode;
	}
	else{
		if (child.hasParent()){
			child.myNode_m->parent_m->lastChild_m = newNode;
		}
	}
	if (child.hasPrev()){
		child.myNode_m->prevSibling_m->nextSibling_m = newNode;
	}
	else{
		if(child.hasParent()) {
			child.myNode_m->parent_m->firstChild_m = newNode;
		}
	}
	child.myNode_m->nextSibling_m = nullptr;
	child.myNode_m->prevSibling_m = nullptr;
	child.myNode_m->parent_m = newNode;
	
	return Iterator(newNode);
}

template<typename T>
PreorderIterator<T> Tree<T>::insert_sibling(
	Iterator &sibling, T const &key)
{
	if (find(key) != end()){ return end(); }
	if(sibling.isRoot() || sibling.isTail()) { return end(); }
	
	Node *newNode = new Node(key);
	keyNodeMap_m.insert(std::make_pair(key, newNode));
	
	newNode->parent_m = sibling.myNode_m->parent_m;
	newNode->prevSibling_m = sibling.myNode_m;
	newNode->nextSibling_m = sibling.myNode_m->nextSibling_m;
	if (sibling.hasNext()) {
		sibling.myNode_m->nextSibling_m->prevSibling_m = newNode;
	}
	else{
		if (newNode->parent_m != nullptr) {
			newNode->parent_m->lastChild_m = newNode;
		}
	}
	sibling.myNode_m->nextSibling_m = newNode;
	
	return Iterator(newNode);
}

template<typename T>
PreorderIterator<T> Tree<T>::insert_child(
	Iterator &parent, const T& key)
{
	if (find(key) != end()){ return end(); }
	if(parent.isHead() || parent.isTail()) { return end(); }

	if(parent.hasChild()) {
		return insert_sibling(Iterator(parent.myNode_m->lastChild_m), key);
	}
	else {
		Node *newNode = new Node(key);
		keyNodeMap_m.insert(std::make_pair(key, newNode));

		newNode->parent_m = parent.myNode_m;
		parent.myNode_m->firstChild_m = newNode;
		parent.myNode_m->lastChild_m = newNode;
		
		return Iterator(newNode);
	}
}

template<typename T>
inline PreorderIterator<T> Tree<T>::find(T const &key)
{
	auto itr = keyNodeMap_m.find(key);
	if(itr != keyNodeMap_m.end()) {
		return Iterator(itr->second);
	}
	else{
		return end();
	}
}

template<typename T>
inline void Tree<T>::initialize()
{
	head_m = new Node();
	tail_m = new Node();

	head_m->nextSibling_m = tail_m;
	tail_m->prevSibling_m = head_m;
}

template<typename T>
inline void Tree<T>::finalize()
{
	clear();
	delete head_m; head_m = nullptr;
	delete tail_m; tail_m = nullptr;
}

template<typename T>
void Tree<T>::copy(Iterator const &begin)
{
	if(begin.isTail() || begin.isHead()) { return; }

	Iterator end = begin;
	while(end.hasParent() && end.isLast()) { end.goParent(); }
	end.goNextSibling();
	
	for(Iterator itr = begin; itr != end; ++itr) {
		if (itr.hasParent()) {
			insert_child(find(itr.myNode_m->parent_m->key_m), *itr);
		}
		else {
			set_root(*itr);
		}
	}
}

template<typename T>
void Tree<T>::disconect(Iterator &pos)
{
	if(pos.isHead() || pos.isTail()) { return; }

	if(pos.hasPrev()) {
		pos.myNode_m->prevSibling_m->nextSibling_m = pos.myNode_m->nextSibling_m;
	}
	else {
		pos.myNode_m->parent_m->firstChild_m = pos.myNode_m->nextSibling_m;
	}
	if(pos.hasNext()) {
		pos.myNode_m->nextSibling_m->prevSibling_m = pos.myNode_m->prevSibling_m;
	}
	else {
		pos.myNode_m->parent_m->lastChild_m = pos.myNode_m->prevSibling_m;
	}
}

/*
size_t Tree<T>::height() const
{
	size_t height = 0;
	
	for (Iterator itr = begin(); itr != end(); ++itr){
		if (itr.isLeaf() && itr.depth() > height){
			height = itr.depth();
		}
	}
	return height;
}

inline PreorderIterator Tree<T>::rbegin() const { return --end(); }

inline PreorderIterator Tree<T>::rend() const{ return Iterator(head_m); }

PreorderIterator Tree<T>::transfer(
	Iterator dst, Iterator src, MoveDirection dir)
{
	if (dst == src) { return end(); }
	if (dst == end() || src == end()){ return end(); }
	Iterator tmp = dst;
	while (tmp.myNode_m->parent_m != nullptr){
		tmp.myNode_m = tmp.myNode_m->parent_m;
		if (tmp == src){ return end(); }
	}
	switch (dir){
	case TO_NEXT_SIBLING:
		if (dst.isRoot()){ return end(); }
		disconect(src);
		connectToNextSibling(dst, src);
		break;
	case TO_PREV_SIBLING:
		if (dst.isRoot()){ return end(); }
		disconect(src);
		connectToPrevSibling(dst, src);
		break;
	case TO_FIRST_CHILD:
		disconect(src);
		connectToFirstChild(dst, src);
		break;
	case TO_LAST_CHILD:
		disconect(src);
		connectToLastChild(dst, src);
		break;
	default:
		return end();
	}

	return src;
}

void Tree<T>::connectToNextSibling(Iterator &dst, Iterator &src)
{
	src.myNode_m->parent_m = dst.myNode_m->parent_m;
	if (dst.myNode_m->nextSibling_m) {
		dst.myNode_m->nextSibling_m->prevSibling_m = src.myNode_m;
	}
	else {
		src.myNode_m->parent_m->lastChild_m = src.myNode_m;
	}
	src.myNode_m->nextSibling_m = dst.myNode_m->nextSibling_m;
	dst.myNode_m->nextSibling_m = src.myNode_m;
	src.myNode_m->prevSibling_m = dst.myNode_m;
}

void Tree<T>::connectToPrevSibling(Iterator &dst, Iterator &src)
{
	src.myNode_m->parent_m = dst.myNode_m->parent_m;
	if (dst.myNode_m->prevSibling_m != nullptr) {
		dst.myNode_m->prevSibling_m->nextSibling_m = src.myNode_m;
	}
	else {
		dst.myNode_m->parent_m->firstChild_m = src.myNode_m;
	}
	src.myNode_m->prevSibling_m = dst.myNode_m->prevSibling_m;
	src.myNode_m->nextSibling_m = dst.myNode_m;
	dst.myNode_m->prevSibling_m = src.myNode_m;
}

void Tree<T>::connectToFirstChild(Iterator &dst, Iterator &src)
{
	if (dst.myNode_m->firstChild_m) {
		dst.myNode_m->firstChild_m->prevSibling_m = src.myNode_m;
	}
	else {
		dst.myNode_m->lastChild_m = src.myNode_m;
	}
	src.myNode_m->parent_m = dst.myNode_m;
	src.myNode_m->nextSibling_m = dst.myNode_m->firstChild_m;
	dst.myNode_m->firstChild_m = src.myNode_m;
	src.myNode_m->prevSibling_m = nullptr;
}

void Tree<T>::connectToLastChild(Iterator &dst, Iterator &src)
{
	if (!dst.myNode_m->firstChild_m) {
		dst.myNode_m->firstChild_m = src.myNode_m;
	}
	if (dst.myNode_m->lastChild_m) {
		dst.myNode_m->lastChild_m->nextSibling_m = src.myNode_m;
	}
	src.myNode_m->parent_m = dst.myNode_m;
	src.myNode_m->prevSibling_m = dst.myNode_m->lastChild_m;
	dst.myNode_m->lastChild_m = src.myNode_m;
	src.myNode_m->nextSibling_m = nullptr;
}
*/
