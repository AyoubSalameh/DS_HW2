#ifndef TREE2NDATTEMPT_TREE_H
#define TREE2NDATTEMPT_TREE_H
#include <iostream>
#include <memory>
#include <cmath>
#include <stdexcept>


enum TreeRes
{
    TREE_SUCCESS = 0,
    TREE_FAILURE,
    ALREADY_IN_TREE,
    NOT_FOUND_IN_TREE
};

template<typename K, typename D> class Node;
template<typename K, typename D> class Tree;

template<typename K, typename D>
class Node
{
private:
    K* key;
    D* data;
    int height;
    int bf;
    Node<K,D>* left;
    Node<K,D>* right;
    Node<K,D>* parent;

    ///added for hw2
    int numOfNodes;            //number of nodes in subtree
    int grade;
    long long sumOfGrades;           //sum of grades in subtree // changed to double

    friend class Tree<K,D>;

    void copyNode(const Node<K,D>& other)
    {
        if(other.key != nullptr) {
            try {
                this->key = new K(*other.key);
            } catch (std::bad_alloc &e) {
                this->key = nullptr;
                throw;
            }
        }
        if(other.data != nullptr) {
            try {
                this->data = new D(*other.data);
            } catch (std::bad_alloc &e) {
                delete this->key;
                this->key = nullptr;
                this->data = nullptr;
                throw;
            }
        }
        if(other.left != nullptr)
        {
            try{
                this->left = new Node<K,D>(*other.left);
            } catch (std::bad_alloc& e) {
                delete this->key;
                this->key = nullptr;
                delete this->data;
                this->data = nullptr;
                this->left = nullptr;  ///maybe this is useless
                throw;
            }
            this->left->parent = this;
        }
        if(other.right != nullptr)
        {
            try {
                this->right = new Node<K,D>(*other.right);
            } catch (std::bad_alloc& e) {
                delete this->key;
                this->key = nullptr;
                delete this->data;
                this->data = nullptr;
                delete this->left;
                this->left = nullptr;
                this->right = nullptr;
                throw;
            }
            this->right->parent = this;
        }
        this->parent = nullptr;
        this->height = other.height;
        this->bf = other.bf;
        this->numOfNodes = other.numOfNodes;
        this->grade = other.grade;
        this->sumOfGrades = other.sumOfGrades;
    }


public:
    ///maybe here the nodenum needs to be one
    Node(): key(nullptr), data(nullptr), height(0), bf(0),
            left(nullptr), right(nullptr),  parent(nullptr),
            numOfNodes(0), grade(0), sumOfGrades(0) {}
    Node(const K& key, const D& data, int grade) : height(0), bf(0),
                                        left(nullptr), right(nullptr),  parent(nullptr),
                                        numOfNodes(1), grade(grade), sumOfGrades(grade)
    {
        try {
            this->key = new K(key);
        } catch (std::bad_alloc& e) {
            this->key = nullptr;
            throw;
        }
        try {
            this->data = new D(data);
        } catch (std::bad_alloc& e) {
            delete this->key;
            this->key = nullptr;
            this->data = nullptr;
            throw;
        }
    }
    Node(const Node<K,D>& other)
    {
        this->copyNode(other);
    }
    ~Node()
    {
        if(this->key != nullptr && this->data != nullptr) {
            this->left = nullptr;
            this->right = nullptr;
            this->parent = nullptr;
            delete this->key;
            delete this->data;
        }
    }
    Node& operator=(const Node<K,D>& other) = default;

    K* getKey()
    {
        return this->key;
    }
    D* getData()
    {
        return this->data;
    }
    int getHeight()
    {
        return this->height;
    }
    int getBF()
    {
        return this->bf;
    }

