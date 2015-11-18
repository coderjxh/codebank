#include "sharedmemory.h"
#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <cstring>


SimpleSharedMemory::SimpleSharedMemory()
    : _key(-1)
    , _shmID(-1)
    , _shmHandle(0)
    , _isServer(true)
    , _size(1)
{

}

SimpleSharedMemory::SimpleSharedMemory(bool isServer, size_t size)
    : _key(-1)
    , _shmID(-1)
    , _shmHandle(0)
    , _isServer(isServer)
    , _size(size)
{
    create(isServer, ".", size);
}

SimpleSharedMemory::SimpleSharedMemory(bool isServer, std::string keyfile, size_t size)
    : _key(-1)
    , _shmID(-1)
    , _shmHandle(0)
    , _isServer(isServer)
    , _size(size)
{
    create(isServer, keyfile, size);
}

SimpleSharedMemory::~SimpleSharedMemory()
{
    shmdt(_shmHandle);
    struct shmid_ds buf;
    shmctl(_shmID, IPC_RMID, &buf);
}

void SimpleSharedMemory::create(bool isServer, std::string keypath, size_t size)
{
    _isServer = isServer;
    _size = size;

    _key = ftok(keypath.c_str(), 'z');
    if ( _key < 0 ) {
        std::cout << "ftok error." << std::endl;
        return;
    } else
        std::cout << "key: " << _key << std::endl;

//    if ( _isServer ) {
        _shmID = shmget(_key, size, IPC_CREAT|0666);
//    } else {
//        _shmID = shmget(_key, size, 0);
//    }
    if ( _shmID < 0 ) {
        std::cout << "shmget error." << std::endl;
        return;
    } else
        std::cout << "shmID: " << _shmID << std::endl;

    _shmHandle =  shmat(_shmID, 0, 0);

    if ( _shmHandle == (void*)-1 ) {
        std::cout << "shmat error." << std::endl;
        return;
    }
    memset( _shmHandle, 0, size);
    char *p = (char*)_shmHandle;
    *p = '0';
}

int SimpleSharedMemory::write(void *obj, int size)
{
    int ret(0);

    char *pt1 = (char*)_shmHandle;
    char *pt0 = pt1;

    int i = atoi(pt1);
    if ( i < 1 ) {
        ++pt1;
        memcpy(pt1, obj, size);

        *pt0 = '1';
        ret = size;
        std::cout << _shmID <<" write " << size << "bytes." << std::endl;
    }

    return ret;
}

int SimpleSharedMemory::read(void *obj)
{
    int ret(0);
    char *pt1 = (char*)_shmHandle;
    char *pt0 = pt1;

    int i = atoi(pt1);
    if ( i > 0 ) {
        ++pt1;
        memcpy(obj, pt1, size()-1);
        memset(pt1, 0, size()-1);

        *pt0 = '0';

        ret = size()-1;
    }

    return ret;
}

int SimpleSharedMemory::read(char *dest)
{
    int ret(0);

    char *pt1 = (char*)_shmHandle;
    char *pt0 = pt1;

    int i = atoi(pt1);
    if ( i > 0 ) {
        ++pt1;
        memcpy(dest, pt1, size()-1);
        memset(pt1, 0, size()-1);

        *pt0 = '0';

        ret = size()-1;
    }

    return ret;
}

