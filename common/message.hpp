#pragma once

#include <string>
#include <memory>

#include "serializable.hpp"
#include "deserializable.hpp"

class Message : public Serializable, public Deserializable
{
public:
    virtual std::string getType() const = 0;

protected:
    Message()
    {
    }

    Message(std::istream& is) :
        Deserializable(is)
    {
    }
};

