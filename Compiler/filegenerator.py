def generate_ast(filename, ast_nodes):
    header_content = f"""#ifndef {filename.upper()}_HPP
#define {filename.upper()}_HPP

#include "token.h"
#include <variant>
"""
    if filename == "stmt":
        header_content += '#include "expr.hpp"\n\n'
    header_content += "using Object = std::variant<int, double, bool, std::string>;\n"
    cpp_content = f"#include \"{filename}.hpp\"\n"

    for name, _ in ast_nodes:
        header_content += f"class {name};\n"

    header_content += f"\nclass {filename.capitalize()}Visitor{{\npublic:\n"
    for name, _ in ast_nodes:
        header_content += f"\tvirtual std::string visit{name}({name}* {name.lower()}) = 0;\n"
    header_content += f"\n}};\n\nclass {filename.capitalize()} {{\npublic:\n    virtual std::string accept({filename.capitalize()}Visitor* visitor) = 0;\n    virtual ~{filename.capitalize()}() = default;\n}};\n\n"

    for name, fields in ast_nodes:
        header_content += f"\nclass {name} : public {filename.capitalize()} {{\npublic:\n"
        for field in fields.split(", "):
            header_content += f"    {field};\n"
        header_content += f"\n    {name}({fields});\n"
        header_content += f"\tvoid printfunc();\n"
        header_content += f"\tstd::string accept({filename.capitalize()}Visitor* visitor);\n"
        header_content += f"\t~{name}() {{}}\n";
        header_content += "};\n"

        cpp_content += f"\n{name}::{name}({fields}) {{\n"
        for field in fields.split(", "):
            field_name = field.split(" ")[-1]
            cpp_content += f"\tthis->{field_name} = {field_name};\n"
        cpp_content += "}\n"
        cpp_content += f"\nvoid {name}::printfunc() {{\n"
        cpp_content += f'\tstd::cout<< "{name} of class"<<std::endl;}}\n'
        cpp_content += f"\nstd::string {name}::accept({filename.capitalize()}Visitor* visitor) {{ return visitor->visit{name}(this); }}\n"
    
    header_content += "\n#endif"

    with open(f"{filename}.hpp", "w") as file:
        file.write(header_content)
    
    with open(f"{filename}.cpp", "w") as file:
        file.write(cpp_content)

# Example usage
ast_nodes = [
    ("Assign", "Token name, Expr* value"),
    ("Binary", "Expr* left, Token op, Expr* right"),
    ("Grouping", "Expr* expression"),
    ("Literal", "Object value"),
    ("Logical", "Expr* left, Token op, Expr* right"),
    ("Unary", "Token op, Expr* right"),
    ("Variable", "Token name")
]

stm_nodes = [
    ("Block", "std::vector<Stmt*> statements"),
    ("Expression", "Expr* expression"),
    ("IfStmt", "Expr* condition, Stmt* thenBranch, Stmt* elseBranch"),
    ("Print", "Expr* expression"),
    ("Var", "Token name, Expr* initializer"),
    ("WhileStmt", "Expr* condition, Stmt* body"),
]

generate_ast("expr", ast_nodes)
generate_ast("stmt", stm_nodes)