#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <cctype>
#include <algorithm>

using namespace std;
using namespace chrono;

/* ---------- STRUCTURES ---------- */

struct User
{
    string username;
    string password;
};

struct Question
{
    string question;
    string optA, optB, optC, optD;
    char correct;
};

/* ---------- FILE NAMES ---------- */

string ADMIN_FILE = "admin.txt";
string TEACHER_FILE = "teacher.txt";
string STUDENT_FILE = "student.txt";
string RESULT_FILE = "result.txt";

string QUIZ_MATH = "quiz_math.txt";
string QUIZ_SCIENCE = "quiz_science.txt";
string QUIZ_COMPUTER = "quiz_computer.txt";

/* ---------- FUNCTION DECLARATIONS ---------- */

bool login(string fileName);
void registerUser(string fileName);
void createQuiz();
void attemptQuiz(string studentName);
string selectSubject();
char calculateGrade(int score, int total);

/* ---------- SUBJECT SELECTION ---------- */

string selectSubject()
{
    int choice;
    cout << "\nSelect Subject:\n";
    cout << "1. Mathematics\n";
    cout << "2. Computer\n";
    cout << "3. Science\n";
    cout << "Choice: ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) return QUIZ_MATH;
    if (choice == 2) return QUIZ_COMPUTER;
    return QUIZ_SCIENCE;
}

/* ---------- GRADE ---------- */

char calculateGrade(int score, int total)
{
    int percent = (score * 100) / total;
    if (percent >= 80) return 'A';
    if (percent >= 60) return 'B';
    if (percent >= 40) return 'C';
    return 'F';
}

/* ---------- LOGIN ---------- */

bool login(string fileName)
{
    User u;
    string user, pass;

    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    ifstream file(fileName);
    while (file >> u.username >> u.password)
    {
        if (u.username == user && u.password == pass)
            return true;
    }
    return false;
}

/* ---------- REGISTER ---------- */

void registerUser(string fileName)
{
    User u;
    cout << "New username: ";
    cin >> u.username;
    cout << "Password: ";
    cin >> u.password;

    ofstream file(fileName, ios::app);
    file << u.username << " " << u.password << endl;

    cout << "User registered successfully!\n";
}

/* ---------- CREATE QUIZ ---------- */

void createQuiz()
{
    string quizFile = selectSubject();
    int n;

    cout << "Number of questions: ";
    cin >> n;
    cin.ignore();

    ofstream quiz(quizFile);
    quiz << n << endl;

    for (int i = 0; i < n; i++)
    {
        Question q;

        cout << "\nEnter Question: ";
        getline(cin, q.question);

        cout << "Option A: ";
        getline(cin, q.optA);
        cout << "Option B: ";
        getline(cin, q.optB);
        cout << "Option C: ";
        getline(cin, q.optC);
        cout << "Option D: ";
        getline(cin, q.optD);

        cout << "Correct option (A/B/C/D): ";
        cin >> q.correct;
        cin.ignore();

        quiz << q.question << endl
             << q.optA << endl
             << q.optB << endl
             << q.optC << endl
             << q.optD << endl
             << q.correct << endl;
    }

    cout << "Quiz created successfully!\n";
}

/* ---------- ATTEMPT QUIZ ---------- */

void attemptQuiz(string studentName)
{
    string quizFile = selectSubject();
    ifstream quiz(quizFile);

    if (!quiz)
    {
        cout << "Quiz not available!\n";
        return;
    }

    int total;
    quiz >> total;
    quiz.ignore();

    Question q[50];

    for (int i = 0; i < total; i++)
    {
        getline(quiz, q[i].question);
        getline(quiz, q[i].optA);
        getline(quiz, q[i].optB);
        getline(quiz, q[i].optC);
        getline(quiz, q[i].optD);
        quiz >> q[i].correct;
        quiz.ignore();
    }

    srand(time(0));
    for (int i = 0; i < total; i++)
    {
        int j = rand() % total;
        swap(q[i], q[j]);
    }

    int score = 0;
    int timeLimit = 60; // total quiz time
    auto startTime = steady_clock::now();

    for (int i = 0; i < total; i++)
    {
        auto currentTime = steady_clock::now();
        int usedTime = duration_cast<seconds>(currentTime - startTime).count();

        if (usedTime >= timeLimit)
        {
            cout << "\nTime is up!\n";
            break;
        }

        cout << "\n" << q[i].question << endl;
        cout << "A. " << q[i].optA << endl;
        cout << "B. " << q[i].optB << endl;
        cout << "C. " << q[i].optC << endl;
        cout << "D. " << q[i].optD << endl;

        char ans;
        cout << "Your answer: ";
        cin >> ans;

        if (toupper(ans) == toupper(q[i].correct))
            score++;
    }

    char grade = calculateGrade(score, total);

    ofstream res(RESULT_FILE, ios::app);
    res << studentName << " Score: " << score << "/" << total
        << " Grade: " << grade << endl;

    cout << "\nFinal Score: " << score << "/" << total;
    cout << "\nGrade: " << grade << endl;
}

/* ---------- MAIN ---------- */

int main()
{
    ofstream admin(ADMIN_FILE, ios::app);
    admin << "admin admin\n";
    admin.close();

    int choice;

    do
    {
        cout << "\n--- SMART QUIZ SYSTEM ---\n";
        cout << "1. Admin Login\n";
        cout << "2. Teacher Login\n";
        cout << "3. Student Login\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            if (login(ADMIN_FILE))
            {
                int ch;
                cout << "1. Register Teacher\n2. Register Student\n";
                cin >> ch;

                if (ch == 1) registerUser(TEACHER_FILE);
                else if (ch == 2) registerUser(STUDENT_FILE);
            }
        }
        else if (choice == 2)
        {
            if (login(TEACHER_FILE))
                createQuiz();
        }
        else if (choice == 3)
        {
        if (login(STUDENT_FILE)) {
                 string name;
                 cout << "Enter your username to save result: ";
                 cin >> name;
                 attemptQuiz(name);
    }
}


    } while (choice != 0);

    return 0;
}
