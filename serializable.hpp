#pragma once

#include <iostream>

class Serializable
{
public:
    virtual ~Serializable() { }

    friend std::istream& operator>>(std::istream& is, Serializable& rhs)
    {
        return rhs.deserialize(is);
    }

    friend std::ostream& operator<<(std::ostream& os, Serializable& rhs)
    {
        return rhs.serialize(os);
    }

protected:
    Serializable() { }

    virtual std::ostream& serialize(std::ostream& os) const = 0;
    virtual std::istream& deserialize(std::istream& is) = 0;
};

