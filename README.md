# Description

This project presents a simple lambda-NFA implementation. It accepts alphabets composed of printable characters. It uses
an OOP approach, based on a `State` class and a `LNFA` class.  
The `State` class provides a graph node type of object, that holds metadata composed of:  
- the position in the word at which the node was visited;  
- a boolean value that indicates if the node is a final state in the automata (false by default);   

The `LNFA` class ties multiple `State` objects togheter and provides functionality for setting their attributes and
testing the acceptance of words.

# Usage

## The `LNFA` class

The user is supposed to only interact with the `LNFA` class.  
The `LNFA` class provides 7 methods that will be described below:

- `add_states(1)`: this method takes a `std::vector<int>` and creates states with those respective IDs, binding them to the automata object from where it is called;  
- `add_arcs(1)`: this method takes a `std::vector<std:;tuple<int, int, char>>` and creates arcs from the respective tuples, treating their members as <source, destination, transition character>;  
- `set_initial_state_id(1)`: this method takes an `int` and sets the state with that ID as the initial state;  
- `set_final_states(1)`: this method takes a `std::vector<int>` and sets the states with those respective IDs as final states;  
- `test_acceptance(1)`: this method takes a `std::string` and returns a boolean value, `true` if the word is accepted by the automata and `false` otherwise;  
- `set_lambda(1)`: this method sets what character is used to represent lambda when reading the inputs ('_' is used by default);
- `get_type(0)`: this method returns an enum value representing the type of the automaton, 0, 1 and 2 for deterministic,
  nondeterministic, lambda-nondeterministic respectively;

To use the `LNFA` class you must construct an object of said type and then use
the `add_states(1)`, `add_arcs(1)`, `set_initial_state_id(1)`, `set_final_states(1)` methods, preferably in this order.
Optionally, you may set the lambda character to something else than the default, using the `set_lambda(1)` method.
Afterwards, call the `test_acceptance(1)` method as many times as desired.

The `LNFA` class does automatic type inference, meaning it automatically detects its type at construction. For this
mechanic to work properly, either the lambda character must not be changed, or changed before adding any arcs.

### Transforming NFA to DFA

The `nfa2dfa(1)` function takes a `LNFA` object of strict nondeterministic type and returns an equivalent, deterministic
automaton.

## The `S_expression` class

This class provides an interface for using RegEx. The name was chosen because it's intended purpose is to take a RegEx
composed of alphanumeric characters and the `*|+_` special character set and transform it into an S-expression form for
later use.
The special characters have the following meanings:

- `*` starring an expression means any (or none) occurrences of the expression;
- `|` expression union;
- `+` expression concatenation, can be omitted;
- `_` lambda character;

To use, instantiate an object of the class, passing a valid RegEx to the constructor.
Then, use the `get_expr_str(0)` or `get_expr_vector(0)` to receive the S-expression as a string or as a token vector.

### Building `LNFA` objects from S-expressions

The purpouse of this class is to generate the S-expressions needed to be able to build an automaton from the RegEx, in
order to use it.
To do so, use the `build_from_S_expr(1)` function, passing the token vector form of the S-expression to it. The return
value will be an automaton for the RegEx that the S-expression represents.

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

## End notes

This project probably received more attention from me than it deserved. Mostly because it has been a great way to dive
deeper into a new subject, which I would have otherwise left unexplored.
It also contains lazy, incomplete, or straight up smelly code. That is a direct result of me being lazy, although I may
argue over my choices (such as not using polymorphism between the 3 automaton types, because other than some conditions
on the content, the structure stays the same). If by any chance you get to reach the end of this README and then have
enough
fucks to give left to look over the code, and can provide (preferably harsh, but constructive) feedback, please do
either via an issue or contact channel you may find on my profile. Thanks for your attention.