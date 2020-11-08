# mipt-flat-2020-practice1

## What task it?
You need to write a program that, given a regular expression in inverted Polish notation, finds the length of the shortest word, which have *k* letters *x* in at 
the begining (prefix *x^k*).

## How to solve?
First of all, we need a stack of *nodes*. 
*Node* - is vector, where on m-th position we will put minimal length of word, what is OK for regexp and contains *k* letters *x* at the begining (prefix).
 By default, node contains *k + 1* *INF* elements.

Let's move from the first symbol to the last one by one. In given regular expression it's possible to find letters from *a, b, c* alphabet, *+*, *.* and *\**
symbols. 

* If next symbol is letter, let's push new node for word with one letter on stack. Shortest word with prefix *x*^0 is just a shortest word for current expression, for letter it's length is 1. If this letter is desired symbol, shortest word with prefix *x*^1 is this letter and it's length is 1.

* If next symbol is *+* (addition), let's get two top nodes (and pop it) from stack and Merge it. Result of merging is node, on each position of it is minimum of two elements from this position from popped states. Then push result state on stack.

* If next symbol is *.* (multiplication), let's concatenate two top nodes. How to find *'m-th'* element of resulting node? If we will find minimum of sums of 
pair, where the first element of pair is *'i-th'* element of first node, which is i (then word with prefix *x^i* for this expression is just *x^i*, then after 
concatenate we will have a word with prefix *x^(i + j)*, where j is length of second prefix) and another one is *(m - i) th* element of second one (if such word 
is exist) (result prefix will be >=*i*). Also, if *'m-th'* element's length larger then *m* (but not *INF*), it can be concatinate ehith each word from second 
node, and shortest length is in *'0-th'* element of this. Therefore, in nested loop we can find each element of resulting node.

* If next symbol is *\** (kleene_star), let's take top and pop the same. To find *'m-th'* element of resulting node we will find minimum of all *'i-th'* elements that 
have length *i* (they all from *x*), let's take them so many times to have a prefix >=*k* length (devision k to i with round up). The same as in multiplication
it can be just *'i-th'* element, if it is exist.

As a result, after all we will have in stack of result only one, *'final'* node, and the last element of this state is the answer. If such word is not exist, then
you will find *INF* message as a result of programe, at the other situation there will be the length of the shortest word with prefix *x^k* for this expretion.

If it is impossible to make an operation (stack contains not enough operands for it) or after reaching the end of expression stack is not empty, given expression is incorrect and you will find *ERROR* message as a result of programm.

## Complexity of solution
Each operation takes *O(k)* memory and at one moment at the worst case stack takes *O(k \* L)* memory, where *L* - length of given expression.

Operation of adding letter takes *O(k)* time. Addition takes *O(k)* time, multiplication and addition takes *O(k^2)* time, so at the worst case time complexity is
*O(L * k^2)*.
