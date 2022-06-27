#ifndef UNIONFIND_H_COMPANY_H
#define UNIONFIND_H_COMPANY_H
#include "Tree.h"
#include "Hash.h"
#include "SalaryTreeKey.h"
#include "Employee.h"

enum CompanyRes
{
    COMPANY_SUCCESS,
    COMPANY_FAILURE
};

class Company
{
private:
    int companyID;
    int numOfEmployees;
    long long sumOfGrades;
    Hash employeeHash;
    Tree<SalaryTreeKey, Employee> salaryTree;
public:
    ///not really sure about constructors and family
    Company();
    explicit Company(int ID);
    Company(const Company& other);
    ~Company() = default;
    Company& operator=(Company& other);

    int getNumOfEmployees() const;
    long long getSumOfGrades() const;
    void setNumOfEmployees(int num);
    void setSumOfGrades(long long sum);

    CompanyRes addEmployeeToHash(int employeeID, int grade, int companyID); ///no salary
    CompanyRes addExistingEmployeeToHash(int employeeID, int grade, int salary, int companyID); ///with salary
    CompanyRes removeEmployee(int employeeID);
    CompanyRes salaryIncrease(int employeeID, int salaryIncrease);
    CompanyRes promoteEmployee(int employeeID, int bumpGrade);

    ListNode* findInHash(int employeeID);
    Node<SalaryTreeKey,Employee>* findInTree(SalaryTreeKey key);

    ListNode** getArray();
    int getHashSize();
    int getTreeSize();

    CompanyRes fillArrayInNodes(SalaryTreeKey* key, Employee* data, int* grades);
    CompanyRes makeNewTree(SalaryTreeKey* key, Employee* data, int* grades, int size);

    Node<SalaryTreeKey, Employee>* companySelect(int k);
    long long companySumOfGrades(SalaryTreeKey& key);

    long long getZeroSalaryGrades();
    int getZeroSalaryEmps();

    CompanyRes avgBetween(int salary, int* numOfEmployees, long long* sumOfGrades);

};


#endif //UNIONFIND_H_COMPANY_H
