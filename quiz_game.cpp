// Task 1: Online Quiz Game (console)
// Features: registration/login, multiple topics, scoring, correct answers review, leaderboard
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

struct Question {
    string topic, text;
    vector<string> options;
    int answer; // 0-based index
};

const string USERS_FILE = "users.txt";
const string SCORES_FILE = "scores.txt";

vector<Question> loadQuestions() {
    return {
        {"Science", "What is the chemical symbol for water?", {"H2O", "CO2", "O2", "NaCl"}, 0},
        {"Science", "Which planet is known as the Red Planet?", {"Venus", "Mars", "Jupiter", "Saturn"}, 1},
        {"Science", "What gas do plants absorb from the air?", {"Oxygen", "Nitrogen", "Carbon dioxide", "Helium"}, 2},
        {"Computers", "What does CPU stand for?", {"Central Processing Unit", "Computer Personal Unit", "Central Program Utility", "Control Process Unit"}, 0},
        {"Computers", "Which data structure uses LIFO order?", {"Queue", "Stack", "Array", "Tree"}, 1},
        {"Computers", "Which number system does a computer use internally?", {"Decimal", "Octal", "Binary", "Roman"}, 2},
        {"Geography", "What is the capital of Pakistan?", {"Karachi", "Lahore", "Islamabad", "Peshawar"}, 2},
        {"Geography", "Which is the largest ocean?", {"Atlantic", "Indian", "Arctic", "Pacific"}, 3},
        {"Geography", "Mount Everest lies in which mountain range?", {"Andes", "Himalayas", "Alps", "Rockies"}, 1},
        {"Math", "What is 12 x 12?", {"122", "144", "124", "132"}, 1},
        {"Math", "What is the square root of 81?", {"7", "8", "9", "10"}, 2},
        {"Math", "Value of pi (approx.)?", {"3.14", "2.71", "1.62", "3.41"}, 0},
    };
}

map<string,string> loadUsers() {
    map<string,string> u; ifstream f(USERS_FILE); string n, p;
    while (f >> n >> p) u[n] = p;
    return u;
}

bool registerUser() {
    auto users = loadUsers();
    string name, pass;
    cout << "Choose username: "; cin >> name;
    if (users.count(name)) { cout << "Username already taken.\n"; return false; }
    cout << "Choose password: "; cin >> pass;
    ofstream(USERS_FILE, ios::app) << name << " " << pass << "\n";
    cout << "Registration successful!\n";
    return true;
}

string loginUser() {
    auto users = loadUsers();
    string name, pass;
    cout << "Username: "; cin >> name;
    cout << "Password: "; cin >> pass;
    if (users.count(name) && users[name] == pass) { cout << "Welcome, " << name << "!\n"; return name; }
    cout << "Invalid credentials.\n";
    return "";
}

void showLeaderboard() {
    ifstream f(SCORES_FILE); string n, t; int s, tot;
    vector<tuple<int,string,string>> rows;
    while (f >> n >> t >> s >> tot) rows.push_back({s, n, t});
    sort(rows.rbegin(), rows.rend());
    cout << "\n--- Leaderboard ---\n";
    if (rows.empty()) cout << "No scores yet.\n";
    int rank = 1;
    for (auto &r : rows) {
        cout << rank++ << ". " << get<1>(r) << " [" << get<2>(r) << "] - " << get<0>(r) << "\n";
        if (rank > 5) break;
    }
}

void playQuiz(const string &user) {
    auto all = loadQuestions();
    vector<string> topics;
    for (auto &q : all) if (find(topics.begin(), topics.end(), q.topic) == topics.end()) topics.push_back(q.topic);

    cout << "\nChoose a topic:\n";
    for (size_t i = 0; i < topics.size(); i++) cout << " " << i + 1 << ". " << topics[i] << "\n";
    cout << " " << topics.size() + 1 << ". All topics\nChoice: ";
    int c; cin >> c;
    vector<Question> qs;
    for (auto &q : all)
        if (c == (int)topics.size() + 1 || (c >= 1 && c <= (int)topics.size() && q.topic == topics[c - 1])) qs.push_back(q);
    if (qs.empty()) { cout << "Invalid choice.\n"; return; }

    vector<int> given;
    int score = 0;
    for (size_t i = 0; i < qs.size(); i++) {
        cout << "\nQ" << i + 1 << " [" << qs[i].topic << "]: " << qs[i].text << "\n";
        for (size_t j = 0; j < qs[i].options.size(); j++) cout << "  " << j + 1 << ") " << qs[i].options[j] << "\n";
        int a = 0;
        do { cout << "Your answer (1-" << qs[i].options.size() << "): "; cin >> a; } while (a < 1 || a > (int)qs[i].options.size());
        given.push_back(a - 1);
        if (a - 1 == qs[i].answer) score++;
    }

    cout << "\n===== QUIZ RESULTS =====\n";
    for (size_t i = 0; i < qs.size(); i++) {
        bool ok = given[i] == qs[i].answer;
        cout << "Q" << i + 1 << ": " << qs[i].text << "\n"
             << "   Your answer   : " << qs[i].options[given[i]] << (ok ? "  [Correct]" : "  [Wrong]") << "\n";
        if (!ok) cout << "   Correct answer: " << qs[i].options[qs[i].answer] << "\n";
    }
    cout << "\nScore: " << score << "/" << qs.size() << " (" << (100 * score / (int)qs.size()) << "%)\n";
    string label = (c >= 1 && c <= (int)topics.size()) ? topics[c - 1] : "All";
    ofstream(SCORES_FILE, ios::app) << user << " " << label << " " << score << " " << qs.size() << "\n";
}

int main() {
    string user;
    while (true) {
        if (user.empty()) {
            cout << "\n=== ONLINE QUIZ GAME ===\n1. Register\n2. Login\n3. Exit\nChoice: ";
            int c; if (!(cin >> c)) break;
            if (c == 1) registerUser();
            else if (c == 2) user = loginUser();
            else break;
        } else {
            cout << "\n--- Menu (" << user << ") ---\n1. Play quiz\n2. Leaderboard\n3. Logout\nChoice: ";
            int c; if (!(cin >> c)) break;
            if (c == 1) playQuiz(user);
            else if (c == 2) showLeaderboard();
            else user.clear();
        }
    }
    cout << "Goodbye!\n";
}
