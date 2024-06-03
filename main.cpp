#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <stdexcept>

using namespace std;

// TreeNode and Stack classes remain the same
struct TreeNode {
    string data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

class Stack {
private:
    struct Node {
        TreeNode* treeNode;
        Node* next;

        Node(TreeNode* tn) {
            treeNode = tn;
            next = nullptr;
        }
    };
    Node* topNode;

public:
    Stack() {
        topNode = nullptr;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(TreeNode* treeNode) {
        Node* newNode = new Node(treeNode);
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (topNode != nullptr) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    TreeNode* top() {
        if (topNode != nullptr) {
            return topNode->treeNode;
        }
        return nullptr;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }
};

class PostfixToInfixCalculator {
public:
    string postfixToInfix(const string& postfix) {
        Stack s;
        for (char c : postfix) {
            if (isOperator(c)) {
                if (s.isEmpty()) throw invalid_argument("Invalid postfix expression");
                TreeNode* operand2 = s.top();
                s.pop();
                if (s.isEmpty()) throw invalid_argument("Invalid postfix expression");
                TreeNode* operand1 = s.top();
                s.pop();
                TreeNode* newNode = new TreeNode("(" + operand1->data + " " + c + " " + operand2->data + ")");
                newNode->left = operand1;
                newNode->right = operand2;
                s.push(newNode);
            } else if (isspace(c)) {
                continue;
            } else if (isalnum(c)) {
                s.push(new TreeNode(string(1, c)));
            } else {
                throw invalid_argument("Invalid character in postfix expression");
            }
        }
        if (s.isEmpty() || s.top()->left == nullptr || s.top()->right == nullptr)
            throw invalid_argument("Invalid postfix expression");
        return s.top()->data;
    }

private:
    bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }
};

class InfixToPostfixCalculator {
public:
    string infixToPostfix(const string& infix) {
        stack<char> s;
        string postfix;
        for (char c : infix) {
            if (isalnum(c)) {
                postfix += c;
            } else if (c == '(') {
                s.push(c);
            } else if (c == ')') {
                while (!s.empty() && s.top() != '(') {
                    postfix += s.top();
                    s.pop();
                }
                if (!s.empty()) s.pop();
            } else if (isOperator(c)) {
                while (!s.empty() && precedence(s.top()) >= precedence(c)) {
                    postfix += s.top();
                    s.pop();
                }
                s.push(c);
            } else if (isspace(c)) {
                continue;
            } else {
                throw invalid_argument("Invalid character in infix expression");
            }
        }
        while (!s.empty()) {
            postfix += s.top();
            s.pop();
        }
        return postfix;
    }

private:
    bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    int precedence(char c) {
        if (c == '+' || c == '-')
            return 1;
        if (c == '*' || c == '/')
            return 2;
        return 0;
    }
};

int main() {
    string expression;
    string conversionType;

    cout << "Enter the expression: ";
    getline(cin, expression);

    cout << "Enter the conversion type (Postfix to Infix / Infix to Postfix): ";
    getline(cin, conversionType);

    try {
        string result;

        if (conversionType == "Postfix to Infix") {
            PostfixToInfixCalculator calculator;
            result = calculator.postfixToInfix(expression);
        } else if (conversionType == "Infix to Postfix") {
            InfixToPostfixCalculator calculator;
            result = calculator.infixToPostfix(expression);
        } else {
            throw invalid_argument("Invalid conversion type");
        }

        cout << "Converted expression: " << result << endl;
    } catch (const std::exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
