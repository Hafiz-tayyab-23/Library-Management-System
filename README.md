# Library Management System

A **C++ console-based application** to manage books, users, and transactions in a library. This project provides a simple yet functional library management system with persistent data storage, allowing librarians to efficiently manage their library inventory and users.

---

## Features

- **Book Management**
  - Add new books with title, author, and ISBN.
  - Display all books in the library.
  - Search books by title, author, or ISBN.
  - Sort books alphabetically by title.

- **User Management**
  - Register new library users with unique ID, name, and contact information.
  - Only registered users can borrow or return books.

- **Borrow & Return System**
  - Borrow books and mark them as unavailable.
  - Return books and mark them as available.
  - Records all transactions with timestamps.

- **Transaction Logging**
  - Maintains a persistent record of all borrow and return transactions.
  - View all past transactions in a structured format.

- **Data Persistence**
  - All data is stored in text files (`books.txt`, `users.txt`, `transactions.txt`) to maintain library information between sessions.

---

## Technology Stack

- **Language:** C++  
- **Data Storage:** Plain text files for persistence  
- **Platform:** Console-based application  

---

## Folder Structure
LibraryManagementSystem/<br>
│<br>
├─ library_management_system.cpp    # Main C++ program <br>
├─ books.txt                        # Persistent storage for books<br>
├─ users.txt                        # Persistent storage for users<br>
├─ transactions.txt                 # Persistent storage for transactions<br>
└─ README.md                        # Project description<br>

---

## Future Enhancements
- Prevent duplicate Book ISBNs or User IDs.<br>
- Implement partial search for books and users.<br>
- Add overdue tracking and fine management.<br>
- Integrate a GUI interface using a library like Qt or SFML.<br>

---

## Project Contributors
- Hafiz Muhammad Tayyab Zia<br>
- Mursaleen Ahmad<br>
- Eesha Siddqui<br>
