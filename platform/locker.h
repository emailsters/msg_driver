#ifndef _LOCKER_H_
#define _LOCKER_H_
class Condition;
class MutexLock
{
public:
    friend class Condition;
    MutexLock()
    {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~MutexLock()
    {
        pthread_mutex_destroy(&mutex_);
    }

    void Lock()
    {
        pthread_mutex_lock(&mutex_);
    }

    void UnLock()
    {
        pthread_mutex_unlock(&mutex_);
    }

private:
    pthread_mutex_t mutex_;
};

class LockGuard
{
public:
    explicit LockGuard(MutexLock* lock) : mutexlock_(lock)
    {
        mutexlock_->Lock();
    }

    ~LockGuard()
    {
        mutexlock_->UnLock();
    }

private:
    MutexLock* mutexlock_;
};

class Condition{
public:
    explicit Condition(MutexLock *mutexlock):mutexlock_(mutexlock)
    {
        pthread_cond_init(&condition_, NULL);
    }
    
    ~Condition()
    {
        pthread_cond_destroy(&condition_);
    }

    void Wait()
    {
        pthread_cond_wait(&condition_, &mutexlock_->mutex_);
    }

    void Notify()
    {
        pthread_cond_signal(&condition_);
    }

    void NotifyAll()
    {
        pthread_cond_broadcast(&condition_);
    }

private:
    MutexLock *mutexlock_;
    pthread_cond_t condition_;
};
#endif
