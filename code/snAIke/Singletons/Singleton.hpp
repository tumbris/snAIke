#pragma once

template <class T>
class Singleton
{
private:
    Singletor() = delete;
public:
    static void CreateInstance();
    static T* GetInstance();
    static void DestroyInstance();
private:
    static T* instance;
};

template <class T>
T* Singleton<T>::instance = nullptr;

template<class T>
inline void Singleton<T>::CreateInstance()
{
    instance = new T();
}

template<class T>
inline T* Singleton<T>::GetInstance()
{
    return instance;
}

template<class T>
inline void Singleton<T>::DestroyInstance()
{
    delete instance;
}
