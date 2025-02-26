#include "environment.hpp"


void Environment::define(Token &token, const std::string value) {
    values[token.lexeme] = value;
}

std::string Environment::get(Token &token) {
    try{
        if (values.find(token.lexeme) == values.end()) {
            throw std::runtime_error("Undefined variable name " + token.lexeme + ".");
        }
        return values[token.lexeme];
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << '\n';
        return nullptr;
    }   
}

void Environment::assign(Token &token, const std::string value) {
    try{
        std::string name = token.lexeme;
        if(values.find(name) == values.end()) {
            throw std::runtime_error("Undefined variable name " + name + ".");
        }
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << '\n';
    }
    
}