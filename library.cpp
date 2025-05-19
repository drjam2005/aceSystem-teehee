#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class Book {
    string title, author, isbn;
    bool isAvailable;

public:
    Book(string t, string a, string i, bool avail = true)
        : title(t), author(a), isbn(i), isAvailable(avail) {}

    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    bool getAvailability() const { return isAvailable; }

    void setAvailability(bool status) { isAvailable = status; }

    string toString() const {
        return title + "," + author + "," + isbn + "," + (isAvailable ? "available" : "borrowed");
    }
};

class LibraryUser {
    string userID, name;
    vector<string> borrowedISBNs;

public:
    LibraryUser(string id, string n) : userID(id), name(n) {}

    string getID() const { return userID; }
    string getName() const { return name; }

    void borrowBook(string isbn) { borrowedISBNs.push_back(isbn); }

    bool returnBook(string isbn) {
        auto it = find(borrowedISBNs.begin(), borrowedISBNs.end(), isbn);
        if (it != borrowedISBNs.end()) {
            borrowedISBNs.erase(it);
            return true;
        }
        return false;
    }

    bool hasBorrowed(string isbn) {
        return find(borrowedISBNs.begin(), borrowedISBNs.end(), isbn) != borrowedISBNs.end();
    }

    vector<string> getBorrowedBooks() const { return borrowedISBNs; }

    string toString() const {
        string result = userID + "," + name;
        for (string isbn : borrowedISBNs)
            result += "," + isbn;
        return result;
    }
};

class Library {
    vector<Book> books;
    vector<LibraryUser> users;
    const string bookFile = "books.txt";
    const string userFile = "users.txt";

public:
    Library() {
        loadBooks();
        loadUsers();
    }

    ~Library() {
        saveBooks();
        saveUsers();
    }

    void loadBooks() {
        ifstream file(bookFile);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string title, author, isbn, status;
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, isbn, ',');
            getline(ss, status, ',');
            books.push_back(Book(title, author, isbn, status == "available"));
        }
    }

    void loadUsers() {
        ifstream file(userFile);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name;
            getline(ss, id, ',');
            getline(ss, name, ',');
            LibraryUser user(id, name);
            string isbn;
            while (getline(ss, isbn, ','))
                user.borrowBook(isbn);
            users.push_back(user);
        }
    }

    void saveBooks() {
        ofstream file(bookFile);
        for (const Book& book : books)
            file << book.toString() << "\n";
    }

    void saveUsers() {
        ofstream file(userFile);
        for (const LibraryUser& user : users)
            file << user.toString() << "\n";
    }

    void addBook(string t, string a, string i) {
        books.push_back(Book(t, a, i));
        cout << "Book added successfully.\n";
    }

    void removeBook(string isbn) {
        books.erase(remove_if(books.begin(), books.end(),
            [isbn](Book b) { return b.getISBN() == isbn; }), books.end());
        cout << "Book removed.\n";
    }

    void registerUser(string id, string name) {
        users.push_back(LibraryUser(id, name));
        cout << "User registered successfully.\n";
    }

    void removeUser(string id) {
        users.erase(remove_if(users.begin(), users.end(),
            [id](LibraryUser u) { return u.getID() == id; }), users.end());
        cout << "User removed.\n";
    }

    void displayAllBooks() {
        for (const Book& b : books) {
            cout << "Title: " << b.getTitle() << ", Author: " << b.getAuthor()
                << ", ISBN: " << b.getISBN() << ", Status: "
                << (b.getAvailability() ? "Available" : "Borrowed") << "\n";
        }
    }

    void displayAllUsers() {
        for (const LibraryUser& u : users) {
            cout << "ID: " << u.getID() << ", Name: " << u.getName() << "\n";
        }
    }

    void borrowBook(string isbn, string userID) {
        auto book = find_if(books.begin(), books.end(),
            [isbn](Book& b) { return b.getISBN() == isbn; });
        auto user = find_if(users.begin(), users.end(),
            [userID](LibraryUser& u) { return u.getID() == userID; });

        if (book == books.end()) {
            cout << "Book not found.\n";
            return;
        }
        if (user == users.end()) {
            cout << "User not found.\n";
            return;
        }

        if (!book->getAvailability()) {
            cout << "Book already borrowed.\n";
            return;
        }

        book->setAvailability(false);
        user->borrowBook(isbn);
        cout << "Book borrowed.\n";
    }

    void returnBook(string isbn, string userID) {
        auto book = find_if(books.begin(), books.end(),
            [isbn](Book& b) { return b.getISBN() == isbn; });
        auto user = find_if(users.begin(), users.end(),
            [userID](LibraryUser& u) { return u.getID() == userID; });

        if (book == books.end() || user == users.end()) {
            cout << "Book or user not found.\n";
            return;
        }

        if (!user->hasBorrowed(isbn)) {
            cout << "This user didn’t borrow this book.\n";
            return;
        }

        book->setAvailability(true);
        user->returnBook(isbn);
        cout << "Book returned.\n";
    }

    void displayBorrowedBooks(string userID) {
        auto user = find_if(users.begin(), users.end(),
            [userID](LibraryUser& u) { return u.getID() == userID; });

        if (user == users.end()) {
            cout << "User not found.\n";
            return;
        }

        vector<string> borrowed = user->getBorrowedBooks();
        if (borrowed.empty()) {
            cout << "No books borrowed.\n";
            return;
        }

        cout << "Books borrowed by " << user->getName() << ":\n";
        for (string isbn : borrowed) {
            auto book = find_if(books.begin(), books.end(),
                [isbn](Book& b) { return b.getISBN() == isbn; });
            if (book != books.end())
                cout << "- " << book->getTitle() << " by " << book->getAuthor() << "\n";
        }
    }
};


int main() {
    Library library;
    int choice;
    string title, author, isbn, id, name;

    do {
        cout << "\n=== Library Menu ===\n";
        cout << "1. Add Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. Register User" << endl;
        cout << "4. Remove User" << endl;
        cout << "5. Borrow Book" << endl;
        cout << "6. Return Book" << endl;
        cout << "7. Display All Books" << endl;
        cout << "8. Display All Users" << endl;
        cout << "9. Display Borrowed Books" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter title: "; getline(cin, title);
            cout << "Enter author: "; getline(cin, author);
            cout << "Enter ISBN: "; getline(cin, isbn);
            library.addBook(title, author, isbn);
            break;
        case 2:
            cout << "Enter ISBN to remove: "; getline(cin, isbn);
            library.removeBook(isbn);
            break;
        case 3:
            cout << "Enter user ID: "; getline(cin, id);
            cout << "Enter name: "; getline(cin, name);
            library.registerUser(id, name);
            break;
        case 4:
            cout << "Enter user ID to remove: "; getline(cin, id);
            library.removeUser(id);
            break;
        case 5:
            cout << "Enter ISBN to borrow: "; getline(cin, isbn);
            cout << "Enter user ID: "; getline(cin, id);
            library.borrowBook(isbn, id);
            break;
        case 6:
            cout << "Enter ISBN to return: "; getline(cin, isbn);
            cout << "Enter user ID: "; getline(cin, id);
            library.returnBook(isbn, id);
            break;
        case 7:
            library.displayAllBooks();
            break;
        case 8:
            library.displayAllUsers();
            break;
        case 9:
            cout << "Enter user ID: "; getline(cin, id);
            library.displayBorrowedBooks(id);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
