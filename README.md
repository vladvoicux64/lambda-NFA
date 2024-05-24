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

## The `CFG` class

The `CFG` implements context-free grammars. It takes grammars in CNF as an input and provides a method to verify inputs.
The `CFG` class provides 2 methods that will be described below:

- `CFG(1)`: this method takes a `std::unordered_map<std::string, std::vector<std::string>>` and initializes the grammar
  with states with productions, mapping variables to vectors of rules (i.e. S -> AB | CD would
  be `{{"S", {"AB", "CD"}}}`);
- `verify_word(1)`: this method takes a `std::string` and returns a boolean value representing whether the word is
  accepted or not;

Usage is similar to other classes in this project, needing an initialized object from which the method for verifying
words is called.

## The `PDA` class

This class is a copy of the `LNFA` class, with slight changes to accommodate for the stack.

The `PDA` class provides 6 methods that will be described below:

- `add_states(1)`: this method takes a `std::vector<int>` and creates states with those respective IDs, binding them to
  the automata object from where it is called;
- `add_arcs(1)`: this method takes a `std::vector<std::tuple<int, int, char, std::pair<char, std::string>>>` and creates
  arcs from the respective tuples, treating their members as <source, destination, transition character, <stack pop
  character, stack push word> - for empty pop/push operations on the stack use `_`;
- `set_initial_state_id(1)`: this method takes an `int` and sets the state with that ID as the initial state;
- `set_final_states(1)`: this method takes a `std::vector<int>` and sets the states with those respective IDs as final
  states;
- `test_acceptance(1)`: this method takes a `std::string` and returns a boolean value, `true` if the word is accepted by
  the automata and `false` otherwise;
- `set_lambda(1)`: this method sets what character is used to represent lambda when reading the inputs ('_' is used by
  default);