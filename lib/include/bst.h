#ifndef H_UTILS_STORAGE_BST_H
#define H_UTILS_STORAGE_BST_H

#include <utility>

namespace utils {
	namespace storage {

		template<typename Key, typename Data>
		class BST
		{
		public:
			enum E_INSERT_RESULT
			{
				OVERWRITE_SAME_VALUE_IR,
				OVERWRITE_DIFFERENT_VALUE_IR,
				NEW_INSERT_IR
			};

			typedef std::pair<bool, E_INSERT_RESULT> INSERT_RESULT;

		private:
			struct Node
			{
				Key mKey;
				Data mData;
				Node* mLeft;
				Node* mRight;
			};

		public:
			BST() : mRoot(nullptr) {}

			INSERT_RESULT insert(const Key& k, const Data& d);
			bool remove(const Key& k);

		private:
			Node* insertHelper(Node* node, const Key& k, const Data& d, INSERT_RESULT& insertResult);
			Node* removeHelper(Node* node, const Key& k);
			Node* findLeftMostNode(Node* node, Node*& result);

		public:
			Node * mRoot;
		};

		template<typename Key, typename Data>
		typename BST<Key, Data>::INSERT_RESULT BST<Key, Data>::insert(const Key& k, const Data& d)
		{
			INSERT_RESULT insertResult;

			if (mRoot) {
				insertHelper(mRoot, k, d, insertResult);
			}
			else {
				insertResult.first = true;
				insertResult.second = NEW_INSERT_IR;
				mRoot = new Node{ k,d,nullptr, nullptr };
			}

			return insertResult;
		}

		template<typename Key, typename Data>
		typename BST<Key, Data>::Node* BST<Key, Data>::insertHelper(typename BST<Key, Data>::Node* node, const Key& k, const Data& d, typename BST<Key, Data>::INSERT_RESULT& insertResult)
		{
			if (node == nullptr) {
				insertResult.first = true;
				insertResult.second = NEW_INSERT_IR;
				return new Node{ k, d, nullptr, nullptr };
			}

			if (node->mKey < k) {
				node->mRight = insertHelper(node->mRight, k, d, insertResult);
			}
			else if (node->mKey > k) {
				node->mLeft = insertHelper(node->mLeft, k, d, insertResult);
			}
			else {

				insertResult.first = true;

				if (node->mData == d) {
					insertResult.second = OVERWRITE_SAME_VALUE_IR;
				}
				else {
					insertResult.second = OVERWRITE_DIFFERENT_VALUE_IR;
				}

				node->mData = d;
			}

			return node;
		}

		template<typename Key, typename Data>
		bool BST<Key, Data>::remove(const Key& k)
		{
			if (mRoot) {
				if (removeHelper(mRoot, k)) {
					return true;
				}
			}

			return false;
		}

		// This is how I would write it.
		// Slightly different as I would use pointers not smart pointers
		// But I am sure you can modify to copensate.
		template<typename Key, typename Data>
		typename BST<Key, Data>::Node* BST<Key, Data>::removeHelper(typename BST<Key, Data>::Node* node, const Key& k)
		{
			// When you fall off the end
			// the key was not found. So just return null.
			if (node == nullptr) {
				return nullptr;
			}


			// In most cases we return the current node.
			// So lets keep track of that separately. If this
			// is the node we are going to delete then we update
			// this with the node we are going to replace it with.
			Node* result = node;


			if (k < node->mKey) {
				// Note if we don't remove node-left then it will
				// return back the same value and nothing happens.
				// If we do delete node->left we will return the replacement value.
				// Note: In this case the node is not deleted so result is not changed.
				node->mLeft = removeHelper(node->mLeft, k);
			}
			else if (k > node->mKey) {
				// See comment above about left.
				node->mRight = removeHelper(node->mRight, k);
			}
			else {

				// We found the node we delete.
				// In here we must change `result` to reflect the new node.

				// If either of the sub-trees is empty this becomes easy as the other
				// sub-tree becomes the new current node.
				if (node->mLeft == nullptr || node->mRight == nullptr) {
					result = node->mLeft == nullptr ? node->mRight : node->mLeft;
					delete node;
				}
				else {
					// OK both sub trees are not null.
					// We must find a node to be used here.
					// A node where all the nodes to the right are larger
					// and all the nodes to the left are smaller.
					//
					// If we look in the right sub-tree. Then the left most
					// node in this sub tree fulfills these requirements.
					// We can also easily remove it as it will not have a
					// left-subtree. Once we have it assign the
					// left and right sub-trees of the current node to it
					// It will be inserted into the tree with the return
					// below.
					node->mRight = findLeftMostNode(node->mRight, result);
					result->mLeft = node->mLeft;
					result->mRight = node->mRight;
					delete node;
				}

			}
			// If you did not fall of the end.
			// Then return the result
			return result;
		}

		template<typename Key, typename Data>
		typename BST<Key, Data>::Node* BST<Key, Data>::findLeftMostNode(typename BST<Key, Data>::Node* node, typename BST<Key, Data>::Node*& result)
		{
			if (node->mLeft == nullptr) {
				// We found the left most node. So this is the result.
				result = node;
				// We return its right-subtree so that it can be glued
				// back into the BST.
				return node->mRight;
			}

			// We have not found the left most node.
			// So keep looking.
			node->mLeft = findLeftMostNode(node->mLeft, result);
			return node;
		}

	}
}

#endif