    Node<K,D>* getRight()
    {
        return this->right;
    }
    Node<K,D>* getLeft()
    {
        return this->left;
    }
    int getNumOfNodes()
    {
        return this->numOfNodes;
    }
    int getGrade()
    {
        return this->grade;
    }
    long long getSumOfGrades()
    {
        return this->sumOfGrades;
    }
    //////////
    void increaseGrade(int bumpGrade)
    {
        if(bumpGrade > 0)
            grade += bumpGrade;
    }

    void updateHeight()
    {
        int l_height = -1;
        int r_height = -1;
        if(this->left != nullptr)
            l_height = this->left->height;
        if(this->right != nullptr)
            r_height = this->right->height;

        this->height = 1 + std::max(l_height, r_height);
    }
    void updateBF()
    {
        int l_height = -1;
        int r_height = -1;
        if(this->left != nullptr)
            l_height = this->left->height;
        if(this->right != nullptr)
            r_height = this->right->height;

        bf = l_height - r_height;
    }
    void updateNumOfNodes()
    {
        int l_numOfNodes = 0;
        int r_numOfNodes = 0;
        if(this->left != nullptr)
            l_numOfNodes = this->left->numOfNodes;
        if(this->right != nullptr)
            r_numOfNodes = this->right->numOfNodes;

        numOfNodes = 1 + l_numOfNodes + r_numOfNodes;
    }
    void updateSumOfGrades()
    {
        long long l_sumOfGrades = 0;
        long long r_sumOfGrades = 0;
        if(this->left != nullptr)
            l_sumOfGrades = this->left->sumOfGrades;
        if(this->right != nullptr)
            r_sumOfGrades = this->right->sumOfGrades;

        sumOfGrades = grade + l_sumOfGrades + r_sumOfGrades;
    }

};



///##################################################################################
///##################################################################################
///##################################################################################
///################################### TREE CLASS ###################################
///##################################################################################
///##################################################################################
///##################################################################################



template <typename K, typename D>
class Tree
{
private:
    Node<K,D>* root;
    int size;

    void destroy(Node<K,D>* toDestroy)
    {
        if(toDestroy == nullptr)
            return;
        destroy(toDestroy->left);
        destroy(toDestroy->right);
        delete toDestroy;
    }

    void LL(Node<K,D>* B)
    {
        Node<K,D>* A = B->left;
        B->left = A->right;
        A->right = B;

        B->updateHeight();
        A->updateHeight();
        B->updateBF();
        A->updateBF();
        B->updateNumOfNodes();
        A->updateNumOfNodes();
        B->updateSumOfGrades();
        A->updateSumOfGrades();

        A->parent = B->parent;
        B->parent = A;

        if(A->right->left != nullptr)
        {
            A->right->left->parent = A->right;
        }
        if(A->parent == nullptr)
        {
            root = A;
        }
        else
        {
            if(*A->key < *A->parent->key)
            {
                A->parent->left = A;
            }
            else
            {
                A->parent->right = A;
            }
        }
    }
    void RR(Node<K,D>* A)
    {
        Node<K,D>* B = A->right;
        A->right = B->left;
        B->left = A;

        B->left->updateHeight();
        B->updateHeight();
        B->left->updateBF();
        B->updateBF();
        B->left->updateNumOfNodes();
        B->updateNumOfNodes();
        B->left->updateSumOfGrades();
        B->updateSumOfGrades();

        B->parent = A->parent;
        B->left->parent = B;

        if(B->left->right != nullptr)
        {
            B->left->right->parent = B->left;
        }
        if(B->parent == nullptr)
        {
            root = B;
        }
        else
        {
            if(*B->key < *B->parent->key)
            {
                B->parent->left = B;
            }
            else
            {
                B->parent->right = B;
            }
        }
    }
    void LR(Node<K,D>* C)
    {
        Node<K,D>* A = C->left;
        RR(A);
        LL(C);
    }
    void RL(Node<K,D>* C)
    {
        Node<K,D>* A = C->right;
        LL(A);
        RR(C);
    }

