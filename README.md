# Description

This project presents a simple lambda-NFA implementation. It accepts alphabets composed of printable characters. It uses an OOP approach, based on a `State` class and a `LNFA` class.  
The `State` class provides a graph node type of object, that holds metadata composed of:  
- the position in the word at which the node was visited;  
- a boolean value that indicates if the node is a final state in the automata (false by default);   

The `LNFA` class ties multiple `State` objects togheter and provides functionality for setting their attributes and testing the acceptance of words.  

# Usage

## The `LNFA` class

The user is supposed to only interact with the `LNFA` class.  
The `LNFA` class provides 6 methods that will be described below:  
- `add_states(1)`: this method takes a `std::vector<int>` and creates states with those respective IDs, binding them to the automata object from where it is called;  
- `add_arcs(1)`: this method takes a `std::vector<std:;tuple<int, int, char>>` and creates arcs from the respective tuples, treating their members as <source, destination, transition character>;  
- `set_initial_state_id(1)`: this method takes an `int` and sets the state with that ID as the initial state;  
- `set_final_states(1)`: this method takes a `std::vector<int>` and sets the states with those respective IDs as final states;  
- `test_acceptance(1)`: this method takes a `std::string` and returns a boolean value, `true` if the word is accepted by the automata and `false` otherwise;  
- `set_lambda(1)`: this method sets what character is used to represent lambda when reading the inputs ('_' is used by default);  

To use the `LNFA` class you must construct an object of said type and then use the `add_states(1)`, `add_arcs(1)`, `set_initial_state_id(1)`, `set_final_states(1)` methods, preferably in this order. Optionally, you may set the lambda character to something else than the default, using the `set_lambda(1)` method.
Afterwards, call the `test_acceptance(1)` method as many times as desired.

## Provided test environment

I've provided a `main.cpp` file that tests the functionality. To use it, you must provide an `input.txt` file with the following structure:
```
<number of states - integer>
<state IDs - whitespace separated integers>
<number of transitions - integer>  
<transitions - sequences of "integer integer character" (without quotes) separated by whitespace>
<initial state ID - integer>  
<number of final states - integer>  
<final state IDs - whitespace separated integers>  
<number of words to test - integer>  
<words to test - whitespace separated strings, use '_' as lambda character>
```
Note: to test the empty word, leave a blank line after all the non-blank words, remembering to also count the blank word when providing the number of words to test;
