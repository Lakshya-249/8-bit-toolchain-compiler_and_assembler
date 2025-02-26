#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include "token.h"
#include <stdexcept>

class Environment{
    public:
        std::unordered_map<std::string,std::string> values;
        void define(Token& token, std::string value);
        std::string get(Token& token);
        void assign(Token& token, std::string value);
};

#endif