    void balance(Node<K,D>* unbalanced)
    {
        if(unbalanced == nullptr)
            return;
        if(unbalanced->bf == 2)
        {
            if(unbalanced->left->bf >= 0)
                LL(unbalanced);
            else if(unbalanced->left->bf == -1)
                LR(unbalanced);
        }
        else if(unbalanced->bf == -2)
        {
            if(unbalanced->right->bf <= 0)
                RR(unbalanced);
            else if(unbalanced->right->bf == 1)
                RL(unbalanced);
        }
    }

    ///basically goes down, balances each subtree, then goes back up and does the same
    void balanceAfterRemoval(Node<K,D>* sub, const K& key)
    {
        //if (sub == nullptr)
          //  return;
        if(sub->key == nullptr)
            return;
        if(key < *sub->key && sub->left != nullptr)
            balanceAfterRemoval(sub->left, key);
        else if(key > *sub->key && sub->right != nullptr)
            balanceAfterRemoval(sub->right, key);

        sub->updateHeight();
        sub->updateBF();
        sub->updateNumOfNodes();
        sub->updateSumOfGrades();
        balance(sub);
    }


    ///################################################################################################
    ///################################################################################################
    TreeRes insert_internal(Node<K,D>* subroot, const K& key, const D& data, int grade)
    {
        ///only if tree is empty
        if(subroot->key == nullptr)
        {
            try {
                subroot->key = new K(key);
            } catch (std::bad_alloc& e) {
                subroot->key = nullptr;
                throw;
            }
            try {
                subroot->data = new D(data);
            } catch (std::bad_alloc& e) {
                delete subroot->key;
                subroot->key = nullptr;
                subroot->data = nullptr;
                throw;
            }
            subroot->height = 0;
            subroot->bf = 0;
            subroot->numOfNodes = 1;
            subroot->grade = grade;
        }
        else if (key == *subroot->key)
        {
            return ALREADY_IN_TREE;
        }
        else if(key < *subroot->key)
        {
            if(subroot->left == nullptr)
            {
                try{
                    subroot->left = new Node<K,D>(key, data, grade);
                } catch (std::bad_alloc& e) {
                    subroot->left = nullptr;
                    throw;
                }
                subroot->left->parent = subroot;
            }
            else
            {
                TreeRes res = this->insert_internal( subroot->left, key, data, grade);
                if (res != TREE_SUCCESS) return res;
            }
        }
        else if(key > *subroot->key)
        {
            if(subroot->right == nullptr)
            {
                try {
                    subroot->right = new Node<K,D>(key, data, grade);
                } catch (std::bad_alloc& e) {
                    subroot->right = nullptr;
                    throw;
                }
                subroot->right->parent = subroot;
            }
            else
            {
                TreeRes res = this->insert_internal(subroot->right, key, data, grade);
                if (res != TREE_SUCCESS) return res;
            }
        }

        subroot->updateHeight();
        subroot->updateBF();
        subroot->updateNumOfNodes();
        subroot->updateSumOfGrades();
        balance(subroot);
        return TREE_SUCCESS;
    }

