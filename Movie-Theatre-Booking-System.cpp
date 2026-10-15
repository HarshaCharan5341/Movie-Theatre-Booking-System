#include <iostream>   // cin, cout, endl                                          
#include <iomanip>    // setw, setfill, fixed, setprecision
#include <string>     // string, to_string
#include <vector>     // vector
#include <fstream>    // ifstream, ofstream, file handling
#include <sstream>    // stringstream
#include <limits>     // numeric_limits
#include <set>        // set
#include <algorithm>  // max, all_of
#include <cctype>     // isdigit, isalpha, isupper, isspace, tolower, toupper
#include <ctime>      // time, time_t, tm, localtime
#include <conio.h>    // _getch() - Windows only
using namespace std;

int getInteger() {
    int value;
    while(true) {
        if(cin >> value)
            return value;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a number: ";
    }
}

const double CGST_RATE=0.09;
const double SGST_RATE=0.09;

vector<string> readDataLines(const string &filename) {
    vector<string> lines;
    ifstream file(filename);
    string line;
    while(getline(file,line)) {
        if(!line.empty() && line.back()=='\r') {
            line.pop_back();
        }
        size_t start=line.find_first_not_of(" \t");
        if(start==string::npos || line[start]=='#') {
            continue;
        }
        lines.push_back(line);
    }
    return lines;
}

vector<string> splitFields(const string &line,char delim='|') {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while(getline(ss,field,delim)) {
        fields.push_back(field);
    }
    return fields;
}

void createFileIfMissing(const string &filename,const vector<string> &defaultLines) {
    ifstream check(filename);
    if(check) {
        return;
    }
    ofstream out(filename);
    for(const string &line:defaultLines) {
        out<<line<<endl;
    }
}

bool luhnValid(const string &number) {
    if(number.size()<13||number.size()>19) {
        return false;
    }
    int sum=0;
    bool doubleIt=false;
    for(int i=(int)number.size()-1;i>=0;i--) {
        if(!isdigit(static_cast<unsigned char>(number[i]))) {
            return false;
        }
        int digit=number[i]-'0';
        if(doubleIt) {
            digit*=2;
            if(digit>9) {
                digit-=9;
            }
        }
        sum+=digit;
        doubleIt=!doubleIt;
    }
    return sum%10==0;
}

set<string> upiSuffixes;

void loadUpiSuffixes() {
    createFileIfMissing("upi_suffixes.txt",{
        "# valid UPI handles, one per line, without the @",
        "okaxis","oksbi","okhdfcbank","okicici","ybl","ibl","axl","paytm","upi","apl","fam"
    });
    upiSuffixes.clear();
    for(const string &line:readDataLines("upi_suffixes.txt")) {
        string handle=line;
        if(!handle.empty()&&handle[0]=='@') {
            handle.erase(0,1);
        }
        for(char &c:handle) {
            c=tolower(static_cast<unsigned char>(c));
        }
        upiSuffixes.insert(handle);
    }
}

bool upiValid(const string &upiId) {
    size_t pos=upiId.find('@');
    if(pos==string::npos||pos==0||pos==upiId.size()-1||upiId.find('@',pos+1)!=string::npos) {
        return false;
    }
    string user=upiId.substr(0,pos);
    string handle=upiId.substr(pos+1);
    for(char c:user) {
        if(!isalnum(static_cast<unsigned char>(c))&&c!='.'&&c!='_'&&c!='-') {
            return false;
        }
    }
    for(char &c:handle) {
        c=tolower(static_cast<unsigned char>(c));
    }
    return upiSuffixes.count(handle)>0;
}

