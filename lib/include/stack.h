#ifndef H_UTILS_STORAGE_STACK_H
#define H_UTILS_STORAGE_STACK_H

//  includes
#include <memory>
#include <limits>	//  needed for clang build

namespace utils {
    namespace storage {

        template<typename T>
        class Stack {

        public:
            friend void swap(Stack<T>& lhs, Stack<T>& rhs) noexcept
            {
                std::size_t tempCapacity(lhs.mCapacity);
                lhs.mCapacity = rhs.mCapacity;
                rhs.mCapacity = tempCapacity;

                std::size_t tempSize(lhs.mSize);
                lhs.mSize = rhs.mSize;
                rhs.mSize = tempSize;

                T* tempBase(lhs.mBase);
                lhs.mBase = rhs.mBase;
                rhs.mBase = tempBase;
            }

        private:
            enum { DEFAULT_SIZE = 5 };

        public:
            Stack() = default;
            Stack(const Stack<T>& rhs);
            Stack(Stack<T>&& rhs) noexcept;
            ~Stack() noexcept;

            Stack& operator=(const Stack<T>& rhs);
            Stack& operator=(Stack<T>&& rhs) noexcept;

            bool empty() const;
            void pop();
            bool push(const T& t);
            bool push(T&& t);
            const T& top() const;
            T& top();

            template<class... Args>
            void emplace(Args&&... args);

            std::size_t getCapacity() const { return mCapacity; }
            std::size_t getSize() const { return mSize; }

        private:
            bool resize(const std::size_t newCapacity);
            T* safeDeepCopy(const T* const source, const std::size_t capacity, const std::size_t size);
            T* move(const T* const source, const std::size_t capacity, const std::size_t size);
            void clear();

        private:
            std::size_t mCapacity = 0;
            std::size_t mSize = 0;
            T* mBase = nullptr;
        };

        template<typename T>
        Stack<T>::Stack(const Stack<T>& rhs)
        {
            try {
                mBase = safeDeepCopy(rhs.mBase, rhs.mCapacity, rhs.mSize);
            } catch (...) {
                throw;
            }

            mCapacity = rhs.mCapacity;
            mSize = rhs.mSize;
        }

        template<typename T>
        Stack<T>::Stack(Stack<T>&& rhs) noexcept
        {
            using std::swap;
            swap(*this, rhs);
        }

        template<typename T>
        Stack<T>::~Stack() noexcept
        {
            try {
                clear();
            } catch (...) {
                //  T's destructor could be set to noexcept(false) and throw during the delete call
                //  do not allow any exceptions to propogate from a destructor
            }
        }

        template<typename T>
        Stack<T>& Stack<T>::operator=(const Stack<T>& rhs)
        {
            //  check for self assignment
            if (this != &rhs) {

                //  make a copy
                Stack<T> rhsCopy(rhs);

                //  swap with copy
                using std::swap;
                swap(*this, rhsCopy);
            }

            //  copy will be destructed when function returns

            return *this;
        }

        template<typename T>
        Stack<T>& Stack<T>::operator=(Stack<T>&& rhs) noexcept
        {
            //  check for self move
            if (this != &rhs) {
                //  swap with rhs to take over its internals
                using std::swap;
                swap(*this, rhs);
            }

            //  rhs is now responsible for "this" resources

            return *this;
        }

        //  no op on an empty stack
        template<typename T>
        void Stack<T>::pop()
        {
            if (empty()) return;

            (mBase + (mSize - 1))->~T();

            --mSize;
        }

        template<typename T>
        bool Stack<T>::push(const T& t)
        {
            if ((mSize + 1) > mCapacity) {
                if (mCapacity < (std::numeric_limits<std::size_t>::max() / 3)) {
                    if (mCapacity == 0) {
                        //  empty stack
                        if (!resize(DEFAULT_SIZE)) {
                            //  failed to resize
                            return false;
                        }
                    } else {
                        if (!resize(mCapacity * 2)) {
                            //  failed to resize
                            return false;
                        }
                    }
                } else {
                    //  cannot grow any larger
                    return false;
                }
            }

            //  T constructor may throw
            new (mBase + mSize) T(t);

            //  successfully pushed
            //  increment
            ++mSize;

            return true;
        }

