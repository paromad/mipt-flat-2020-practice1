#include <iostream>
#include <stack>
#include <vector>


using namespace std;


class Regular {
    typedef vector<size_t> Node;

    string regular_;
    stack<Node> stack_of_nodes_;
    char symbol_;
    size_t count_of_symbol_;
    size_t size_of_node_;

    void add_letter(char letter) {
        Node node_of_letter(size_of_node_, SIZE_MAX);
        node_of_letter[0] = 1;
        if (letter == symbol_) {
            node_of_letter[1] = 1;
        }
        stack_of_nodes_.push(node_of_letter);
    }

    Node take_node() {
        Node node = stack_of_nodes_.top();
        stack_of_nodes_.pop();
        return node;
    }

    void addition() {
        if (stack_of_nodes_.size() < 2) {
            throw logic_error("not enough arguments");
        }

        Node second_node = take_node();
        Node first_node = take_node();

        Node node_of_addition(size_of_node_);

        for (size_t i = 0; i <= count_of_symbol_; ++i) {
            node_of_addition[i] = min(first_node[i], second_node[i]);
        }

        stack_of_nodes_.push(node_of_addition);
    }

    void multiplication() {
        if (stack_of_nodes_.size() < 2) {
            throw logic_error("not enough arguments");
        }

        Node second_node = take_node();
        Node first_node = take_node();

        Node node_of_multiplication(size_of_node_, SIZE_MAX);

        node_of_multiplication[0] = first_node[0] + second_node[0];

        for (size_t i = 1; i < size_of_node_; ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (first_node[j] == j && second_node[i - j] != SIZE_MAX) {
                    node_of_multiplication[i] = min(node_of_multiplication[i], first_node[j] + second_node[i - j]);
                }
            }
            if (first_node[i] != SIZE_MAX) {
                node_of_multiplication[i] = min(node_of_multiplication[i], first_node[i] + second_node[0]);
            }
        }

        stack_of_nodes_.push(node_of_multiplication);
    }

    void kleene_star() {
        if (stack_of_nodes_.empty()) {
            throw logic_error("not enough arguments");
        }

        Node node = take_node();

        Node node_of_kleene_star(size_of_node_, SIZE_MAX);

        node_of_kleene_star[0] = 0;

        for (size_t i = 1; i < size_of_node_; ++i) {
            for (size_t j = 1; j < i; ++j) {
                if (node[j] == j) {
                    node_of_kleene_star[i] = min(node_of_kleene_star[i], ((i - 1) / j + 1) * j);
                }
            }
            node_of_kleene_star[i] = min(node_of_kleene_star[i], node[i]);
        }

        stack_of_nodes_.push(node_of_kleene_star);
    }

public:

    Regular() = delete;

    Regular(string regular, char symbol, size_t count_of_symbol) : regular_(std::move(regular)), symbol_(symbol),
                                                                   count_of_symbol_(count_of_symbol),
                                                                   size_of_node_(count_of_symbol + 1) {};

    void add_symbol(char symbol) {
        if (isalpha(symbol)) {
            add_letter(symbol);
            return;
        }
        switch (symbol) {
            case '+':
                addition();
                break;
            case '.':
                multiplication();
                break;
            case '*':
                kleene_star();
                break;
            default:
                throw logic_error("unexpected symbol");
        }
    }

    bool parsing_of_regular() {
        for (char symbol : regular_) {
            add_symbol(symbol);
        }

        return true;
    }

    string min_length() {
        try {
            parsing_of_regular();
        } catch (const logic_error &error) {
            return "ERROR";
        }

        Node result = take_node();

        if (!stack_of_nodes_.empty()) {
            return "ERROR";
        }

        int length = result[count_of_symbol_];

        if (length == SIZE_MAX) {
            return "INF";
        }

        return to_string(length);
    }
};


int main() {
    string reg;
    cin >> reg;

    char symbol;
    size_t count_of_symbol;
    cin >> symbol >> count_of_symbol;

    Regular regular(reg, symbol, count_of_symbol);

    cout << regular.min_length();

    return 0;
}


//ab+c.aba.*.bac.+.+* c 4

//acb..bab.c.*.ab.ba.+.+*a. b 2