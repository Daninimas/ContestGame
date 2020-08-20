#pragma once

#include <cstdint>

class Component {
public:
    explicit Component(int _id) : id(_id) {}
    virtual ~Component() = default;
    int	id 	{};
};
