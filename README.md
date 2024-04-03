# first-C-database


# Database Management System

This is a simple command-line database management system written in C. It allows users to manage records containing handles, follower counts, timestamps, and comments.

## Features

- **Add Record**: Add a new record to the database.
- **Update Record**: Update an existing record in the database.
- **List Records**: Display all records in the database.
- **Save Database**: Save the current state of the database to a CSV file.
- **Exit Program**: Exit the program, optionally force exiting without saving changes.

## Usage

1. **Compile**: Compile the program using a C compiler. For example:
   ```
   gcc -o database main.c database.c
   ```

2. **Run**: Execute the compiled program. For example:
   ```
   ./database
   ```

3. **Commands**:
   - `add HANDLE FOLLOWERS`: Add a new record with the specified handle and follower count.
   - `update HANDLE FOLLOWERS`: Update the follower count of the record with the specified handle.
   - `list`: Display all records in the database.
   - `save`: Save the database to a CSV file.
   - `exit`: Exit the program.
     - Use `exit fr` to force exit without saving changes.

## Requirements

- C Compiler (e.g., GCC)
- Standard C Libraries
- POSIX-compliant Operating System (for `getline` function)


---

You can customize this README file further based on your project's specific details and requirements.
