
#ifndef JP_BST
#define JP_BST

#ifdef _DEBUG
#include <iostream>
#endif

#include "bst_node.h"
#include "tree_iterator.h"
#include "const_tree_iterator.h"

namespace jp
{
    template <typename K, typename V, typename Compare = std::less<K>, typename Allocator = std::allocator<Pair<K, V>>>
	class Tree
	{
	public:
		typedef TreeIterator<K, V> iterator;
        typedef ConstTreeIterator<K, V> const_iterator;

		Tree() noexcept;
		Tree(const Tree<K, V, Compare, Allocator>& other) noexcept;
		Tree(Tree<K, V, Compare, Allocator>&& other) noexcept;
		~Tree() noexcept;
		Tree<K, V, Compare, Allocator>& operator=(const Tree<K, V, Compare, Allocator>& other) noexcept;
		Tree<K, V, Compare, Allocator>& operator=(Tree<K, V, Compare, Allocator>&& other) noexcept;

		void insert(const K& key, const V& value) noexcept(false);
		iterator find(const K& key) noexcept;
		void remove(const K& key) noexcept;
		void clear() noexcept;
		void print() const noexcept;

		TreeNode<K, V>* min(TreeNode<K, V>* node) noexcept;
		TreeNode<K, V>* max(TreeNode<K, V>* node) noexcept;
        TreeNode<K, V>* min(TreeNode<K, V>* node) const noexcept;
        TreeNode<K, V>* max(TreeNode<K, V>* node) const noexcept;

        iterator begin() noexcept;
        const_iterator begin() const noexcept;
		iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() noexcept;
        const_iterator cend() noexcept;

	private:
		TreeNode<K, V>* insert(TreeNode<K,V>*& node, TreeNode<K, V>* prev, const K& key, const V& value) noexcept(false);
		void insertFixup(TreeNode<K, V>* node) noexcept;
		void leftRotate(TreeNode<K, V>* node) noexcept;
		void rightRotate(TreeNode<K, V>* node) noexcept;
		TreeNode<K, V>* find(TreeNode<K, V>* node, const K& key) noexcept;
		void removeFixup(TreeNode<K, V>* node) noexcept;
		void clear (TreeNode<K, V>* node) noexcept;
		void print(TreeNode<K, V>* node, int l = 0) const noexcept;
		void traverse(const TreeNode<K, V>* node) noexcept;
		void transplant(TreeNode<K, V>* first, TreeNode<K, V>* second) noexcept;

	private:
		
		TreeNode<K, V>* m_root;
		Compare m_compare;

		using allocator_type_internal = typename std::allocator_traits<Allocator>::template rebind_alloc<TreeNode<K, V>>;
		allocator_type_internal m_allocator;
		

        template <typename K1, typename V1, typename Compare1, typename Allocator1>
        friend void swap(Tree<K1, V1, Compare1, Allocator1>&& lhs, Tree<K1, V1, Compare1, Allocator1>&& rhs) noexcept;
	};

