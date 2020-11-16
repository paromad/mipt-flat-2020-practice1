#include <iostream>
#include <stack>
#include <vector>


class Regular {
    typedef std::vector<size_t> Node;
    const size_t INF = std::numeric_limits<size_t>::max();

    std::string regular_;
    std::stack<Node> stack_of_nodes_;
    char symbol_;
    size_t count_of_symbol_;
    size_t size_of_node_;

    void addOne() {
        Node node_of_one(size_of_node_, INF);
        node_of_one[0] = 0;
        stack_of_nodes_.push(std::move(node_of_one));
    }

    void addLetter(char letter) {
        Node node_of_letter(size_of_node_, INF);
        node_of_letter[0] = 1;
        if (letter == symbol_) {
            node_of_letter[1] = 1;
        }
        stack_of_nodes_.push(std::move(node_of_letter));
    }

    Node takeNode() {
        Node node = stack_of_nodes_.top();
        stack_of_nodes_.pop();
        return node;
    }

    void applyAdd() {
        if (stack_of_nodes_.size() < 2) {
            throw std::logic_error("not enough arguments");
        }

        Node second_node = takeNode();
        Node first_node = takeNode();

        for (size_t i = 0; i <= count_of_symbol_; ++i) {
            first_node[i] = std::min(first_node[i], second_node[i]);
        }

        stack_of_nodes_.push(std::move(first_node));
    }

    void applyMultiply() {
        if (stack_of_nodes_.size() < 2) {
            throw std::logic_error("not enough arguments");
        }

        Node second_node = takeNode();
        Node first_node = takeNode();

        Node node_of_multiplication(size_of_node_, INF);

        node_of_multiplication[0] = first_node[0] + second_node[0];

        for (size_t i = 1; i < size_of_node_; ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (first_node[j] == j && second_node[i - j] != INF) {
                    node_of_multiplication[i] = std::min(node_of_multiplication[i], first_node[j] + second_node[i - j]);
                }
            }
            if (first_node[i] != INF) {
                node_of_multiplication[i] = std::min(node_of_multiplication[i], first_node[i] + second_node[0]);
            }
        }

        stack_of_nodes_.push(std::move(node_of_multiplication));
    }

    void applyKleeneStar() {
        if (stack_of_nodes_.empty()) {
            throw std::logic_error("not enough arguments");
        }

        Node node = takeNode();

        Node node_of_kleene_star(size_of_node_, INF);

        node_of_kleene_star[0] = 0;

        for (size_t i = 1; i < size_of_node_; ++i) {
            for (size_t j = 1; j < i; ++j) {
                if (node[j] == j) {
                    node_of_kleene_star[i] = std::min(node_of_kleene_star[i], ((i - 1) / j + 1) * j);
                    node_of_kleene_star[i] = std::min(node_of_kleene_star[i], ((i - 1) / j) * j + node_of_kleene_star[i % j]);
                }
            }
            node_of_kleene_star[i] = std::min(node_of_kleene_star[i], node[i]);
        }

        stack_of_nodes_.push(node_of_kleene_star);
    }

public:

    Regular() = delete;

    Regular(std::string regular, char symbol, size_t count_of_symbol) : regular_(std::move(regular)), symbol_(symbol),
                                                                        count_of_symbol_(count_of_symbol),
                                                                        size_of_node_(count_of_symbol + 1) {};

    void addSymbol(char symbol) {
        if (isalpha(symbol)) {
            addLetter(symbol);
            return;
        }
        switch (symbol) {
            case '1':
                addOne();
                break;
            case '+':
                applyAdd();
                break;
            case '.':
                applyMultiply();
                break;
            case '*':
                applyKleeneStar();
                break;
            default:
                throw std::logic_error("unexpected symbol");
        }
    }

    bool parsingOfRegular() {
        for (char symbol : regular_) {
            addSymbol(symbol);
        }

        if (stack_of_nodes_.size() != 1) {
            throw std::logic_error("too many arguments");
        }

        return true;
    }

    std::string minLength() {
        try {
            parsingOfRegular();
        } catch (const std::logic_error &error) {
            return "ERROR";
        }

        Node result = takeNode();

        int length = result[count_of_symbol_];

        if (length == SIZE_MAX) {
            return "INF";
        }

        return std::to_string(length);
    }
};


int main() {
    std::string reg;
    std::cin >> reg;

    char symbol;
    size_t count_of_symbol;
    std::cin >> symbol >> count_of_symbol;

    Regular regular(reg, symbol, count_of_symbol);

    std::cout << regular.minLength();

    return 0;
}


//ab+c.aba.*.bac.+.+* c 4

//acb..bab.c.*.ab.ba.+.+*a. b 2