string getPassword() {
    string password;
    char ch;
    while(true) {
        ch = _getch();
        if(ch == '\r') {
            cout << endl;
            break;
        }
        else if(ch == '\b') {
            if(!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else if(ch != '\0' && ch != -32) {
            password += ch;
            cout << '*';
        }
    }
    return password;
}

string passwordProblems(const string &password) {
    bool hasLetter=false;
    bool hasUpper=false;
    bool hasDigit=false;
    bool hasSpecial=false;
    for(char c:password) {
        unsigned char uc=static_cast<unsigned char>(c);
        if(isalpha(uc)) {
            hasLetter=true;
            if(isupper(uc)) {
                hasUpper=true;
            }
        }
        else if(isdigit(uc)) {
            hasDigit=true;
        }
        else if(!isspace(uc)) {
            hasSpecial=true;
        }
    }
    string problems;
    if(password.size()<6) {
        problems+="  - must be at least 6 characters long\n";
    }
    if(!hasLetter) {
        problems+="  - must contain at least one letter\n";
    }
    if(!hasUpper) {
        problems+="  - must contain at least one capital letter\n";
    }
    if(!hasDigit) {
        problems+="  - must contain at least one number\n";
    }
    if(!hasSpecial) {
        problems+="  - must contain at least one special character (for example @ # $ % & *)\n";
    }
    return problems;
}

bool isExitCommand(string text) {
    size_t start=text.find_first_not_of(" \t");
    size_t end=text.find_last_not_of(" \t");
    if(start==string::npos) {
        return false;
    }
    text=text.substr(start,end-start+1);
    for(char &c:text) {
        c=tolower(static_cast<unsigned char>(c));
    }
    return text=="exit";
}

class Movie {
    public:
    int id;
    string name;
    string genre;
    int duration;
    string language;
    string certificate;
    Movie() {
        id = 0;
        duration = 0;
    }
    Movie(int id, string name, string genre, int duration, string language, string certificate) {
        this->id = id;
        this->name = name;
        this->genre = genre;
        this->duration = duration;
        this->language = language;
        this->certificate = certificate;
    }
};

class Seat {
    public:
    char row;
    int number;
    string category;
    int price;
    char status;
    Seat() {
        row = 'A';
        number = 0;
        price = 0;
        status = 'O';
    }
    Seat(char row, int number, string category, int price) {
        this->row = row;
        this->number = number;
        this->category = category;
        this->price = price;
        status = 'O';
    }
    string getSeatName() {
        return string(1,row) + to_string(number);
    }
};

class MenuItem {
public:
    int id;
    string name;
    double price;

    MenuItem() {
        id = 0;
        price = 0;
    }

    MenuItem(int id, string name, double price) {
        this->id = id;
        this->name = name;
        this->price = price;
    }
};

class Food : public MenuItem {
public:
    Food() : MenuItem() {}
    Food(int id, string name, double price)
        : MenuItem(id, name, price) {}
};

class Beverage : public MenuItem {
public:
    Beverage() : MenuItem() {}
    Beverage(int id, string name, double price)
        : MenuItem(id, name, price) {}
};

class Show {
    public:
    int showId;
    int movieId;
    string date;
    string time;
    int screen;
    vector<Seat> seats;

    void addRow(char row, int count, string category, int price) {
        for(int i=1;i<=count;i++) {
            seats.push_back(Seat(row,i,category,price));
        }
    }

    void createSeats() {
        seats.clear();
        addRow('A',12,"Recliner",470);
        addRow('B',17,"Prime Plus",310);
        addRow('C',17,"Prime Plus",310);
        addRow('D',17,"Prime Plus",310);
        addRow('E',17,"Prime Plus",310);
        addRow('F',17,"Prime Plus",310);
        addRow('G',17,"Extra Legroom",330);
        addRow('H',14,"Prime",250);
        addRow('I',14,"Prime",250);
        addRow('J',14,"Classic Plus",230);
        addRow('K',14,"Classic Plus",230);
    }
    Show() {
        showId = 0;
        movieId = 0;
        screen = 0;
    }

    Show(int showId, int movieId, string date, string time, int screen) {
        this->showId = showId;
        this->movieId = movieId;
        this->date = date;
        this->time = time;
        this->screen = screen;
        createSeats();
    }

    Seat* findSeat(string seatName) {
        for(Seat &seat:seats) {
            if(seat.getSeatName()==seatName) {
                return &seat;
            }
        }
        return nullptr;
    }

    bool bookSeat(string seatName) {
        Seat* seat = findSeat(seatName);
        if(seat==nullptr||seat->status!='O') {
            return false;
        }
        seat->status = 'X';
        return true;
    }

    void cancelSeat(string seatName) {
        Seat* seat = findSeat(seatName);
        if(seat!=nullptr) {
            seat->status = 'O';
        }
    }

    void displayRow(char row) {
        for(Seat &seat:seats) {
            if(seat.row == row) {
                if(seat.status == 'X') {
                    cout<<" X ";
                } else {
                    cout<<setw(2)<<setfill('0')<<seat.number<<setfill(' ')<<" ";
                }
                if(row>='B'&&row<='G'&&(seat.number==2||seat.number==14)) {
                    cout<<"  ";
                }
                if(row=='A') {
                cout<<"   ";
            } else {
                cout<<" ";
            }

            }
        }
        cout<<endl;
    }

    void displaySeats()
    {
        cout<<endl;
        cout<<"                                   SEAT LAYOUT"<<endl;
        cout<<"==============================================================================="<<endl;
        cout<<endl;
        cout<<"                    Rs.470 RECLINER"<<endl;
        cout<<"A       ";
        displayRow('A');
        cout<<endl;
        cout<<"                    Rs.310 PRIME PLUS"<<endl;
        cout<<"B       ";
        displayRow('B');
        cout<<"C       ";
        displayRow('C');
        cout<<"D       ";
        displayRow('D');
        cout<<"E       ";
        displayRow('E');
        cout<<"F       ";
        displayRow('F');
        cout<<endl;
        cout<<"                    Rs.330 EXTRA LEGROOM"<<endl;
        cout<<"G       ";
        displayRow('G');
        cout<<endl;
        cout<<"                    Rs.250 PRIME"<<endl;
        cout<<"H       ";
        displayRow('H');
        cout<<"I       ";
        displayRow('I');
        cout<<endl;
        cout<<"                    Rs.230 CLASSIC PLUS"<<endl;
        cout<<"J       ";
        displayRow('J');
        cout<<"K       ";
        displayRow('K');
        cout<<endl;
        cout<<"                                  SCREEN "<<screen<<endl;
        cout<<"=============================================================================="<<endl;
    }
};

class User {
    public:
    int userId;
    string name;
    string username;
    string password;
    User() {
        userId = 0;
    }
    User(int userId, string name, string username, string password) {
        this->userId = userId;
        this->name = name;
        this->username = username;
        this->password = password;
    }
};

class Booking {
    public:
    int bookingId;
    int userId;
    int showId;
    vector<string> seats;
    double amount;
    double beverageAmount;
    double foodAmount;
    Booking() {
        bookingId = 0;
        userId = 0;
        showId = 0;
        amount = 0;
        beverageAmount = 0;
        foodAmount = 0;
    }
};

vector<Movie> movies;
vector<Show> shows;
vector<User> users;
vector<Booking> bookings;
vector<Food> foods;
vector<Beverage> beverages;
int nextUserId = 1;
int nextBookingId = 1001;

Movie* findMovie(int movieId) {
    for(Movie &movie:movies) {
        if(movie.id==movieId) {
            return &movie;
        }
    }
    return nullptr;
}

Show* findShow(int showId) {
    for(Show &show:shows) {
        if(show.showId==showId) {
            return &show;
        }
    }
    return nullptr;
}

Food* findFood(int foodId) {
    for(Food &food:foods) {
        if(food.id==foodId) {
            return &food;
        }
    }
    return nullptr;
}

Beverage* findBeverage(int beverageId) {
    for(Beverage &beverage:beverages) {
        if(beverage.id==beverageId) {
            return &beverage;
        }
    }
    return nullptr;
}

int numberOfScreens=0;

void loadScreens()
{
    createFileIfMissing("screens.txt",{
        "# number of screens in the theatre (screens are numbered 1, 2, 3 ...)",
        "4"
    });
    numberOfScreens=0;
    vector<string> lines=readDataLines("screens.txt");
    if(!lines.empty()) {
        try {
            numberOfScreens=stoi(lines[0]);
        }
        catch(...) {
            numberOfScreens=0;
        }
    }
    if(numberOfScreens<=0) {
        cout<<"[screens.txt] missing or invalid number of screens, using 1 screen."<<endl;
        numberOfScreens=1;
    }
}

void loadMovies()
{
    createFileIfMissing("movies.txt",{
        "# movie_id|name|genre|duration_min|language|certificate",
        "101|Avengers: Endgame|Action|181|English|U/A",
        "102|Baahubali 2|Fantasy|171|Telugu|U/A",
        "103|Dhurandhar|Thriller|214|Hindi|A",
        "104|Pushpa|Drama|179|Telugu|U/A"
    });
    movies.clear();
    for(const string &line:readDataLines("movies.txt")) {
        vector<string> f=splitFields(line);
        if(f.size()!=6) {
            cout<<"[movies.txt] skipped malformed line: "<<line<<endl;
            continue;
        }
        try {
            movies.push_back(Movie(stoi(f[0]),f[1],f[2],stoi(f[3]),f[4],f[5]));
        }
        catch(...) {
            cout<<"[movies.txt] skipped invalid line: "<<line<<endl;
        }
    }
}

void loadShows()
{
    createFileIfMissing("shows.txt",{
        "# show_id|movie_id|date|time|screen",
        "1|101|13-10-2026|08:00 AM|1",
        "2|101|13-10-2026|06:00 PM|1",
        "3|102|13-10-2026|10:00 AM|2",
        "4|102|13-10-2026|03:00 PM|2",
        "5|102|13-10-2026|09:00 PM|2",
        "6|103|13-10-2026|12:00 PM|3",
        "7|103|13-10-2026|07:00 PM|3",
        "8|103|13-10-2026|11:00 PM|3",
        "9|104|13-10-2026|02:00 PM|4",
        "10|104|13-10-2026|08:00 PM|4"
    });
    shows.clear();
    for(const string &line:readDataLines("shows.txt")) {
        vector<string> f=splitFields(line);
        if(f.size()!=5) {
            cout<<"[shows.txt] skipped line (expected show_id|movie_id|date|time|screen): "<<line<<endl;
            continue;
        }
        try {
            int movieId=stoi(f[1]);
            int screen=stoi(f[4]);
            if(findMovie(movieId)==nullptr) {
                cout<<"[shows.txt] skipped show with unknown movie id: "<<line<<endl;
                continue;
            }
            if(screen<1||screen>numberOfScreens) {
                cout<<"[shows.txt] skipped show, screen must be between 1 and "<<numberOfScreens<<": "<<line<<endl;
                continue;
            }
            bool clash=false;
            for(Show &existing:shows) {
                if(existing.screen==screen&&existing.date==f[2]&&existing.time==f[3]) {
                    clash=true;
                    break;
                }
            }
            if(clash) {
                cout<<"[shows.txt] skipped show, screen already in use at that date and time: "<<line<<endl;
                continue;
            }
            shows.push_back(Show(stoi(f[0]),movieId,f[2],f[3],screen));
        }
        catch(...) {
            cout<<"[shows.txt] skipped invalid line: "<<line<<endl;
        }
    }
}

void loadBeverages()
{
    createFileIfMissing("beverages.txt",{
        "# id|name|price",
        "1|Water|60","2|Coke|100","3|Pepsi|100","4|Sprite|100","5|Fanta|100"
    });
    beverages.clear();
    for(const string &line:readDataLines("beverages.txt")) {
        vector<string> f=splitFields(line);
        if(f.size()!=3) {
            cout<<"[beverages.txt] skipped malformed line: "<<line<<endl;
            continue;
        }
        try {
            beverages.push_back(Beverage(stoi(f[0]),f[1],stod(f[2])));
        }
        catch(...) {
            cout<<"[beverages.txt] skipped invalid line: "<<line<<endl;
        }
    }
}

void loadFoods()
{
    createFileIfMissing("foods.txt",{
        "# id|name|price",
        "1|Popcorn|220","2|Samosa|80","3|Nachos|120","4|French Fries|130",
        "5|Sandwich|150","6|Pizza|280","7|Burger|160"
    });
    foods.clear();
    for(const string &line:readDataLines("foods.txt")) {
        vector<string> f=splitFields(line);
        if(f.size()!=3) {
            cout<<"[foods.txt] skipped malformed line: "<<line<<endl;
            continue;
        }
        try {
            foods.push_back(Food(stoi(f[0]),f[1],stod(f[2])));
        }
        catch(...) {
            cout<<"[foods.txt] skipped invalid line: "<<line<<endl;
        }
    }
}

void saveUsers() {
    ofstream file("users.txt");
    for(User &user:users) {
        file<<user.userId<<"|"<<user.name<<"|"<<user.username<<"|"<<user.password<<endl;
    }
}

void loadUsers() {
    ifstream file("users.txt");
    if(!file) {
        return;
    } 
    string line;
    while(getline(file,line)) {
        stringstream ss(line);
        string id,name,username,password;
        getline(ss,id,'|');
        getline(ss,name,'|');
        getline(ss,username,'|');
        getline(ss,password,'|');
        if(id.empty()) {
            continue;
        }
        try {
            int userId=stoi(id);
            users.push_back(User(userId,name,username,password));
            nextUserId=max(nextUserId,userId+1);
        }
        catch(...) {
        }
    }
}

void saveBookings() {
    ofstream file("bookings.txt");
    file<<fixed<<setprecision(2);
    for(Booking &booking:bookings) {
        file<<booking.bookingId<<"|";
        file<<booking.userId<<"|";
        file<<booking.showId<<"|";
        file<<booking.amount<<"|";
        for(int i=0;i<(int)booking.seats.size();i++) {
            file<<booking.seats[i];
            if(i+1<(int)booking.seats.size()) {
                file<<",";
            }
        }
        file<<"|";
        file<<booking.beverageAmount<<"|";
        file<<booking.foodAmount<<endl;
    } 
}

void loadBookings() {
    ifstream file("bookings.txt");
    if(!file) {
        return;
    }
    string line;
    while(getline(file,line)) {
        stringstream ss(line);
        string bookingId,userId,showId,amount,seatList,beverageAmount,foodAmount;
        getline(ss,bookingId,'|');
        getline(ss,userId,'|');
        getline(ss,showId,'|');
        getline(ss,amount,'|');
        getline(ss,seatList,'|');
        getline(ss,beverageAmount,'|');
        getline(ss,foodAmount,'|');
        if(bookingId.empty()) {
            continue;
        }
        try {
            Booking booking;
            booking.bookingId=stoi(bookingId);
            booking.userId=stoi(userId);
            booking.showId=stoi(showId);
            booking.amount=stod(amount);
            if(!beverageAmount.empty()) {
                booking.beverageAmount=stod(beverageAmount);
            }
            if(!foodAmount.empty()) {
                booking.foodAmount=stod(foodAmount);
            }
            stringstream seatStream(seatList);
            string seatName;
            Show* show=findShow(booking.showId);
            while(getline(seatStream,seatName,',')) {
                if(!seatName.empty()) {
                    booking.seats.push_back(seatName);
                    if(show!=nullptr) {
                        show->bookSeat(seatName);
                    }
                }
            }
            bookings.push_back(booking);
            nextBookingId=max(nextBookingId,booking.bookingId+1);
        }
        catch(...) {
        }
    }
}

void displayMovies() {
    cout<<endl;
    cout<<"============================================"<<endl;
    cout<<"                  MOVIES"<<endl;
    cout<<"============================================"<<endl;
    for(Movie &movie:movies) {
        cout<<"Movie ID   : "<<movie.id<<endl;
        cout<<"Name       : "<<movie.name<<endl;
        cout<<"Genre      : "<<movie.genre<<endl;
        cout<<"Duration   : "<<movie.duration<<" minutes"<<endl;
        cout<<"Language   : "<<movie.language<<endl;
        cout<<"Certificate: "<<movie.certificate<<endl;
        cout<<"--------------------------------------------"<<endl;
    }
}

void displayShows(int movieId) {
    Movie* movie=findMovie(movieId);
    if(movie==nullptr) {
        cout<<"Movie not found."<<endl;
        return;
    }
    cout<<endl;
    cout<<"Available Shows for "<<movie->name<<":"<<endl;
    cout<<"============================================"<<endl;
    for(Show &show:shows) {
        if(show.movieId==movieId) {
            cout<<"Show ID : "<<show.showId<<" | Date : "<<show.date<<" | Time : "<<show.time<<" | Screen : "<<show.screen<<endl;
            cout<<"Seat Prices : Recliner Rs.470 | Prime Plus Rs.310 | Extra Legroom Rs.330 | Prime Rs.250 | Classic Plus Rs.230"<<endl;
            cout<<"--------------------------------------------"<<endl;
        }
    }
}

double selectBeverages() {
    string choice;
    while(true) {
        cout<<"Would you like beverages? (Yes/No): ";
        cin>>choice;
        for(char &c:choice) {
            c=tolower(c);
        }
        if(choice=="n"||choice=="no") {
            return 0;
        }
        if(choice=="y"||choice=="yes") {
            break;
        }
        cout<<"Invalid choice. Please enter Y, Yes, N or No."<<endl;
    }
    double beverageTotal=0;
    while(true) {
        cout<<endl;
        cout<<"============================================"<<endl;
        cout<<"                BEVERAGES"<<endl;
        cout<<"============================================"<<endl;
        for(Beverage &beverage:beverages) {
            cout<<beverage.id<<". "<<beverage.name<<" - Rs."<<fixed<<setprecision(2)<<beverage.price<<endl;
        }
        int doneOption=(int)beverages.size()+1;
        cout<<doneOption<<". Done"<<endl;
        cout<<"Enter choice: ";
        int choiceId;
        choiceId=getInteger();
        if(choiceId==doneOption) {
            break;
        }
        Beverage* beverage=findBeverage(choiceId);
        if(beverage==nullptr) {
            cout<<"Invalid choice."<<endl;
            continue;
        }
        int quantity;
        cout<<"Enter quantity: ";
        quantity=getInteger();
        if(quantity<=0) {
            cout<<"Invalid quantity."<<endl;
            continue;
        }
        double itemTotal=beverage->price*quantity;
        beverageTotal+=itemTotal;
        cout<<beverage->name<<" x "<<quantity<<" added. Rs."<<itemTotal<<endl;
    }
    return beverageTotal;
}

double selectFood() {
    string choice;
    while(true) {
        cout<<"Would you like food? (Yes/No): ";
        cin>>choice;
        for(char &c:choice) {
            c=tolower(c);
        }
        if(choice=="n"||choice=="no") {
            return 0;
        }
        if(choice=="y"||choice=="yes") {
            break;
        }
        cout<<"Invalid choice. Please enter Yes or No."<<endl;
    }
    double foodTotal=0;
    while(true) {
        cout<<endl;
        cout<<"============================================"<<endl;
        cout<<"                  FOOD"<<endl;
        cout<<"============================================"<<endl;
        for(Food &food:foods) {
            cout<<food.id<<". "<<food.name<<" - Rs."<<fixed<<setprecision(2)<<food.price<<endl;
        }
        int doneOption=(int)foods.size()+1;
        cout<<doneOption<<". Done"<<endl;
        cout<<"Enter choice: ";
        int choiceId;
        choiceId=getInteger();
        if(choiceId==doneOption) {
            break;
        }
        Food* food=findFood(choiceId);
        if(food==nullptr){
            cout<<"Invalid choice."<<endl;
            continue;
        }
        int quantity;
        cout<<"Enter quantity: ";
        quantity=getInteger();
        if(quantity<=0) {
            cout<<"Invalid quantity."<<endl;
            continue;
        }
        double itemTotal=food->price*quantity;
        foodTotal+=itemTotal;
        cout<<food->name<<" x "<<quantity<<" added. Rs."<<itemTotal<<endl;
    }
    return foodTotal;
}

int searchUserFile(const string &username,const string &password,User &found) {
    ifstream file("users.txt");
    string line;
    while(getline(file,line)) {
        if(!line.empty()&&line.back()=='\r') {
            line.pop_back();
        }
        vector<string> f=splitFields(line);
        if(f.size()<4) {
            continue;
        }
        if(f[2]==username) {
            if(f[3]!=password) {
                return 1;
            }
            try {
                found=User(stoi(f[0]),f[1],f[2],f[3]);
            }
            catch(...) {
                return 0;
            }
            return 2;
        }
    }
    return 0;
}

bool usernameInFile(const string &username) {
    User ignored;
    return searchUserFile(username,"",ignored)!=0;
}

void registerUser() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string name, username, password;
    cout << endl;
    cout << "Enter Name (type exit to go back) : ";
    getline(cin, name);
    if(isExitCommand(name)) {
        cout << "Returning to main menu." << endl;
        return;
    }
    cout << "Enter Username (type exit to go back) : ";
    getline(cin, username);
    while(true) {
        if(isExitCommand(username)) {
            cout << "Returning to main menu." << endl;
            return;
        }
        if(username.empty()) {
            cout << "Username cannot be empty. Please enter again: ";
            getline(cin, username);
            continue;
        }
        if(username.find(' ') != string::npos) {
            cout << "Username should not contain spaces. Please enter again: ";
            getline(cin, username);
            continue;
        }
        bool exists = false;
        for(User &user : users) {
            if(user.username == username) {
                exists = true;
                break;
            }
        }
        if(!exists&&usernameInFile(username)) {
            exists = true;
        }
        if(exists) {
            cout << "Username already exists. Enter another username: ";
            getline(cin, username);
            continue;
        }
        break;
    }
    cout << "Password rules: at least 6 characters, with a capital letter, a number and a special character." << endl;
    while(true) {
        cout << "Enter Password (type exit to go back) : ";
        password = getPassword();
        if(isExitCommand(password)) {
            cout << "Returning to main menu." << endl;
            return;
        }
        if(password.empty()) {
            cout << "Password cannot be empty. Please enter again.\n";
            continue;
        }
        if(password.find(' ') != string::npos) {
            cout << "Password should not contain spaces. Please enter again.\n";
            continue;
        }
        string problems = passwordProblems(password);
        if(!problems.empty()) {
            cout << "Weak password:" << endl << problems << "Please enter again." << endl;
            continue;
        }
        break;
    }
    users.push_back(User(nextUserId, name, username, password));
    nextUserId++;
    saveUsers();
    cout << "Registration successful." << endl;
}

User* login() {
    string username,password;
    cout<<endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while(true) {
        cout<<"Enter Username (type exit to go back): ";
        getline(cin,username);
        if(isExitCommand(username)) {
            cout<<"Returning to main menu."<<endl;
            return nullptr;
        }
        if(username.empty()) {
            cout<<"Username cannot be empty. Please enter again."<<endl;
            continue;
        }
        if(username.find(' ') != string::npos) {
            cout<<"Username should not contain spaces. Please enter again."<<endl;
            continue;
        }
        break;
    }
    while(true) {
        cout << "Enter Password (type exit to go back): ";
        password = getPassword();

        if(isExitCommand(password)) {
            cout<<"Returning to main menu."<<endl;
            return nullptr;
        }

        if(password.empty()) {
            cout<<"Password cannot be empty. Please enter again."<<endl;
            continue;
        }

        if(password.find(' ') != string::npos) {
            cout<<"Password should not contain spaces. Please enter again."<<endl;
            continue;
        }

        break;
    }
    User fileUser;
    int result=searchUserFile(username,password,fileUser);
    if(result==0) {
        cout<<"Username not found."<<endl;
        return nullptr;
    }
    if(result==1) {
        cout<<"Incorrect password."<<endl;
        return nullptr;
    }
    for(User &user:users) {
        if(user.userId==fileUser.userId) {
            user=fileUser;
            cout<<"Login successful. Welcome "<<user.name<<"!"<<endl;
            return &user;
        }
    }
    users.push_back(fileUser);
    nextUserId=max(nextUserId,fileUser.userId+1);
    cout<<"Login successful. Welcome "<<fileUser.name<<"!"<<endl;
    return &users.back();
}

class Payment {
public:
    virtual bool pay(double amount) = 0;
    virtual ~Payment() {}
};

class UPIPayment : public Payment {
public:
    bool pay(double amount) override {
        string upiId;
        cout << "Enter UPI ID: ";
        cin >> upiId;
        if (!upiValid(upiId)) {
            cout << "Payment Unsuccessful! Invalid UPI ID (check the username and the @handle)."
                 << endl;
            return false;
        }
        cout << "Payment Successful!" << endl;
        cout << "Payment Method: UPI" << endl;
        cout << "Transaction ID: TXN"
             << time(nullptr) << endl;
        return true;
    }
};

class CardPayment : public Payment {
public:
    bool pay(double amount) override {
        string name, cardNumber, cvv, expiry;
        cout << "Enter cardholder name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        if (name.empty() ||
            name.find_first_not_of(" \t") == string::npos) {
            cout << "Payment Unsuccessful! Invalid cardholder name."
                 << endl;
            return false;
        }
        cout << "Enter 16-digit card number: ";
        cin >> cardNumber;
        if (cardNumber.size() != 16 ||
            !all_of(cardNumber.begin(), cardNumber.end(),
                [](char c) {
                    return isdigit(static_cast<unsigned char>(c));
                })) {
            cout << "Payment Unsuccessful! Card number must contain 16 digits."
                 << endl;
            return false;
        }
        if (!luhnValid(cardNumber)) {
            cout << "Payment Unsuccessful! Invalid card number."
                 << endl;
            return false;
        }
        cout << "Enter 3-digit CVV: ";
        cvv = getPassword();
        if (cvv.size() != 3 ||
            !all_of(cvv.begin(), cvv.end(),
                [](char c) {
                    return isdigit(static_cast<unsigned char>(c));
                })) {
            cout << "Payment Unsuccessful! CVV must contain 3 digits."
                 << endl;
            return false;
        }
        cout << "Enter expiry date (MM/YY): ";
        cin >> expiry;
        if (expiry.size() != 5 || expiry[2] != '/' ||
            !isdigit(static_cast<unsigned char>(expiry[0])) ||
            !isdigit(static_cast<unsigned char>(expiry[1])) ||
            !isdigit(static_cast<unsigned char>(expiry[3])) ||
            !isdigit(static_cast<unsigned char>(expiry[4]))) {
            cout << "Payment Unsuccessful! Use MM/YY format, for example 11/28."
                 << endl;
            return false;
        }
        int month = stoi(expiry.substr(0, 2));
        int year = stoi(expiry.substr(3, 2));
        if (month < 1 || month > 12) {
            cout << "Payment Unsuccessful! Invalid expiry month."
                 << endl;
            return false;
        }
        time_t now = time(nullptr);
        tm* current = localtime(&now);
        if (current == nullptr) {
            cout << "Unable to verify expiry date." << endl;
            return false;
        }
        int currentYear = (current->tm_year + 1900) % 100;
        int currentMonth = current->tm_mon + 1;
        if (year < currentYear ||
            (year == currentYear && month < currentMonth)) {
            cout << "Payment Unsuccessful! Card has expired."
                 << endl;
            return false;
        }
        cout << "Payment Successful!" << endl;
        cout << "Payment Method: Credit/Debit Card" << endl;
        cout << "Transaction ID: TXN"
             << time(nullptr) << endl;
        return true;
    }
};

