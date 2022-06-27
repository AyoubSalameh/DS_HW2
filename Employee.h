#ifndef TREE2NDATTEMPT_EMPLOYEE_H
#define TREE2NDATTEMPT_EMPLOYEE_H

#include "Tree.h"
#include "SalaryTreeKey.h"

class Employee
{
private:
    int id;
    int salary;
    int grade;
    int companyID;
    Node<SalaryTreeKey, Employee>* inGeneralTree;

public:
    Employee();
    Employee(int id, int salary, int grade, int companyID,
             Node<SalaryTreeKey, Employee>* inGeneralTree = nullptr);
    Employee(const Employee& other) = default;
    ~Employee() = default;
    Employee& operator=(const Employee& other) = default;
    Employee& operator=(Employee& other) = default;


    int getID() const;
    int getSalary() const;
    int getGrade() const;
    int getCompanyID() const;

    ///this pointers is useless in the general employee trees, therefore will be NULL
    Node<SalaryTreeKey, Employee>* getEmployeeInGeneralTree();

    void setSalary(int newSalary);
    void setGrade(int newGrade);
    void increaseSalary(int salaryIncrease);
    void increaseGrade(int bumpGrade);
    void changeCompany(int newCompanyID);
    void setPointerToSalaryTree(Node<SalaryTreeKey, Employee>* empInSalaryTree);

};



#endif //TREE2NDATTEMPT_EMPLOYEE_H
