from email import header


ast_nodes = [
    ("Binary", "Expr* left, Token op, Expr* right"),
    ("Grouping", "Expr* expression"),
    ("Literal", "Object value"),
    ("Unary", "Token op, Expr* right")
]

header_content = """#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.h"
#include <variant>

using Object = std::variant<int, double, bool, std::string>;

"""

cpp_content = """#include "expr.hpp" 
#include "token.h"
"""

for name,_ in ast_nodes:
    header_content += f"class {name};\n"

header_content += """\nclass Visitor{
public:
"""

for name,_ in ast_nodes:
    header_content += f"\tvirtual std::string visit{name}({name}* {name.lower()}) = 0;\n"

header_content += """
};

class Expr {
public:
    virtual std::string accept(Visitor* visitor) = 0;
    virtual ~Expr() = default;
};


"""

for name, fields in ast_nodes:
    header_content += f"\nclass {name} : public Expr {{\npublic:\n"
    for field in fields.split(", "):
        header_content += f"    {field};\n"
    header_content += f"\n    {name}({fields});\n"
    header_content += f"\tvoid printfunc();\n"
    header_content += f"\tstd::string accept(Visitor* visitor);\n"
    header_content += f"\t~{name}() {{}}\n"
    header_content += "};\n"

    cpp_content += f"\n{name}::{name}({fields}) {{\n"
    for field in fields.split(", "):
        field_name = field.split(" ")[-1]
        cpp_content += f"\tthis->{field_name} = {field_name};\n"

    cpp_content += "}\n"
    cpp_content += f"\nvoid {name}::printfunc() {{\n"
    cpp_content += f'\tstd::cout<< "{name} of class"<<std::endl;}}\n' 
    cpp_content += f"\nstd::string {name}::accept(Visitor* visitor) {{ return visitor->visit{name}(this) ;}}\n"
header_content += "\n#endif"

# Write to expr.hpp
with open("expr.hpp", "w") as file:
    file.write(header_content)

with open("expr.cpp", "w") as file:
    file.write(cpp_content)