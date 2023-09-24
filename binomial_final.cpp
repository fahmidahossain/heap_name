
#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;
struct Node
{
    int value, degree;
    Node *l_child, *i_sibling, *parent;//Binomial tree creation where Bk holds two Bk-1 trees
    // Node* head;
};

Node* create_node(int key)
{
    Node *dump = new Node;
    dump->value = key;
    dump->degree = 0;
    dump->l_child = dump->parent = dump->i_sibling = NULL;//New node creation with five attributes
    return dump;
}

Node* Join_Trees(Node *tree1, Node *tree2)
{

    if (tree1->value > tree2->value)
        swap(tree1, tree2);
    tree2->parent = tree1;
    tree2->i_sibling = tree1->l_child;
    tree1->l_child = tree2;
    tree1->degree++;

    return tree1;
}


list<Node*> Union(list<Node*> heap1,list<Node*> heap2)
{

    list<Node*>new_heap;
    list<Node*>::iterator m = heap1.begin();
    list<Node*>::iterator n = heap2.begin();
    while (m!=heap1.end() && n!=heap2.end())
    {

        if((*m)->degree <= (*n)->degree)
        {
            new_heap.push_back(*m);
            m++;
        }

        else
        {
            new_heap.push_back(*n);
            n++;
        }
    }


    while (m != heap1.end())
    {
        new_heap.push_back(*m);
        m++;
    }


    while (n!=heap2.end())
    {
        new_heap.push_back(*n);
        n++;
    }
    return new_heap;
}

list<Node*> Combine_To_Adjust(list<Node*>list_of_nodes)
{
    if (list_of_nodes.size() <= 1)
        return list_of_nodes;
    list<Node*> new_heap;
    list<Node*>::iterator n1,n2,n3;
    n1 = n2 = n3 = list_of_nodes.begin();

    if (list_of_nodes.size() == 2)
    {
        n2 = n1;
        n2++;
        n3 = list_of_nodes.end();
    }
    else
    {
        n2++;
        n3=n2;
        n3++;
    }
    while (n1 != list_of_nodes.end())
    {

        if (n2 == list_of_nodes.end())
            n1++;


        else if ((*n1)->degree < (*n2)->degree)
        {
            n1++;
            n2++;
            if(n3!=list_of_nodes.end())
                n3++;
        }


        else if (n3!=list_of_nodes.end() &&
                 (*n1)->degree == (*n2)->degree &&
                 (*n1)->degree == (*n3)->degree)
        {
            n1++;
            n2++;
            n3++;
        }


        else if ((*n1)->degree == (*n2)->degree)
        {
            Node *temp;
            *n1 = Join_Trees(*n1,*n2);
            n2 = list_of_nodes.erase(n2);
            if(n3 != list_of_nodes.end())
                n3++;
        }
    }
    return list_of_nodes;
}


list<Node*> sub_insert(list<Node*>list_of_nodes,Node *value)
{

    list<Node*> dump;
    dump.push_back(value);
    dump = Union(list_of_nodes,dump);
    return Combine_To_Adjust(dump);
}




Node* getMin(list<Node*>list_of_nodes)//search for roots as there must not be any smaller node than root
{
    list<Node*>::iterator m = list_of_nodes.begin();
    Node *dump = *m;
    while (m != list_of_nodes.end())
    {
        if ((*m)->value < dump->value)
            dump = *m;
        m++;
    }
    return dump;
}
list<Node*>heap_After_removeMin(Node *one_heap_only)
{
    list<Node*> tree;
    Node *dump = one_heap_only->l_child;
    Node *min_root;
    while (dump)
    {
        min_root = dump;
        dump = dump->i_sibling;
        min_root->i_sibling = NULL;
        tree.push_front(min_root);
    }
    return tree;
}


list<Node*> Extract_Min(list<Node*>list_of_nodes)//It will find out the min element which must be in the series of root nodes,then delete it
//the scattered node will be arranged in reverse order and make the tree ,then union will happen
{
    list<Node*>list_of_new_nodes,min_root;
    Node *dump;
    dump = getMin(list_of_nodes);
    list<Node*>::iterator m;
    m = list_of_nodes.begin();
    while (m != list_of_nodes.end())
    {
        if (*m != dump)
        {

            list_of_new_nodes.push_back(*m);
        }
        m++;
    }
    min_root = heap_After_removeMin(dump);
    list_of_new_nodes = Union(list_of_new_nodes,min_root);
    list_of_new_nodes = Combine_To_Adjust(list_of_new_nodes);
    return list_of_new_nodes;
}
list<Node*> insert(list<Node*>list_of_nodes, int value)
{
    Node *dump = create_node(value);
    return sub_insert(list_of_nodes,dump);
}

