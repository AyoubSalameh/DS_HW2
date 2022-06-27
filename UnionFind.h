#ifndef DS_WET_2_UNIONFIND_H
#define DS_WET_2_UNIONFIND_H
#include <memory>
#include <iostream>

class UnionFind
{
private:
    int numOfElements;
    int* sizeArr;
    int* parentsArr;
    int* companyParents;    ///helper array that tells us which
    long double* accuValue;      ///helper array for aquireCompany & CompanyValue

    int findParent(int i)
    {
        int index = i;
        while(parentsArr[index] != -1)
        {
            index = parentsArr[index];
        }
        return index;
    }

    long double getValueTillIndex(int i)
    {
        long double val = 0;
        while(parentsArr[i] != -1)
        {
            val += accuValue[i];
            i = parentsArr[i];
        }
        return val;
    }

public:
    UnionFind(int k): numOfElements(k)
    {
        try
        {
            sizeArr = new int[k+1];
        }
        catch (std::bad_alloc& e)
        {
            this->sizeArr = nullptr;
            throw;
        }
        try
        {
            parentsArr = new int[k+1];
        }
        catch (std::bad_alloc& e)
        {
            delete[] sizeArr;
            this->parentsArr = nullptr;
            throw;
        }
        try
        {
            accuValue = new long double[k+1];
        }
        catch (std::bad_alloc& e)
        {
            delete[] sizeArr;
            delete[] parentsArr;
            this->accuValue = nullptr;
            throw;
        }
        try
        {
            companyParents = new int[k+1];
        }
        catch (std::bad_alloc& e)
        {

            delete[] sizeArr;
            delete[] parentsArr;
            delete[] accuValue;
            this->companyParents = nullptr;
            throw;
        }
        for(int i = 0; i < k + 1; i++)
        {
            sizeArr[i] = 1;            ///only one node upon initialization
            parentsArr[i] = -1;        ///no parents upon initialization, -1 = no parent
            companyParents[i] = i;     ///no company has been bought
            accuValue[i] = i;
        }
    }

    UnionFind(const UnionFind& other) : numOfElements(other.numOfElements)
    {
        try
        {
            sizeArr = new int[numOfElements+1];
        }
        catch (std::bad_alloc& e)
        {
            this->sizeArr = nullptr;
            throw;
        }
        try
        {
            parentsArr = new int[numOfElements+1];
        }
        catch (std::bad_alloc& e)
        {
            delete[] sizeArr;
            this->parentsArr = nullptr;
            throw;
        }
        try
        {
            accuValue = new long double[numOfElements+1];
        }
        catch (std::bad_alloc& e)
        {
            delete[] sizeArr;
            delete[] parentsArr;
            this->accuValue = nullptr;
            throw;
        }
        try
        {
            companyParents = new int[numOfElements+1];
        }
        catch (std::bad_alloc& e)
        {

            delete[] sizeArr;
            delete[] parentsArr;
            delete[] accuValue;
            this->companyParents = nullptr;
            throw;
        }

        for(int i = 0; i < numOfElements + 1; i++)
        {
            this->sizeArr[i] = other.sizeArr[i];
            this->parentsArr[i] = other.parentsArr[i];
            this->accuValue[i] = other.accuValue[i];
            this->companyParents[i] = other.companyParents[i];
        }
    }
    ~UnionFind()
    {
        delete[] sizeArr;
        delete[] parentsArr;
        delete[] companyParents;
        delete[] accuValue;
    }

    UnionFind& operator=(const UnionFind& other) = delete;

    ///need to shrink paths to both parents array and companyParents array
    int Find(int index)
    {
        int root = findParent(index);       ///finding the parent
        long double sumR = getValueTillIndex(index);
        long double toSubtract = 0;

        long double temp;
        int parent;
        while(index != root)
        {
            temp = accuValue[index];
            accuValue[index] = sumR - toSubtract;
            toSubtract += temp;

            parent = parentsArr[index];
            parentsArr[index] = root;
            index = parent;
        }
        return root;
    }

    /// maybe this is not needed
    /*
    int FindMainCompanyParent(int index)
    {
        int root = findParentCompany(index);
        int compParent;
        while(index != root)
        {
            compParent = companyParents[index];
            companyParents[index] = root;
            index = compParent;
        }
        return root;
    }
*/

    void Union(int acquirer, int target, double factor)
    {
        int acquirerParent = findParent(acquirer);
        int targetParent = findParent(target);

        if(acquirerParent == targetParent)
            return;

        int acquirerCompany = companyParents[acquirerParent];
        int targetCompany = companyParents[targetParent];

        int acquirerSize = sizeArr[acquirerParent];
        int targetSize = sizeArr[targetParent];

        if(acquirerSize >= targetSize)
        {
            long double targetCompanyValue = this->getChangedValue(targetCompany);
            //accuValue[acquirerParent] += factor * accuValue[targetParent];
            accuValue[acquirerParent] += factor * targetCompanyValue;
            accuValue[targetParent] = accuValue[targetParent] - accuValue[acquirerParent];

            parentsArr[targetParent] = acquirerParent;
            sizeArr[acquirerParent] += sizeArr[targetParent];
            sizeArr[targetParent] = 0;
        }
        else
        {
            //accuValue[acquirerParent] = factor * accuValue[targetParent] + accuValue[acquirerParent] - accuValue[targetParent];
            long double targetCompanyValue = this->getChangedValue(targetCompany);
            accuValue[acquirerParent] = factor * targetCompanyValue + accuValue[acquirerParent] - accuValue[targetParent];
            parentsArr[acquirerParent] = targetParent;
            sizeArr[targetParent] += sizeArr[acquirerParent];
            sizeArr[acquirerParent] = 0;
        }
        companyParents[targetParent] = acquirerCompany;

    }

    int getParent(int i)
    {
        return this->parentsArr[i];
    }
    int getSize(int i)
    {
        return this->sizeArr[i];
    }

    long double getChangedValue(int i)
    {
        long double changedValue = accuValue[i];
        int index = parentsArr[i];
        while (index != -1)
        {
            changedValue += accuValue[index];
            index = parentsArr[index];
        }
        return changedValue;
    }


};

#endif //DS_WET_2_UNIONFIND_H