    TreeRes remove_internal(Node<K,D>* subroot, const K& key)
    {
        Node<K,D>* toRemove = find_internal(subroot, key);
        if(toRemove == nullptr)
            return NOT_FOUND_IN_TREE;
        Node<K,D>* parent = toRemove->parent;


        if(toRemove->left == nullptr && toRemove->right == nullptr)         ///no sons
        {
            if(parent == nullptr)                                       ///meaning only root
            {
                delete toRemove->key;
                delete toRemove->data;
                toRemove->key = nullptr;
                toRemove->data = nullptr;
                toRemove->sumOfGrades = 0;
                toRemove->grade = 0;
                toRemove->numOfNodes = 0;
            }
            else
            {
                if(*toRemove->key < *parent->key)
                {
                    delete parent->left;
                    parent->left = nullptr;
                }
                else if(*toRemove->key > *parent->key)
                {
                    delete parent->right;
                    parent->right = nullptr;
                }
                balanceAfterRemoval(root, *parent->key);
            }
        }

        else if(toRemove->left != nullptr && toRemove->right == nullptr)    ///left son only
        {
            if(parent == nullptr)                                           ///no parent
            {
                root = toRemove->left;
                root->parent = nullptr;
            }
            else                                                            ///parent
            {
                if(*toRemove->key < *parent->key)
                {
                    parent->left = toRemove->left;
                    parent->left->parent = parent;
                }
                else
                {
                    parent->right = toRemove->left;
                    parent->right->parent = parent;
                }
                balanceAfterRemoval(root,*parent->key);

            }
            toRemove->left = nullptr;
            delete toRemove;
        }

        else if(toRemove->left == nullptr && toRemove->right != nullptr)    ///right son only
        {
            if(parent == nullptr)
            {
                root = toRemove->right;
                root->parent = nullptr;
            }
            else
            {
                if(*toRemove->key > *parent->key)
                {
                    parent->right = toRemove->right;
                    parent->right->parent = parent;
                }
                else
                {
                    parent->left = toRemove->right;
                    parent->left->parent = parent;
                }
                balanceAfterRemoval(root, *parent->key);

            }
            toRemove->right = nullptr;
            delete toRemove;
        }

        else                                                                ///two sons
        {
            Node<K,D>* replacement = findMax_internal(toRemove->left);
            Node<K,D>* temp = (replacement->parent == toRemove) ? nullptr : replacement->parent;
            replacement->right = toRemove->right;


            if (replacement->right != nullptr)
                replacement->right->parent = replacement;
            if(replacement->left != nullptr)
                replacement->left->parent = replacement->parent;
            if(replacement->parent == toRemove)
                toRemove->left = replacement->left;
            else
                replacement->parent->right = replacement->left;
            replacement->left = toRemove->left;
            if(replacement->left != nullptr)
                replacement->left->parent = replacement;

            replacement->parent = parent;
            if(parent == nullptr)
                root = replacement;
            else
            {
                if(*toRemove->key < *parent->key)
                    parent->left = replacement;
                else
                    parent->right = replacement;
            }

            if(temp != nullptr)
                balanceAfterRemoval(root, *temp->key);
            else
                balanceAfterRemoval(root, *replacement->key);

            delete toRemove;
        }


        return TREE_SUCCESS;
    }

    Node<K,D>* find_internal(Node<K,D>* sub, const K& key)
    {
        if(sub == nullptr || sub->key == nullptr)
        {
            return nullptr;
        }
        if(key == *sub->key)
        {
            return sub;
        }
        if(key < *sub->key && sub->left != nullptr)
            return find_internal(sub->left, key);
        if(key > *sub->key && sub->right != nullptr)
            return find_internal(sub->right, key);

        return nullptr;
    }
    Node<K,D>* findMax_internal(Node<K,D>* sub)
    {
        if(sub == nullptr)
            return nullptr;
        while(sub->right != nullptr)
            sub = sub->right;
        return sub;
    }
    Node<K,D>* findMin_internal(Node<K,D>* sub)
    {
        if(sub == nullptr)
            return nullptr;
        while(sub->left != nullptr)
            sub = sub->left;
        return sub;
    }

