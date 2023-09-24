#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;


struct Node
{
    int key;
    Node* left;
    Node* right;
    int height;

    bool Is_Leaf()
    {
        return left==nullptr && right == nullptr;
    }
};

class AVLTree
{
public:
    Node* root;

    int getHeight(Node* node)
    {
        if (node == nullptr)
            return -1;
        return node->height;
    }

    int getBalance(Node* node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    Node* rightRotate(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node* leftRotate(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    Node* insert(Node* node, int key, ofstream& outFile)
    {
        if (node == nullptr)
        {
            Node* newNode = new Node;
            newNode->key = key;
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->height = 0;
            // outFile << key; // Print the key at the leaf node
            return newNode;
        }

        if (key < node->key)
            node->left = insert(node->left, key, outFile);
        else if (key > node->key)
            node->right = insert(node->right, key, outFile);

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);

        if (balance > 1)
        {
            if (key < node->left->key)
                return rightRotate(node);
            else
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }

        if (balance < -1)
        {
            if (key > node->right->key)
                return leftRotate(node);
            else
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
//std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return node;

    }

    Node* findMin(Node* node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, int key)
    {
        if (node == nullptr)
            return node;

        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                Node* temp = (node->left) ? node->left : node->right;

                delete node;
                node=temp;
            }
            else
            {
                Node* temp = findMin(node->right);
                node->key = temp->key;
                node->right = deleteNode(node->right, temp->key);
            }
        }

        if (node == nullptr)
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);

        if (balance > 1)
        {
            if (getBalance(node->left) >= 0)
                return rightRotate(node);
            else
            {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }

        if (balance < -1)
        {
            if (getBalance(node->right) <= 0)
                return leftRotate(node);
            else
            {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }

        return node;
    }

    bool search(Node* node, int key)
    {
        if (node == nullptr)
            return false;

        if (key == node->key)
            return true;
        else if (key < node->key)
            return search(node->left, key);
        else
            return search(node->right, key);
    }

    void inOrderTraversal(Node* node, ofstream& outFile)
    {
        if (node == nullptr)
            return;

        inOrderTraversal(node->left, outFile);
        outFile << node->key << " ";
        inOrderTraversal(node->right, outFile);
    }
    void Sub_Print(Node* root,ofstream &outfile)
    {
        if(root == nullptr)
            return;
        outfile<<root->key;
        if(root->Is_Leaf())
            return;

        outfile<<"(";
        Sub_Print(root->left,outfile);
        outfile<<",";

        //outfile<<"(";
        Sub_Print(root->right,outfile);
        outfile<<")";
    }

public:
    AVLTree()
    {
        root = nullptr;
    }

    void insert(int key, ofstream& outFile)
    {
        // auto start_time = high_resolution_clock::now();
        //root = insert(root, key);
        //auto end_time = high_resolution_clock::now();
        //return duration_cast<milliseconds>(end_time - start_time).count();
        //auto start_time = high_resolution_clock::now();
        root = insert(root, key, outFile);
        //auto end_time = high_resolution_clock::now();
        outFile << endl; // Print a newline after each insertion
        //return duration_cast<milliseconds>(end_time - start_time).count();
    }

    void remove(int key)
    {
        root = deleteNode(root, key);
    }

    bool find(int key)
    {
        return search(root, key);
    }

    void traverse(ofstream& outFile)
    {
        inOrderTraversal(root, outFile);
        outFile << endl;
    }
    void print(ofstream &outfile)
    {
        Sub_Print(root,outfile);
        outfile<<endl;
    }
    bool removecheck(const int & value)
    {
        if(find(value))
        {
            //root = deleteNode(root,value);
            //level--;
            return true;
        }
        return false;
    }
};

int main()
{
    ifstream inFile("in.txt");
    ofstream outFile("out_avl.txt");
    ofstream reportFile("report_avl.txt");
    float total_insert_count = 0;
    float total_delete_count = 0;
    float total_search_count = 0;
    float total_traverse_count = 0;
    float insert_time=0;
    float delete_time=0;
    float search_time=0;
    float traverse_time=0;
    int c1,c2,c3,c4;


    AVLTree avl;

    char operation;
    int key;

    while (inFile >> operation)
    {
        if (operation == 'I')
        {
            //auto start_time = high_resolution_clock::now();
            // root = insert(root, num, total_insert_count);
            //auto start_time = high_resolution_clock::now();
            //int insert_time = duration_cast<milliseconds>(end_time - start_time).count();
            inFile >> key;
            auto start_time = high_resolution_clock::now();
            avl.insert(key, outFile);
            auto end_time = high_resolution_clock::now();
            insert_time = duration_cast<milliseconds>(end_time - start_time).count();
            //cout<<insert_time;
            total_insert_count=total_insert_count+insert_time;
            avl.print(outFile);
            //reportFile<<m<<endl;
            c1++;
        }
        //cout<<total_insert_count;
        else if (operation == 'D')
        {
            inFile >> key;
            //int key_to_delete = num; // Replace this with the key to delete
            //start_time = high_resolution_clock::now();
            //root = deleteNode(root, key_to_delete, total_delete_count);
            //end_time = high_resolution_clock::now();
            // int delete_time = duration_cast<milliseconds>(end_time - start_time).count();

            if(avl.removecheck(key))
            {
                auto start_time = high_resolution_clock::now();
                avl.remove(key);
                auto end_time = high_resolution_clock::now();
                delete_time = duration_cast<milliseconds>(end_time - start_time).count();
                //cout<<delete_time;
                total_delete_count=total_delete_count+delete_time;

                avl.print(outFile);

            }

            else
            {
                //outFile<<" no such node"<<endl;
                avl.print(outFile);
            }
            c2++;

        }
        else if (operation == 'F')
        {
            //int key_to_search = num; // Replace this with the key to search
            //start_time = high_resolution_clock::now();
            //bool found = search(root, key_to_search, total_search_count);
            //end_time = high_resolution_clock::now();
            //int search_time = duration_cast<milliseconds>(end_time - start_time).count();
            auto start_time = high_resolution_clock::now();
            inFile >> key;
            if (avl.find(key))
                outFile << "found" << endl;
            else
                outFile << "not found" << endl;
            auto end_time = high_resolution_clock::now();
            search_time = duration_cast<milliseconds>(end_time - start_time).count();
            //cout<<search_time;
            total_search_count=total_search_count+search_time;
            c3++;
        }
        else if (operation == 'T')
        {
            //start_time = high_resolution_clock::now();
            //inorderTraversal(root, total_traverse_count);
            //end_time = high_resolution_clock::now();
            // int traverse_time = duration_cast<milliseconds>(end_time - start_time).count();
            auto start_time = high_resolution_clock::now();
            avl.traverse(outFile);
            auto  end_time = high_resolution_clock::now();
            traverse_time = duration_cast<milliseconds>(end_time - start_time).count();
            //cout<<traverse_time;
            total_traverse_count=total_traverse_count+traverse_time;
            c4++;
        }
    }

    inFile.close();
    outFile.close();

    // Writing report (Note: The time values are placeholders, you should measure them properly)
    //float total_time = total_insert_count + total_delete_count + total_search_count + total_traverse_count;
    //reportFile << "operation time(ms)" << endl;
    //reportFile << "insert " <<total_insert_count<<endl;
    //reportFile << "delete " <<total_delete_count<<endl;
    //reportFile << "search " <<total_search_count<<endl;
    //reportFile << "trav " <<total_traverse_count<<endl;
    //reportFile << "total " <<total_time<<endl;
    //amortized runtime
    float total_time2 = total_insert_count/c1 + total_delete_count/c2 + total_search_count/c3 + total_traverse_count/c4;
    reportFile << "operation time(ms)" << endl;
    reportFile << "insert " <<total_insert_count/c1<<endl;
    reportFile << "delete " <<total_delete_count/c2<<endl;
    reportFile << "search " <<total_search_count/c3<<endl;
    reportFile << "trav " <<total_traverse_count/c4<<endl;
    reportFile << "total " <<total_time2<<endl;


    reportFile.close();

    return 0;
}
