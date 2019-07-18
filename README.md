# SQL-Database

## Introduction
  Building a SQL like database from scratch. 
  
## Program Library Hierarchy
<pre>
├── SQL
| ├── Table
| | ├── MMap
| | | ├── BPlusTree
| | | | ├── Array Utilities
| | | ├── MMPair
| | ├── Map
| | | ├── BPlusTree
| | | | ├── Array Utilities
| | | ├── MPair
| | ├── Stack
| | | ├── List
| | ├── Queue
| | | ├── Linked List
| | ├── Record
| ├── STokenizer
| | ├── Token
| | ├── State Machine
| ├── Parser
| | ├── MMap
| | ├── Map
| | ├── STokenizer
| | ├── FStateMachine
</pre>

## Purpose of major library
1. SQL - Orchestrate all the pieces. From the user input -> processing -> read/write to file -> output display.
2. Parser - Parsers the user input, validation, and creating the parse tree. 
3. Table - Manage all the tables that have been created from previous sessions and new tables from current session.

## Journey of input
