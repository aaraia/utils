#ifndef H_UTILS_STORAGE_STACK_H
#define H_UTILS_STORAGE_STACK_H

//  includes
#include <memory>
#include <limits>	//  needed for clang build

namespace utils {
	namespace storage {

		template<class T>
		class Stack
		{

		private:
			enum { DEFAULT_SIZE = 5 };

		public:
			Stack() noexcept;
			Stack(const Stack<T>& rhs);
			Stack(Stack<T>&& rhs) noexcept;
			Stack& operator=(Stack<T> rhs) noexcept;
			~Stack();

			bool empty() const;
			void pop();
			bool push(const T& t);
			bool push(T&& t);
			const T& top() const;
			T& top();

			template<class... Args>
			void emplace(Args&&... args);

		private:
			bool resize(const std::size_t iNewCapacity);
			T* safeDeepCopy(const T* const pSource, const std::size_t iCapacity, const std::size_t iSize);
			T* move(const T* const pSource, const std::size_t iCapacity, const std::size_t iSize);
			void swap(Stack<T>& rhs) noexcept;
			void clear();

		private:
			std::size_t m_iCapacity;
			std::size_t m_iSize;
			T* m_pBase;
		};

		template<class T>
		template<class... Args>
		void Stack<T>::emplace(Args&&... args)
		{
			push(std::forward<Args>(args)...);
		}

		template<class T>
		Stack<T>::Stack() noexcept
			: m_iCapacity(0)
			, m_iSize(0)
			, m_pBase(nullptr)
		{

		}

		template<class T>
		Stack<T>::Stack(const Stack<T>& rhs)
			: m_iCapacity(0)
			, m_iSize(0)
			, m_pBase(nullptr)
		{
			try {
				m_pBase = safeDeepCopy(rhs.m_pBase, rhs.m_iCapacity, rhs.m_iSize);
			}
			catch (...) {
				throw;
			}

			m_iCapacity = rhs.m_iCapacity;
			m_iSize = rhs.m_iSize;
		}

		template<class T>
		Stack<T>::Stack(Stack<T>&& rhs) noexcept
			: m_iCapacity(0)
			, m_iSize(0)
			, m_pBase(nullptr)
		{
			swap(rhs);
		}

		template<class T>
		Stack<T>& Stack<T>::operator=(Stack<T> rhs) noexcept
		{
			//  taking parameter by value
			//  1) with rvalue will move
			//  2) with lvalue will copy 

			swap(rhs);

			return *this;
		}

		template<class T>
		Stack<T>::~Stack()
		{
			//  T destructor could throw
			//  if it does do not allow the exception 
			//  to propagate out of the destructor
			try {
				clear();
			}
			catch (...) {
				//  maybe some error logging if the client has enabled it? CERR_ENABLED
			}
		}

		//  no op on an empty stack
		template<class T>
		void Stack<T>::pop()
		{
			if (empty()) return;

			try {
				(m_pBase + (m_iSize - 1))->~T();
			}
			catch (...) {
				throw;
			}

			--m_iSize;
		}

		template<class T>
		bool Stack<T>::push(const T& t)
		{
			if ((m_iSize + 1) > m_iCapacity) {
				if (m_iCapacity < (std::numeric_limits<std::size_t>::max() / 3)) {
					if (!resize((m_iCapacity == 0 ? DEFAULT_SIZE : m_iCapacity * 2))) {
						//  failed to resize
						return false;
					}
				}
				else {
					//  cannot grow any larger
					return false;
				}
			}

			try {
				//  T constructor may throw
				new (m_pBase + m_iSize) T(t);
			}
			catch (...) {
				throw;
			}

			//  successfully pushed
			//  increment
			++m_iSize;

			return true;
		}

		template<class T>
		bool Stack<T>::push(T&& t)
		{
			if ((m_iSize + 1) > m_iCapacity) {
				if (m_iCapacity < (std::numeric_limits<std::size_t>::max() / 3)) {
					if (!resize(m_iCapacity * 2)) {
						//  failed to resize
						return false;
					}
				}
				else {
					//  cannot grow any larger
					return false;
				}
			}

			try {
				//  T move constructor may throw, shouldnt but may do
				new (m_pBase + m_iSize) T(std::move(t));
			}
			catch (...) {
				throw;
			}

			++m_iSize;

			return true;
		}

		//  returns a const reference to the top element in the stack
		//  it is up to the caller to make sure stack is non-empty using Empty() function
		template<class T>
		const T& Stack<T>::top() const
		{
			return *(m_pBase + (m_iSize - 1));
		}

		//  returns a reference to the top element in the stack
		//  it is up to the caller to make sure stack is non-empty using Empty() function
		template<class T>
		T& Stack<T>::top()
		{
			return *(m_pBase + (m_iSize - 1));
		}

		//  checks size of stack and returns true (if empty) or false 
		template<class T>
		bool Stack<T>::empty() const
		{
			return m_iSize == 0;
		}

		//  increases the size of the stack by allocating a larger block of memory
		//  and copying the stacks elements to it.
		template<class T>
		bool Stack<T>::resize(const std::size_t iNewCapacity)
		{
			try {
				T* p = move(m_pBase, iNewCapacity, m_iSize);
				m_pBase = p;
				m_iCapacity = iNewCapacity;
				return true;
			}
			catch (...) {
				return false;
			}
		}

		template<class T>
		T* Stack<T>::safeDeepCopy(const T* const pSource, const std::size_t iCapacity, const std::size_t iSize)
		{
			T* p = nullptr;
			std::size_t iCurrent = 0;

			try {
				p = static_cast<T*>(::operator new(sizeof(T) * iCapacity));

				for (std::size_t i = 0; i < iSize; ++i, ++iCurrent) {
					new (p + i) T(*(pSource + i));
				}

				return p;
			}
			catch (...) {
				for (std::size_t i = 0; i < iCurrent; ++i) {
					(p + i)->~T();
				}

				::operator delete (p);

				throw;
			}
		}

		template<class T>
		T* Stack<T>::move(const T* const pSource, const std::size_t iCapacity, const std::size_t iSize)
		{
			T* p = nullptr;
			std::size_t iCurrent = 0;

			try {
				p = static_cast<T*>(::operator new(sizeof(T) * iCapacity));

				for (std::size_t i = 0; i < iSize; ++i, ++iCurrent) {
					new (p + i) T(std::move(*(pSource + i)));
				}

				return p;
			}
			catch (...) {
				for (std::size_t i = 0; i < iCurrent; ++i) {
					(p + i)->~T();
				}

				::operator delete (p);

				throw;
			}
		}

		template<class T>
		void Stack<T>::swap(Stack<T>& rhs) noexcept
		{
			using std::swap;
			swap(m_pBase, rhs.m_pBase);
			swap(m_iCapacity, rhs.m_iCapacity);
			swap(m_iSize, rhs.m_iSize);
		}

		template<class T>
		void Stack<T>::clear()
		{
			if (!m_pBase) return;

			for (std::size_t i = 0; i < m_iSize; ++i) {
				(m_pBase + i)->~T();
			}

			::operator delete (m_pBase);
		}
	}
}

#endif











