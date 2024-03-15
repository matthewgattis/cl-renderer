#pragma once

#include <iostream>

class Serializable
{
public:
    virtual ~Serializable() { }

    friend std::ostream& operator<<(std::ostream& os, const Serializable& rhs)
    {
        return rhs.serialize(os);
    }

protected:
    Serializable() { }

    virtual std::ostream& serialize(std::ostream& os) const = 0;
};