bool processPayment(double amount) {
    int choice;
    cout << endl;
    cout << "============================================" << endl;
    cout << "              PAYMENT GATEWAY" << endl;
    cout << "============================================" << endl;
    cout << fixed << setprecision(2);
    cout << "Amount to Pay: Rs." << amount << endl;
    cout << "1. UPI" << endl;
    cout << "2. Credit/Debit Card" << endl;
    cout << "3. Cancel Payment" << endl;
    cout << "Enter payment method: ";
    choice = getInteger();
    if (choice==3) {
        cout<<"Payment cancelled."<<endl;
        return false;
    }
    UPIPayment upi;
    CardPayment card;
    Payment* payment = nullptr;
    if (choice==1) {
        payment=&upi;
    }
    else if (choice==2) {
        payment=&card;
    }
    else {
        cout<<"Invalid payment method."<<endl;
        return false;
    }
    return payment->pay(amount);
}

void bookTicket(User &user){
    displayMovies();
    int movieId;
    cout<<"Enter Movie ID: ";
    movieId=getInteger();
    Movie* movie=findMovie(movieId);
    if(movie==nullptr) {
        cout<<"Invalid Movie ID."<<endl;
        return;
    }
    displayShows(movieId);
    int showId;
    cout<<"Enter Show ID: ";
    showId=getInteger();
    Show* show=findShow(showId);
    if(show==nullptr||show->movieId!=movieId) {
        cout<<"Invalid Show ID."<<endl;
        return;
    }
    show->displaySeats();
    int numberOfSeats;
    cout<<"Enter number of seats: ";
    numberOfSeats=getInteger();
    if(numberOfSeats<=0||numberOfSeats>(int)show->seats.size()) {
        cout<<"Invalid number of seats."<<endl;
        return;
    }
    vector<string> selectedSeats;
    double ticketAmount=0;
    for(int i=0;i<numberOfSeats;i++) {
        string seatName;
        while(true) {
            cout<<"Enter seat "<<i+1<<" (Example A7): ";
            cin>>seatName;
            for(char &c:seatName) {
                c=toupper(c);
            }
            bool duplicate=false;
            for(string &selected:selectedSeats) {
                if(selected==seatName) {
                    duplicate=true;
                    break;
                }
            }
            if(duplicate) {
                cout<<"You already selected this seat."<<endl;
                continue;
            }
            Seat* seat=show->findSeat(seatName);
            if(seat==nullptr) {
                cout<<"Invalid seat."<<endl;
                continue;
            }
            if(seat->status!='O') {
                cout<<"Seat is already booked."<<endl;
                continue;
            }
            selectedSeats.push_back(seatName);
            ticketAmount+=seat->price;
            break;
        }
    }
    double foodAmount=selectFood();
    double beverageAmount=selectBeverages();
    double subtotal=ticketAmount+beverageAmount+foodAmount;
    double cgst=subtotal*CGST_RATE;
    double sgst=subtotal*SGST_RATE;
    double totalAmount=subtotal+cgst+sgst;
    cout<<endl;
    cout<<"============================================"<<endl;
    cout<<"               CHECKOUT"<<endl;
    cout<<"============================================"<<endl;
    cout<<"Selected Seats  : ";
    for(string &seat:selectedSeats) {
        cout<<seat<<" ";
    }
    cout<<endl;
    cout<<fixed<<setprecision(2);
    cout<<"Ticket Amount   : Rs."<<ticketAmount<<endl;
    cout<<"Food Amount     : Rs."<<foodAmount<<endl;
    cout<<"Beverage Amount : Rs."<<beverageAmount<<endl;
    cout<<"Subtotal        : Rs."<<subtotal<<endl;
    cout<<"CGST (9%)      : Rs."<<cgst<<endl;
    cout<<"SGST (9%)      : Rs."<<sgst<<endl;
    cout<<"Total Amount    : Rs."<<totalAmount<<endl;
    cout<<"============================================"<<endl;
    string choice;
    while(true) {
        cout<<"Confirm booking? (Yes/No): ";
        cin>>choice;
        for(char &c:choice) {
            c=tolower(c);
        }
        if(choice=="y"||choice=="yes") {
            break;
        }
        if(choice=="n"||choice=="no") {
            cout<<"Booking cancelled."<<endl;
            return;
        }
        cout<<"Invalid choice. Please enter Yes or No."<<endl;
    }
    if(!processPayment(totalAmount)) {
    cout<<"Booking cancelled because payment was not completed."<<endl;
    return;
    }
    for(string &seat:selectedSeats) {
        show->bookSeat(seat);
    }
    Booking booking;
    booking.bookingId=nextBookingId;
    booking.userId=user.userId;
    booking.showId=showId;
    booking.seats=selectedSeats;
    booking.amount=totalAmount;
    booking.beverageAmount=beverageAmount;
    booking.foodAmount=foodAmount;
    bookings.push_back(booking);
    saveBookings();
    cout<<endl;
    cout<<"============================================"<<endl;
    cout<<"          BOOKING CONFIRMED"<<endl;
    cout<<"============================================"<<endl;
    cout<<"Booking ID      : "<<booking.bookingId<<endl;
    cout<<"Movie           : "<<movie->name<<endl;
    cout<<"Date            : "<<show->date<<endl;
    cout<<"Time            : "<<show->time<<endl;
    cout<<"Screen          : "<<show->screen<<endl;
    cout<<"Seats           : ";
    for(string &seat:selectedSeats) {
        cout<<seat<<" ";
    }
    cout<<endl;
    cout<<"Ticket Amount   : Rs."<<ticketAmount<<endl;
    cout<<"Food Amount     : Rs."<<foodAmount<<endl;
    cout<<"Beverage Amount : Rs."<<beverageAmount<<endl;
    cout<<"Subtotal        : Rs."<<subtotal<<endl;
    cout<<"CGST (9%)      : Rs."<<cgst<<endl;
    cout<<"SGST (9%)      : Rs."<<sgst<<endl;
    cout<<"Total Amount    : Rs."<<totalAmount<<endl;
    cout<<"============================================"<<endl;
    nextBookingId++;
}

