# NFA-with-lambda-transitions

[![Build Status](https://travis-ci.org/marius92mc/NFA-with-lambda-transitions.svg?branch=master)](https://travis-ci.org/marius92mc/NFA-with-lambda-transitions)

Nondeterministic finite automaton - http://en.wikipedia.org/wiki/Nondeterministic_finite_automaton

Given a NFA that can also have lambda transitions, and a set of words, 
the program outputs whether or not those words are recognized by the automaton or not.


    The input is of the following form:

    N M
    q0
    Nf
    f1 f2 ... fn
    qi c qj
    ...
    n
    word1
    word2
    ...

    where:
    N       - number of states, vertices
    M       - number of transitions, edges
    Nf      - number of final states
    fi       - i = 1, ..., n  represents the final states
    qi c qj - represents a state, a transition, and the next state
    n       - number of words to test
    word    - a string

    Input example:
    5 9
    0
    2
    2 3
    0 a 4
    0 a 1
    0 0 1
    4 a 3
    1 0 3
    1 b 2
    2 b 3
    2 0 3
    3 0 1
    6
    abb
    abbb
    aa
    aab
    bbaa
    bbbbbbbbbbbbbbbbbbbb


    Output example:
    The word abb is accepted by the NFA.
    The word abbb is accepted by the NFA.
    The word aa is accepted by the NFA.
    The word aab is accepted by the NFA.
    The word bbaa is not accepted by the NFA.
    The word bbbbbbbbb is accepted by the NFA.

