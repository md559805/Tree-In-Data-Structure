#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <cctype>
#include <vector>
#include <cmath>

using namespace std;

struct Node
{
    string data;
    Node *left;
    Node *right;
    Node(string val) : data(val), left(nullptr), right(nullptr) {}
};

// ---------- Operator Utilities ----------

bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int priority(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

// ---------- Build Expression Tree ----------

Node *buildExpressionTree(const string &exp)
{
    stack<Node *> nodes;
    stack<char> ops;

    for (size_t i = 0; i < exp.length(); i++)
    {
        if (isspace(exp[i]))
            continue;

        // Operand
        if (isalnum(exp[i]))
        {
            string val;
            while (i < exp.length() && (isalnum(exp[i]) || exp[i] == '.'))
            {
                val += exp[i++];
            }
            i--;
            nodes.push(new Node(val));
        }
        // (
        else if (exp[i] == '(')
        {
            ops.push(exp[i]);
        }
        // )
        else if (exp[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                Node *t = new Node(string(1, ops.top()));
                ops.pop();

                t->right = nodes.top();
                nodes.pop();
                t->left = nodes.top();
                nodes.pop();
                nodes.push(t);
            }
            ops.pop(); // remove '('
        }
        // Operator
        else if (isOperator(exp[i]))
        {
            while (!ops.empty() &&
                   (priority(ops.top()) > priority(exp[i]) ||
                    (priority(ops.top()) == priority(exp[i]) && exp[i] != '^')))
            {

                Node *t = new Node(string(1, ops.top()));
                ops.pop();

                t->right = nodes.top();
                nodes.pop();
                t->left = nodes.top();
                nodes.pop();
                nodes.push(t);
            }
            ops.push(exp[i]);
        }
    }

    while (!ops.empty())
    {
        Node *t = new Node(string(1, ops.top()));
        ops.pop();

        t->right = nodes.top();
        nodes.pop();
        t->left = nodes.top();
        nodes.pop();
        nodes.push(t);
    }

    return nodes.top();
}

// ---------- Traversals ----------

void levelOrder(Node *root)
{
    if (!root)
        return;
    queue<Node *> q;
    q.push(root);
    while (!q.empty())
    {
        Node *cur = q.front();
        q.pop();
        cout << cur->data << " ";
        if (cur->left)
            q.push(cur->left);
        if (cur->right)
            q.push(cur->right);
    }
}

void preorder(Node *root)
{
    if (!root)
        return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node *root)
{
    if (!root)
        return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

void postorder(Node *root)
{
    if (!root)
        return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}

void RNL(Node *root)
{
    if (!root)
        return;
    RNL(root->right);
    cout << root->data << " ";
    RNL(root->left);
}

void RLN(Node *root)
{
    if (!root)
        return;
    RLN(root->right);
    RLN(root->left);
    cout << root->data << " ";
}

// ---------- Vertical Tree Printing ----------

int treeHeight(Node *root)
{
    if (!root)
        return 0;
    return 1 + max(treeHeight(root->left), treeHeight(root->right));
}

void printLevel(vector<Node *> nodes, int level, int maxLevel)
{
    int floor = maxLevel - level;
    int edgeLines = (int)pow(2, max(floor - 1, 0));
    int firstSpaces = (int)pow(2, floor) - 1;
    int betweenSpaces = (int)pow(2, floor + 1) - 1;

    for (int i = 0; i < firstSpaces; i++)
        cout << " ";

    vector<Node *> newNodes;
    for (Node *node : nodes)
    {
        if (node)
        {
            cout << node->data;
            newNodes.push_back(node->left);
            newNodes.push_back(node->right);
        }
        else
        {
            cout << " ";
            newNodes.push_back(nullptr);
            newNodes.push_back(nullptr);
        }
        for (int i = 0; i < betweenSpaces; i++)
            cout << " ";
    }
    cout << endl;

    for (int i = 1; i <= edgeLines; i++)
    {
        for (size_t j = 0; j < nodes.size(); j++)
        {
            for (int k = 0; k < firstSpaces - i; k++)
                cout << " ";

            if (!nodes[j])
            {
                for (int k = 0; k < edgeLines * 2 + i + 1; k++)
                    cout << " ";
                continue;
            }

            cout << (nodes[j]->left ? "/" : " ");
            for (int k = 0; k < i * 2 - 1; k++)
                cout << " ";
            cout << (nodes[j]->right ? "\\" : " ");

            for (int k = 0; k < edgeLines * 2 - i; k++)
                cout << " ";
        }
        cout << endl;
    }

    if (level < maxLevel)
        printLevel(newNodes, level + 1, maxLevel);
}

void printTreeVertical(Node *root)
{
    int h = treeHeight(root);
    vector<Node *> nodes;
    nodes.push_back(root);
    printLevel(nodes, 1, h);
}

// ---------- Main ----------

int main()
{
    string input;
    cout << "Enter expression: ";
    getline(cin, input);

    Node *root = buildExpressionTree(input);

    cout << "\n1. Level Order: ";
    levelOrder(root);
    cout << "\n2. Preorder (NLR): ";
    preorder(root);
    cout << "\n3. Inorder (LNR): ";
    inorder(root);
    cout << "\n4. Postorder (LRN): ";
    postorder(root);
    cout << "\n5. RNL: ";
    RNL(root);
    cout << "\n6. RLN: ";
    RLN(root);

    cout << "\n\nExpression Tree (Vertical View):\n";
    printTreeVertical(root);

    return 0;
}
