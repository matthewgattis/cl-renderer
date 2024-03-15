#pragma once

#include <iostream>

class Deserializable
{
protected:
    friend std::istream& operator>>(std::istream& is, Deserializable& rhs)
    {
        return rhs.deserialize(is);
    }

    virtual std::istream& deserialize(std::istream& is) = 0;

protected:
    Deserializable() { }

    virtual ~Deserializable() { }
};

