#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>
using namespace std;
using ull = unsigned long long;

struct Node
{
    enum Type {VAL, PAIR};
    enum ChildType {ROOT, L, R};

    Type type;
    ChildType childType;
    int depth = -1;
    int value = -1;
    shared_ptr<Node> left = NULL, right = NULL, parent = NULL;

    void SetDepth(int _depth)
    {
        depth = _depth;
        if (left != NULL)
        {
            left->SetDepth(depth + 1);
        }
        if (right != NULL)
        {
            right->SetDepth(depth + 1);
        }
    }
};

// #define VERBOSE;

ostream& operator<< (ostream& os, const Node n)
{
#ifdef VERBOSE
    string childType = n.childType == Node::ChildType::ROOT ? "ROOT" : n.childType == Node::ChildType::L ? "L" : "R";

    os<<"Node: ("<<childType<<" - D:"<<n.depth<<"){ ";
    if (n.type == Node::Type::VAL)
    {
        os<<"val:"<<n.value<<" }";
    }
    if (n.type == Node::Type::PAIR)
    {
        string tabs = string(n.depth+1,'\t');
        os<<endl<<tabs<<*n.left<<endl<<tabs<<*n.right<<endl<<string(n.depth,'\t')<<"}";
    }
#else
    if (n.type == Node::Type::VAL)
    {
        os<<n.value;
    }
    if (n.type == Node::Type::PAIR)
    {
        os<<"["<<*n.left<<","<<*n.right<<"]";
    }
#endif
    return os;
}

shared_ptr<Node> CopyTree(const shared_ptr<Node> tree)
{
    shared_ptr<Node> node = make_shared<Node>();
    node->type = tree->type;
    node->childType = tree->childType;
    node->value = tree->value;
    node->depth = tree->depth;

    if (tree->left != NULL)
    {
        node->left = CopyTree(tree->left);
        node->left->parent = node;
    }
    if (tree->right != NULL)
    {
        node->right = CopyTree(tree->right);
        node->right->parent = node;
    }

    return node;
}

void DeleteTree(const shared_ptr<Node> tree)
{
    if (tree->parent != NULL)
    {
        tree->parent = NULL;
    }
    if (tree->left != NULL)
    {
        DeleteTree(tree->left);
        tree->left = NULL;
    }
    if (tree->right != NULL)
    {
        DeleteTree(tree->right);
        tree->right = NULL;
    }
}

vector<string> SplitTreeString(const string str)
{
    string lhs = "";
    string rhs = "";
    size_t index = 0;
    if (str[index] != '[')
    {
        // pure value
        lhs = str.substr(0,1);
        // 2 to chop off the ","
        rhs = str.substr(2);

        return {lhs, rhs};
    }
    int bracketCount = 1;
    index++;
    while (bracketCount > 0)
    {
        if (str[index] == ']')
        {
            bracketCount--;
        }
        if (str[index] == '[')
        {
            bracketCount++;
        }
        index++;
    }
    // [[5,6]],5
    // 012345678
    //        ^
    //      index
    lhs = str.substr(0,index);
    rhs = str.substr(index + 1);
    return {lhs, rhs};
}

shared_ptr<Node> ParseLine(const string line)
{
    if (line.size() == 1 && line[0] >= '0' && line[0] <= '9')
    {
        shared_ptr<Node> n = make_shared<Node>();
        n->type = Node::Type::VAL;
        n->value = line[0] - '0';
        return n;
    }
    else if (line.front() == '[' && line.back() == ']')
    {
        shared_ptr<Node> n = make_shared<Node>();
        n->type = Node::Type::PAIR;
        string aux = line.substr(1,line.size()-2);
        vector<string> children = SplitTreeString(aux);
        shared_ptr<Node> lhs = ParseLine(children[0]);
        shared_ptr<Node> rhs = ParseLine(children[1]);
        lhs->childType = Node::ChildType::L;
        rhs->childType = Node::ChildType::R;
        lhs->parent = n;
        rhs->parent = n;
        n->left = lhs;
        n->right = rhs;
        return n;
    }
    else
    {
        cout<<"EX: Line format (line:"<<line<<")"<<endl;
        assert(false);
        return {};
    }
}

vector<shared_ptr<Node>> ParseInput()
{
    vector<shared_ptr<Node>> nodes;
    ifstream inFile("./inputs/day-18.input");
    // ifstream inFile("./inputs/test.input");
    string strLine;
    while (getline(inFile,strLine))
    {
        shared_ptr<Node> n = ParseLine(strLine);
        n->SetDepth(0);
        n->childType = Node::ChildType::ROOT;
        nodes.push_back(n);
    }

    return nodes;
}

shared_ptr<Node> PartialAdd(const shared_ptr<Node> lhs, const shared_ptr<Node> rhs)
{
    shared_ptr<Node> n = make_shared<Node>();
    n->type = Node::Type::PAIR;
    n->childType = Node::ChildType::ROOT;

    n->left = lhs;
    n->left->childType = Node::ChildType::L;
    n->left->parent = n;

    n->right = rhs;
    n->right->childType = Node::ChildType::R;
    n->right->parent = n;
    return n;
}

