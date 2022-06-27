//
// Created by Ayoub Salameh on 04/06/2022.
//

#include "Company.h"

Company::Company() : companyID(0), numOfEmployees(0), sumOfGrades(0), employeeHash(), salaryTree()
{
}

Company::Company(int ID) : companyID(ID), numOfEmployees(0), sumOfGrades(0), employeeHash(), salaryTree()
{
}

int Company::getNumOfEmployees() const
{
    return this->numOfEmployees;
}

long long Company::getSumOfGrades() const
{
    return this->sumOfGrades;
}

void Company::setNumOfEmployees(int num)
{
    this->numOfEmployees = num;
}

void Company::setSumOfGrades(long long sum)
{
    this->sumOfGrades = sum;
}

///adding employee when his salary is 0
CompanyRes Company::addEmployeeToHash(int employeeID, int grade, int companyID)
{
    Employee emp(employeeID, 0, grade, companyID);

    HashRes res = employeeHash.insert(emp);
    if (res == HASH_FAILURE)
        return COMPANY_FAILURE;

    numOfEmployees++;
    sumOfGrades += grade;

    return COMPANY_SUCCESS;
}


CompanyRes Company::removeEmployee(int employeeID)
{
    try {
        ListNode *found = employeeHash.find(employeeID);
        if (found == nullptr)
            return COMPANY_FAILURE;
        int salary = found->getEmployee()->getSalary();
        int grade = found->getEmployee()->getGrade();
        HashRes hres = employeeHash.remove(employeeID);
        if (hres != HASH_SUCCESS)
            return COMPANY_FAILURE;
        if (salary != 0) {
            SalaryTreeKey key(employeeID, salary);
            TreeRes res = salaryTree.remove(key);
            if (res != TREE_SUCCESS) {
                Node<SalaryTreeKey, Employee> *emp = findInTree(key);
                this->employeeHash.insert(*emp->getData());
                return COMPANY_FAILURE;
            }

        }
        numOfEmployees--;
        sumOfGrades -= grade;
    }
    catch (std::bad_alloc& e) {
        return COMPANY_FAILURE;
    }
    return COMPANY_SUCCESS;
}

CompanyRes Company::salaryIncrease(int employeeID, int salaryIncrease)
{
    ListNode* found = employeeHash.find(employeeID);
    if(found == nullptr)
        return COMPANY_FAILURE;
    int old_salary = found->getEmployee()->getSalary();
    int grade = found->getEmployee()->getGrade();
    found->getEmployee()->increaseSalary(salaryIncrease);
    if(old_salary == 0)
    {
        SalaryTreeKey key(employeeID, salaryIncrease);
        TreeRes res = salaryTree.insert(key, *found->getEmployee(), grade);
        if(res == TREE_FAILURE)
        {
            found->getEmployee()->setSalary(old_salary);
            return COMPANY_FAILURE;
        }
    }
    else
    {
        TreeRes res = salaryTree.remove(SalaryTreeKey(employeeID,old_salary));
        if(res != TREE_SUCCESS)
        {
            found->getEmployee()->setSalary(old_salary);
            return COMPANY_FAILURE;
        }
        SalaryTreeKey newKey(employeeID, old_salary + salaryIncrease);
        res = salaryTree.insert(newKey, *found->getEmployee(), grade);
        if(res == TREE_FAILURE)
        {
            found->getEmployee()->setSalary(old_salary);
            salaryTree.insert(SalaryTreeKey(employeeID, old_salary),*found->getEmployee(), grade);
            return COMPANY_FAILURE;
        }
    }
    return COMPANY_SUCCESS;
}

Company &Company::operator=(Company& other)
{
    this->companyID = other.companyID;
    this->salaryTree = Tree<SalaryTreeKey,Employee>();
    this->employeeHash = Hash();
    this->sumOfGrades = 0;
    this->numOfEmployees = 0;
    return *this;
}

Company::Company(const Company &other)
{
    this->companyID = other.companyID;
    this->sumOfGrades = 0;
    this->numOfEmployees = 0;
    this->salaryTree = Tree<SalaryTreeKey, Employee>();
    this->employeeHash = Hash();
}

ListNode *Company::findInHash(int employeeId)
{
    ListNode* found = employeeHash.find(employeeId);
    return found;
}

Node<SalaryTreeKey, Employee> *Company::findInTree(SalaryTreeKey key)
{
    return salaryTree.find(key);
}

CompanyRes Company::promoteEmployee(int employeeID, int bumpGrade)
{
    ListNode* found = findInHash(employeeID);
    if(found == nullptr)
        return COMPANY_FAILURE;

    int salary = found->getEmployee()->getSalary();
    SalaryTreeKey key(employeeID, salary);
    found->getEmployee()->increaseGrade(bumpGrade);

    if(salary != 0)
    {
        Node<SalaryTreeKey, Employee> *emp = findInTree(key);
        emp->getData()->increaseGrade(bumpGrade);
        salaryTree.raiseGrade(key, bumpGrade);
    }
    sumOfGrades += bumpGrade;

    return COMPANY_SUCCESS;
}

ListNode **Company::getArray()
{
    return employeeHash.getArray();
}

int Company::getHashSize()
{
    return employeeHash.getMaxSize();
}

CompanyRes Company::addExistingEmployeeToHash(int employeeID, int grade, int salary, int companyID) {
    Employee emp(employeeID, salary, grade, companyID);

    HashRes res = employeeHash.insert(emp);
    if (res == HASH_FAILURE)
        return COMPANY_FAILURE;
    return COMPANY_SUCCESS;
}

int Company::getTreeSize()
{
    return this->salaryTree.getSize();
}

CompanyRes Company::fillArrayInNodes(SalaryTreeKey *key, Employee *data, int *grades)
{
    TreeRes res = salaryTree.inOrderFillKeyAndData(key, data, grades);
    if(res != TREE_SUCCESS)
        return COMPANY_FAILURE;
    return COMPANY_SUCCESS;
}

CompanyRes Company::makeNewTree(SalaryTreeKey *key, Employee *data, int *grades, int size)
{
    TreeRes res = salaryTree.ConvertArrayToTree(key, data, grades, size);
    if(res != TREE_SUCCESS)
        return COMPANY_FAILURE;
    return COMPANY_SUCCESS;
}

Node<SalaryTreeKey, Employee>* Company::companySelect(int k)
{
    return salaryTree.select(k);
}

long long Company::companySumOfGrades(SalaryTreeKey &key)
{
    return this->salaryTree.sumOfGrades(key);
}


CompanyRes Company::avgBetween(int salary, int *numOfEmployees, long long *sumOfGrades)
{
    TreeRes res = salaryTree.statsBiggerThan(salary, numOfEmployees, sumOfGrades);
    if(res == TREE_FAILURE)
        return COMPANY_FAILURE;
    return COMPANY_SUCCESS;
}

long long Company::getZeroSalaryGrades()
{
    return sumOfGrades - salaryTree.getTotalGrades();
}

int Company::getZeroSalaryEmps()
{
    return numOfEmployees - salaryTree.getSize();
}




