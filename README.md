# Cash Exchange Optimizer

This project is a cash exchange optimizer written in C. It is designed to simplify the process of settling debts within a group of people. The program takes as input the transactions that have occurred and outputs the optimal way to settle all debts, minimizing the number of transactions needed.

## Features

- Supports both individual and group payments.
- Allows for debts to be split equally, by amount, or by percentage.
- Optimizes the number of transactions needed to settle all debts.

## How to Use

1. Compile the C file using a C compiler.
2. Run the compiled file.
3. Follow the prompts in the console to input the transactions.
4. The program will output the optimal way to settle all debts.

## Code Structure

The code uses a HashMap to store the debts of each individual. The HashMap is implemented using an array of key-value pairs, where the key is the name of the individual and the value is their current debt. The HashMap is dynamically resized as needed.

The main functions in the code are:

- `initHashMap()`: Initializes the HashMap.
- `put()`: Adds a key-value pair to the HashMap.
- `max()`: Finds the key with the maximum value in the HashMap.
- `min()`: Finds the key with the minimum value in the HashMap.
- `findPath()`: Finds the optimal way to settle all debts.
- `addDebt()`: Adds a debt to the HashMap.
