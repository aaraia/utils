#ifndef H_UTILS_STORAGE_LIST_H
#define H_UTILS_STORAGE_LIST_H


//  includes
#include <initializer_list>
#include <utility>

namespace utils {
	namespace storage {

		template <typename T>
		class List {

			friend void swap(List<T>& lhs, List<T>& rhs) noexcept
			{
				Node* tempHead(lhs.mHead);
				lhs.mHead = rhs.mHead;
				rhs.mHead = tempHead;

				std::size_t tempSize(lhs.mSize);
				lhs.mSize = rhs.mSize;
				rhs.mSize = tempSize;
			}

		private:
			//  Node
			struct Node {
				T mData;
				Node* mNext = nullptr;
			};

		public:
			List() = default;
			explicit List(const std::size_t size);
			explicit List(const std::initializer_list<T>& il);
			List(const List& rhs);
			List(List&& rhs) noexcept;
			~List();

			List& operator=(const List& rhs);
			List& operator=(List&& rhs);

			template<typename ...Args>
			void emplace(Args&&... args);

			void insert(const T& t);
			void insert(T&& t);

			void remove(const T& t);
			void clear();

			std::size_t getSize() const { return mSize; }

		private:
			Node* mHead = nullptr;
			std::size_t mSize = 0;
		};

		//  custom constructor - create a list of N size
		template <typename T>
		List<T>::List(const std::size_t size)
		{
			//  empty list?
			if (size == 0) return;

			//  create head
			mHead = new Node{};

			//  create a temporary pointer to the head of the list
			Node* node = mHead;
			for (int i = 0; i < size - 1; ++i) {
				//  this is a valid node
				//  create a new node
				//  link it to the list
				node->mNext = new Node{};

				//  iterate to the next node
				node = node->mNext;
			}

			mSize = size;
		}

		//  custom constructor - create a list populated with data
		template <typename T>
		List<T>::List(const std::initializer_list<T>& il)
		{
			//  empty list?
			if (il.size() == 0) return;

			//  get first element in list and make the head node
			auto it = il.begin();
			mHead = new Node{ (*it), nullptr };
			++it;

			//  create a temporary pointer to the head of the list
			Node* node = mHead;
			for (; it != il.end(); ++it) {
				//  this is a valid node
				//  create a new node
				//  link it to the list
				node->mNext = new Node{ (*it), nullptr };

				//  iterate to the next node
				node = node->mNext;
			}

			mSize = il.size();
		}

		template <typename T>
		List<T>::List(const List<T>& rhs)
		{
			Node* lhsNode = nullptr;
			Node* rhsNode = rhs.mHead;

			//  create the head node
			if (rhsNode) {								
				lhsNode = new Node{ rhsNode->mData, nullptr };
				mHead = lhsNode;

				//  iterate to next rhs node
				rhsNode = rhsNode->mNext;
			}

			//  create rest of list
			while (rhsNode && lhsNode) {

				//  create a copy of the rhs node
				//  link it to lhs list
				lhsNode->mNext = new Node{ rhsNode->mData, nullptr };

				//  iterate to next lhs node
				lhsNode = lhsNode->mNext;

				//  iterate to next rhs node
				rhsNode = rhsNode->mNext;
			}

			mSize = rhs.mSize;
		}

		template <typename T>
		List<T>::List(List<T>&& rhs) noexcept
		{
			using std::swap;
			swap(*this, rhs);
		}

		template <typename T>
		List<T>::~List()
		{
			try {
				clear();
			} catch (...) {
				//  T's destructor could be set to noexcept(false) and throw during the delete call
				//  do not allow any exceptions to propogate from a destructor
			}
		}

		template <typename T>
		List<T>& List<T>::operator=(const List<T>& rhs)
		{
			//  check for self assignment
			if (this != &rhs) {

				//  make a copy of rhs
				List<T> rhsCopy(rhs);

				//  swap
				using std::swap;
				swap(*this, rhsCopy);
			}

			return *this;
		}

		template <typename T>
		List<T>& List<T>::operator=(List<T>&& rhs)
		{
			//  check for self move
			if (this != &rhs) {
				
				//  swap
				using std::swap;
				swap(*this, rhs);
			}

			return *this;
		}

		template<typename T>
		template<typename ...Args>
		void List<T>::emplace(Args&&... args)
		{
			//  forward to the relevant insert
			insert(std::forward<Args>(args)...);
		}

		template <typename T>
		void List<T>::insert(const T& t)
		{
			Node* tail = mHead;

			//  find the tail of the list
			while (tail && tail->mNext) {
				tail = tail->mNext;
			}

			if (tail) {
				//  list has a tail
				//  append this node to the end of the list
				tail->mNext = new Node{ t, nullptr };
			} else {
				//  empty list
				//  append to head
				mHead = new Node{ t, nullptr };
			}

			++mSize;
		}

		template <typename T>
		void List<T>::insert(T&& t)
		{
			Node* tail = mHead;

			//  find the tail of the list
			while (tail && tail->mNext) {
				tail = tail->mNext;
			}

			if (tail) {
				//  list has a tail
				//  append this node to the end of the list
				tail->mNext = new Node{ std::move(t), nullptr };
			} else {
				//  empty list
				//  append to head
				mHead = new Node{ std::move(t), nullptr };
			}

			++mSize;
		}

		template <typename T>
		void List<T>::remove(const T& t)
		{
			Node* node = mHead;
			Node* prev = nullptr;

			while (node) {
				//  found?
				if (node->mData == t) {
					if (prev) {
						//  unlink from list, delete and decrease size of list
						Node* next = node->mNext;
						delete node;
						node = nullptr;
						prev->mNext = next;						
						--mSize;
					} else {
						//  remove head
						//  decrease size of list
						delete node;
						node = nullptr;
						mHead = nullptr;
						--mSize;
					}
				} else {
					//  data not found
					//  iterate to next node
					prev = node;
					node = node->mNext;
				}
			}
		}

		template<typename T>
		void List<T>::clear()
		{
			Node* node = mHead;
			while (node) {
				//  store the pointer to the next node
				Node* next = node->mNext;

				//  delete current node
				delete node;

				//  T's destructor could be set to noexcept(false) and throw
				//  could wrap "delete node" in a try/catch to suppress any exceptions
				//  and continue deleting the rest of the nodes else there could be leaked memory
				//

				//  iterate to next node
				node = next;
			}

			mHead = nullptr;
			mSize = 0;
		}
	}
}

#endif
