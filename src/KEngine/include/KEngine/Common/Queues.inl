#pragma once

namespace ke
{

template<typename T>
inline void ThreadSafeQueue<T>::push(T p_Value)
{
    // block execution here, if other thread already locked mMute!
    std::lock_guard<std::mutex> lock(m_Mut);
    // if weare here no other thread is owned/locked mMute. so we can modify the internal data
    m_Data.push(p_Value);
    // OK! notify one of threads which was blocked because , queue empty and make them happy!
    m_EmptyCondition.notify_one();
} // //lock.unlock(); !!!! does not have such method!
// now mMute is unlocked in destcutor of lock_guard!

template<typename T>
inline void ThreadSafeQueue<T>::push(T && p_rrArg)
{
    // block execution here, if other thread already locked mMute!
    std::lock_guard<std::mutex> lock(m_Mut);
    // if weare here no other thread is owned/locked mMute. so we can modify the internal data
    m_Data.push(std::forward(p_rrArg));
    // OK! notify one of threads which was blocked because , queue empty and make them happy!
    m_EmptyCondition.notify_one();
} // //lock.unlock(); !!!! does not have such method!
// now mMute is unlocked in destcutor of lock_guard!

template<class T> template<typename... ArgTs>
void ThreadSafeQueue<T>::emplace(ArgTs && ... p_rrArgs)
{
    std::lock_guard<std::mutex> lock(m_Mut);
    m_Data.emplace(std::forward<ArgTs>(p_rrArgs)...);
    m_EmptyCondition.notify_one();
}

template<typename T>
inline T ThreadSafeQueue<T>::pop(void)
{
    // own the mMute like in Push method excep that lock can be unlocked manually without need to destructor of lock!
    std::unique_lock<std::mutex> lock(m_Mut);
    // if we are here. mMute is locked and no other thread can access/modify the data!
    // wait() method first checks if mData is not empty, allowes execution to go on. else :
    // unlocks the mMut and waits for signla.
    // because mMute is released other threads have a chance to Push new data into queue
    // ... in notify this condition variable!
    m_EmptyCondition.wait(lock,[this] { return !m_Data.empty();});
    // if we are are here, mData is not empty and mMut is locked !
    // be careful ! std::shared_ptr<T> ret(new T(mData.front()) performs 2 memory allocation!
    auto ret(m_Data.front());
    m_Data.pop();
    return std::move(ret);
}

template<typename T>
inline void ThreadSafeQueue<T>::clear(void)
{
    std::unique_lock<std::mutex> lock(m_Mut);
    while (!m_Data.empty())
        m_Data.pop();
    /*
        This method is custom wrote. Therefore it could be incorrect.
        e.g. Not sure if I'm suppose to call notify_one() on m_EmptyCondition below.
    */
    m_EmptyCondition.notify_one();
}

template<typename T>
inline std::shared_ptr<T> ThreadSafeQueue<T>::tryPop(void)
{
    std::lock_guard<std::mutex> lock(m_Mut);
    if (m_Data.empty())
        return std::shared_ptr<T>(); // null
    std::shared_ptr<T> ret = std::shared_ptr<T>(std::make_shared<T>(m_Data.front()));
    m_Data.pop();
    return ret;
}

template<typename T>
inline bool ThreadSafeQueue<T>::poll(T & p_Value)
{
    std::lock_guard<std::mutex> lock(m_Mut);
    if (m_Data.empty())
        return false;
    p_Value = m_Data.front();
    m_Data.pop();
    return true;
}

template<typename T>
inline bool ThreadSafeQueue<T>::isEmpty() const
{
    //  this function is "const", so why are we allowed to modify a member vaiable "mMute"?....
    //we can, because we marked it "mutable"! see definition of mMute.
    std::lock_guard<std::mutex> lock(m_Mut);
    return m_Data.empty();
}

template <class T>
inline void Queue<T>::push(T p_Value)
{
    m_Data.push(p_Value);
}

template <class T>
inline void Queue<T>::push(T && p_rrValue)
{
    m_Data.push(std::forward(p_rrValue));
}

template <class T>
inline std::shared_ptr<T> Queue<T>::pop(void)
{
    if (m_Data.empty())
        return std::shared_ptr<T>(); // null
    std::shared_ptr<T> ret = std::shared_ptr<T>(std::make_shared<T>(m_Data.front()));
    m_Data.pop();
    return ret;
}

template<class T>
inline void Queue<T>::clear(void)
{
    while (!m_Data.empty()) m_Data.pop();
}

template <class T>
inline bool Queue<T>::poll(T & p_Value)
{
    if (m_Data.empty())
        return false;
    p_Value = m_Data.front();
    m_Data.pop();
    return true;
}

template <class T>
inline bool Queue<T>::isEmpty(void) const
{
    return m_Data.empty();
}


} // KE ns
