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

       
        template<typename Key, typename Data>
        typename BST<Key, Data>::Node* BST<Key, Data>::removeHelper(typename BST<Key, Data>::Node* node, const Key& k)
        {
            if (node == nullptr) {
                return nullptr;
            }

            Node* result = node;

            if (k < node->mKey) {
                node->mLeft = removeHelper(node->mLeft, k);
            }
            else if (k > node->mKey) {
                node->mRight = removeHelper(node->mRight, k);
            }
            else {
               
                if (node->mLeft == nullptr || node->mRight == nullptr) {
                    result = node->mLeft == nullptr ? node->mRight : node->mLeft;
                    delete node;
                }
                else {
                  
                    node->mRight = findLeftMostNode(node->mRight, result);
                    result->mLeft = node->mLeft;
                    result->mRight = node->mRight;
                    delete node;
                }
            }
           
            return result;
        }

        template<typename Key, typename Data>
        typename BST<Key, Data>::Node* BST<Key, Data>::findLeftMostNode(typename BST<Key, Data>::Node* node, typename BST<Key, Data>::Node*& result)
        {
            if (node->mLeft == nullptr) {                
                result = node;                
                return node->mRight;
            }
          
            node->mLeft = findLeftMostNode(node->mLeft, result);
            return node;
        }

    }
}

#endif
