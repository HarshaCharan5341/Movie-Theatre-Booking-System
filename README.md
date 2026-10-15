# 🎬 Movie Theatre Booking System

A console-based Movie Theatre Booking System developed using **C++**. The application allows users to register, log in, browse movies and shows, select seats, order food and beverages, and complete a simulated payment process.

## 🚀 Features

- **User Authentication:** User registration and login with password validation and masked password input.
- **Movie Management:** Displays movie details, including genre, duration, language, and certificate.
- **Show Management:** Lists available show dates, times, and screen numbers.
- **Seat Booking:** Displays seat layouts, categories, prices, and booking status.
- **Booking Management:** View previous bookings and cancel tickets.
- **Food and Beverages:** Select menu items and quantities during checkout.
- **Payment Simulation:** UPI ID validation and card input validation.
- **Tax Calculation:** Calculates 9% CGST and 9% SGST.
- **File Handling:** Reads and writes application data using text files.
- **Input Validation:** Handles invalid menu choices, quantities, passwords, and payment details.

## 🧠 Object-Oriented Programming Concepts

### 1. Classes and Objects
Classes such as `Movie`, `Seat`, `Show`, `User`, `Booking`, `Food`, and `Beverage` represent the entities in the application.

### 2. Encapsulation
Data and related functions are grouped into classes. Some classes expose their data publicly for use by other parts of the program.

### 3. Inheritance
`Food` and `Beverage` inherit from the `MenuItem` base class, reusing its ID, name, and price properties.

### 4. Polymorphism
The `Payment` base class declares a pure virtual `pay()` function. `UPIPayment` and `CardPayment` override it to implement different payment-validation flows.

### 5. Abstraction
The payment interface provides a common way to process payments without requiring the booking code to handle each payment method's internal validation details.

## 🛠️ Technologies Used

- C++
- Object-Oriented Programming (OOP)
- Standard Template Library (STL)
- File handling (`ifstream`, `ofstream`)
- String processing (`stringstream`)
- Input validation and exception handling

## 📁 Project Structure

```text
Movie-Theatre-Booking-System/
├── Movie-Theatre-Booking-System.cpp
├── README.md
├── movies.txt
├── shows.txt
├── screens.txt
├── foods.txt
├── beverages.txt
└── upi_suffixes.txt
```

The application creates several configuration files with default data when they are missing. User and booking records are stored separately and should not be published if they contain private information.

## 💻 Requirements

- A C++ compiler such as MinGW-w64 GCC
- Windows, because the current implementation uses `conio.h` and `_getch()`
- A terminal or Visual Studio Code

## ▶️ How to Run

### 1. Clone the repository

```bash
git clone https://github.com/HarshaCharan5341/Movie-Theatre-Booking-System.git
```

### 2. Open the project directory

```bash
cd Movie-Theatre-Booking-System
```

### 3. Compile the program

```bash
g++ Movie-Theatre-Booking-System.cpp -o theatre
```

### 4. Run the program

In Windows PowerShell:

```powershell
.\theatre.exe
```

## 💳 Payment Disclaimer

The payment gateway is an educational simulation. It validates entered payment details but does **not** connect to a bank, UPI provider, or real payment gateway. A successful validation does not mean that a real payment has occurred.

## 🔐 Security Note

This project is intended for learning purposes. The current implementation stores passwords in plain text. Do not use real passwords or publish actual user credentials. A production system should use secure password hashing, protected storage, and a trusted payment provider.

## 👨‍💻 Author

**Harshacharan Reddy**

- GitHub: [HarshaCharan5341](https://github.com/HarshaCharan5341)
- Project: [Movie Theatre Booking System](https://github.com/HarshaCharan5341/Movie-Theatre-Booking-System)
