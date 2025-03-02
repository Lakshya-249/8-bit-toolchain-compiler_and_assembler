#include "environment.hpp"

Environment::Environment() {
    this->enclosing = nullptr;
    this->depth = 0;
}

Environment::Environment(Environment* enclosing) {
    this->enclosing = enclosing;
    this->depth = enclosing->depth + 1;
}

void Environment::define(Token &token, const std::string value) {
    std::string scopename = (depth>0) ? token.lexeme + "_local" + std::to_string(depth) : token.lexeme;
    values[scopename] = value;
}

void Environment::defineFunctions(Token& token, ASTCallable* callable) {
    std::string scopename = (depth>0) ? token.lexeme + "_local" + std::to_string(depth) : token.lexeme + "_def";
    funcValues[scopename] = callable;
}

std::string Environment::get(Token &token) {
    try{
        std::string scopename = (depth>0) ? token.lexeme + "_local" + std::to_string(depth) : token.lexeme;
        if (values.find(scopename) != values.end()) {
            return scopename;
        }
        if (enclosing != nullptr) return enclosing->get(token);
        throw std::runtime_error("Undefined variable name " + token.lexeme + ".");
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << '\n';
        // return "";
        std::exit(1);
    }   
}

ASTCallable* Environment::getFunction(std::string token) {
    try{
        std::string scopename = token + "_def";
        if (funcValues.find(scopename)!= funcValues.end()) {
            return funcValues[scopename];
        }
        if (enclosing!= nullptr) return enclosing->getFunction(token);
        throw std::runtime_error("Undefined function name " + token + ".");
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << '\n';
        // return nullptr;
        std::exit(1);
    } 
}

std::string Environment::assign(Token &token, const std::string value) {
    try{
        std::string scopename = (depth>0) ? token.lexeme + "_local" + std::to_string(depth) : token.lexeme;
        if (values.find(scopename) != values.end()) {
            return scopename;
        }
        if (enclosing != nullptr) {
            return enclosing->assign(token, value);
        }
        throw std::runtime_error("Undefined variable name " + scopename + ".");
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << '\n';
        // return "";
        std::exit(1);
    }
}