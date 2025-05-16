#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

/**
 * Represents a book in the library system.
 * Stores book details and availability status.
 */
class Book {
public:
    string title;     // Title of the book
    string author;    // Author of the book
    string ISBN;      // Unique International Standard Book Number
    bool available;   // Availability status (true if available to borrow)

    /**
     * Constructor to initialize a Book object.
     * parameter t Title of the book
     * parameter a Author of the book
     * parameter i ISBN of the book
     * Books are available by default when created
     */
    Book(string t, string a, string i) 
        : title(t), author(a), ISBN(i), available(true) {}
};

/**
 * Represents a library user/member.
 * Tracks user information and borrowed books.
 */
class User {
public:
    string name;                // Full name of the user
    string ID;                  // Unique user identification
    vector<string> borrowedBooks; // List of ISBNs of borrowed books

    /**
     * Constructor to initialize a User object.
     * @param n Name of the user
     * @param id Unique user ID
     * Borrowed books list is empty by default
     */
    User(string n, string id) : name(n), ID(id) {}
};

/**
 * Main library management system class.
 * Handles all library operations and data persistence.
 */
class Library {
private:
    vector<Book> books;  // Database of all books in the library
    vector<User> users;  // Database of all registered users

    /**
     * Saves all library data to text files for persistence.
     * Creates/overwrites two files:
     * - books.txt: Stores all book records
     * - users.txt: Stores all user records with their borrowed books
     * Called automatically when program exits (via destructor)
     */
    void saveData() {
        // Save books data (title, author, ISBN, availability)
        ofstream bookFile("books.txt");
        for (auto& book : books) {
            bookFile << book.title << endl
                    << book.author << endl
                    << book.ISBN << endl
                    << book.available << endl;
        }
        bookFile.close();

        // Save users data (name, ID) and their borrowed books
        ofstream userFile("users.txt");
        for (auto& user : users) {
            userFile << user.name << endl
                    << user.ID << endl
                    << user.borrowedBooks.size() << endl; // Store count first
            for (auto& isbn : user.borrowedBooks) {
                userFile << isbn << endl; // Store each borrowed book's ISBN
            }
        }
        userFile.close();
    }

    /**
     * Loads library data from text files at program start.
     * Attempts to read from:
     * - books.txt: Restores book database
     * - users.txt: Restores user database with their borrowed books
     * Silently continues if files don't exist (first run scenario)
     */
    void loadData() {
        // Load books data
        ifstream bookFile("books.txt");
        if (bookFile) {
            string title, author, ISBN;
            bool available;
            while (getline(bookFile, title)) {
                getline(bookFile, author);
                getline(bookFile, ISBN);
                bookFile >> available;
                bookFile.ignore(); // Clear newline after bool read
                books.push_back(Book(title, author, ISBN));
                books.back().available = available; // Set correct availability
            }
            bookFile.close();
        }

        // Load users data and their borrowed books
        ifstream userFile("users.txt");
        if (userFile) {
            string name, ID, isbn;
            int bookCount;
            while (getline(userFile, name)) {
                getline(userFile, ID);
                userFile >> bookCount;
                userFile.ignore(); // Clear newline after int read
                users.push_back(User(name, ID));
                for (int i = 0; i < bookCount; i++) {
                    getline(userFile, isbn);
                    users.back().borrowedBooks.push_back(isbn);
                }
            }
            userFile.close();
        }
    }

public:
    /**
     * Constructor - automatically loads saved data when Library object is created
     */
    Library() { loadData(); }

    /**
     * Destructor - automatically saves data when Library object is destroyed
     */
    ~Library() { saveData(); }

    /**
     * Adds a new book to the library collection.
     * Prompts user for book details and creates new Book object.
     * Book is set as available by default.
     */
    void addBook() {
        string title, author, ISBN;
        cout << "Enter book title: ";
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        getline(cin, ISBN);
        books.push_back(Book(title, author, ISBN));
        cout << "Book added successfully!\n";
    }

