#include "ASTAssembly.hpp"

void Compiler::pushToStack(char reg) {
    instructions << "push " << reg << "\n";
}

void Compiler::popToRegister(char reg) {
    instructions << "pop " << reg << "\n";
}

void Compiler::moveBtwRegister(char reg1, char reg2) {
    instructions << "mov " << reg1 << " " << reg2 << "\n";
}

void Compiler::moveMemoryToRegister(char reg,std::string memadd) {
    instructions << "mov " << reg << " M %" << memadd << "\n";
}

void Compiler::moveRegisterToMemory(char reg,std::string memadd) {
    instructions << "mov M " << reg << " %" << memadd << "\n";
}

void Compiler::loadDataRegister(char reg, std::string data) {
    instructions << "ldi " << reg << " " << data << "\n";
}

std::string Compiler::evaluate(Expr* expr) { 
    return expr->accept(this); 
}

std::string Compiler::getOpCode(const std::string &op){
    if (op == "+") return "add";
    else if (op == "-") return "sub";
    else if (op == "*") return "mul";
    else if (op == "/") return "div";
    return "UNKNOWN_OP";
}

void Compiler::multiply(Binary* binary){
    evaluate(binary->right);
    pushToStack('A');
    evaluate(binary->left);
    moveBtwRegister('C', 'A');
    popToRegister('B');
    loadDataRegister('A', "0");
    pushToStack('A');
    instructions << "startM" << mulCount << ":\n";
    popToRegister('A');
    instructions << "add\n";
    pushToStack('A');
    moveBtwRegister('A','C');
    instructions << "dec\n";
    moveBtwRegister('C','A');
    instructions << "jnz %startM" << mulCount << "\n";
    popToRegister('A');
    mulCount++;
}

void Compiler::divide(Binary* binary, std::string opCode){
    evaluate(binary->right);
    pushToStack('A');
    evaluate(binary->left);
    popToRegister('B');
    pushToStack('A');
    loadDataRegister('C', "0");
    instructions << "startD" << divCount << ":\n";
    popToRegister('A');
    instructions << "cmp\n";
    pushToStack('A');
    instructions << "jc %remainder" << divCount << "\n";
    moveBtwRegister('A','C');
    instructions << "inc\n";
    moveBtwRegister('C','A');
    popToRegister('A');
    instructions << "sub\n";
    pushToStack('A');
    instructions << "jmp %startD" << divCount << "\n";
    instructions << "remainder" << divCount << ":\n";
    if (opCode == "div") {
        popToRegister('A');
        moveBtwRegister('A','C');
    }
    else if (opCode == "rem") {
        popToRegister('A');
    }
    divCount++;
}

void Compiler::evalAddSub(Binary* binary, std::string opCode) {
    evaluate(binary->right);
    pushToStack('A');
    evaluate(binary->left);
    popToRegister('B');
    instructions << opCode << "\n";
}

std::string Compiler::visitBinary(Binary* binary) {
    std::string opCode = getOpCode(binary->op.lexeme);
    if (opCode == "mul") multiply(binary);
    else if (opCode == "div") divide(binary, opCode);
    else evalAddSub(binary, opCode);
    return "";
}

std::string Compiler::visitGrouping(Grouping* grouping) {
    evaluate(grouping->expression);
    return "";
}

std::string Compiler::visitLiteral(Literal* literal) {
    if (std::holds_alternative<std::string>(literal->value)){
        std::string data = std::get<std::string>(literal->value);
        loadDataRegister('A', data);
    }
    return std::get<std::string>(literal->value);
}

std::string Compiler::visitUnary(Unary* unary) {
    evaluate(unary->right);
    instructions << getOpCode(unary->op.lexeme) << "\n";
    return "";
}

std::string Compiler::visitVariable(Variable* variable) {
    moveMemoryToRegister('A', variable->name.lexeme);
    return "";
}

std::string Compiler::visitAssign(Assign* assign) {
    std::string value = evaluate(assign->value);
    env.assign(assign->name, value);
    moveRegisterToMemory('A', assign->name.lexeme);
    return "";
}

std::string Compiler::visitExpression(Expression* expression) {
    evaluate(expression->expression);
    return "";
}

std::string Compiler::visitPrint(Print* print) {
    evaluate(print->expression);
    instructions << "out 0\n";
    return "";
}

std::string Compiler::visitVar(Var* var) {
    std::string value = "0";
    if (var->initializer != nullptr) {
        evaluate(var->initializer);
        moveRegisterToMemory('A', var->name.lexeme);
    }
    env.define(var->name, value);
    return "";
}

void Compiler::execute(Stmt* stmt) {
    stmt->accept(this);
}

std::string Compiler::compile(std::vector<Stmt*> &statements){
    instructions.str("");
    instructions << ".text\n";
    for (auto &stmt : statements) {
        execute(stmt);
    }
    // instructions << "out 0\n";
    instructions << "hlt\n";
    instructions << ".data\n";
    for (auto &var : env.values) {
        instructions << var.first << "=" << var.second << "\n";
    }
    return instructions.str();
}