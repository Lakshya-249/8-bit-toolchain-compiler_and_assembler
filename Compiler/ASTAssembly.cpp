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

std::string Compiler::getOpCode(const TokenType &op){
    if (op == PLUS) return "add";
    else if (op == MINUS) return "sub";
    else if (op == STAR) return "mul";
    else if (op == SLASH) return "div";
    else if (op == GREATER) return "gt";
    else if (op == LESS) return "lt";
    else if (op == EQUAL_EQUAL) return "eq";
    else if (op == NOT_EQUAL) return "neq";
    else if (op == GREATER_EQUAL) return "gte";
    else if (op == LESS_EQUAL) return "lte";
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

void Compiler::evalCondition(Binary* binary, std::string opCode) {
    if (opCode == "lt" || opCode == "gte"){
        evaluate(binary->left);
        pushToStack('A');
        evaluate(binary->right);
        popToRegister('B');
    }else {
        evaluate(binary->right);
        pushToStack('A');
        evaluate(binary->left);
        popToRegister('B');
    }
    instructions << "cmp\n";
    if (opCode == "eq") instructions << "jne %bool" << boolCount << "\n";
    else if (opCode == "neq") instructions << "je %bool" << boolCount << "\n";
    else if (opCode == "gte" || opCode == "lte") instructions << "jnc %bool" << boolCount << "\n";
    else instructions << "jc %bool" << boolCount << "\n";
    loadDataRegister('A', "1");
    instructions << "jmp %endbool" << boolCount << "\n";
    instructions << "bool" << boolCount << ":\n";
    loadDataRegister('A', "0");
    instructions << "endbool" << boolCount << ":\n";
    boolCount++;
}

std::string Compiler::visitBinary(Binary* binary) {
    std::string opCode = getOpCode(binary->op.type);
    if (opCode == "mul") multiply(binary);
    else if (opCode == "div") divide(binary, opCode);
    else if (opCode == "gt" 
        || opCode == "lt" 
        || opCode == "eq" 
        || opCode == "neq" 
        || opCode == "lte"
        || opCode == "gte") evalCondition(binary, opCode);
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
    instructions << getOpCode(unary->op.type) << "\n";
    return "";
}

std::string Compiler::visitVariable(Variable* variable) {
    std::string address = env->get(variable->name);
    moveMemoryToRegister('A', address);
    return address;
}

std::string Compiler::visitAssign(Assign* assign) {
    std::string value = evaluate(assign->value);
    std::string scopename = env->assign(assign->name, value);
    moveRegisterToMemory('A', scopename);
    return "";
}

std::string Compiler::visitLogical(Logical* logical) {
    int cnt = orAndCount;
    orAndCount++;
    evaluate(logical->left);
    loadDataRegister('B', "1");
    instructions<< "cmp\n";
    if(logical->op.type == OR) instructions << "jc %orand" << cnt << "\n";
    else instructions << "jnc %orand" << cnt << "\n";
    instructions << "jmp %orandEnd" << cnt << "\n";
    instructions << "orand" << cnt << ":\n";
    evaluate(logical->right);
    instructions << "orandEnd" << cnt << ":\n";
    return "";

}

std::string Compiler::visitBlock(Block* block) {
    executeBlock(block->statements, new Environment(env));
    return "";
}

std::string Compiler::visitCall(Call* call) {
try{
    std::string callee = evaluate(call->callee);
    ASTCallable* callb = env->getFunction(callee);
    std::vector<Expr*>arguments; 

    for (auto &arg : call->arguments) {
        if (arg != nullptr) arguments.push_back(arg);
    }
    if(typeid(*callb) != typeid(ASTFunction)){
        throw std::runtime_error("Can only call functions and classes.");
        
    }
    if (arguments.size() > callb->arity()){
        throw std::runtime_error("Too many arguments for function.");
    }

    if( arguments.size() < callb->arity()) {
        throw std::runtime_error("Too Less arguments for function.");
    }
    // std::cout<<"Checker value:" <<callee <<std::endl;
    callb->call(this, arguments);
    instructions << "call %" << callee << "\n";
    return "";
}
catch(std::runtime_error& e){
    std::cerr << "Error: " << e.what() << std::endl;
    // return "";
    std::exit(1);
}
}

void Compiler::executeBlock(std::vector<Stmt*> statements, Environment* environment) {
    Environment* prev = this->env;
    scopeStack.push_back(environment);
    this->env = environment;
    for (auto &stmt : statements) {
        execute(stmt);
        ptrstatements.push_back(stmt);
    }
    this->env = prev;
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

std::string Compiler::visitIfStmt(IfStmt* ifStmt) {
    int cnt = ifCount;
    ifCount++;
    bool flag = (ifStmt->elseBranch != nullptr);
    evaluate(ifStmt->condition);
    instructions << "dec\n";
    if (flag) instructions << "jc %else"  << cnt << "\n";
    else instructions <<  "jc %endif" << cnt << "\n";
    execute(ifStmt->thenBranch);
    if (flag) {
        instructions << "jmp %endif" << cnt << "\n";
        instructions << "else" << cnt << ":\n";
        execute(ifStmt->elseBranch);
    }
    instructions << "endif" << cnt << ":\n";
    return "";
}

std::string Compiler::visitWhileStmt(WhileStmt* whilestmt) {
    int cnt = whileCount;
    whileCount++;
    if (whilestmt->body != nullptr) {
        instructions << "while" << cnt << ":\n";
        evaluate(whilestmt->condition);
        instructions << "dec\n";
        instructions << "jc %whileEnd" << cnt << "\n";
        execute(whilestmt->body);
        instructions << "jmp %while" << cnt << "\n";
        instructions << "whileEnd" << cnt << ":\n";
    }
    return "";
}

std::string Compiler::visitReturnStmt(ReturnStmt* returnstmt) {
    Expr* expr = nullptr;
    if (returnstmt->value!= nullptr) {
        expr = returnstmt->value;
        evaluate(expr);
        instructions << "ret\n";
    }
    // throw Return(expr);
    return "";
}

std::string Compiler::visitVar(Var* var) {
    std::string value = "0";
    std::string scopename = (env->depth > 0) ? var->name.lexeme + "_local" + std::to_string(env->depth) : var->name.lexeme;
    if (var->initializer != nullptr) {
        evaluate(var->initializer);
        moveRegisterToMemory('A', scopename);
    }
    env->define(var->name, value);
    return "";
}

std::string Compiler::visitFunction(Function* stmt) {
    ASTFunction* function = new ASTFunction(stmt);
    env->define(stmt->name, "0");
    env->defineFunctions(stmt->name, function);
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
        ptrstatements.push_back(stmt);
    }
    // instructions << "out 0\n";
    instructions << "hlt\n";
    for(auto &it: funcStatements) {
        instructions << it.second.second << ":\n";
        executeBlock(it.first, it.second.first);
        loadDataRegister('A', "0");
        instructions << "ret\n";
    }
    instructions << ".data\n";
    std::unordered_map<std::string, std::string> processedVars;
    for (auto &var : env->values) {
        if (processedVars.find(var.first) == processedVars.end()) {
            instructions << var.first << "=" << var.second << "\n";
            processedVars[var.first] = var.second;
        }
    }

    for (auto &var : global->values){
        if (processedVars.find(var.first) == processedVars.end()) {
            instructions << var.first << "=" << var.second << "\n";
            processedVars[var.first] = var.second;
        }
    }
    
    // Then process all environments in the scope stack
    for (auto &scopeEnv : scopeStack) {
        for (auto &var : scopeEnv->values) {
            if (processedVars.find(var.first) == processedVars.end()) {
                instructions << var.first << "=" << var.second << "\n";
                processedVars[var.first] = var.second;
            }
        }
    }
    return instructions.str();
}

ASTFunction::ASTFunction(Function* declaration) {
    this->declaration = declaration;
}

std::string ASTFunction::call(Compiler* compiler, std::vector<Expr*> arguments) {
    Environment* environment = new Environment(compiler->global);
    Environment* prev = compiler->env;
    for (size_t i = 0; i < arguments.size(); i++) {
        environment->define(declaration->params[i], "0");
    }
    for (size_t i = 0; i < arguments.size(); i++) {
        compiler->evaluate(arguments[i]);
        compiler->moveRegisterToMemory('A', environment->get(declaration->params[i]));
    }
    // std::cout<<"Checker value:" <<environment->get(declaration->name) <<std::endl;
    compiler->funcStatements.push_back({declaration->body, {environment, environment->get(declaration->name)}});
    // try
    // {
    //     compiler->executeBlock(declaration->body, environment);
    // }
    // catch(const Return& returnException)
    // {
    //     compiler->evaluate(returnException.value);
    //     compiler->env = prev;
    //     return "";
    // }
    // compiler->loadDataRegister('A', "0");
    
    return "";
}

int ASTFunction::arity() {
    return declaration->params.size();
}

Compiler::~Compiler() {
    for (auto it = scopeStack.begin(); it != scopeStack.end(); ++it) {
        if (*it != env && *it != global) { // Assuming you have a global environment
            delete *it;
        }
    }
    scopeStack.clear();
    
    if (env) {
        delete env;
    }
    
    // for (auto &it : ptrstatements) {
    //     if (it) {
    //         delete it;
    //         it = nullptr;
    //     }
    // }
}