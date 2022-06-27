#include "EmployeeManager.h"
#include <stdio.h>

EmployeeManager::EmployeeManager(int size) : size(size), CompanyUF(size)
{
    try
    {
        Companies = new Company*[size + 1];
    }
    catch (std::bad_alloc& e)
    {
        throw;
    }
    for(int i = 0; i <size + 1; i++)
    {
        Companies[i] = new Company(i);
    }
}

EmployeeManager::~EmployeeManager()
{
    for(int i =0; i <size + 1; i++)
    {
        delete Companies[i];
    }
    delete[] Companies;
}

StatusType EmployeeManager::addEmployee(int EmployeeID, int CompanyID, int Grade)
{
    try {
        if (EmployeeID <= 0 || CompanyID <= 0 || Grade < 0 || CompanyID > size)
            return INVALID_INPUT;

        int parentCompany = CompanyUF.Find(CompanyID);

        CompanyRes res = Companies[0]->addEmployeeToHash(EmployeeID, Grade, parentCompany);
        if (res != COMPANY_SUCCESS)
            return FAILURE;
        res = Companies[parentCompany]->addEmployeeToHash(EmployeeID, Grade, parentCompany);
        if (res != COMPANY_SUCCESS) {
            Companies[parentCompany]->removeEmployee(EmployeeID);
            return FAILURE;
        }
    }
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

StatusType EmployeeManager::removeEmployee(int EmployeeID) {
    try {
        if (EmployeeID <= 0)
            return INVALID_INPUT;
        ListNode *exist = Companies[0]->findInHash(EmployeeID);
        if (exist == nullptr)
            return FAILURE;

        int company = exist->getEmployee()->getCompanyID();   ///employee has the most updated company
        int grade = exist->getEmployee()->getGrade();
        int salary = exist->getEmployee()->getSalary();

        CompanyRes res = Companies[0]->removeEmployee(EmployeeID);
        if (res != COMPANY_SUCCESS)
            return FAILURE;

        res = Companies[company]->removeEmployee(EmployeeID);
        if (res != COMPANY_SUCCESS) {
            Companies[0]->addEmployeeToHash(EmployeeID, grade, company);
            this->employeeSalaryIncrease(EmployeeID, salary);
            return FAILURE;
        }
    }
    catch (std::bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType EmployeeManager::employeeSalaryIncrease(int EmployeeID, int SalaryIncrease)
{
    if(EmployeeID <= 0 || SalaryIncrease <= 0)
        return INVALID_INPUT;

    ListNode* exist = Companies[0]->findInHash(EmployeeID);
    if(exist == nullptr)
        return FAILURE;

    int salary = exist->getEmployee()->getSalary();
    int newSalary = salary + SalaryIncrease;
    int company = exist->getEmployee()->getCompanyID();

    CompanyRes res = Companies[0]->salaryIncrease(EmployeeID, SalaryIncrease);
    if(res != COMPANY_SUCCESS)
        return FAILURE;

    res = Companies[company]->salaryIncrease(EmployeeID,SalaryIncrease);
    if(res != COMPANY_SUCCESS)
    {
        //do things we  guess
    }
    SalaryTreeKey newSalKey(EmployeeID, newSalary);
    Node<SalaryTreeKey, Employee>* empInCompany = Companies[company]->findInTree(newSalKey);
    Node<SalaryTreeKey, Employee>* empInGeneral = Companies[0]->findInTree(newSalKey);
    empInCompany->getData()->setPointerToSalaryTree(empInGeneral);
    return SUCCESS;
}

Company* EmployeeManager::getCompany(int i)
{
    return Companies[i];
}

StatusType EmployeeManager::promoteEmployee(int EmployeeID, int bumpGrade)
{
    if(EmployeeID <= 0)
        return INVALID_INPUT;
    ListNode* found = Companies[0]->findInHash(EmployeeID);
    if(found == nullptr)
        return FAILURE;
    if(bumpGrade <= 0)
        return SUCCESS;

    int company = found->getEmployee()->getCompanyID();
    Companies[0]->promoteEmployee(EmployeeID, bumpGrade);
    Companies[company]->promoteEmployee(EmployeeID, bumpGrade);

    return SUCCESS;

}

StatusType EmployeeManager::companyValue(int companyID)
{
    if(companyID <= 0 || companyID > size)
        return INVALID_INPUT;

    long double val = CompanyUF.getChangedValue(companyID);
    printf("CompanyValue: %.1Lf\n", val);
    return SUCCESS;
}

StatusType EmployeeManager::acquireCompany(int AcquirerID, int TargetID, double factor) {
    try {
        if (AcquirerID <= 0 || AcquirerID > size || TargetID <= 0 || TargetID > size || factor <= 0.0)
            return INVALID_INPUT;


        int acquirerParent = CompanyUF.Find(AcquirerID);
        int targetParent = CompanyUF.Find(TargetID);

        int companiesBoughtByAcquirer = CompanyUF.getSize(acquirerParent);
        int companiesBoughtByTarget = CompanyUF.getSize(targetParent);

        if(companiesBoughtByTarget > companiesBoughtByAcquirer)
        {
            int temp = acquirerParent;
            acquirerParent = targetParent;
            targetParent = temp;
        }

        if (targetParent == acquirerParent)
            return INVALID_INPUT;

        ///dealing with the hash
        ListNode **arr = Companies[targetParent]->getArray();
        int targetHashSize = Companies[targetParent]->getHashSize();


        for (int i = 0; i < targetHashSize; i++) {
            ListNode *current = arr[i];
            while (current != nullptr) {
                int id = current->getID();
                Companies[0]->findInHash(id)->getEmployee()->changeCompany(acquirerParent);
                int grade = current->getEmployee()->getGrade();
                int salary = current->getEmployee()->getSalary();
                Companies[acquirerParent]->addExistingEmployeeToHash(id, grade, salary, acquirerParent);
                current = current->getNext();
            }
        }


        long long targetSum = Companies[targetParent]->getSumOfGrades();
        long long acquirerSum = Companies[acquirerParent]->getSumOfGrades();
        Companies[acquirerParent]->setSumOfGrades(acquirerSum + targetSum);

        int targetNum = Companies[targetParent]->getNumOfEmployees();
        int acquirerNum = Companies[acquirerParent]->getNumOfEmployees();
        Companies[acquirerParent]->setNumOfEmployees(targetNum + acquirerNum);


        ///dealing with the tree
        int targetSize = Companies[targetParent]->getTreeSize();
        int acquirerSize = Companies[acquirerParent]->getTreeSize();

        //target company has no workers in tree
        if (targetSize == 0) {
            //nothing to be done
        }

            //acquirer company has no workers in tree
        else if (acquirerSize == 0) {
            //putting them in array and changing them in general tree
            int *targetGrades = new int[targetSize];
            SalaryTreeKey *targetKeys = new SalaryTreeKey[targetSize];
            Employee *targetEmployees = new Employee[targetSize];
            CompanyRes res = Companies[targetParent]->fillArrayInNodes(targetKeys, targetEmployees, targetGrades);
            if (res == COMPANY_FAILURE) {
                delete[] targetGrades;
                delete[] targetKeys;
                delete[] targetEmployees;
            }
            for (int i = 0; i < targetSize; i++) {
                targetEmployees[i].changeCompany(acquirerParent);
                targetEmployees[i].getEmployeeInGeneralTree()->getData()->changeCompany(acquirerParent);
            }
            res = Companies[acquirerParent]->makeNewTree(targetKeys, targetEmployees, targetGrades, targetSize);
            delete[] targetGrades;
            delete[] targetKeys;
            delete[] targetEmployees;
        }
            //both acquirer and target have emps in tree
        else {
            int *targetGrades = new int[targetSize];
            SalaryTreeKey *targetKeys = new SalaryTreeKey[targetSize];
            Employee *targetEmployees = new Employee[targetSize];

            int *acquirerGrades = new int[acquirerSize];
            SalaryTreeKey *acquirerKeys = new SalaryTreeKey[acquirerSize];
            Employee *acquirerEmployees = new Employee[acquirerSize];

            CompanyRes res1 = Companies[targetParent]->fillArrayInNodes(targetKeys, targetEmployees, targetGrades);
            CompanyRes res2 = Companies[acquirerParent]->fillArrayInNodes(acquirerKeys, acquirerEmployees,
                                                                          acquirerGrades);
            if (res1 == COMPANY_FAILURE || res2 == COMPANY_FAILURE) {
                delete[] targetGrades;
                delete[] targetKeys;
                delete[] targetEmployees;
                delete[] acquirerGrades;
                delete[] acquirerKeys;
                delete[] acquirerEmployees;
            }
            for (int i = 0; i < targetSize; i++) {
                targetEmployees[i].changeCompany(acquirerParent);
                targetEmployees[i].getEmployeeInGeneralTree()->getData()->changeCompany(acquirerParent);
            }

            int *mergedGrades = new int[acquirerSize + targetSize];
            SalaryTreeKey *mergedKeys = new SalaryTreeKey[acquirerSize + targetSize];
            Employee *mergedEmployees = new Employee[acquirerSize + targetSize];

            //merging array
            int targetCount = 0;
            int acquirerCount = 0;
            for (int i = 0; i < acquirerSize + targetSize; i++) {
                if ((targetCount == targetSize) ||
                    (acquirerCount != acquirerSize && acquirerKeys[acquirerCount] < targetKeys[targetCount])) {
                    mergedKeys[i] = acquirerKeys[acquirerCount];
                    mergedEmployees[i] = acquirerEmployees[acquirerCount];
                    mergedGrades[i] = acquirerGrades[acquirerCount];
                    acquirerCount++;
                } else if ((acquirerCount == acquirerSize) ||
                           (targetCount != targetSize && targetKeys[targetCount] < acquirerKeys[acquirerCount])) {
                    mergedKeys[i] = targetKeys[targetCount];
                    mergedEmployees[i] = targetEmployees[targetCount];
                    mergedGrades[i] = targetGrades[targetCount];
                    targetCount++;
                }
            }
            Companies[acquirerParent]->makeNewTree(mergedKeys, mergedEmployees, mergedGrades,
                                                   acquirerSize + targetSize);
            delete[] targetGrades;
            delete[] targetKeys;
            delete[] targetEmployees;
            delete[] acquirerGrades;
            delete[] acquirerKeys;
            delete[] acquirerEmployees;
            delete[] mergedGrades;
            delete[] mergedKeys;
            delete[] mergedEmployees;
        }
        delete Companies[targetParent];
        Companies[targetParent] = new Company(targetParent);

        CompanyUF.Union(AcquirerID, TargetID, factor);

        return SUCCESS;
    }
    catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
}

StatusType EmployeeManager::sumOfBumpGradeBetweenTopWorkersByGroup(int CompanyID, int m)
{
    if(CompanyID < 0 || CompanyID > size || m <= 0)
        return INVALID_INPUT;

    int parent = CompanyUF.Find(CompanyID);
    int companySize = Companies[parent]->getTreeSize();

    if(m > companySize)
        return FAILURE;

    Node<SalaryTreeKey, Employee>* found = Companies[parent]->companySelect(companySize - m + 1);
    if(found == nullptr)
        return FAILURE;

    long long sum = Companies[parent]->companySumOfGrades(*found->getKey());
    printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", sum);

    return SUCCESS;
}

StatusType
EmployeeManager::AverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary)
{
    if(higherSalary < 0 || lowerSalary < 0 || higherSalary < lowerSalary || companyID < 0 || companyID > size)
        return INVALID_INPUT;

    long long sum = 0;
    int num = 0;

    long long* sumLower = new long long(0);
    long long* sumHigher = new long long(0);
    int* numLower = new int(0);
    int* numHigher = new int(0);

    int parent = CompanyUF.Find(companyID);

    if(higherSalary == 0)
    {
        num = Companies[parent]->getZeroSalaryEmps();
        sum = Companies[parent]->getZeroSalaryGrades();
    }
    else
    {
        Companies[parent]->avgBetween(lowerSalary, numLower, sumLower);
        Companies[parent]->avgBetween(higherSalary + 1, numHigher, sumHigher);
        num = *numLower - *numHigher;
        sum = *sumLower - *sumHigher;
        if(lowerSalary == 0)
        {
            num += Companies[parent]->getZeroSalaryEmps();
            sum += Companies[parent]->getZeroSalaryGrades();
        }
    }
    ///changed something here too
    if(num <= 0/* || sum <= 0*/)
    {
        delete sumLower;
        delete sumHigher;
        delete numLower;
        delete numHigher;
        return FAILURE;
    }

    long double avg = ((double)sum)/num;

    printf("AverageBumpGradeBetweenSalaryByGroup: %.1Lf\n", avg);

    delete sumLower;
    delete sumHigher;
    delete numLower;
    delete numHigher;
    return SUCCESS;
}




