/*
 * ============================================================
 * LIBRARY MANAGEMENT SYSTEM
 * A C++ program to manage library books, users, and transactions
 * Features: Add/Search books, Register users, Borrow/Return books
 * Data Persistence: All data is saved to text files
 * ============================================================
 */

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
using namespace std;

// Define maximum capacity for storage arrays
#define MAX_BOOKS 100 // Maximum number of books
#define MAX_USERS 100 // Maximum number of users
#define MAX_TRANS 200 // Maximum number of transactions

// ============================================================
// BOOK CLASS
// ============================================================
// Represents a book entity with properties: title, author, ISBN, availability
class Book
{
public:
    string title, author, isbn; // Book details
    bool available;             // Availability status (true = available, false = borrowed)

    // Constructor: Initialize all book properties to empty/true
    Book()
    {
        title = author = isbn = "";  // Set to empty Strings
        available = true;
    }

    // Input method: Prompt user to enter book details
    void addBook()
    {
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        getline(cin, isbn);
        available = true;
    }

    // Output method: Display book information in formatted manner
    void displayBook()
    {
        cout << "Title: " << title << " | Author: " << author
             << " | ISBN: " << isbn
             << " | Status: " << (available ? "Available" : "Issued") << endl;
    }

    // Save book data to file in pipe-delimited format
    void saveToFile(ofstream &out)
    {
        out << title << "|" << author << "|" << isbn << "|" << available << "\n";
    }

    // Load book data from file by parsing pipe-delimited line
    bool loadFromFile(string line)
    {
        // Find positions of delimiters
        int p1 = line.find("|");
        int p2 = line.find("|", p1 + 1);
        int p3 = line.find("|", p2 + 1);

        // Check if all delimiters found
        if (p1 == -1 || p2 == -1 || p3 == -1)
            return false;

        // Extract each field using substring positions
        title = line.substr(0, p1);
        author = line.substr(p1 + 1, p2 - p1 - 1);
        isbn = line.substr(p2 + 1, p3 - p2 - 1);
        available = (line.substr(p3 + 1) == "1"); // Convert "1" to true, "0" to false
        return true;
    }
};

// ============================================================
// USER CLASS
// ============================================================
// Represents a user/member with properties: ID, name, contact information
class User
{
public:
    string id, name, contact; // User identification and contact details

    // Constructor: Initialize all user properties to empty strings
    User()
    {
        id = name = contact = "";
    }

    // Input method: Prompt user to enter member details
    void addUser()
    {
        cout << "Enter User ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Contact: ";
        getline(cin, contact);
    }

    // Output method: Display user information in formatted manner
    void displayUser()
    {
        cout << "User ID: " << id << " | Name: " << name
             << " | Contact: " << contact << endl;
    }

    // Save user data to file in pipe-delimited format
    void saveToFile(ofstream &out)
    {
        out << id << "|" << name << "|" << contact << "\n";
    }

    // Load user data from file by parsing pipe-delimited line
    bool loadFromFile(string line)
    {
        // Find positions of delimiters
        int p1 = line.find("|");
        int p2 = line.find("|", p1 + 1);

        // Check if all delimiters found
        if (p1 == -1 || p2 == -1)
            return false;

        // Extract each field using substring positions
        id = line.substr(0, p1);
        name = line.substr(p1 + 1, p2 - p1 - 1);
        contact = line.substr(p2 + 1);
        return true;
    }
};

// ============================================================
// TRANSACTION CLASS
// ============================================================
// Records borrowing/returning transactions with timestamp
class Transaction
{
public:
    string userId, bookISBN, action, timestamp; // Transaction details

    // Constructor: Initialize all transaction properties to empty strings
    Transaction()
    {
        userId = bookISBN = action = timestamp = "";
    }

    // Set transaction details and capture current date/time
    void setTransaction(string u, string b, string a)
    {
        userId = u;
        bookISBN = b;
        action = a; // Action: "Borrowed" or "Returned"

        // Capture current system time and convert to string
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // Remove trailing newline character
    }