    /**
     * Registers a new library user.
     * Prompts for user details and creates new User object.
     */
    void addUser() {
        string name, ID;
        cout << "Enter user name: ";
        getline(cin, name);
        cout << "Enter user ID: ";
        getline(cin, ID);
        users.push_back(User(name, ID));
        cout << "User registered successfully!\n";
    }

    /**
     * Handles book borrowing process.
     * Checks:
     * 1. If book exists and is available
     * 2. If user exists
     * Updates both book and user records if successful.
     */
    void borrowBook() {
        string ISBN, userID;
        cout << "Enter book ISBN: ";
        getline(cin, ISBN);
        cout << "Enter user ID: ";
        getline(cin, userID);

        // Search for the book
        for (auto& book : books) {
            if (book.ISBN == ISBN && book.available) {
                // Search for the user
                for (auto& user : users) {
                    if (user.ID == userID) {
                        // Update records
                        book.available = false;
                        user.borrowedBooks.push_back(ISBN);
                        cout << "Book successfully borrowed!\n";
                        return;
                    }
                }
                cout << "Error: User not found!\n";
                return;
            }
        }
        cout << "Error: Book not available or not found!\n";
    }

    /**
     * Displays all books in the library.
     * Shows title, author and availability status.
     * Handles empty library case.
     */
    void showBooks() {
        if (books.empty()) {
            cout << "The library currently has no books.\n";
            return;
        }
        
        cout << "\nLibrary Book Collection:\n";
        for (auto& book : books) {
            cout << "- " << book.title << " by " << book.author 
                 << " [" << (book.available ? "AVAILABLE" : "BORROWED") << "]\n";
        }
    }

    /**
     * Displays all registered users and their borrowed books.
     * Shows user name, ID and list of borrowed books (if any).
     * Handles empty user database case gracefully.
     */
    void showUsers() {
        if (users.empty()) {
            cout << "No users currently registered.\n";
            return;
        }
        
        cout << "\nRegistered Users:\n";
        for (auto& user : users) {
            cout << "- " << user.name << " (ID: " << user.ID << ")\n";
            if (!user.borrowedBooks.empty()) {
                cout << "  Borrowed books: ";
                for (auto& isbn : user.borrowedBooks) {
                    cout << isbn << " ";
                }
                cout << endl;
            }
        }
    }

    /**
     * Searches books by title, author or ISBN.
     * Performs partial matching (checks if query is contained in any field).
     * Displays all matching books with their availability status.
     */
    void searchBooks() {
        string query;
        cout << "Enter search term (title/author/ISBN): ";
        getline(cin, query);
        
        bool found = false;
        cout << "\nSearch Results:\n";
        for (auto& book : books) {
            if (book.title.find(query) != string::npos || 
                book.author.find(query) != string::npos || 
                book.ISBN.find(query) != string::npos) {
                cout << "- " << book.title << " by " << book.author  
                     << " [" << (book.available ? "AVAILABLE" : "BORROWED") << "]\n";
                found = true;
            }
        }   
        
        if (!found) {
            cout << "No books matching your search.\n";
        }
    }
};

/**
 * Main program entry point.
 * Creates Library instance and displays interactive menu.
 * Handles user input and calls appropriate library methods.
 */
int main() {
    Library lib;
    int choice;

    while (true) {
        cout << "\n===== Library Management System =====\n"
             << "1. Add a New Book\n"
             << "2. Register a New User\n"
             << "3. Borrow a Book\n"
             << "4. Display All Books\n"
             << "5. Display All Users\n"
             << "6. Search Books\n"
             << "0. Exit\n"
             << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.addUser(); break;
            case 3: lib.borrowBook(); break;
            case 4: lib.showBooks(); break;
            case 5: lib.showUsers(); break;
            case 6: lib.searchBooks(); break;
            case 0: 
                cout << "Saving data and exiting...\n";
                return 0;
            default: 
                cout << "Invalid choice! Please try again.\n";
        }
    }
}