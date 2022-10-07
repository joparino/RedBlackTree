
#ifndef JP_TREE_ITERATOR
#define JP_TREE_ITERATOR

#include "bst_node.h"

namespace jp
{
	template <typename K, typename V>
	class TreeIterator
	{
	public:
		TreeIterator(TreeNode<K, V>* node) :
			m_node(node)
		{
		}


		bool operator==(const TreeIterator<K,V>& other)
		{
			return m_node == other.m_node;
		}


		bool operator!=(const TreeIterator<K, V>& other)
		{
			return !(*this == other);
		}


		TreeIterator<K, V>& operator++()
		{
			if (m_node->right != TreeNode<K, V>::nil)
			{
				TreeNode<K, V>* node = m_node->right;
				while (node != TreeNode<K, V>::nil && node->left != TreeNode<K, V>::nil)
				{
					node = node->left;
				}
				m_node = node;
				return *this;
			}
			
			TreeNode<K, V>* parent = m_node->parent;
			TreeNode<K, V>* node = m_node;
			while (parent != TreeNode<K, V>::nil && node == parent->right)
			{
				node = parent;
				parent = parent->parent;
			}
			m_node = parent;
			return *this;
		}


		TreeIterator<K, V> operator++(int)
		{
			TreeIterator<K, V> old = *this;
			operator++();
			return old;
		}


		TreeIterator<K, V>& operator+=(size_t count)
		{
			for (size_t i = 0; i < count; ++i)
			{
				++(*this);
			}
			return *this;
		}


		friend TreeIterator<K, V> operator+(TreeIterator<K, V> lhs, size_t count)
		{
			lhs += count;
			return lhs;
		}


		TreeIterator<K, V>& operator--()
		{
			if (m_node->left != TreeNode<K, V>::nil)
			{
				TreeNode<K, V>* node = m_node->left;
				while (node != TreeNode<K, V>::nil && node->right != TreeNode<K, V>::nil)
				{
					node = node->right;
				}
				m_node = node;
				return *this;
			}

			TreeNode<K, V>* parent = m_node->parent;
			TreeNode<K, V>* node = m_node;
			while (parent != TreeNode<K, V>::nil && node == parent->left)
			{
				node = parent;
				parent = parent->parent;
			}
			m_node = parent;
			return *this;
		}


		TreeIterator<K, V> operator--(int)
		{
			TreeIterator<K, V> old = *this;
			operator--();
			return old;
		}


		TreeIterator<K, V>& operator-=(size_t count)
		{
			for (size_t i = 0; i < count; ++i)
			{
				--(*this);
			}
			return *this;
		}


		friend TreeIterator<K, V> operator-(TreeIterator<K, V> lhs, size_t count)
		{
			lhs -= count;
			return lhs;
		}


		Pair<K, V>& operator*() noexcept
		{
			return m_node->pair;
		}


		const Pair<K, V>& operator*() const noexcept
		{
			return m_node->pair;
		}


		Pair<K, V>* operator->() noexcept
		{
			return &(m_node->pair);
		}


		const Pair<K, V>* operator->() const noexcept
		{
			return &(m_node->pair);
		}

	private:
		TreeNode<K, V>* m_node;
	};
}

#endif
