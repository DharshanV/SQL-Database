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
1. User input gets passed to parser.
2. Parse tree (MMap of string and string) is created by state machine from the designed grammer and command line interpretation. 
3. From the parse tree, the command type is determined i.e. create (make), insert, and select. Also if the command is valid.
   It also identifes what the fields and values of each command will be from the state machine.
4. If its valid, then from this point the input has been broken down and has been identifed. If not user notify for unrecogined command.
5. If create then use the tree to make binary files for actual data and text files for fields.
6. If insert then open the binary files, create a record object to write back.
7. If select, it falls under three categories:
   1. Select all - selects all gets the field values from the tree, and reads all the data into record. 
   2. Select fields - same as select all, but specified fields.


## Grammer
<pre>
&lt;CREATE | MAKE&gt; : {  &lt;create | make&gt; table &lt;TABLE_NAME&gt; fields &lt;FIELD_NAME&gt; [, &lt;FIELD_NAME&gt;...]  }
&lt;INSERT&gt; : { insert &lt;INTO&gt; &lt;TABLE_NAME&gt; values &lt;VALUE&gt; [, &lt;VALUE&gt;...]      }
&lt;SELECT&gt; : {  select &lt;* | FIELD_NAME&gt; [, ,FIELD_NAME&gt;...]
					from &lt;TABLE_NAME&gt;
					where &lt;FIELD_NAME&gt; &lt;RELATIONAL_OPERATOR&gt; &lt;VALUE&gt;
						[&lt;LOGICAL_OPERATOR&gt;
							&lt;FIELD_NAME&gt; &lt;RELATIONAL_OPERATOR&gt; &lt;VALUE&gt;...]
          }

&lt;VALUE&gt;  : A string of alphanumeric characters, or a string of alphanumeric
 			characters and spaces enclosed by double quotation marks:
 			"Jean Luise", Finch, 1923
&lt;RELATIONAL OPERATOR&gt; : [ = | &gt; | &lt; | &gt;= | &lt;= ]
&lt;LOGICAL OPERATOR&gt;    : [and | or]
  </pre>
