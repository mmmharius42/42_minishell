#!/bin/bash

echo "Running output redirection tests..."

# Tester les redirections de sortie simple
echo "Test 1: ls > out1.txt"
echo "ls > out1.txt" | ./minishell
cat out1.txt
echo "----------------"

# Tester les redirections de sortie avec append
echo "Test 2: echo 'append test' >> out1.txt"
echo "echo 'append test' >> out1.txt" | ./minishell
cat out1.txt
echo "----------------"

# Tester les redirections avec pipes
echo "Test 3: ls | grep .c > out2.txt"
echo "ls | grep .c > out2.txt" | ./minishell
cat out2.txt
echo "----------------"

# Nettoyage
rm -f out1.txt out2.txt

echo "Tests completed."