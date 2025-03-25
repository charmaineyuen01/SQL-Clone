Project Overview

This project is a SQL Clone, emulating the core functionalities of a SQL database, using C++. It allows users to manage the database (obtain desired information, insert new data, update data, remove data) by inserting SQL-like commands. A variety of fundamental data structures and algorithms are used to handle data storage and manipulation. 

---

File Structure and Data Structures
- Binary files: Store and manage fixed-size records from binary files.
- B+ Tree: For indexing and efficient data storage and retrieving.
- Parser: Breaks string input into logical pieces to work with other structures.
- Queue: For sequencing SQL queries pieces.
- SQL: Core logic for SQL-like query execution.
- Shunting Yard: For converting SQL queries into a format that can be executed by the database engine.
- Table: For managing rows and columns of the database.
- Tokenizer: Breaks down SQL queries into tokens for easier processing.

---

Binary Files Overview
- student.bin: Binary files storing serialized student data.
- student.txt: Text files storing human-readable student data.
