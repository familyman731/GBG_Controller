#pragma once

#include <stdio.h>
#include <pthread.h>

namespace GBG
{
class BtControl
{
    pthread_mutex_t btMutex;
    bool fwd;
    bool rvr;
    bool left;
    bool right;
    bool stop;
public:
    BtControl():
        fwd{false},
        left{false},
        right{false},
        stop{false}
    {
        if(pthread_mutex_init (&btMutex, NULL) != 0){
            printf("Failed to initialize the bt mutex");
        }
    }

    void clear()
    {
        if(pthread_mutex_lock(&btMutex) == 0){
            fwd = false;
            rvr = false;
            left = false;
            right = false;
            stop = false;
            pthread_mutex_unlock(&btMutex);
        }
    }

    void write(const int val)
    {
        if(pthread_mutex_lock(&btMutex) == 0){
            switch (val)
            {
            case 6:
                fwd = true;
                rvr = false;
                break;
            
            default:
                break;
            }
            pthread_mutex_unlock(&btMutex);
        }
    }

    bool fwd() {if(pthread_mutex_lock(&btMutex) == 0){return fwd;}}
    bool rvr() {if(pthread_mutex_lock(&btMutex) == 0){return rvr;}}
    bool right() {if(pthread_mutex_lock(&btMutex) == 0){return right;}}
    bool left() {if(pthread_mutex_lock(&btMutex) == 0){return left;}}
    bool stop() {if(pthread_mutex_lock(&btMutex) == 0){return stop;}}
};
}