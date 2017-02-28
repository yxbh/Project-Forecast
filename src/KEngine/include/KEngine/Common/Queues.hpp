#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace ke
{


/** \class ThreadSafeQueue
    \brief A thread safe queue modified from the one found at: http://codingways.blogspot.com.au/2012/08/implementing-thread-safe-queue-in-c11.html
    Credits for the original work to the author.
*/
template <class T>
class ThreadSafeQueue
{
private :
    std::queue<T>           m_Data;
    mutable std::mutex      m_Mut; // check empty() method for description of "mutable"
    std::condition_variable m_EmptyCondition;

public:
    /** Push a copy of \a pValue to the back of the queue. Execution is blocked if another thread is accessing the queue. */
    void push(T p_Value);
    /** Push \a pValue to the back of the queue. Execution is blocked if another thread is accessing the queue.*/
    void push(T && p_rrValue);
    /** Emplace an element directly. Execution is blocked if another thread is accessing the queue. */
    template<typename... ArgTs >
    void emplace(ArgTs && ... p_rrArgs);
    /** Remove and return the first element in the queue. Thread is blocked if the queue is empty. */
    T pop(void);
    /** Empty the queue. */
    void clear(void);
    /** @return a shared pointer to the first element if there is one, otherwise nullptr is returned. */
    std::shared_ptr<T> tryPop(void);
    /** copy of tryPop(void) */
    bool poll(T & p_Value);
    /** @return true if queue is empty. */
    bool isEmpty(void) const;
    /** @return the number of items in the queue. */
    size_t size() const;

}; // ThreadSafeQueue class

} // KE ns

#include "Queues.inl"