void viewBookings(User &user) {
    bool found=false;
    cout<<endl;
    cout<<"============================================"<<endl;
    cout<<"              MY BOOKINGS"<<endl;
    cout<<"============================================"<<endl;
    for(Booking &booking:bookings) {
        if(booking.userId==user.userId) {
            found=true;
            Show* show=findShow(booking.showId);
            Movie* movie=nullptr;
            if(show!=nullptr) {
                movie=findMovie(show->movieId);
            }
            cout<<"Booking ID: "<<booking.bookingId<<endl;
            if(movie!=nullptr) {
                cout<<"Movie     : "<<movie->name<<endl;
            }
            if(show!=nullptr) {
                cout<<"Date      : "<<show->date<<endl;
                cout<<"Time      : "<<show->time<<endl;
                cout<<"Screen    : "<<show->screen<<endl;
            }
            cout<<"Seats     : ";
            for(string &seat:booking.seats) {
                cout<<seat<<" ";
            }
            cout<<endl;
            cout<<fixed<<setprecision(2);
            cout<<"Food      : Rs."<<booking.foodAmount<<endl;
            cout<<"Beverages : Rs."<<booking.beverageAmount<<endl;
            cout<<"Total     : Rs."<<booking.amount<<endl;
            cout<<"--------------------------------------------"<<endl;
        }
    }
    if(!found) {
        cout<<"You have no bookings."<<endl;
    }
}

