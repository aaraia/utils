#ifndef H_UTILS_STORAGE_QUEUE_H
#define H_UTILS_STORAGE_QUEUE_H

//  includes
#include <stdexcept>

namespace utils {
	namespace storage {

		template<typename T>
		class Queue {

		//  friends
		friend void swap(Queue<T>& lhs, Queue<T>& rhs) noexcept
		{
			Node* temp = lhs.mHead;
			lhs.mHead = rhs.mHead;
			rhs.mHead = temp;				
		}

		private:
			struct Node {
				T mData;
				Node* mNext = nullptr;
			};  //  Node

		public:
			Queue() = default;
			Queue(const Queue<T>& rhs);
			Queue(Queue<T>&& rhs) noexcept;
			~Queue();

			Queue<T>& operator=(const Queue<T>& rhs);
			Queue<T>& operator=(Queue<T>&& rhs) noexcept;

			T& front();
			const T& front() const;
			void pop_front() noexcept;

			template<typename ...Args>
			void emplace_front(Args&&... args);

			template<typename ...Args>
			void emplace_back(Args&&... args);

			void push_front(const T& data);
			void push_front(T&& data);
			void push_back(const T& data);
			void push_back(T&& data);

			void clear();

		private:
			Node* deepCopy(Node* node);

		private:
			Node* mHead = nullptr;

		};  //  Queue

		//  Queue
		template<typename T>
		Queue<T>::Queue(const Queue<T>& rhs)
		{
			if (rhs.mHead) {
				mHead = deepCopy(rhs.mHead);
			}
		}

		template<typename T>
		Queue<T>::Queue(Queue<T>&& rhs) noexcept
		{
			using std::swap;
			swap(*this, rhs);
		}

		template<typename T>
		Queue<T>::~Queue()
		{
			try {
				clear();
			} catch (...) {
				//  T's destructor could be set to noexcept(false) and throw during the delete call
				//  do not allow any exceptions to propogate from a destructor
			}
		}

		template<typename T>
		Queue<T>& Queue<T>::operator=(const Queue<T>& rhs)
		{
			//  check for self-assignment
			if (this != &rhs) {

				//  make a copy of rhs
				Queue<T> rhsCopy(rhs);

				//  swap that copy with this object
				using std::swap;
				swap(*this, rhsCopy);
			}

			return *this;
		}

		template<typename T>
		Queue<T>& Queue<T>::operator=(Queue<T>&& rhs) noexcept
		{
			//  check for self-move
			if (this != &rhs) {

				//  swap with rhs
				using std::swap;
				swap(*this, rhs);
			}

			return *this;
		}

		template<typename T>
		typename Queue<T>::Node* Queue<T>::deepCopy(typename Queue<T>::Node* node)
		{
			if (nullptr == node->mNext) {
				//  at tail

				//  if an exception is thrown here allow it to progate out of this function
				//  to be handled by the calling code
				//  no memory will be leaked
				return new Node{ node->mData, nullptr };
			}

			Node* next = deepCopy(node->mNext);
			Node* current = nullptr;

			try {
				current = new Node{ node->mData, next };
			} catch (...) {
				//  something went wrong
				//  recursively deallocate any nodes that have been allocated up to this point
				Node* i = next;
				while (i) {
					//  save a copy of the next node
					Node* copy = i->mNext;
					//  delete current node
					delete i;
					//  iterate to next node
					i = copy;
				}
			}

			return current;
		}

		template<typename T>
		T& Queue<T>::front()
		{
			if (mHead) {
				return mHead->mData;
			} else {
				throw std::logic_error("trying to get front of empty queue");
			}
		}

		template<typename T>
		const T& Queue<T>::front() const
		{
			if (mHead) {
				return mHead->mData;
			} else {
				throw std::logic_error("trying to get front of empty queue");
			}
		}

		template<typename T>
		void Queue<T>::pop_front() noexcept
		{
			if (mHead) {
				Node* newHead = mHead->mNext;
				Node* oldHead = mHead;
				mHead = newHead;
				delete oldHead;
			}
		}

		template<typename T>
		template<typename ...Args>
		void Queue<T>::emplace_front(Args&&... args)
		{
			push_front(std::forward<Args>(args)...);
		}

		template<typename T>
		template<typename ...Args>
		void Queue<T>::emplace_back(Args&&... args)
		{
			push_back(std::forward<Args>(args)...);
		}

		template<typename T>
		void Queue<T>::push_front(const T& data)
		{
			Node* prevHead = mHead;
			Node* newHead = new Node{ data, prevHead };
			//  if new throws queue will still be in the same state it was before call to push_front
			//  only once we successfully get to this stage do we set the head pointer
			mHead = newHead;
		}

		template<typename T>
		void Queue<T>::push_front(T&& data)
		{
			Node* prevHead = mHead;
			Node* newHead = new Node{ std::move(data), prevHead };
			//  if new throws queue will still be in the same state it was before call to push_front
			//  only once we successfully get to this stage do we set the head pointer
			mHead = newHead;
		}

		template<typename T>
		void Queue<T>::push_back(const T& data)
		{
			//  find the tail
			Node* tail = mHead;
			while (tail) {
				if (nullptr == tail->mNext) break;

				tail = tail->mNext;
			}

			if (tail) {
				Node* newHead = new Node{ data, nullptr };
				//  if new throws the queue will still be in the same state it was before call to push_back
				tail->mNext = newHead;
			} else {
				Node* newHead = new Node{ data, nullptr };
				//  if new throws the queue will still be in the same state is was before call to push_back
				mHead = newHead;
			}

		}

		template<typename T>
		void Queue<T>::push_back(T&& data)
		{
			//  find the tail
			Node* tail = mHead;
			while (tail) {
				if (nullptr == tail->mNext) break;

				tail = tail->mNext;
			}

			if (tail) {
				Node* newHead = new Node{ std::move(data), nullptr };
				//  if new throws queue will still be in the same state as before push_back was called
				tail->mNext = newHead;
			} else {
				Node* newHead = new Node{ std::move(data), nullptr };
				//  if new throws queue will still be in the same state as before push_back was called
				mHead = newHead;
			}

		}

		template<typename T>
		void Queue<T>::clear()
		{
			//  iterate through the queue and destroy all the nodes
			Node* node = mHead;
			while (node) {
				Node* next = node->mNext;
				delete node;
				node = next;
			}

			mHead = nullptr;
		}
	}  //  storage
}  //  utils

#endif 
