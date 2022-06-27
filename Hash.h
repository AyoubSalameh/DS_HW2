#ifndef DS_WET_2_HASH_H
#define DS_WET_2_HASH_H
#include <memory>
#include <iostream>
#include "Employee.h"

enum HashRes
{
    HASH_SUCCESS,
    HASH_FAILURE
};

class Hash;

class ListNode
{
private:
    Employee* data;
    ListNode* next;
    ListNode* prev;

    friend class Hash;

public:
    ListNode():data(nullptr), next(nullptr), prev(nullptr) {}
    ListNode(const Employee& data): next(nullptr), prev(nullptr)
    {
        try
        {
            this->data = new Employee(data);
        }
        catch (std::bad_alloc& e)
        {
            this->data = nullptr;
            throw;
        }
    }
    ListNode(const ListNode& other)
    {
        if(other.data != nullptr) {
            try {
                this->data = new Employee(*other.data);
            }
            catch (std::bad_alloc &e) {
                this->data = nullptr;
                throw;
            }
        }
        if(other.next != nullptr)
        {
            try {
                this->next = new ListNode(*other.next);
            }
            catch (std::bad_alloc &e) {
                delete this->data;
                this->data = nullptr;
                this->next = nullptr;
                throw;
            }
            this->next->prev = this;
        }
        if(other.prev != nullptr)
        {
            try {
                this->prev = new ListNode(*other.prev);
            }
            catch (std::bad_alloc& e) {
                delete this->data;
                this->data = nullptr;
                delete this->next;
                this->next = nullptr;
                this->prev = nullptr;
                throw;
            }
            this->prev->next = this;
        }

    }
    ~ListNode()
    {
        if(this->data != nullptr)
        {
            delete this->data;
            this->data = nullptr;
        }
        this->next = nullptr;
        this->prev = nullptr;
    }

    Employee* getEmployee()
    {
        return this->data;
    }

    int getID()
    {
        return this->data->getID();
    }
    ///need get and set functions

    ListNode* getNext()
    {
        return this->next;
    }
};

class Hash
{
private:
    int numElements;    ///how many elements are in the hash table atp
    int maxSize;        ///max amount of elements
    ListNode** array;

    int insertIDX(int id, int m) const
    {
        return (id % m);
    }

    void resize(int newMaxSize)
    {
        if(newMaxSize == 0)
            return;
        ListNode **resized;
        try {
            resized = new ListNode *[newMaxSize];
        }
        catch (std::bad_alloc& e) {
            resized = nullptr;
            throw;
        }

        for(int i =0; i < newMaxSize; i++)
            resized[i] = nullptr;

        for(int i = 0 ; i < maxSize; i++)
        {
            ListNode* current = array[i];

            while(current != nullptr)
            {
                Employee emp = *(current->data);
                int index = insertIDX(current->data->getID(), newMaxSize); ///maybe data is null
                if(resized[index] == nullptr) {
                    resized[index] = new ListNode(emp);

                }
                else
                {
                    ListNode *next = resized[index];
                    resized[index] = new ListNode(*current->data);
                    resized[index]->next = next;
                    next->prev = resized[index];
                }
               current = current->next;
            }
        }

        for(int i = 0; i < maxSize; i++)
        {
            ListNode* current = array[i];
            ListNode* next;
            while(current != nullptr)
            {
                next = current->next;
                delete current;
                current = next;
            }
        }

        maxSize = newMaxSize;
        delete[] array;
        array = resized;
    }

public:
    Hash()
    {
        numElements = 0;
        maxSize = 100;
        try
        {
            array = new ListNode*[maxSize];
        }
        catch (std::bad_alloc& e) {
            array = nullptr;
            throw;
        }
        for(int i =0; i < maxSize; i++)
        {
            array[i] = nullptr;
        }
    }
    Hash(const Hash& other) = delete;
    ~Hash()
    {
        if(array != nullptr)
        {
            for(int i =0; i < maxSize; i++)
            {
                ListNode* current = array[i];
                ListNode* next;
                while(current != nullptr)
                {
                    next = current->next;
                    delete current;
                    current = next;
                }
            }
            delete[] array;
        }
    }

    ListNode* find(int id)
    {
        int index = insertIDX(id, maxSize);
        ListNode* current = array[index];
        while(current != nullptr)
        {
            if(current->data->getID() == id)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    HashRes insert(const Employee& employee) {
        ListNode *exists = find(employee.getID());
        if (exists != nullptr)
            return HASH_FAILURE;

        int index = insertIDX(employee.getID(), maxSize);
        ListNode *next;

        if(array[index] == nullptr)
        {
            this->array[index] = new ListNode(employee);
        }
        else {
            next = array[index];

            try {
                array[index] = new ListNode(employee);
            }
            catch (std::bad_alloc &e) {
                array[index] = next;
                throw;
            }
            if (next != nullptr) {
                array[index]->next = next;
                next->prev = array[index];
            }
        }

        numElements++;
        try {
            if (numElements >= maxSize)
                resize(2 * maxSize);
        }
        catch (std::bad_alloc& e) {
            numElements--;
            remove(employee.getID());
            throw;
        }
        return HASH_SUCCESS;
    }

    HashRes remove(int id)
    {
        ListNode* exists = this->find(id);
        if(exists == nullptr)
            return HASH_FAILURE;

        int index = insertIDX(id, maxSize);
        ListNode* current = array[index];
        ///
        while(current != nullptr)
        {
            if(current->data->getID() == id)
            {
                if(current->next != nullptr && current->prev != nullptr)
                {
                    current->next->prev = current->prev;
                    current->prev->next = current->next;
                }

                else if(current->next == nullptr && current->prev != nullptr)
                {
                    current->prev->next = nullptr;
                }

                else if(current->next != nullptr && current->prev == nullptr)
                {
                    array[index] = current->next;
                    array[index]->prev = nullptr;
                }
                else if(current->next == nullptr && current->prev == nullptr)
                {
                    array[index] = nullptr;
                }

                delete current;

                numElements--;

                try {
                    if (numElements <= maxSize / 4)
                        resize(maxSize / 4);
                }
                catch (std::bad_alloc& e)
                {
                    numElements++;
                    this->insert(*exists->data);
                    throw;
                }
                return HASH_SUCCESS;
            }
            current = current->next;
        }
        ///
        return HASH_FAILURE;
    }

    void changeEmployeeCompany(int id)
    {
        for(int i = 0; i < maxSize; i++)
        {
            ListNode* current = array[i];
            while(current != nullptr)
            {
                current->data->changeCompany(id);
                current = current->next;
            }
        }
    }

    ListNode** getArray()
    {
        return this->array;
    }
    int getMaxSize() const
    {
        return this->maxSize;
    }

};



#endif //DS_WET_2_HASH_H