    template <typename K1, typename V1, typename Compare1, typename Allocator1>
    void swap(Tree<K1, V1, Compare1, Allocator1>&& lhs, Tree<K1, V1, Compare1, Allocator1>&& rhs) noexcept;
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::Tree<K, V, Compare, Allocator>::Tree() noexcept :
	m_root(TreeNode<K, V>::nil)
{
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::Tree<K, V, Compare, Allocator>::Tree(const Tree<K, V, Compare, Allocator>& other) noexcept :
	m_root(TreeNode<K, V>::nil)
{
	traverse(other.m_root);
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::Tree<K, V, Compare, Allocator>::Tree(Tree<K, V, Compare, Allocator>&& other) noexcept :
	m_root(other.m_root)
{
	other.m_root = TreeNode<K, V>::nil;
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::Tree<K, V, Compare, Allocator>::~Tree() noexcept
{
	clear();
}


template<typename K, typename V, typename Compare, typename Allocator>
jp::Tree<K, V, Compare, Allocator>& jp::Tree<K, V, Compare, Allocator>::operator=(const Tree<K, V, Compare, Allocator>& other) noexcept
{
	Tree<K, V> temp(other);
	swap(std::move(*this), std::move(temp));
	return *this;
}


template<typename K, typename V, typename Compare, typename Allocator>
jp::Tree<K, V, Compare, Allocator>& jp::Tree<K, V, Compare, Allocator>::operator=(Tree<K, V, Compare, Allocator>&& other) noexcept
{
	clear();
	m_root = other.m_root;
	other.m_root = TreeNode<K, V>::nil;
	return *this;
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::insert(const K& key, const V& value) noexcept(false)
{
	TreeNode<K, V>* node = insert(m_root, TreeNode<K, V>::nil, key, value);
	insertFixup(node);
}


template<typename K, typename V, typename Compare, typename Allocator>
typename jp::Tree<K, V, Compare, Allocator>::iterator jp::Tree<K, V, Compare, Allocator>::find(const K& key) noexcept
{
	return iterator(find(m_root, key));
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::clear() noexcept
{
	clear(m_root);
	m_root = TreeNode<K, V>::nil;
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::print() const noexcept
{
	print(m_root);
}


template<typename K, typename V, typename Compare, typename Allocator>
jp::TreeNode<K, V>* jp::Tree<K,V, Compare, Allocator>::insert(TreeNode<K, V>*& node, TreeNode<K, V>* prev, const K& key, const V& value) noexcept(false)
{ 
	if (node == TreeNode<K, V>::nil)
	{
        node = m_allocator.allocate(1);
        node = new (node) TreeNode<K, V>; // Placement new
		node->pair.first = key;
		node->pair.second = value;
		node->left = TreeNode<K, V>::nil;
		node->right = TreeNode<K, V>::nil;
		node->parent = prev;
		node->color = TreeNode<K, V>::Color::RED;
		return node;
	}
	else if (key == node->pair.first)
	{
		node->pair.second = value;
		return node;
	}
	else if (m_compare(key, node->pair.first))
	{
		return insert(node->left, node, key, value);
	}
	else // if (key > node->pair.first)
	{
		return insert(node->right, node, key, value);
	}
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::insertFixup(TreeNode<K, V>* node) noexcept
{
	while (node->parent->color == TreeNode<K, V>::Color::RED)
	{
		if (node->parent == node->parent->parent->left)
		{
			TreeNode<K, V>* y = node->parent->parent->right;

			if (y->color == TreeNode<K, V>::Color::RED)
			{
				node->parent->color = TreeNode<K, V>::Color::BLACK;
				y->color = TreeNode<K, V>::Color::BLACK;
				node->parent->parent->color = TreeNode<K, V>::Color::RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->right)
				{
					node = node->parent;
					leftRotate(node);
				}
				node->parent->color = TreeNode<K, V>::Color::BLACK;
				node->parent->parent->color = TreeNode<K, V>::Color::RED;
				rightRotate(node->parent->parent);
			}
		}
		else
		{
			TreeNode<K, V>* y = node->parent->parent->left;

			if (y->color == TreeNode<K, V>::Color::RED)
			{
				node->parent->color = TreeNode<K, V>::Color::BLACK;
				y->color = TreeNode<K, V>::Color::BLACK;
				node->parent->parent->color = TreeNode<K, V>::Color::RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->left)
				{
					node = node->parent;
					rightRotate(node);
				}
				node->parent->color = TreeNode<K, V>::Color::BLACK;
				node->parent->parent->color = TreeNode<K, V>::Color::RED;
				leftRotate(node->parent->parent);
			}
		}
	}
	m_root->color = TreeNode<K, V>::Color::BLACK;
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::leftRotate(TreeNode<K, V>* node) noexcept
{
	TreeNode<K, V>* y = node->right;
	node->right = y->left;

	if (y->left != TreeNode<K, V>::nil)
	{
		y->left->parent = node;
	}
	y->parent = node->parent;
	if (node->parent == TreeNode<K, V>::nil)
	{
		m_root = y;
	}
	else if (node == node->parent->left)
	{
		node->parent->left = y;
	}
	else
	{
		node->parent->right = y;
	}
	y->left = node;
	node->parent = y;
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::rightRotate(TreeNode<K, V>* node) noexcept
{
	TreeNode<K, V>* y = node->left;
	node->left = y->right;

	if (y->right != TreeNode<K, V>::nil)
	{
		y->right->parent = node;
	}
	y->parent = node->parent;
	if (node->parent == TreeNode<K, V>::nil)
	{
		m_root = y;
	}
	else if (node == node->parent->right)
	{
		node->parent->right = y;
	}
	else
	{
		node->parent->left = y;
	}
	y->right = node;
	node->parent = y;
}


template<typename K, typename V, typename Compare, typename Allocator>
jp::TreeNode<K, V>* jp::Tree<K, V, Compare, Allocator>::find(TreeNode<K, V>* node, const K& key) noexcept
{
	if (node != TreeNode<K, V>::nil)
	{
		if (key == node->pair.first)
		{
			return node;
		}
		else if (m_compare(key, node->pair.first))
		{
			return find(node->left, key);
		}
		else // if (key > node->pair.first)
		{
			return find(node->right, key);
		}
	}
	else
	{
		return TreeNode<K, V>::nil; // TODO.
	}
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::remove(const K& key) noexcept
{
	TreeNode<K, V>* node = find(m_root, key);
	TreeNode<K, V>* y = node;
	typename TreeNode<K, V>::Color color = y->color;
	TreeNode<K, V>* x = TreeNode<K, V>::nil;
	if (node->left == TreeNode<K, V>::nil)
	{
		x = node->right;
		transplant(node, node->right);
	}
	else if (node->right == TreeNode<K, V>::nil)
	{
		x = node->left;
		transplant(node, node->left);
	}
	else
	{
		y = min(node->right);
		color = y->color;
		x = y->right;
		if (y->parent == node)
		{
			x->parent = y;
		}
		else
		{
			transplant(y, y->right);
			y->right = node->right;
			y->right->parent = y;
		}
		transplant(node, y);
		y->left = node->left;
		y->left->parent = y;
		y->color = node->color;
	}
	if (color == TreeNode<K, V>::Color::BLACK)
	{
		removeFixup(x);
	}
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::removeFixup(TreeNode<K, V>* node) noexcept
{
	while (node != m_root && node->color == TreeNode<K, V>::Color::BLACK)
	{
		if (node == node->parent->left)
		{
			TreeNode<K, V>* w = node->parent->right;
			if (w->color == TreeNode<K, V>::Color::RED)
			{
				w->color = TreeNode<K, V>::Color::BLACK;
				node->parent->color = TreeNode<K, V>::Color::RED;
				leftRotate(node->parent);
				w = node->parent->right;
			}
			if (w->left->color == TreeNode<K, V>::Color::BLACK
				&& w->right->color == TreeNode<K, V>::Color::BLACK)
			{
				w->color = TreeNode<K, V>::Color::RED;
				node = node->parent;
			}
			else
			{
				if (w->right->color == TreeNode<K, V>::Color::BLACK)
				{
					w->left->color = TreeNode<K, V>::Color::BLACK;
					w->color = TreeNode<K, V>::Color::RED;
					rightRotate(w);
					w = node->parent->right;
				}
				w->color = node->parent->color;
				node->parent->color = TreeNode<K, V>::Color::BLACK;
				w->right->color = TreeNode<K, V>::Color::BLACK;
				leftRotate(node->parent);
				node = m_root;
			}
		}
		else
		{
			TreeNode<K, V>* w = node->parent->left;
			if (w->color == TreeNode<K, V>::Color::RED)
			{
				w->color = TreeNode<K, V>::Color::BLACK;
				node->parent->color = TreeNode<K, V>::Color::RED;
				rightRotate(node->parent);
				w = node->parent->left;
			}
			if (w->right->color == TreeNode<K, V>::Color::BLACK
				&& w->left->color == TreeNode<K, V>::Color::BLACK)
			{
				w->color = TreeNode<K, V>::Color::RED;
				node = node->parent;
			}
			else
			{
				if (w->left->color == TreeNode<K, V>::Color::BLACK)
				{
					w->right->color = TreeNode<K, V>::Color::BLACK;
					w->color = TreeNode<K, V>::Color::RED;
					leftRotate(w);
					w = node->parent->left;
				}
				w->color = node->parent->color;
				node->parent->color = TreeNode<K, V>::Color::BLACK;
				w->left->color = TreeNode<K, V>::Color::BLACK;
				rightRotate(node->parent);
				node = m_root;
			}
		}
	}
	node->color = TreeNode<K, V>::Color::BLACK;
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::clear(TreeNode<K, V>* node) noexcept
{
	if (node != TreeNode<K, V>::nil)
	{
		clear(node->left);
		clear(node->right);
		m_allocator.deallocate(node, 1);
	}
}


template<typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::print(TreeNode<K, V>* node, int l) const noexcept
{
	if (node != TreeNode<K, V>::nil)
	{
		print(node->right, l + 3);
		for (int i = 0; i < l; ++i)
		{
			std::cout << ' ';
		}
		std::cout << node->pair.first << std::endl;
		print(node->left, l + 3);
	}
}


template <typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::traverse(const TreeNode<K, V>* node) noexcept
{
	if (node != TreeNode<K, V>::nil)
	{
		insert(node->pair.first, node->pair.second);
		traverse(node->left);
		traverse(node->right);
	}
}


template <typename K, typename V, typename Compare, typename Allocator>
void jp::Tree<K, V, Compare, Allocator>::transplant(TreeNode<K, V>* first, TreeNode<K, V>* second) noexcept
{
	if (first->parent == TreeNode<K, V>::nil)
	{
		m_root = second;
	}
	else if (first == first->parent->left)
	{
		first->parent->left = second;
	}
	else
	{
		first->parent->right = second;
	}
	second->parent = first->parent;
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::TreeNode<K, V>* jp::Tree<K, V, Compare, Allocator>::min(TreeNode<K, V>* node) noexcept
{
	while (node != TreeNode<K, V>::nil && node->left != TreeNode<K, V>::nil)
	{
		node = node->left;
	}
	return node;
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::TreeNode<K, V>* jp::Tree<K, V, Compare, Allocator>::max(TreeNode<K, V>* node) noexcept
{
	while (node != TreeNode<K, V>::nil && node->right != TreeNode<K, V>::nil)
	{
		node = node->right;
	}
	return node;
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::TreeNode<K, V>* jp::Tree<K, V, Compare, Allocator>::min(TreeNode<K, V>* node) const noexcept
{
    return const_cast<jp::Tree<K, V, Compare, Allocator>*>(this)->min(m_root);
}


template <typename K, typename V, typename Compare, typename Allocator>
jp::TreeNode<K, V>* jp::Tree<K, V, Compare, Allocator>::max(TreeNode<K, V>* node) const noexcept
{
    return const_cast<jp::Tree<K, V, Compare, Allocator>*>(this)->max(m_root);
}


template <typename K, typename V, typename Compare, typename Allocator>
typename jp::Tree<K, V, Compare, Allocator>::iterator jp::Tree<K, V, Compare, Allocator>::begin() noexcept
{
	return iterator(min(m_root));
}


template <typename K, typename V, typename Compare, typename Allocator>
typename jp::Tree<K, V, Compare, Allocator>::const_iterator jp::Tree<K, V, Compare, Allocator>::begin() const noexcept
{
    return const_iterator(min(m_root));
}


template <typename K, typename V, typename Compare, typename Allocator>
typename jp::Tree<K, V, Compare, Allocator>::iterator jp::Tree<K, V, Compare, Allocator>::end() noexcept
{
	return iterator(TreeNode<K, V>::nil);
}


template <typename K, typename V, typename Compare, typename Allocator>
typename jp::Tree<K, V, Compare, Allocator>::const_iterator jp::Tree<K, V, Compare, Allocator>::end() const noexcept
{
     return const_iterator(TreeNode<K, V>::nil);
}


template <typename K, typename V, typename Compare, typename Allocator>
typename jp::Tree<K, V, Compare, Allocator>::const_iterator jp::Tree<K, V, Compare, Allocator>::cbegin() noexcept
{
    return const_iterator(min(m_root));
}


template <typename K, typename V, typename Compare, typename Allocator>
typename jp::Tree<K, V, Compare, Allocator>::const_iterator jp::Tree<K, V, Compare, Allocator>::cend() noexcept
{
    return const_iterator(TreeNode<K, V>::nil);
}


template <typename K, typename V, typename Compare, typename Allocator>
void jp::swap(Tree<K, V, Compare, Allocator>&& lhs, Tree<K, V, Compare, Allocator>&& rhs) noexcept
{
	std::swap(lhs.m_root, rhs.m_root);
}

#endif
