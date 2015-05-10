#pragma once

#include "BaseTreeIterator.h"

template<typename T>
class DepthFirstIterator : public BaseTreeIterator<T>
{
	friend class Tree<T>;
protected:
	DepthFirstIterator() : BaseTreeIterator<T>(){}
public:
	explicit DepthFirstIterator(Node* node) : BaseTreeIterator<T>(node){}
	void postorderIncrement();
	void postorderDecrement();
	void preorderIncrement();
	void preorderDecrement();
};

template<typename T>
void DepthFirstIterator<T>::preorderIncrement()
{
	assert(!isNull()); assert(!isTail());

	if(hasChild()) { goFirstChild(); }
	else {
		while(hasParent() && isLast()) {
			goParent();
		}
		goNextSibling();
	}
}

template<typename T>
void DepthFirstIterator<T>::postorderIncrement()
{
	assert(!isNull()); assert(!isTail());

	if(hasNext()) {
		goNextSibling();
		while(hasChild()) {
			goFirstChild();
		}
	}
	else { goParent(); }
}

template<typename T>
void DepthFirstIterator<T>::preorderDecrement()
{
	assert(!isNull()); assert(!isHead());

	if(hasPrev()) {
		goPrevSibling();
		while(hasChild()) {
			goLastChild();
		}
	}
	else { goParent(); }
}

template<typename T>
void DepthFirstIterator<T>::postorderDecrement()
{
	assert(!isNull()); assert(!isHead());

	if(hasChild()) { goLastChild(); }
	else {
		while(hasParent() && isFirst()) {
			goParent();
		}
		goPrevSibling();
	}
}


template<typename T>
class PreorderIterator : public DepthFirstIterator<T>
{
	friend class Tree<T>;
private:
	PreorderIterator() = default;
public:
	explicit PreorderIterator(Node* node) : DepthFirstIterator<T>(node) {}
	PreorderIterator(PreorderIterator const &) = default;
	PreorderIterator &operator++();
	PreorderIterator operator++(int);
	PreorderIterator &operator--();
	PreorderIterator operator--(int);
};

template<typename T>
inline PreorderIterator<T> &PreorderIterator<T>::operator++()
{
	preorderIncrement();
	return *this;
}

template<typename T>
inline PreorderIterator<T> PreorderIterator<T>::operator++(int)
{
	PreorderIterator tmp = ++(*this);
	return tmp;
}

template<typename T>
inline PreorderIterator<T> &PreorderIterator<T>::operator--()
{
	preorderDecrement();
	return *this;
}

template<typename T>
inline PreorderIterator<T> PreorderIterator<T>::operator--(int)
{
	PreorderIterator tmp = --(*this);
	return tmp;
}

/*
template<typename T>
class PostorderIterator : public DepthFirstIterator<T>
{
	friend class Tree<T>;
private:
	PostorderIterator() {}
public:
	explicit PostorderIterator(Node* node) : DepthFirstIterator(node) {}
	PostorderIterator(PostorderIterator const &) = default;
	PostorderIterator &operator++();
	PostorderIterator operator++(int);
	PostorderIterator &operator--();
	PostorderIterator operator--(int);
};


template<typename T>
inline PostorderIterator<T> &PostorderIterator<T>::operator++()
{
	postorderIncrement();
	return *this;
}

template<typename T>
inline PostorderIterator<T> PostorderIterator<T>::operator++(int)
{
	PostorderIterator tmp = ++(*this);
	return tmp;
}

template<typename T>
inline PostorderIterator<T> &PostorderIterator<T>::operator--()
{
	postorderDecrement();
	return *this;
}

template<typename T>
inline PostorderIterator<T> PostorderIterator<T>::operator--(int)
{
	PostorderIterator tmp = --(*this);
	return tmp;
}
*/
