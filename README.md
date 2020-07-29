# Nondeterministic Turing Machine Simulator
Algorithms and Data Structures - Politecnico di Milano <br/>
Academic Year: 2017/2018
# General Overview
The following source code simulates a non-deterministic single tape acceptor Turing Machine. <br/>

The solution has been implemented exploiting the Breadth-First Search algorithm, representing the tapes of the Turing Machine as dynamic arrays. It uses a queue implemented through a linked list to store the possible transitions of the machine's computation tree at each machine step.
# Input
The input is characterized by 4 parts: 
1. The first part, preceded by ```tr```, contains the list of transitions, one on each line (e.g: ```0 a b R 1``` indicates that this transition starts from state ```0``` and goes to state ```1``` reading ```a``` on the tape, writing ```b``` and moving the head on the right).
2. The next part, starting with ```acc```, contains the list of acceptation states, one on each line.
3. To avoid endless computations, after the word ```max```, a maximum number of computational steps is provided.
4. After the word ```run``` there's the list of strings to be computed, separated by a new line.
# Output
For every input string, one of the following outputs will be provided:
1. If the machine reaches an acceptance state, the output for that string will be ```1```
2. If the computation ends without reaching an acceptation state, the machine will give a ```0``` as a result
3. If the computation does not reach an acceptation state within the maximum number of computational steps, ```U``` will be the result.
# Example
Here is given an example of Turing Machine that accepts strings composed by a substring and the same substring written from the last letter to the first one, for example `bbaabb`:  
```  
tr  
0 a a R 0  
0 b b R 0  
0 a c R 1  
0 b c R 2  
1 a c L 3  
2 b c L 3  
3 c c L 3  
3 a c R 4  
3 b c R 5  
4 c c R 4  
4 a c L 3  
5 c c R 5  
5 b c L 3  
3 _ _ R 6  
6 c c R 6  
6 _ _ S 7  
acc  
7  
max  
800  
run  
aababbabaa  
aababbabaaaababbabaa  
aababbabaaaababbabaab  
aababbabaaaababbabaabbaababbabaaaababbabaa  
aababbabbaaababbabaabbaababbabaaaababbabaa  
```
The relative output will be:
```
1  
1  
0  
U  
0
```
