#include <snAike/Utility/Ref/ReferenceCountable.hpp>
#include <cassert>

void ReferenceCountable<DefaultThreadPolicy>::AddRef()
{
    ++counter;
}

void ReferenceCountable<ThreadSafePolicy>::AddRef()
{
    counter.fetch_add(1);
}

void ReferenceCountable<DefaultThreadPolicy>::Release()
{
    assert(counter > 0);
    if (--counter == 0)
    {
        delete this;
    }
}

void ReferenceCountable<ThreadSafePolicy>::Release()
{
    assert(counter.load() > 0);
    if (counter.fetch_sub(1) == 1)
    {
        delete this;
    }
}

size_t ReferenceCountable<ThreadSafePolicy>::Count() const
{
    return counter.load();
}

size_t ReferenceCountable<DefaultThreadPolicy>::Count() const
{
    return counter;
}
