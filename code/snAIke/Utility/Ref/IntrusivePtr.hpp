#pragma once

#include <snAike/Utility/Ref/ReferenceCountable.hpp>
#include <type_traits>

template <
    class T,
    class = std::enable_if_t<std::is_base_of_v<ReferenceCountable<typename T::thread_policy>, T>, T>
>
class IntrusivePtr
{
private:
    T* data;
public:
    IntrusivePtr() : data(nullptr) {}
    ~IntrusivePtr()
    {
        if (data)
        {
            data->Release();
        }
    }

    IntrusivePtr(T* data) : data(data) {}

    IntrusivePtr(const IntrusivePtr& other)
        : data(other.data)
    {
        data->AddRef();
    }

    IntrusivePtr(IntrusivePtr&& other)
        : data(other.data)
    {
        other.data = nullptr;
    }

    template <class U>
    IntrusivePtr(const IntrusivePtr& other)
        : data(other.data)
    {
        static_assert(std::is_base_of_v<T, U>, "U should derrive T!");
        data->AddRef();
    }

    template <class U>
    IntrusivePtr(IntrusivePtr&& other)
        : data(other.data)
    {
        static_assert(std::is_base_of_v<T, U>, "U should derrive T!");
        other.data = nullptr;
    }

    IntrusivePtr& operator=(const IntrusivePtr& other)
    {
        if (data)
        {
            data->Release();
        }
        if ((data = other.data))
        {
            data->AddRef();
        }
        return *this;
    }
    IntrusivePtr& operator=(IntrusivePtr&& other)
    {
        if (data)
        {
            data->Release();
        }
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    template <class U>
    IntrusivePtr& operator=(const IntrusivePtr<U>& other)
    {
        if constexpr (std::is_base_of_v<T, U>)
        {
            if (data)
            {
                data->Release();
            }
            if ((data = other.data))
            {
                data->AddRef();
            }
            return *this;
        }
        else
        {
            static_assert(false, "U should derrive T!");
        }
    }

    template <class U>
    IntrusivePtr& operator=(IntrusivePtr<U>&& other)
    {
        if constexpr (std::is_base_of_v<T, U>)
        {
            if (data)
            {
                data->Release();
            }
            data = other.data;
            other.data = nullptr;
            return *this;
        }
        else
        {
            static_assert(false, "U should derrive T!");
        }
    }


    T& operator*() { return *data; }
    T* operator->() { return data; }
    const T& operator*() const { return *data; }
    const T* operator->() const { return data; }

    T* Get() { return data; }
    const T* Get() const { return data; }

    operator bool() { return data != nullptr; }

private:

    template <class U, class>
    friend class IntrusivePtr;
};

template <class T, class... Args>
IntrusivePtr<T> MakeIntrusive(Args&&... args)
{
    return IntrusivePtr<T>(new T(std::forward<Args>(args)...));
}