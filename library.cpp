#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Book {
public:
    string title;
    string author;
    string ISBN;
    bool available;

    Book(string t, string a, string i) 
        : title(t), author(a), ISBN(i), available(true) {}
};

class User {
public:
    string name;
    string ID;
    vector<string> borrowedBooks;

    User(string n, string id) : name(n), ID(id) {}
};

class Library {
private:
    vector<Book> books;
    vector<User> users;

    void saveData() {
        ofstream bookFile("books.txt");
        for (auto& book : books) {
            bookFile << book.title << endl
                    << book.author << endl
                    << book.ISBN << endl
                    << book.available << endl;
        }
        bookFile.close();

        ofstream userFile("users.txt");
        for (auto& user : users) {
            userFile << user.name << endl
                    << user.ID << endl
                    << user.borrowedBooks.size() << endl;
            for (auto& isbn : user.borrowedBooks) {
                userFile << isbn << endl;
            }
        }
        userFile.close();
    }

    void loadData() {
        ifstream bookFile("books.txt");
        if (!bookFile) {
            cout << "File is Missing or Corrupted\n";
            return;
        }

        string title, author, ISBN;
        bool available;
        while (getline(bookFile, title)) {
            getline(bookFile, author);
            getline(bookFile, ISBN);
            bookFile >> available;
            bookFile.ignore();
            books.push_back(Book(title, author, ISBN));
            books.back().available = available;
        }
        bookFile.close();

        ifstream userFile("users.txt");
        if (!userFile) {
            cout << "File is Missing or Corrupted\n";
            return;
        }

        string name, ID, isbn;
        int bookCount;
        while (getline(userFile, name)) {
            getline(userFile, ID);
            userFile >> bookCount;
            userFile.ignore();
            users.push_back(User(name, ID));
            for (int i = 0; i < bookCount; i++) {
                getline(userFile, isbn);
                users.back().borrowedBooks.push_back(isbn);
            }
        }
        userFile.close();
    }

public:
    Library() { loadData(); }
    ~Library() { saveData(); }

    void addBook() {
        string title, author, ISBN;
        cout << "Enter book title: ";
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        getline(cin, ISBN);
        books.push_back(Book(title, author, ISBN));
        cout << "Book added!\n";
    }

    void addUser() {
        string name, ID;
        cout << "Enter user name: ";
        getline(cin, name);
        cout << "Enter user ID: ";
        getline(cin, ID);
        users.push_back(User(name, ID));
        cout << "User added!\n";
    }

    void borrowBook() {
        string ISBN, userID;
        cout << "Enter book ISBN: ";
        getline(cin, ISBN);
        cout << "Enter user ID: ";
        getline(cin, userID);

        for (auto& book : books) {
            if (book.ISBN == ISBN && book.available) {
                for (auto& user : users) {
                    if (user.ID == userID) {
                        book.available = false;
                        user.borrowedBooks.push_back(ISBN);
                        cout << "Book borrowed!\n";
                        return;
                    }
                }
                cout << "User not found!\n";
                return;
            }
        }
        cout << "Book not available or not found!\n";
    }

    void showBooks() {
        cout << "\nLibrary Books:\n";
        for (auto& book : books) {
            cout << book.title << " by " << book.author 
                 << " (" << (book.available ? "Available" : "Borrowed") << ")\n";
        }
    }

    void showUsers() {
        cout << "\nLibrary Users:\n";
        for (auto& user : users) {
            cout << user.name << " (ID: " << user.ID << ") - ";
            if (user.borrowedBooks.empty()) {
                cout << "No books borrowed\n";
            } else {
                cout << "Borrowed books: ";
                for (auto& isbn : user.borrowedBooks) {
                    cout << isbn << " ";
                }
                cout << endl;
            }
        }
    }

    void searchBooks(){
        string query;
        cout << "Enter Title/Author/ISBN to search: ";
        getline(cin, query);
        bool found = false;
        for (int i = 0; i < books.size(); i++) {
            if (books[i].title.find(query) != string::npos || books[i].author.find(query) != string::npos || books[i].ISBN.find(query) != string::npos) {
                cout << books[i].title << " by " << books[i].author  << " (" << (books[i].available ? "Available" : "Borrowed") << ")\n";
                found = true;
            }
        }   
            if (!found) {
                cout << "No matching books found.\n";
            }
        }
    };


int main() {
    Library lib;
    int choice;

    while (true) {
        cout << "JAMES THE SIGMA WAS HERE!!!";
        cout << "\nLibrary Menu:\n"
             << "1. Add Book\n"
             << "2. Add User\n"
             << "3. Borrow Book\n"
             << "4. Show All Books\n"
             << "5. Show All Users\n"
             << "6. Search Books\n"
             << "0. Exit\n"
             << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.addUser(); break;
            case 3: lib.borrowBook(); break;
            case 4: lib.showBooks(); break;
            case 5: lib.showUsers(); break;
            case 6: lib.searchBooks(); break;
            case 0: return 0;
            default: cout << "Invalid choice!\n";
        }
    }
}
