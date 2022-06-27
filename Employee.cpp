//
// Created by Ayoub Salameh on 20/04/2022.
//

#include "Employee.h"

Employee::Employee() : id(0), salary(0), grade(-1), companyID(0), inGeneralTree(nullptr)
{}

Employee::Employee(int id, int salary, int grade, int companyID,
                   Node<SalaryTreeKey, Employee> *inGeneralTree) :
                   id(id), salary(salary), grade(grade), companyID(companyID),
                   inGeneralTree(inGeneralTree)
                   {}

int Employee::getID() const
{
    return this->id;
}

int Employee::getSalary() const
{
    return this->salary;
}

int Employee::getGrade() const
{
    return this->grade;
}

int Employee::getCompanyID() const
{
    return this->companyID;
}

Node<SalaryTreeKey, Employee>* Employee::getEmployeeInGeneralTree()
{
    return this->inGeneralTree;
}

void Employee::increaseSalary(int salaryIncrease)
{
    salary += salaryIncrease;
}

void Employee::increaseGrade(int bumpGrade)
{
    if(bumpGrade > 0)
        grade += bumpGrade;
}

void Employee::changeCompany(int newCompanyID)
{
    companyID = newCompanyID;
}

void Employee::setPointerToSalaryTree(Node<SalaryTreeKey, Employee> *empInSalaryTree)
{
    inGeneralTree = empInSalaryTree;
}

void Employee::setSalary(int newSalary)
{
    this->salary = newSalary;
}

void Employee::setGrade(int newGrade)
{
    this->grade = newGrade;
}

