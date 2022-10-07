
#ifndef JP_CONST_TREE_ITERATOR
#define JP_CONST_TREE_ITERATOR

#include "bst_node.h"

namespace jp
{
	template <typename K, typename V>
    class ConstTreeIterator
	{
	public:
        ConstTreeIterator(TreeNode<K, V>* node) :
			m_node(node)
		{
		}


        bool operator==(const ConstTreeIterator<K,V>& other) const
		{
			return m_node == other.m_node;
		}


        bool operator!=(const ConstTreeIterator<K, V>& other) const
		{
			return !(*this == other);
		}


        ConstTreeIterator<K, V>& operator++()
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


        ConstTreeIterator<K, V> operator++(int)
		{
            ConstTreeIterator<K, V> old = *this;
			operator++();
			return old;
		}


        ConstTreeIterator<K, V>& operator+=(size_t count)
		{
			for (size_t i = 0; i < count; ++i)
			{
				++(*this);
			}
			return *this;
		}


        friend ConstTreeIterator<K, V> operator+(ConstTreeIterator<K, V> lhs, size_t count)
		{
			lhs += count;
			return lhs;
		}


        ConstTreeIterator<K, V>& operator--()
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


        ConstTreeIterator<K, V> operator--(int)
		{
            ConstTreeIterator<K, V> old = *this;
			operator--();
			return old;
		}


        ConstTreeIterator<K, V>& operator-=(size_t count)
		{
			for (size_t i = 0; i < count; ++i)
			{
				--(*this);
			}
			return *this;
		}


        friend ConstTreeIterator<K, V> operator-(ConstTreeIterator<K, V> lhs, size_t count)
		{
			lhs -= count;
			return lhs;
        }


		const Pair<K, V>& operator*() const noexcept
		{
			return m_node->pair;
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