    // Output method: Display transaction information
    void displayTransaction()
    {
        cout << "User ID: " << userId << " | Book ISBN: " << bookISBN
             << " | Action: " << action << " | Time: " << timestamp << endl;
    }

    // Save transaction data to file in pipe-delimited format
    void saveToFile(ofstream &out)
    {
        out << userId << "|" << bookISBN << "|" << action << "|" << timestamp << "\n";
    }

    // Load transaction data from file by parsing pipe-delimited line
    bool loadFromFile(string line)
    {
        // Find positions of delimiters
        int p1 = line.find("|");
        int p2 = line.find("|", p1 + 1);
        int p3 = line.find("|", p2 + 1);

        // Check if all delimiters found
        if (p1 == -1 || p2 == -1 || p3 == -1)
            return false;

        // Extract each field using substring positions
        userId = line.substr(0, p1);
        bookISBN = line.substr(p1 + 1, p2 - p1 - 1);
        action = line.substr(p2 + 1, p3 - p2 - 1);
        timestamp = line.substr(p3 + 1);
        return true;
    }
};

// ============================================================
// LIBRARY CLASS
// ============================================================
// Main library management system class handling all operations
class Library
{
private:
    // Storage arrays for books, users, and transactions
    Book books[MAX_BOOKS];
    User users[MAX_USERS];
    Transaction trans[MAX_TRANS];

    // Counter variables to track number of items stored
    int bookCount, userCount, transCount;

public:
    // Constructor: Initialize counters and load all persisted data from files
    Library()
    {
        bookCount = userCount = transCount = 0;
        loadAllData();
    }

