#ifndef H_UTILS_STORAGE_QUEUE_H
#define H_UTILS_STORAGE_QUEUE_H

#include <stdexcept>

namespace utils {
	namespace storage {

		template<class T>
		class Queue {

		private:

			//  Node
			class Node {
			public:
				explicit Node(const T& data, Node* next);
				explicit Node(T&& data, Node* next);
				Node(const Node& other);
				Node(Node&& other) noexcept;
				~Node();

				Node& operator=(Node other) noexcept;

			private:
				void swap(Node& other) noexcept;

			public:
				T mData;
				Node* mNext;

			};  //  Node

		public:
			Queue();
			Queue(const Queue<T>& other);
			Queue(Queue<T>&& other) noexcept;
			~Queue();

			Queue<T>& operator=(Queue<T> other) noexcept;

			T& front();
			void pop_front() noexcept;
			void push_front(const T& data);
			void push_back(const T& data);
			void push_back(T&& data);

		private:
			void swap(Queue<T>& other) noexcept;
			Node* deepCopy(Node* node);

		private:
			Node * mHead;

		};  //  Queue


		//  Node
		template<class T>
		Queue<T>::Node::Node(const T& data, typename Queue<T>::Node* next)
			: mData(data)
			, mNext(next)
		{

		}

		template<class T>
		Queue<T>::Node::Node(T&& data, typename Queue<T>::Node* next)
			: mData(std::move(data))
			, mNext(next)
		{

		}

		template<class T>
		Queue<T>::Node::Node(typename const Queue<T>::Node& other)
			: mData()
			, mNext(nullptr)
		{
			mData = other.mData;
			mNext = other.mNext;
		}

		template<class T>
		Queue<T>::Node::Node(typename Queue<T>::Node&& other) noexcept
			: mData()
			, mNext(nullptr)
		{
			swap(other);
		}

		template<class T>
		Queue<T>::Node::~Node()
		{
			if (mNext) {
				delete mNext;
				mNext = nullptr;
			}
		}

		template<class T>
		typename Queue<T>::Node& Queue<T>::Node::operator=(typename Queue<T>::Node other) noexcept
		{
			if (this != &other) {
				swap(other);
			}

			return *this;
		}

		template<class T>
		void Queue<T>::Node::swap(typename Queue<T>::Node& other) noexcept
		{
			using std::swap;
			swap(mData, other.mData);
			swap(mNext, other.mNext);
		}


		//  Queue
		template<class T>
		Queue<T>::Queue()
			: mHead(nullptr)
		{

		}

		template<class T>
		Queue<T>::Queue(const Queue<T>& other)
			: mHead(nullptr)
		{
			if (other.mHead) {
				mHead = deepCopy(other.mHead);
			}
		}

		template<class T>
		Queue<T>::Queue(Queue<T>&& other) noexcept
			: mHead(nullptr)
		{
			swap(other);
		}

		template<class T>
		Queue<T>::~Queue()
		{
			if (mHead) {
				delete mHead;
				mHead = nullptr;
			}
		}

		template<class T>
		Queue<T>& Queue<T>::operator=(Queue<T> other) noexcept
		{
			if (this != &other) {
				swap(other);
			}

			return *this;
		}

		template<class T>
		void Queue<T>::swap(Queue<T>& other) noexcept
		{
			using std::swap;
			swap(mHead, other.mHead);
		}

		template<class T>
		typename Queue<T>::Node* Queue<T>::deepCopy(typename Queue<T>::Node* node)
		{
			if (node->mNext == nullptr) {
				//  at tail

				//  if an exception is thrown here allow it to progate out of this function
				//  to be handled by the calling code
				//  no memory will be leaked
				return new Node(node->mData, nullptr);
			}

			Node* next = deepCopy(node->mNext);
			Node* current = nullptr;

			try {
				current = new Node(node->mData, next);
			}
			catch (...) {
				//  something went wrong
				//  recursively deallocate any nodes that have been allocated up to this point
				Node* i = next;
				while (i) {
					Node* copy = i->mNext;
					delete i;
					i = copy;
				}
			}

			return current;
		}

		template<class T>
		T& Queue<T>::front()
		{
			if (mHead) {
				return mHead->mData;
			}
			else {
				throw std::logic_error("trying to get front of empty queue");
			}
		}

		template<class T>
		void Queue<T>::pop_front() noexcept
		{
			if (mHead) {
				Node* newHead = mHead->mNext;
				Node* oldHead = mHead;
				mHead = newHead;
				delete oldHead;
			}
		}

		template<class T>
		void Queue<T>::push_front(const T& data)
		{
			Node* prevHead = mHead;
			Node* newHead = new Node(data, prevHead);
			//  if new throws queue witll still be in the same state it was before call to push_front
			//  only once we successfully get to this stage do we set the head pointer
			mHead = newHead;
		}

		template<class T>
		void Queue<T>::push_back(const T& data)
		{
			//  find the tail
			Node* tail = mHead;
			while (tail) {
				if (tail->mNext == nullptr) break;

				tail = tail->mNext;
			}

			if (tail) {
				Node* newHead = new Node(data, nullptr);
				//  if new throws the queue will still be in the same state is was before call to push_back
				tail->mNext = newHead;
			}
			else {
				Node* newHead = new Node(data, nullptr);
				//  if new throws the queue will still be in the same state is was before call to push_back
				mHead = newHead;
			}

		}

		template<class T>
		void Queue<T>::push_back(T&& data)
		{
			//  find the tail
			Node* tail = mHead;
			while (tail) {
				if (tail->mNext == nullptr) break;

				tail = tail->mNext;
			}

			if (tail) {
				Node* newHead = new Node(std::move(data), nullptr);
				//  if new throws queue will still be in the same state as before push_back was called
				tail->mNext = newHead;
			}
			else {
				Node* newHead = new Node(std::move(data), nullptr);
				//  if new throws queue will still be in the same state as before push_back was called
				mHead = newHead;
			}

		}
	}
}

#endif 
