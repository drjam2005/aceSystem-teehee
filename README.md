
# 📚 Library Management System CSDC FINALS


## SIGMA SIGMA BOY!!!
# SIGMA BOY

A lightweight console-based library management system written in C++ that handles book and user management with file persistence.

## ✨ Features

- **Book Management**
  - Add new books with title, author, and ISBN
  - Track book availability status
  - View all books in the library

- **User Management**
  - Register new library users
  - Track borrowed books per user
  - View all registered users

- **Borrowing System**
  - Borrow available books
  - Prevent borrowing of unavailable books

- **Data Persistence**
  - Automatic saving to text files
  - Data loads on program start
  - Simple human-readable storage format

## 🖥️ Usage

After launching the program, you'll see a simple menu interface:

```
Library Menu:
1. Add Book
2. Add User
3. Borrow Book
4. Show All Books
5. Show All Users
0. Exit
Enter choice: 
```

## 📂 Project Structure

```
library-management-system/
├── library.cpp        # Main program source file
├── books.txt          # Database of books (auto-generated)
├── users.txt          # Database of users (auto-generated)
└── README.md          # This documentation
```

## 📝 Data Storage Format

The system uses simple text files for data persistence:

**books.txt**
```
Book Title 1
Author Name 1
ISBN123
1
Book Title 2
Author Name 2
ISBN456
0
```

**users.txt**
```
John Doe
001
2
ISBN123
ISBN456
Jane Smith
002
0
```


