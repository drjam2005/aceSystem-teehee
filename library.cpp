#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
/*
class new hell yeah
code
cod
eoce
doed

d
*/
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
    LibraryUser(string id = "", string n = "") : userID(id), name(n) {}

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

    bool hasBorrowed(string isbn) const {
        return find(borrowedISBNs.begin(), borrowedISBNs.end(), isbn) != borrowedISBNs.end();
    }

    const vector<string>& getBorrowedBooks() const { return borrowedISBNs; }

    string toString() const {
        string result = userID + "," + name;
        for (const string& isbn : borrowedISBNs)
            result += "," + isbn;
        return result;
    }
};

class Library {
    vector<Book> books;
    vector<LibraryUser> users;
    const string bookFile = "books.txt";
    const string userFile = "users.txt";

    void loadBooks() {
        ifstream file(bookFile);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string title, author, isbn, status;
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, isbn, ',');
            getline(ss, status, ',');
            books.emplace_back(title, author, isbn, status == "available");
        }
    }

    void loadUsers() {
        ifstream file(userFile);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string id, name;
            getline(ss, id, ',');
            getline(ss, name, ',');
            LibraryUser user(id, name);
            
            string isbn;
            while (getline(ss, isbn, ',')) {
                if (!isbn.empty()) {
                    user.borrowBook(isbn);
                }
            }
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

public:
    Library() {
        loadBooks();
        loadUsers();
    }

    ~Library() {
        saveBooks();
        saveUsers();
    }

    bool bookExists(const string& isbn) const {
        return find_if(books.begin(), books.end(), 
            [&isbn](const Book& b) { return b.getISBN() == isbn; }) != books.end();
    }

    bool userExists(const string& id) const {
        return find_if(users.begin(), users.end(), 
            [&id](const LibraryUser& u) { return u.getID() == id; }) != users.end();
    }

    void addBook(const string& t, const string& a, const string& i) {
        if (bookExists(i)) {
            cout << "Error: Book with this ISBN already exists.\n";
            return;
        }
        books.emplace_back(t, a, i);
        cout << "Book added successfully.\n";
    }

    void removeBook(const string& isbn) {
        auto it = remove_if(books.begin(), books.end(),
            [&isbn](const Book& b) { return b.getISBN() == isbn; });
        
        if (it == books.end()) {
            cout << "Error: Book not found.\n";
            return;
        }
        
    
        for (const auto& user : users) {
            if (user.hasBorrowed(isbn)) {
                cout << "Error: Cannot remove a book that is currently borrowed.\n";
                return;
            }
        }
        
        books.erase(it, books.end());
        cout << "Book removed successfully.\n";
    }

    void registerUser(const string& id, const string& name) {
        if (userExists(id)) {
            cout << "Error: User with this ID already exists.\n";
            return;
        }
        users.emplace_back(id, name);
        cout << "User registered successfully.\n";
    }

    void removeUser(const string& id) {
        auto it = find_if(users.begin(), users.end(),
            [&id](const LibraryUser& u) { return u.getID() == id; });
        
        if (it == users.end()) {
            cout << "Error: User not found.\n";
            return;
        }
        
        if (!it->getBorrowedBooks().empty()) {
            cout << "Error: Cannot remove a user who has borrowed books.\n";
            return;
        }
        
        users.erase(it);
        cout << "User removed successfully.\n";
    }

    void displayAllBooks() const {
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;
        }
        for (const Book& b : books) {
            cout << "Title: " << b.getTitle() << "\nAuthor: " << b.getAuthor()
                << "\nISBN: " << b.getISBN() << "\nStatus: "
                << (b.getAvailability() ? "Available" : "Borrowed") << "\n\n";
        }
    }

    void displayAllUsers() const {
        if (users.empty()) {
            cout << "No registered users.\n";
            return;
        }
        for (const LibraryUser& u : users) {
            cout << "ID: " << u.getID() << "\nName: " << u.getName() << "\n";
            if (!u.getBorrowedBooks().empty()) {
                cout << "Borrowed books:\n";
                for (const string& isbn : u.getBorrowedBooks())
                    cout << "- " << isbn << "\n";
            }
            cout << "\n";
        }
    }

    void borrowBook(const string& isbn, const string& userID) {
        auto book = find_if(books.begin(), books.end(),
            [&isbn](const Book& b) { return b.getISBN() == isbn; });
        auto user = find_if(users.begin(), users.end(),
            [&userID](const LibraryUser& u) { return u.getID() == userID; });

        if (book == books.end()) {
            cout << "Error: Book not found.\n";
            return;
        }
        if (user == users.end()) {
            cout << "Error: User not found.\n";
            return;
        }
        if (!book->getAvailability()) {
            cout << "Error: Book is already borrowed.\n";
            return;
        }

        book->setAvailability(false);
        user->borrowBook(isbn);
        cout << "Book borrowed successfully.\n";
    }

    void returnBook(const string& isbn, const string& userID) {
        auto book = find_if(books.begin(), books.end(),
            [&isbn](const Book& b) { return b.getISBN() == isbn; });
        auto user = find_if(users.begin(), users.end(),
            [&userID](const LibraryUser& u) { return u.getID() == userID; });

        if (book == books.end()) {
            cout << "Error: Book not found.\n";
            return;
        }
        if (user == users.end()) {
            cout << "Error: User not found.\n";
            return;
        }
        if (!user->hasBorrowed(isbn)) {
            cout << "Error: This user hasn't borrowed this book.\n";
            return;
        }

        book->setAvailability(true);
        user->returnBook(isbn);
        cout << "Book returned successfully.\n";
    }

    void displayBorrowedBooks(const string& userID) const {
        auto user = find_if(users.begin(), users.end(),
            [&userID](const LibraryUser& u) { return u.getID() == userID; });

        if (user == users.end()) {
            cout << "Error: User not found.\n";
            return;
        }

        const vector<string>& borrowed = user->getBorrowedBooks();
        if (borrowed.empty()) {
            cout << "No books currently borrowed.\n";
            return;
        }

        cout << "Books borrowed by " << user->getName() << ":\n";
        for (const string& isbn : borrowed) {
            auto book = find_if(books.begin(), books.end(),
                [&isbn](const Book& b) { return b.getISBN() == isbn; });
            if (book != books.end()) {
                cout << "- " << book->getTitle() << " by " << book->getAuthor() 
                     << " (ISBN: " << isbn << ")\n";
            }
        }
    }
};

