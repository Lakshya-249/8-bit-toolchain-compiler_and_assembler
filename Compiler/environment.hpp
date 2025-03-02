#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <unordered_map>
#include "token.h"
#include <stdexcept>
#include "callableFunc.hpp"

class Environment{
    public:
        int depth;
        Environment* enclosing;
        Environment();
        Environment(Environment* enclosing);
        std::unordered_map<std::string,std::string> values;
        std::unordered_map<std::string,ASTCallable*>funcValues;
        void define(Token& token, std::string value);
        void defineFunctions(Token& token, ASTCallable* callable);
        std::string get(Token& token);
        ASTCallable* getFunction(std::string functionName);
        std::string assign(Token& token, std::string value);
        ~Environment() {};
};

#endif