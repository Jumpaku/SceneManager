#pragma once

#include <iterator>
#include <cassert>
#include "TreeNode.h"

template<typename T>
class Tree;

template<typename T>
class BaseTreeIterator : public std::iterator<std::forward_iterator_tag, TreeNode<T>>
{
protected:
	friend class Tree<T>;
	typedef TreeNode<T> Node;
	Node* myNode_m;
protected:
	BaseTreeIterator() : myNode_m() {}
public:
	explicit BaseTreeIterator(Node* node) : myNode_m(node){}

	bool isNull() const;
	bool isTail()const;
	bool isHead()const;
	bool isRoot() const;
	bool isLeaf() const;
	bool isFirst() const;
	bool isLast() const;
	bool hasChild() const;
	bool hasParent()const;
	bool hasNext()const;
	bool hasPrev()const;
	void goParent();
	void goNextSibling();
	void goPrevSibling();
	void goFirstChild();
	void goLastChild();
	T& operator*() const;
	T* operator->() const;
	bool operator==(const BaseTreeIterator iterator) const;
	bool operator!=(const BaseTreeIterator iterator) const;
	operator bool() const;
	bool operator!() const;
};


template<typename T>
inline bool BaseTreeIterator<T>::isNull() const { return myNode_m == nullptr; }

template<typename T>
inline bool BaseTreeIterator<T>::isTail() const
{
	assert(!isNull());
	return myNode_m->parent_m == nullptr &&
		myNode_m->nextSibling_m == nullptr &&
		myNode_m->prevSibling_m != nullptr;
}

template<typename T>
inline bool BaseTreeIterator<T>::isHead() const
{
	assert(!isNull());
	return myNode_m->parent_m == nullptr &&
		myNode_m->nextSibling_m != nullptr &&
		myNode_m->prevSibling_m == nullptr;
}

template<typename T>
inline bool BaseTreeIterator<T>::isRoot() const
{
	assert(!isNull());
	return myNode_m->parent_m == nullptr &&
		myNode_m->nextSibling_m != nullptr &&
		myNode_m->prevSibling_m != nullptr;
}

template<typename T>
inline bool BaseTreeIterator<T>::isLeaf() const
{
	assert(!isNull());
	return !isTail() && !isHead() &&
		myNode_m->firstChild_m == nullptr &&
		myNode_m->lastChild_m == nullptr;
}

template<typename T>
inline bool BaseTreeIterator<T>::isFirst() const
{
	assert(!isNull());
	return !isTail() && !isHead() && 
		(isRoot() || myNode_m->prevSibling_m == nullptr);
}

template<typename T>
inline bool BaseTreeIterator<T>::isLast() const
{
	assert(!isNull());
	return !isTail() && !isHead() &&
		(isRoot() || myNode_m->nextSibling_m == nullptr);
}

template<typename T>
inline bool BaseTreeIterator<T>::hasChild() const
{
	assert(!isNull());
	return myNode_m->firstChild_m != nullptr &&
		myNode_m->lastChild_m != nullptr;
}

template<typename T>
inline bool BaseTreeIterator<T>::hasParent()const
{
	assert(!isNull());
	return myNode_m->parent_m != nullptr;
}

template<typename T>
inline bool BaseTreeIterator<T>::hasNext()const
{
	assert(!isNull());
	return myNode_m->nextSibling_m != nullptr;
}

template<typename T>
inline bool BaseTreeIterator<T>::hasPrev()const
{
	assert(!isNull());
	return myNode_m->prevSibling_m != nullptr;
}

template<typename T>
inline void BaseTreeIterator<T>::goParent()
{
	assert(!isNull());
	assert(hasParent());

	myNode_m = myNode_m->parent_m;
}

template<typename T>
inline void BaseTreeIterator<T>::goNextSibling()
{
	assert(!isNull());
	assert(hasNext());

	myNode_m = myNode_m->nextSibling_m;
}

template<typename T>
inline void BaseTreeIterator<T>::goPrevSibling()
{
	assert(!isNull());
	assert(!isFirst());

	myNode_m = myNode_m->prevSibling_m;
}
template<typename T>

inline void BaseTreeIterator<T>::goFirstChild()
{
	assert(!isNull());
	assert(hasChild());

	myNode_m = myNode_m->firstChild_m;
}

template<typename T>
inline void BaseTreeIterator<T>::goLastChild()
{
	assert(!isNull());
	assert(hasChild());

	myNode_m = myNode_m->lastChild_m;
}

template<typename T>
inline T& BaseTreeIterator<T>::operator*() const
{ 
	assert(!isNull());
	return myNode_m->key_m;
}

template<typename T>
inline T* BaseTreeIterator<T>::operator->() const
{
	assert(!isNull());
	return &(operator*());
}

template<typename T>
inline bool BaseTreeIterator<T>::operator==(
	const BaseTreeIterator<T> iterator) const
{
	if(isNull() || iterator.isNull()) { return false; }
	return myNode_m == iterator.myNode_m;
}

template<typename T>
inline bool BaseTreeIterator<T>::operator!=(
	const BaseTreeIterator<T> iterator) const
{
	if(isNull() || iterator.isNull()) { return false; }
	return !operator==(iterator);
}

template<typename T>
inline BaseTreeIterator<T>::operator bool() const
{
	return !isNull();
}

template<typename T>
inline bool BaseTreeIterator<T>::operator!() const
{
	return !operator bool();
}
