#pragma once

#include <snAIke/Singletons/Director/Types.hpp>

#include <functional>

class Updator
{
public:
public:
    template <class Fn>
    Updator(Fn&& callback, UpdatorIdType id) : updateCallback(std::forward<Fn>(callback)), id(id) {}

    void Call(float dt) { updateCallback(dt); }
    UpdatorIdType GetId() const { return id; }

private:
    std::function<void(float)> updateCallback;

    UpdatorIdType id;
};