void cancelBooking(User &user) {
    bool found=false;
    for(Booking &booking:bookings) {
        if(booking.userId==user.userId) {
            found=true;
            break;
        }
    }
    if(!found) {
        cout<<"You have no bookings to cancel."<<endl;
        return;
    }
    viewBookings(user);
    int bookingId;
    cout<<"Enter Booking ID to cancel: ";
    bookingId=getInteger();
    for(auto it=bookings.begin();it!=bookings.end();it++) {
        if(it->bookingId==bookingId&&it->userId==user.userId) {
            Show* show=findShow(it->showId);
            if(show!=nullptr) {
                for(string &seat:it->seats) {
                    show->cancelSeat(seat);
                }
            }
            bookings.erase(it);
            saveBookings();
            cout<<"Booking cancelled successfully."<<endl;
            return;
        }
    }
    cout<<"Invalid Booking ID."<<endl;
}

void customerMenu(User &user) {
    while(true) {
        cout<<endl;
        cout<<"============================================"<<endl;
        cout<<"               CUSTOMER MENU"<<endl;
        cout<<"============================================"<<endl;
        cout<<"1. View Movies"<<endl;
        cout<<"2. Book Ticket"<<endl;
        cout<<"3. View My Bookings"<<endl;
        cout<<"4. Cancel Ticket"<<endl;
        cout<<"5. Logout"<<endl;
        int choice;
        cout<<"Enter choice: ";
        choice=getInteger();
        if(choice==1) {
            displayMovies();
        } else if(choice==2) {
            bookTicket(user);
        } else if(choice==3) {
            viewBookings(user);
        } else if(choice==4) {
            cancelBooking(user);
        } else if(choice==5) {
            cout<<"Logged out successfully."<<endl;
            break;
        } else {
            cout<<"Invalid choice."<<endl;
        }
    }
}

int main() {
    loadMovies();
    loadScreens();
    loadShows();
    loadUpiSuffixes();
    loadFoods();
    loadBeverages();
    loadUsers();
    loadBookings();
    while(true)
    {
        cout<<endl;
        cout<<"============================================"<<endl;
        cout<<"       MOVIE THEATRE BOOKING SYSTEM"<<endl;
        cout<<"============================================"<<endl;
        cout<<"1. Register"<<endl;
        cout<<"2. Login"<<endl;
        cout<<"Enter choice: ";
        int choice;
        choice=getInteger();
        if(choice==1)
        {
            registerUser();
        }
        else if(choice==2)
        {
            User* user=login();
            if(user!=nullptr)
            {
                customerMenu(*user);
            }
        }
        else
        {
            cout<<"Invalid choice."<<endl;
        }
    }
    return 0;
}