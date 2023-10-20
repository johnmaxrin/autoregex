# Regular Expression to DFA Conversion

This repository contains code for converting regular expressions into deterministic finite automata (DFA) and using the resulting DFA to match patterns. The algorithm employed in this conversion is based on the well-known "Dragon Book" (Compilers Principles, Techniques and Tool by ALSU).

## Overview
This repository provides a C implementation of the regular expression to the DFA conversion process. It allows you to:

1. Convert regular expressions into equivalent DFAs.
2. Use the DFA to match patterns against input text efficiently.
3. Understand and implement the conversion algorithm specified in the "Dragon Book."

## How to Use
Clone this repository to your local machine.
```bash
git clone https://github.com/yourusername/regex-to-dfa-conversion.git
```
Next, Navigate to the project directory.
```bash
cd autoregex
```
Then build executables
```bash
make
```
Finally, For running the executable
```bash
./autoregex
```
## Suported Operations
OR ( **|** )<br>
Eg: a|b : either a or b can be matched

STAR ( **\*** )<br> 
Eg: a* : a, aa, aaa, etc can be matched

WILD (**.**)<br> 
Eg: a.b : acb, atb, etc can be matched

