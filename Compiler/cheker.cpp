#include<iostream>
#include<string>
#include<memory>

class Teacher;
class Student;

class Visitor{
public:
    virtual void visitTeacher(Teacher* teacher) = 0;
    virtual void visitStudent(Student* student) = 0;
};

class Museum{
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~Museum(){};
};

class Student: public Museum{
public:
    int x = 10;
    void accept(Visitor* visitor) override{
        visitor->visitStudent(this);
    }
};

class Teacher: public Museum{
public:
    int y = 20;
    void accept(Visitor* visitor) override{
        visitor->visitTeacher(this);
    }
};

class PlaceVisiter: public Visitor{
    void visitTeacher(Teacher* teacher) override{
        std::cout << "Visiting teacher's place: " << teacher->y << std::endl;
    }
    void visitStudent(Student* student) override{
        std::cout << "Visiting student's place: " << student->x << std::endl;
    }
};

int main(){
    std::shared_ptr<Museum> std = std::make_shared<Student>();
    std::shared_ptr<Museum> tea = std::make_shared<Teacher>();
    PlaceVisiter vis;
    std->accept(&vis);
    tea->accept(&vis);
    return 0;
}