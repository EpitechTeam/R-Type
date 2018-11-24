//
// Created by kahoul on 23/11/18.
//

#ifndef R_TYPE_SCOPELOCK_HPP
#define R_TYPE_SCOPELOCK_HPP

#include <mutex>

class ScopeLock {
public:
    explicit ScopeLock(std::mutex &mutex)
            : mtx(&mutex)
    {
        this->mtx->lock();
    }

    ~ScopeLock() {
        this->mtx->unlock();
    }

private:
    std::mutex *mtx;
};

#endif //R_TYPE_SCOPELOCK_HPP