int main() {
    Library library;
    int choice;
    string title, author, isbn, id, name;

    do {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Add Book\n";
        cout << "2. Remove Book\n";
        cout << "3. Register User\n";
        cout << "4. Remove User\n";
        cout << "5. Borrow Book\n";
        cout << "6. Return Book\n";
        cout << "7. Display All Books\n";
        cout << "8. Display All Users\n";
        cout << "9. Display Borrowed Books by User\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "Enter book title: ";
            getline(cin, title);
            cout << "Enter author: ";
            getline(cin, author);
            cout << "Enter ISBN: ";
            getline(cin, isbn);
            library.addBook(title, author, isbn);
            break;
        case 2:
            cout << "Enter ISBN of book to remove: ";
            getline(cin, isbn);
            library.removeBook(isbn);
            break;
        case 3:
            cout << "Enter user ID: ";
            getline(cin, id);
            cout << "Enter user name: ";
            getline(cin, name);
            library.registerUser(id, name);
            break;
        case 4:
            cout << "Enter user ID to remove: ";
            getline(cin, id);
            library.removeUser(id);
            break;
        case 5:
            cout << "Enter ISBN of book to borrow: ";
            getline(cin, isbn);
            cout << "Enter user ID: ";
            getline(cin, id);
            library.borrowBook(isbn, id);
            break;
        case 6:
            cout << "Enter ISBN of book to return: ";
            getline(cin, isbn);
            cout << "Enter user ID: ";
            getline(cin, id);
            library.returnBook(isbn, id);
            break;
        case 7:
            library.displayAllBooks();
            break;
        case 8:
            library.displayAllUsers();
            break;
        case 9:
            cout << "Enter user ID: ";
            getline(cin, id);
            library.displayBorrowedBooks(id);
            break;
        case 0:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