    int inOrderFillData_internal(Node<K,D>* sub, D* dataArr, int index)
    {
        if(sub == nullptr)
            return index;
        if(sub->left != nullptr)
            index = inOrderFillData_internal(sub->left, dataArr, index);
        dataArr[index] = *sub->data;
        index++;
        if(sub->right != nullptr)
            index = inOrderFillData_internal(sub->right, dataArr, index);
        return index;
    }
    int inOrderFillKeys_internal(Node<K,D>* sub, K* keyArr, int index)
    {
        if(sub == nullptr)
            return index;
        if(sub->left != nullptr)
            index = inOrderFillKeys_internal(sub->left, keyArr, index);
        keyArr[index] = *sub->key;
        index++;
        if(sub->right != nullptr)
            index = inOrderFillKeys_internal(sub->right, keyArr, index);
        return index;
    }
    int reverseInOrderFillKeyInArray_internal(Node<K,D>* sub, K* keyArr, int index)
    {
        if(sub == nullptr)
            return index;
        if(sub->right != nullptr)
            index = reverseInOrderFillKeyInArray_internal(sub->right, keyArr, index);
        keyArr[index] = *sub->key;
        index++;
        if(sub->left != nullptr)
            index = reverseInOrderFillKeyInArray_internal(sub->left, keyArr, index);
        return index;
    }

    int limitedIterationsInOrder_internal(Node<K,D>* sub, int* dataArr, int iter, int index)
    {
        if(sub == nullptr || iter == index)
            return index;
        if(sub->left != nullptr)
        {
            index = limitedIterationsInOrder_internal(sub->left, dataArr, iter, index);
            if(index == iter) return index;
        }
        dataArr[index] = (*sub->data)();   //overloaded () operator for HiringCompany Type
        index++;
        if(index == iter) return index;
        if(sub->right != nullptr) {
            index = limitedIterationsInOrder_internal(sub->right, dataArr, iter, index);
            if(index == iter) return index;
        }
        return index;
    }

    int countAmount(Node<K,D>* sub, const K& minKey, const K& maxKey, int index)
    {
        if(sub == nullptr || sub->key == nullptr)
            return index;
        if(minKey <= *sub->key && *sub->key <= maxKey)
        {
            return countAmount(sub->left, minKey, maxKey, index) +
                   countAmount(sub->right, minKey, maxKey, index) + 1;
        }
        if(*sub->key < minKey)
            return countAmount(sub->right, minKey, maxKey, index);
        if(maxKey < *sub->key)
            return countAmount(sub->left, minKey, maxKey, index);

        return index;
    }

    int fillMatchingInArr_internal(Node<K,D>* sub, D* arr, const K& minKey, const K& maxKey, int index)
    {
        if(sub == nullptr || sub->key == nullptr)
            return index;
        if(minKey <= *sub->key && *sub->key <= maxKey)
        {
            arr[index] = *sub->data;
            index++;
            index = fillMatchingInArr_internal(sub->left, arr, minKey, maxKey, index);
            index = fillMatchingInArr_internal(sub->right,arr, minKey, maxKey, index);
        }
        if(*sub->key < minKey)
            index = fillMatchingInArr_internal(sub->right, arr, minKey, maxKey, index);
        if(maxKey < *sub->key)
            index = fillMatchingInArr_internal(sub->left, arr, minKey, maxKey, index);
        return index;
    }

    int inOrderFillKeyAndData_internal(Node<K,D>* sub, K* keyArr, D* dataArr, int* grades, int index)
    {
        if(sub == nullptr)
            return index;
        if(sub->left != nullptr)
            index = inOrderFillKeyAndData_internal(sub->left, keyArr, dataArr, grades, index);

        keyArr[index] = *sub->key;
        dataArr[index] = *sub->data;
        grades[index] = sub->grade;
        index++;

        if(sub->right != nullptr)
            index = inOrderFillKeyAndData_internal(sub->right, keyArr, dataArr, grades, index);

        return index;
    }

