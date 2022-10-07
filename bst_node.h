
#ifndef JP_BST_NODE 
#define JP_BST_NODE

namespace jp
{
	template <typename K, typename V>
	struct Pair
	{
		K first;
		V second;
	};


	template <typename K, typename V>
	struct TreeNode
	{
		enum Color
		{
			RED,
			BLACK
		} color;
		TreeNode<K,V>* left;
		TreeNode<K,V>* right;
		TreeNode<K, V>* parent;
		Pair<K, V> pair;

		static TreeNode<K, V>* nil;
	};
}

template <typename K, typename V>
jp::TreeNode<K, V>* jp::TreeNode<K, V>::nil
    = new jp::TreeNode<K, V>{ jp::TreeNode<K, V>::Color::BLACK };

#endif