void printHeap(list<Node*> _heap)
{
    list<Node*>::iterator it;
    it = _heap.begin();
    while(it!= _heap.end())
    {
        cout<<"Binomial Tree, B"<<(*it)->degree<<endl;

        queue<Node*> q;
        q.push(*it);

        vector<Node*>v;
        while(!q.empty())
        {
            Node* n = q.front();
            q.pop();
            v.push_back(n);
            Node* c = n->l_child;
            while(c)
            {
                q.push(c);
                c = c->i_sibling;
            }
        }
        int counter = 0;
        int p = 1;
        int l = 0;
        int d = (*it)->degree;
        cout<<"print level 0 : ";

        for(int i = 0; i<v.size(); i++)
        {
            cout<<v[i]->value<<" ";
            ++counter;
            if (counter == p)
            {
                counter = 0;
                p *= (d - l);
                p=p/(l+1);
                ++l;
                if (i != v.size() - 1) cout<<endl << "print level "<<l <<" "<< ":"<<" ";

            }
        }
        cout<<endl;
        it++;
    }
}

int main()
{
    ifstream inputFile("in2.txt");
    ofstream outputFile("output.txt");
    char operation;
    int operand1, operand2,operand3;

    Node *temp;
    int ch,key;
    char firstChar;

    list<Node*> _heap;
    while (inputFile >> operation)
    {
        //cout<<operation;
        switch (operation)
        {
        case 'F':
            //outputFile << "Find-Min returned " << heap.findMin() << endl;
            temp = getMin(_heap);
            cout << "Find-Min returned  "<< temp->value << "\n";

            break;
        case 'E':
            //outputFile << "Extract-Min returned " << heap.extractMin() << endl;
            //*temp = getMin(_heap);
            temp = getMin(_heap);
            _heap = Extract_Min(_heap);
            cout << "Extract-Min returned "<<temp->value<< "\n";
            break;
        case 'I':
            inputFile >> operand3;
            _heap = insert(_heap,operand3);

            break;
        case 'U':
        {list<Node*>heap_to_store;
            //inputFile >> operand1 >> operand2;
            //_heap=unionBionomialHeap(operand1,operand2);
            /*istringstream iss(line);

            // To store the first character of the line ('U')

            // Read the 'U' character
            iss >> firstChar;

            int num;
            while (iss >> num)
            {
                numbers.push_back(num);
            }

            //BinomialHeap otherHeap;
            //_heap = insert(_heap,operand1);
            //cout << "Integers from line starting with 'U': ";
            for (int num : numbers)
            {
                cout << num << " ";
            }
            cout << endl;
            //_heap = insert(_heap,operand2);*/
            std::string line;
            std::getline(inputFile, line); // Read a line from the file

            std::list<int> integerList;
            std::istringstream lineStream(line);
            int num;

            while (lineStream >> num)
            {
                integerList.push_back(num);
            }

            // Print the list of integers
            for (int num : integerList)
            {
                //std::cout << num << " ";
                heap_to_store = insert(heap_to_store,num);

            }
            _heap = Union(_heap,heap_to_store);
            _heap=Combine_To_Adjust(_heap);
            std::cout << std::endl;


        }

            // inputFile.close(); // Close the input file

            // Print the stored integers




        break;



        case 'P':
            cout << "Printing Binomial Heap...\n";
            printHeap(_heap);

            break;
        default:
            cerr << "Invalid operation.\n";
            break;
        }
    }

    // Insert data in the heap
    //_heap = insert(_heap,5);
    //_heap = insert(_heap,2);
//
    //cout << "Heap elements after insertion:\n";
    //printHeap(_heap);
    //_heap = insert(_heap,10);
    //printHeap(_heap);

    //Node *temp = getMin(_heap);
    //cout << "\nMinimum element of heap "
    //<< temp->data << "\n";

    // Delete minimum element of heap

    // printHeap(_heap);

    return 0;
}