    // ============================================================
    // HELPER METHOD
    // ============================================================
    // Check if user exists in system by ID
    bool isRegisteredUser(const string &userId)
    {
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].id == userId)
                return true; // Found
        }
        return false; // Not found
    }

    // ============================================================
    // ADD BOOK METHOD
    // ============================================================
    void addBook()
    {
        if (bookCount < MAX_BOOKS)
        {
            books[bookCount].addBook(); // Get book details from user
            bookCount++;                // Increment book counter
            saveBooks();                // Persist changes to file
            cout << "Book added and saved successfully!\n";
        }
        else
        {
            cout << "Book storage full!\n";
        }
    }

    // ============================================================
    // ADD USER METHOD
    // ============================================================
    void addUser()
    {
        if (userCount < MAX_USERS)
        {
            users[userCount].addUser(); // Get user details from user
            userCount++;                // Increment user counter
            saveUsers();                // Persist changes to file
            cout << "User added and saved successfully!\n";
        }
        else
        {
            cout << "User storage full!\n";
        }
    }

    // ============================================================
    // SEARCH BOOK METHOD
    // ============================================================
    void searchBook()
    {
        string keyword;
        cin.ignore();
        cout << "Enter Title/Author/ISBN to search: ";
        getline(cin, keyword);

        bool found = false;
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].title == keyword || books[i].author == keyword || books[i].isbn == keyword)
            {
                books[i].displayBook();
                found = true;
            }
        }

        if (!found)
            cout << "Book not found.\n";
    }

    // ============================================================
    // BORROW BOOK METHOD
    // ============================================================
    void borrowBook()
    {
        string userId, isbn;
        cout << "Enter User ID: ";
        cin >> userId;

        // Check if user is registered
        if (!isRegisteredUser(userId))
        {
            cout << "User not registered! Cannot borrow book.\n";
            return;
        }

        cout << "Enter Book ISBN: ";
        cin >> isbn;

        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].isbn == isbn)
            {
                if (books[i].available)
                {
                    books[i].available = false;
                    trans[transCount].setTransaction(userId, isbn, "Borrowed");
                    ofstream fout("transactions.txt", ios::app);
                    trans[transCount++].saveToFile(fout);
                    fout.close();
                    saveBooks();
                    cout << "Book borrowed successfully!\n";
                    return;
                }
                else
                {
                    cout << "Book already borrowed.\n";
                    return;
                }
            }
        }
        cout << "Book not found.\n";
    }

    // ============================================================
    // RETURN BOOK METHOD
    // ============================================================
    void returnBook()
    {
        string userId, isbn;
        cout << "Enter User ID: ";
        cin >> userId;

        // Check if user is registered
        if (!isRegisteredUser(userId))
        {
            cout << "User not registered! Cannot return book.\n";
            return;
        }

        cout << "Enter Book ISBN: ";
        cin >> isbn;

        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].isbn == isbn)
            {
                if (!books[i].available)
                {
                    books[i].available = true;
                    trans[transCount].setTransaction(userId, isbn, "Returned");
                    ofstream fout("transactions.txt", ios::app);
                    trans[transCount++].saveToFile(fout);
                    fout.close();
                    saveBooks();
                    cout << "Book returned successfully!\n";
                    return;
                }
                else
                {
                    cout << "Book was not borrowed.\n";
                    return;
                }
            }
        }
        cout << "Book not found.\n";
    }

    // ============================================================
    // DISPLAY ALL BOOKS METHOD
    // ============================================================
    void displayBooks()
    {
        if (bookCount == 0)
        {
            cout << "No books available.\n";
            return;
        }

        for (int i = 0; i < bookCount; i++)
            books[i].displayBook();
    }

    // ============================================================
    // DISPLAY ALL TRANSACTIONS METHOD
    // ============================================================
    void displayTransactions()
    {
        ifstream in("transactions.txt");

        if (!in)
        {
            cout << "No transactions recorded yet.\n";
            return;
        }

        string line;
        while (getline(in, line))
        {
            cout << line << endl;
        }
        in.close();
    }

    // ============================================================
    // SORT BOOKS BY TITLE METHOD
    // ============================================================
    void sortBooksByTitle()
    {
        for (int i = 1; i < bookCount; i++)
        {
            Book key = books[i];
            int j = i - 1;
            while (j >= 0 && books[j].title > key.title)
            {
                books[j + 1] = books[j];
                j--;
            }
            books[j + 1] = key;
        }

        saveBooks();
        cout << "Books sorted by title and saved successfully!\n";
    }

    // ============================================================
    // FILE I/O METHODS
    // ============================================================
    void saveBooks()
    {
        ofstream out("books.txt");
        for (int i = 0; i < bookCount; i++)
            books[i].saveToFile(out);
        out.close();
    }

    void saveUsers()
    {
        ofstream out("users.txt");
        for (int i = 0; i < userCount; i++)
            users[i].saveToFile(out);
        out.close();
    }

    void loadBooks()
    {
        ifstream in("books.txt");
        string line;
        bookCount = 0;
        while (getline(in, line))
        {
            if (books[bookCount].loadFromFile(line))
                bookCount++;
        }
        in.close();
    }

    void loadUsers()
    {
        ifstream in("users.txt");
        string line;
        userCount = 0;
        while (getline(in, line))
        {
            if (users[userCount].loadFromFile(line))
                userCount++;
        }
        in.close();
    }

    void loadAllData()
    {
        loadBooks();
        loadUsers();
    }
};

// ============================================================
// MAIN FUNCTION
// ============================================================
// Entry point: Displays menu and handles user choice for operations
int main()
{
    Library lib; // Create library instance (loads all persisted data)
    int choice;

    // Main menu loop - continues until user chooses to exit (0)
    do
    {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Add Book\n2. Add User\n3. Borrow Book\n4. Return Book\n";
        cout << "5. Search Book\n6. Display Books\n7. Display Transactions\n";
        cout << "8. Sort Books by Title\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            lib.addBook();
            break;
        case 2:
            lib.addUser();
            break;
        case 3:
            lib.borrowBook();
            break;
        case 4:
            lib.returnBook();
            break;
        case 5:
            lib.searchBook();
            break;
        case 6:
            lib.displayBooks();
            break;
        case 7:
            lib.displayTransactions();
            break;
        case 8:
            lib.sortBooksByTitle();
            break;
        case 0:
            cout << "Exiting... Data saved successfully.\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 0); // Loop until exit choice selected

    return 0; // Program termination
}