bool ExplodeTreeRec(shared_ptr<Node> tree)
{
    if (tree->type == Node::Type::PAIR &&
        tree->depth >= 4 &&
        tree->left->type == Node::Type::VAL &&
        tree->right->type == Node::Type::VAL)
    {
        // explode
        shared_ptr<Node> aux;
        // left side
        {
            if (tree->childType == Node::ChildType::L)
            {
                // we are the left child so go up until we are the right or root
                aux = tree->parent;
                while (aux->childType == Node::ChildType::L)
                {
                    aux = aux->parent;
                }
            }
            else
            {
                aux = tree;
            }

            // if root do nothing
            if (aux->childType != Node::ChildType::ROOT)
            {
                // else we found a node that is the right child so go down on the right side of the left part of the parent until you find a value
                aux = aux->parent->left;
                while (aux->type != Node::Type::VAL)
                {
                    aux = aux->right;
                }
                aux->value += tree->left->value;
            }
        }

        // right side
        {
            if (tree->childType == Node::ChildType::R)
            {
                // we are the right child so go up until we are the left or root
                aux = tree->parent;
                while (aux->childType == Node::ChildType::R)
                {
                    aux = aux->parent;
                }
            }
            else
            {
                aux = tree;
            }

            // if root do nothing
            if (aux->childType != Node::ChildType::ROOT)
            {
                // else we found a node that is the left child so go down on the left side of the right part of the parent until you find a value
                aux = aux->parent->right;
                while (aux->type != Node::Type::VAL)
                {
                    aux = aux->left;
                }
                aux->value += tree->right->value;
            }
        }
        aux = NULL;
        tree->type = Node::Type::VAL;
        tree->value = 0;
        DeleteTree(tree->left);
        tree->left = NULL;
        DeleteTree(tree->right);
        tree->right = NULL;

        return true;
    }

    if (tree->type == Node::Type::PAIR)
    {
        bool treeChanged = ExplodeTreeRec(tree->left);
        if (!treeChanged)
        {
            treeChanged = ExplodeTreeRec(tree->right);
        }
        return treeChanged;
        return false;
    }
    else
    {
        return false;
    }
}

bool SplitTreeRec(shared_ptr<Node> tree)
{
    if (tree->type == Node::Type::VAL && tree->value >= 10)
    {
        // split
        shared_ptr<Node> lhs = make_shared<Node>();
        lhs->childType = Node::ChildType::L;
        lhs->type = Node::Type::VAL;
        lhs->value = tree->value / 2;
        lhs->depth = tree->depth + 1;
        lhs->parent = tree;

        shared_ptr<Node> rhs = make_shared<Node>();
        rhs->childType = Node::ChildType::R;
        rhs->type = Node::Type::VAL;
        rhs->value = (tree->value + 1) / 2;
        rhs->depth = tree->depth + 1;
        rhs->parent = tree;

        tree->value = -1;
        tree->type = Node::Type::PAIR;
        tree->left = lhs;
        tree->right = rhs;

        return true;
    }

    if (tree->type == Node::Type::PAIR)
    {
        bool treeChanged = SplitTreeRec(tree->left);
        if (!treeChanged)
        {
            treeChanged = SplitTreeRec(tree->right);
        }
        return treeChanged;
        return false;
    }
    else
    {
        return false;
    }
}

bool ReduceTreeRec(shared_ptr<Node> tree)
{
    bool changedTree = ExplodeTreeRec(tree);
    if (!changedTree)
    {
        changedTree = SplitTreeRec(tree);
    }
    return changedTree;
}

void ReduceTree(shared_ptr<Node> tree)
{
    bool treeChanged = true;
    while (treeChanged)
    {
        treeChanged = ReduceTreeRec(tree);
    }
}

shared_ptr<Node> Add(const shared_ptr<Node> lhs, const shared_ptr<Node> rhs)
{
    shared_ptr<Node> tree = PartialAdd(lhs, rhs);
    tree->SetDepth(0);
    ReduceTree(tree);
    return tree;
}

ull TreeMagnitude(shared_ptr<Node> tree)
{
    ull sum = 0;
    if (tree->type == Node::Type::VAL)
    {
        sum = tree->value;
    }
    if (tree->type == Node::Type::PAIR)
    {
        sum = 3 * TreeMagnitude(tree->left) + 2 * TreeMagnitude(tree->right);
    }
    return sum;
}

ull part1(const vector<shared_ptr<Node>> numbers)
{
    shared_ptr<Node> acc = CopyTree(numbers.at(0));
    shared_ptr<Node> x;
    for (size_t i = 1; i < numbers.size(); i++)
    {
        x = CopyTree(numbers.at(i));
        acc = Add(acc, x);
    }
    acc->SetDepth(0);
    ull res = TreeMagnitude(acc);
    DeleteTree(acc);
    return res;
}

ull part2(const vector<shared_ptr<Node>> numbers)
{
    ull maxSum = 0, sum = 0;

    for (size_t i = 0; i < numbers.size() - 1; i++)
    {
        for (size_t j = i + 1; j < numbers.size(); j++)
        {
            shared_ptr<Node> a, b, res;
            a = CopyTree(numbers.at(i));
            b = CopyTree(numbers.at(j));

            res = Add(a, b);
            sum = TreeMagnitude(res);
            if (sum > maxSum)
            {
                maxSum = sum;
            }

            res = Add(b, a);
            sum = TreeMagnitude(res);
            if (sum > maxSum)
            {
                maxSum = sum;
            }
            DeleteTree(a);
            DeleteTree(b);
            DeleteTree(res);
        }
    }

    return maxSum;
}

int main()
{
    vector<shared_ptr<Node>> nodes = ParseInput();
    cout<<"Part1:"<<part1(nodes)<<endl;
    cout<<"Part2:"<<part2(nodes)<<endl;

    for (size_t i = 0; i < nodes.size(); i++)
    {
        DeleteTree(nodes[i]);
    }
    nodes.clear();
    return 0;
}