    Node<K,D>* convert_internal(Node<K,D>* sub, K* keyArr, D* dataArr, int* grades, int first, int last)
    {
        if(first > last)
            return nullptr;
        int mid = (first + last)/2;

        if(sub == root)
        {
            sub->key = new K(keyArr[mid]);
            sub->data = new D(dataArr[mid]);
            sub->grade = grades[mid];
        }
        else
            sub = new Node<K,D>(keyArr[mid], dataArr[mid], grades[mid]);

        sub->left = convert_internal(sub->left, keyArr, dataArr, grades, first, mid - 1);
        if(sub->left != nullptr)
            sub->left->parent = sub;

        sub->right = convert_internal(sub->right, keyArr, dataArr, grades, mid + 1, last);
        if(sub->right != nullptr)
            sub->right->parent = sub;

        sub->updateHeight();
        sub->updateBF();
        sub->updateNumOfNodes();
        sub->updateSumOfGrades();
        balance(sub);
        return sub;
    }

    TreeRes raiseGrade_internal(Node<K,D>* sub, const K& key, int bumpGrade)
    {
        if(sub == nullptr || sub->key == nullptr)
            return TREE_FAILURE;
        if(key == *sub->key)
            sub->grade += bumpGrade;
        else if(key < *sub->key && sub->left != nullptr)
            raiseGrade_internal(sub->left, key, bumpGrade);
        else if(key > *sub->key && sub->right != nullptr)
            raiseGrade_internal(sub->right, key, bumpGrade);
        else
            return TREE_FAILURE;

        sub->updateSumOfGrades();
        return TREE_SUCCESS;
    }

    Node<K,D>* select_internal(Node<K,D>* sub, int k)
    {
        if(sub == nullptr || sub->key == nullptr)
            return nullptr;
        if(k > size || k <= 0)
            return nullptr;

        int numOfNodesLeft = 0;
        if(sub->left != nullptr)
            numOfNodesLeft = sub->left->numOfNodes;

        if(numOfNodesLeft == k - 1)
            return sub;
        else if(numOfNodesLeft > k - 1)
            return select_internal(sub->left, k);
        else if(numOfNodesLeft < k - 1)
            return select_internal(sub->right, k - numOfNodesLeft - 1);

        return nullptr;
    }

    long long sumOfGrades_internal(Node<K,D>* sub, const K& key)
    {
        if(sub == nullptr || sub->key == nullptr)
            return 0;
        long long sum = 0;
        while(sub != nullptr)
        {
            if(*sub->key == key)
            {
                if(sub->right != nullptr)
                    sum += sub->right->getSumOfGrades();
                sum += sub->grade;
                return sum;
            }
            else if(*sub->key > key)
            {
                if(sub->right != nullptr)
                    sum += sub->right->getSumOfGrades();
                sum += sub->grade;
                sub = sub->left;
            }
            else if(*sub->key < key)
            {
                sub = sub->right;
            }
        }
        return sum;
    }

    TreeRes statsBiggerThan_internal(Node<K,D>* sub, int salary, int* numOfEmployees, long long* sumOfGrades)
    {
        if(sub == nullptr || sub->key == nullptr)
        {
            *numOfEmployees = 0;
            *sumOfGrades = 0;
            return TREE_FAILURE;
        }

        long long sum = 0;
        int num = 0;

        while(sub != nullptr)
        {
            if((*sub->key)() >= salary)
            {
                if(sub->right != nullptr) {
                    num += sub->right->numOfNodes;
                    sum += sub->right->sumOfGrades;
                }
                num++;
                sum+=sub->grade;

                sub = sub->left;
            }
            else if((*sub->key)() < salary)
            {
                sub = sub->right;
            }
        }
        *sumOfGrades = sum;
        *numOfEmployees = num;
        return TREE_SUCCESS;
    }

public:
    Tree()
    {
        this->size = 0;
        this->root = new Node<K,D>();
    }

    Tree(const Tree<K,D>& other) : size(other.size)
    {
        try { this->root = new Node<K,D>(*other.root); }
        catch (std::bad_alloc& e)
        {
            if(this->root != nullptr)
                destroy(this->root);
            this->root = nullptr;
            throw;
        }
    }

    ~Tree()
    {
        destroy(this->root);
        root = nullptr;
    }

