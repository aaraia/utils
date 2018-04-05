#ifndef H_UTILS_STORAGE_LIST_H
#define H_UTILS_STORAGE_LIST_H


//  includes
#include <initializer_list>

namespace utils {
	namespace storage {

		template <class T>
		class CList
		{
			class CNode
			{
			public:
				CNode() : m_Data(), m_pNext(nullptr) {}
				CNode(const T& t) : m_Data(t), m_pNext(nullptr) {}

				T& GetData() { return m_Data; }
				const T& GetData() const { return m_Data; }

				CNode* GetNext() { return m_pNext; }
				const CNode* GetNext() const { return m_pNext; }

				void SetNext(CNode* pNode) { m_pNext = pNode; }

			private:
				T m_Data;
				CNode* m_pNext;
			};

		public:
			CList();
			explicit CList(const int iSize);
			explicit CList(const std::initializer_list<T>& il);
			CList(const CList& rhs);
			CList(CList&& rhs);

			CList& operator=(const CList& rhs);
			CList& operator=(CList&& rhs);

			~CList();

			void Insert(const T& t);
			void Remove(const T& t);

		private:
			CNode * m_pHead;

		};

		//  default constructor
		template <class T>
		CList<T>::CList()
			: m_pHead(nullptr)
		{

		}

		//  custom constructor - create a list of N size
		template <class T>
		CList<T>::CList(const int iSize)
			: m_pHead(nullptr)
		{
			//  create a temporary pointer to the head of the list
			CNode* pNode = nullptr;
			for (int i = 0; i < iSize; ++i) {
				if (pNode) {
					//  this is a valid node
					//  create a new node
					CNode* pNext = new CNode;

					//  link it to the list
					pNode->SetNext(pNext);

					//  iterate to the next node
					pNode = pNext;
				}
				else {
					//  at the start of the list
					pNode = new CNode;

					//  store the head of the list
					m_pHead = pNode;
				}
			}
		}

		//  custom constructor - create a list populated with data
		template <class T>
		CList<T>::CList(const std::initializer_list<T>& il)
			: m_pHead(nullptr)
		{
			//  create a temporary pointer to the head of the list
			CNode* pNode = m_pHead;
			for (const auto& data : il) {
				if (pNode) {
					//  this is a valid node
					//  create a new node
					CNode* pNext = new CNode(data);

					//  link it to the list
					pNode->SetNext(pNext);

					//  iterate to the next node
					pNode = pNext;
				}
				else {
					//  at start of list
					pNode = new CNode(data);

					//  store the head of the list
					m_pHead = pNode;
				}
			}
		}

		template <class T>
		CList<T>::CList(const CList<T>& rhs)
		{
			CNode* pLhsNode = nullptr;
			CNode* pRhsNode = rhs.m_pHead;
			while (pRhsNode) {
				if (pLhsNode) {
					//  create a copy of the rhs node
					CNode* temp = new CNode(pRhsNode->GetData());

					//  link it to lhs list
					pLhsNode->SetNext(temp);

					//  iterate to next lhs node
					pLhsNode = temp;
				}
				else {
					pLhsNode = new CNode(pRhsNode->GetData());
				}

				//  iterate to next rhs node
				pRhsNode = pRhsNode->GetNext();
			}
		}

		template <class T>
		CList<T>::CList(CList<T>&& rhs)
		{
			//  take ownership of the rhs head pointer
			m_pHead = std::move(rhs.m_pHead);

			//  make sure rhs points to null as it no longer owns the list that rhs.m_pHead use to point to
			rhs.m_pHead = nullptr;
		}

		template <class T>
		CList<T>& CList<T>::operator=(const CList<T>& rhs)
		{
			//  copy
			CList<T> rhsCopy(rhs);

			//  swap
			std::swap(m_pHead, rhs.m_pHead);

			return this;
		}

		template <class T>
		CList<T>& CList<T>::operator=(CList<T>&& rhs)
		{
			//  check for self move
			if (this == &rhs) return *this;

			//  free lhs list resources
			CNode* pNode = m_pHead;
			while (pNode) {
				//  keep a record of the next node in the list
				CNode* pNext = pNode->GetNext();

				//  delete current node
				delete pNode;

				//  iterate to the next node
				pNode = pNext;
			}

			//  take ownership of the internals of rhs
			m_pHead = std::move(rhs.m_pHead);

			//  make sure rhs does not point to same list as lhs
			rhs.m_pHead = nullptr;
		}

		template <class T>
		CList<T>::~CList()
		{
			CNode* pNode = m_pHead;
			while (pNode) {
				//  store the pointer to the next node
				CNode* pNext = pNode->GetNext();

				//  delete current node
				delete pNode;

				//  iterate to next node
				pNode = pNext;
			}

			m_pHead = nullptr;
		}

		template <class T>
		void CList<T>::Insert(const T& t)
		{
			CNode* pNode = m_pHead;
			CNode* pLastNode = nullptr;

			while (pNode) {
				pLastNode = pNode;
				pNode = pNode->GetNext();
			}

			if (pLastNode) {
				//  list has a tail
				//  create a node
				CNode* pTemp = new CNode(t);

				//  append this node to the end of the list
				pLastNode->SetNext(pTemp);
			}
			else {
				//  empty list
				//  append to head
				m_pHead = new CNode(t);
			}
		}

		template <class T>
		void CList<T>::Remove(const T& t)
		{
			CNode* pLastNode = nullptr;
			CNode* pNode = m_pHead;

			while (pNode) {
				if (pNode->GetData() == t) {
					//  found this data in the list

					if (pLastNode) {
						//  unlink the current node and link to the one after it
						pLastNode->SetNext(pNode->GetNext());

						delete pNode;

						//  iterate to next node
						pNode = pLastNode->GetNext();

						//  last node will stay the same as we deleted an item
					}
					else {
						//  item to be removed is at head of list

						//  create a copy of next
						CNode* pNext = pNode->GetNext();

						//  change the head pointer to point to the next node
						m_pHead = pNext;

						//  delete the current node
						delete pNode;

						//  iterate to the next node
						pNode = pNext;
					}
				}
				else {
					//  data not found

					//  keep a record of the last node
					pLastNode = pNode;

					//  iterate to next node
					pNode = pNode->GetNext();
				}
			}
		}
	}
}

#endif
