#pragma once

template <class value_t>
class DirtyValue
{
public:
    DirtyValue(const value_t& val) : prev(val), cur(val), forceDirty(true) {}

    bool IsDirty() const { return forceDirty || (prev != cur); }
    void Reset()
    {
        prev = cur;
        forceDirty = false;
    }

    void Set(const value_t& val)
    {
        cur = val;
    }

    const value_t& Get() const
    {
        return cur;
    }

    void SetForceDirty() { forceDirty = true; }

private:
    value_t prev;
    value_t cur;

    bool forceDirty = true;
};