#pragma once

#include <string>
#include <iostream>
#include <utility>
/*struct T
{
	std::string data_m;
	static int num_m;
	T() { std::cout << data_m <<" "<<++num_m<< "\tT()" << std::endl; }
	T(T const &t) :data_m(t.data_m) { std::cout << data_m << " " << ++num_m << "\tT(T const &)" << std::endl; }
	T(T &&t) :data_m(std::move(t.data_m)) { std::cout << data_m << " " << ++num_m << "\tT(T &&)" << std::endl; }
	~T() { std::cout << data_m << " " << --num_m << "\t~T()" << std::endl; }
	template<typename A>
	explicit T(A const &a) : data_m(a) { std::cout << data_m << " " << ++num_m << "\tT(" << a << ")" << std::endl; }
	T &operator=(T const&t) { data_m = t.data_m; std::cout << data_m << "\toperator=(T const &)" << std::endl; return *this; }
	T &operator=(T &&t) { data_m = std::move(t.data_m); std::cout << data_m << "\toperator=(T &&)" << std::endl; return *this; }
	bool operator==(T const&t)const { return data_m == t.data_m; }
	bool operator!=(T const&t)const { return data_m != t.data_m; }
	bool operator<(T const&t)const { return data_m < t.data_m; }
	bool operator>(T const&t)const { return data_m > t.data_m; }
	bool operator<=(T const&t)const { return data_m <= t.data_m; }
	bool operator>=(T const&t)const { return data_m >= t.data_m; }
};
*/
template<typename T>
struct TreeNode;

template<typename T>
struct BaseTreeNode
{
	typedef TreeNode<T> Node;
	Node* parent_m = nullptr;
	Node* firstChild_m = nullptr;
	Node* lastChild_m = nullptr;
	Node* prevSibling_m = nullptr;
	Node* nextSibling_m = nullptr;
};

template<typename T>
struct TreeNode :public BaseTreeNode<T>
{
	T key_m;
	
	TreeNode() = default;
	TreeNode(Node const &n) = default;
	//TreeNode(Node &&n) = default;
	~TreeNode() = default;
	template<typename A>
	explicit TreeNode(A const &a);
	Node &operator=(Node const&n) = default;
	//Node &operator=(Node &&t) = default;
};

template<typename T>template<typename A>
inline TreeNode<T>::TreeNode(A const &a) :BaseTreeNode<T>(), key_m(a) {}

template<typename T>
inline bool operator==(TreeNode<T> const &nl, TreeNode<T> const &nr)
{
	return nl.key_m == nr.key_m;
}

template<typename T>
inline bool operator!=(TreeNode<T> const &nl, TreeNode<T> const &nr)
{
	return !(nl == nr);
}
