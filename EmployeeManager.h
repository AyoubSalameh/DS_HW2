#ifndef UNIONFIND_H_EMPLOYEEMANAGER_H
#define UNIONFIND_H_EMPLOYEEMANAGER_H
#include <iostream>
#include <memory>
#include "UnionFind.h"
#include "Company.h"
#include "library2.h"

class EmployeeManager
{
private:
    int size;
    UnionFind CompanyUF;
    Company** Companies;
public:
    EmployeeManager(int size);
    EmployeeManager(const EmployeeManager& other) = delete;
    ~EmployeeManager();

    StatusType addEmployee(int EmployeeID, int CompanyID, int Grade);
    StatusType removeEmployee(int EmployeeID);
    StatusType employeeSalaryIncrease(int EmployeeID, int SalaryIncrease);
    StatusType promoteEmployee(int EmployeeID, int bumpGrade);
    StatusType companyValue(int companyID);

    StatusType acquireCompany(int AcquirerID, int TargetID, double factor);

    StatusType sumOfBumpGradeBetweenTopWorkersByGroup(int CompanyID, int m);

    StatusType AverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary);

    Company* getCompany(int i);


};


#endif //UNIONFIND_H_EMPLOYEEMANAGER_H