    Tree& operator=(const Tree<K,D>& other)
    {
        if(&other == this)
        {
            return *this;
        }
        destroy(this->root);
        try { this->root = new Node<K,D>(*other.root); }
        catch (std::bad_alloc& e) {
            if(this->root != nullptr)
                destroy(this->root);
            this->root = nullptr;
            throw;
        }
        return *this;
    }


    TreeRes insert(const K& key, const D& data, int grade)
    {
        TreeRes res = this->insert_internal(root, key, data, grade);
        if (res == TREE_SUCCESS)
        {
            size++;
        }
        return res;
    }

    TreeRes remove(const K& key)
    {
        TreeRes res = this->remove_internal(root, key);
        if(res == TREE_SUCCESS)
        {
            size--;
        }
        return  res;
    }

    Node<K,D>* findMin()
    {
        return findMin_internal(root);
    }
    Node<K,D>* findMax()
    {
        return findMax_internal(root);
    }
    Node<K,D>* find(const K& key)
    {
        return find_internal(root, key);
    }

    TreeRes raiseGrade(const K& key ,int bumpGrade)
    {
        return raiseGrade_internal(root, key, bumpGrade);
    }

    TreeRes inOrderFillDataInArray(D* dataArr)
    {
        int arrSize = inOrderFillData_internal(root, dataArr, 0);
        if(this->size == arrSize)
            return TREE_SUCCESS;
        return TREE_FAILURE;
    }

    TreeRes inOrderFillKeyInArray(K* keyArr)
    {
        int arrSize = inOrderFillKeys_internal(root, keyArr, 0);
        if(this->size == arrSize)
            return TREE_SUCCESS;
        return TREE_FAILURE;
    }
    TreeRes reverseInOrderFillKeyInArray(K* keyArr)
    {
        int arrSize = reverseInOrderFillKeyInArray_internal(root, keyArr, 0);
        if(this->size == arrSize)
            return TREE_SUCCESS;
        return TREE_FAILURE;
    }



    ///this function is solely for GetHighestInEachCompany
    TreeRes limitedIterationsInOrder(int* dataArr, int iter)
    {
        int index = limitedIterationsInOrder_internal(root, dataArr, iter, 0);
        if(index == iter)
            return TREE_SUCCESS;
        return TREE_FAILURE;
    }

    int findAmountInInterval(const K& minKey, const K& maxKey)
    {
        return countAmount(root, minKey, maxKey, 0);
    }

    TreeRes fillMatchingInArr(D* arr, const K& minKey, const K& maxKey)
    {
        fillMatchingInArr_internal(root, arr, minKey, maxKey, 0);
        return TREE_SUCCESS;
    }

    TreeRes inOrderFillKeyAndData(K* keyArr, D* dataArr, int* grades)
    {
        int index = inOrderFillKeyAndData_internal(root, keyArr, dataArr, grades, 0);
        if(index != size)
            return TREE_FAILURE;
        return TREE_SUCCESS;
    }

    TreeRes ConvertArrayToTree(K* keyArr, D* dataArr, int* grades, int size)
    {
        destroy(root);  ///
        root = new Node<K,D>;     ///
        root = convert_internal(root, keyArr, dataArr, grades, 0, size - 1);
        this->size = size;
        return TREE_SUCCESS;
    }

    Node<K,D>* select(int k)
    {
        return select_internal(root, k);
    }

    ///might need more functions
    int getSize()
    {
        return this->size;
    }
    long long getTotalGrades()
    {
        return this->root->sumOfGrades;
    }

    ///not for avg
    long long sumOfGrades(const K& key)
    {
        return sumOfGrades_internal(root, key);
    }

    TreeRes statsBiggerThan(int salary, int* numOfEmployees, long long* sumOfGrades)
    {
        return statsBiggerThan_internal(root, salary, numOfEmployees, sumOfGrades);
    }
};


#endif //TREE2NDATTEMPT_TREE_H
