#include "library2.h"
#include "EmployeeManager.h"

void *Init(int k)
{
    EmployeeManager* DS;
    try
    {
        DS = new EmployeeManager(k);
    }
    catch (std::bad_alloc& e) {
        return NULL;
    }
    return (void*)DS;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade) {
    if (DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager *) DS)->addEmployee(employeeID, companyID, grade);
}

StatusType RemoveEmployee(void *DS, int employeeID)
{
    if(DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->removeEmployee(employeeID);
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor)
{
    if(DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->acquireCompany(companyID1, companyID2, factor);
}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease)
{
    if(DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->employeeSalaryIncrease(employeeID, salaryIncrease);
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade)
{
    if(DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->promoteEmployee(employeeID, bumpGrade);
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m)
{
    if(DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->sumOfBumpGradeBetweenTopWorkersByGroup(companyID, m);
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary)
{
    if(DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->AverageBumpGradeBetweenSalaryByGroup(companyID, lowerSalary, higherSalary);
}

StatusType CompanyValue(void *DS, int companyID)
{
    if(DS == NULL)
        return INVALID_INPUT;
    return ((EmployeeManager*)DS)->companyValue(companyID);
}

void Quit(void** DS)
{
    auto pm = (EmployeeManager**)DS;
    delete *pm;
    *pm = nullptr;
}
