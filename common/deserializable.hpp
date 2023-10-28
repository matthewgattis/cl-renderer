#pragma once

#include <iostream>

class Deserializable
{
public:
    virtual ~Deserializable() { }

    friend std::istream& operator>>(std::istream& is, Deserializable& rhs)
    {
        return rhs.deserialize(is);
    }

protected:
    Deserializable() { }

    virtual std::istream& deserialize(std::istream& is) = 0;

protected:
    Deserializable(std::istream& is)
    {
        is >> *this;
    }
};