        template<typename T>
        bool Stack<T>::push(T&& t)
        {
            if ((mSize + 1) > mCapacity) {
                if (mCapacity < (std::numeric_limits<std::size_t>::max() / 3)) {
                    if (mCapacity == 0) {
                        //  empty stack
                        if (!resize(DEFAULT_SIZE)) {
                            //  failed to resize
                            return false;
                        }

                    } else {
                        if (!resize(mCapacity * 2)) {
                            //  failed to resize
                            return false;
                        }
                    }
                } else {
                    //  cannot grow any larger
                    return false;
                }
            }

            //  T move constructor may throw, shouldnt but may do
            new (mBase + mSize) T(std::move(t));

            ++mSize;

            return true;
        }

        //  returns a const reference to the top element in the stack
        //  it is up to the caller to make sure stack is non-empty using Empty() function
        template<typename T>
        const T& Stack<T>::top() const
        {
            return *(mBase + (mSize - 1));
        }

        //  returns a reference to the top element in the stack
        //  it is up to the caller to make sure stack is non-empty using Empty() function
        template<typename T>
        T& Stack<T>::top()
        {
            return *(mBase + (mSize - 1));
        }

        template<typename T>
        template<class... Args>
        void Stack<T>::emplace(Args&&... args)
        {
            //  depending on type deduction call the right push method
            push(std::forward<Args>(args)...);
        }

        //  checks size of stack and returns true (if empty) or false 
        template<typename T>
        bool Stack<T>::empty() const
        {
            return mSize == 0;
        }

        //  increases the size of the stack by allocating a larger block of memory
        //  and copying the stacks elements to it.
        template<typename T>
        bool Stack<T>::resize(const std::size_t newCapacity)
        {
            try {
                T* p = move(mBase, newCapacity, mSize);
                mBase = p;
                mCapacity = newCapacity;
                return true;
            } catch (...) {
                return false;
            }
        }

        template<typename T>
        T* Stack<T>::safeDeepCopy(const T* const source, const std::size_t capacity, const std::size_t size)
        {
            T* p = nullptr;
            std::size_t current = 0;

            //  we try our deep copy
            //  if any exceptions are thrown
            //  we call the destructor of any already copy-constructed Ts
            //  and clear up any memory

            try {
                p = static_cast<T*>(::operator new(sizeof(T) * capacity));

                for (std::size_t i = 0; i < size; ++i, ++current) {
                    new (p + i) T(*(source + i));
                }

                return p;
            } catch (...) {
                for (std::size_t i = 0; i < current; ++i) {
                    (p + i)->~T();
                }

                ::operator delete (p);

                throw;
            }
        }

        template<typename T>
        T* Stack<T>::move(const T* const source, const std::size_t capacity, const std::size_t size)
        {
            T* p = nullptr;
            std::size_t current = 0;

            //  we try our move
            //  if any exceptions are thrown
            //  we call the destructor of any already move constructed Ts
            //  and clear up any memory
            try {
                p = static_cast<T*>(::operator new(sizeof(T) * capacity));

                for (std::size_t i = 0; i < size; ++i, ++current) {
                    new (p + i) T(std::move(*(source + i)));
                }

                return p;
            } catch (...) {
                for (std::size_t i = 0; i < current; ++i) {
                    (p + i)->~T();
                }

                ::operator delete (p);

                throw;
            }
        }

        template<typename T>
        void Stack<T>::clear()
        {
            if (!mBase) return;

            for (std::size_t i = 0; i < mSize; ++i) {
                (mBase + i)->~T();
            }

            ::operator delete (mBase);
        }
    }
}

#endif











