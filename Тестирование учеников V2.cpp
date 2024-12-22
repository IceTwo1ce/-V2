#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Структура для хранения данных пользователя
struct User {
    string username;
    string password;
    string role; // "admin" или "student"
};

// Функции для сохранения/загрузки пользователей
void saveUsersToFile(const vector<User>& users, const string& filename) {
    ofstream outfile(filename);
    if (outfile.is_open()) {
        for (const auto& user : users) {
            outfile << user.username << " " << user.password << " " << user.role << endl;
        }
        outfile.close();
    }
    else {
        cerr << "Невозможно открыть файл для сохранения пользователей: " << filename << endl;
    }
}

vector<User> loadUsersFromFile(const string& filename) {
    vector<User> users;
    ifstream infile(filename);
    if (infile.is_open()) {
        User user;
        while (infile >> user.username >> user.password >> user.role) {
            users.push_back(user);
        }
        infile.close();
    }
    return users;
}

// Класс для хранения и обработки заданий
class Task {
public:
    string question;
    vector<string> options;
    vector<int> correctAnswers;

    Task() {}
    Task(const string& q, const vector<string>& o, const vector<int>& ca)
        : question(q), options(o), correctAnswers(ca) {
    }
};

// Функции для работы с заданиями
void saveTasksToFile(const vector<Task>& tasks, const string& filename) {
    ofstream outfile(filename);
    if (outfile.is_open()) {
        for (const auto& task : tasks) {
            outfile << task.question << "\n";
            outfile << task.options.size() << "\n";
            for (const auto& option : task.options) {
                outfile << option << "\n";
            }
            outfile << task.correctAnswers.size() << "\n";
            for (const auto& answer : task.correctAnswers) {
                outfile << answer << "\n";
            }
        }
        outfile.close();
    }
    else {
        cerr << "Невозможно открыть файл для сохранения заданий." << endl;
    }
}

vector<Task> loadTasksFromFile(const string& filename) {
    vector<Task> tasks;
    ifstream infile(filename);
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            Task task;
            task.question = line;

            int optionsSize;
            if (getline(infile, line))
                optionsSize = stoi(line);
            else
                break;
            for (int i = 0; i < optionsSize; ++i) {
                if (getline(infile, line))
                    task.options.push_back(line);
                else
                    break;

            }

            int answersSize;
            if (getline(infile, line))
                answersSize = stoi(line);
            else
                break;

            for (int i = 0; i < answersSize; ++i) {
                if (getline(infile, line))
                    task.correctAnswers.push_back(stoi(line));
                else
                    break;
            }


            tasks.push_back(task);
        }
        infile.close();
    }
    else {
        cerr << "Невозможно открыть файл для чтения заданий." << endl;
    }
    return tasks;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    time_t start, finish;
    start = clock();

    string usersFilenameStudents = "Students.txt";
    string usersFilenameAdmins = "Admins.txt";
    string tasksFilename = "tasks.txt";


    vector<User> students = loadUsersFromFile(usersFilenameStudents);
    vector<User> admins = loadUsersFromFile(usersFilenameAdmins);
    vector<Task> tasks = loadTasksFromFile(tasksFilename);



    string rpassword;
    string upassword;
    string user;
    char choice;
    string role;
    bool loggedIn = false;
    string pinCode;
    const string adminPin = "1488";


    while (true) {
        if (!loggedIn) {
            cout << "Вы попали в систему тестирования студентов!\n";
            cout << "1. Войти\n";
            cout << "2. Зарегистрироваться\n";
            cout << "3. Выйти из программы\n";
            cout << "Выберите действие (1/2/3, или 'назад' для выхода): ";
            cin >> choice;
            cin.ignore();

            if (choice == 'н' || choice == 'H' || choice == 'b' || choice == 'B')
            {
                cout << "Выход из системы.\n";
                break;
            }
            if (choice == '1') {
                cout << "Введите свой логин (или 'назад' для выхода в меню):\n";
                getline(cin, user);

                if (user == "назад") {
                    continue;
                }
                bool userFound = false;
                for (const auto& registeredUser : students) {
                    if (user == registeredUser.username) {
                        rpassword = registeredUser.password;
                        role = registeredUser.role;
                        userFound = true;
                        break;
                    }
                }
                if (!userFound)
                {
                    for (const auto& registeredUser : admins) {
                        if (user == registeredUser.username) {
                            rpassword = registeredUser.password;
                            role = registeredUser.role;
                            userFound = true;
                            break;
                        }
                    }
                }



                if (!userFound) {
                    cout << "Такого логина не существует, попробуйте снова!\n";
                    continue;
                }

                cout << "Введите ваш пароль, " << user << "! (или 'назад' для выхода в меню)\n";

                int x = 5;
                while (true) {
                    getline(cin, upassword);

                    if (upassword == "назад") {
                        break;
                    }

                    if (upassword == rpassword) {
                        break;
                    }
                    else {
                        cout << "Пароль неправильный, повторите снова\n";
                        int z = 0;

                        while (z < x) {
                            Sleep(1000);
                            cout << x - z << " секунд до разблокировки! " << endl;
                            z++;
                        }
                        cout << "Вводите внимательнее:\n";

                        x *= 3;
                    }
                }
                if (upassword == "назад") continue;
                finish = clock();
                double dif = difftime(finish, start);
                cout << "Добро пожаловать, " << user << "! \n";
                if (role == "admin") {
                    cout << "Вы вошли как администратор.\n";
                }
                else {
                    cout << "Вы вошли как ученик.\n";
                }
                printf("Вы вошли в систему за %.2lf секунды", dif / 1000);
                if (dif < 4000) {
                    cout << " это было реально быстро!\n";
                }
                else {
                    cout << " можно было и побыстрей!\n";
                }
                loggedIn = true;

            }
            else if (choice == '2') {
                while (true) {
                    cout << "Вы хотите зарегистрироваться как (1 - Администратор, 2 - Студент, или 'назад' для выхода в меню)?\n";
                    char registerChoice;
                    cin >> registerChoice;
                    cin.ignore();

                    if (registerChoice == 'н' || registerChoice == 'H' || registerChoice == 'b' || registerChoice == 'B') {
                        break;
                    }

                    if (registerChoice == '1') {
                        role = "admin";
                        while (true) {
                            cout << "Введите пин-код для создания администратора: (или 'назад' для выхода в меню): " << endl;
                            getline(cin, pinCode);
                            if (pinCode == "назад") {
                                break;
                            }
                            if (pinCode == adminPin) {
                                break;
                            }
                            cout << "Неверный пин-код. Попробуйте снова или введите 'назад' для возврата: " << endl;
                        }
                        if (pinCode == "назад") continue;

                    }
                    else if (registerChoice == '2') {
                        role = "student";
                    }
                    else {
                        cout << "Некорректный выбор. Пожалуйста, выберите 1 или 2." << endl;
                        continue;
                    }
                    cout << "Введите желаемый логин (или 'назад' для выхода в меню):\n";
                    getline(cin, user);

                    if (user == "назад") {
                        break;
                    }
                    bool userNameExists = false;
                    for (const auto& regUser : students) {
                        if (user == regUser.username) {
                            cout << "Это имя уже занято! Попробуйте другое!\n";
                            userNameExists = true;
                            break;
                        }
                    }
                    if (!userNameExists)
                    {
                        for (const auto& regUser : admins) {
                            if (user == regUser.username) {
                                cout << "Это имя уже занято! Попробуйте другое!\n";
                                userNameExists = true;
                                break;
                            }
                        }
                    }
                    if (userNameExists) continue;

                    cout << "Введите желаемый пароль (или 'назад' для выхода в меню):\n";
                    getline(cin, upassword);
                    if (upassword == "назад") {
                        break;
                    }
                    User newUser = { user, upassword, role };
                    if (role == "admin")
                    {
                        admins.push_back(newUser);
                        saveUsersToFile(admins, usersFilenameAdmins);

                    }
                    else if (role == "student")
                    {
                        students.push_back(newUser);
                        saveUsersToFile(students, usersFilenameStudents);
                    }

                    cout << "Вы успешно зарегистрировали пользователя, " << user << "!\n";
                    loggedIn = true;
                    break;
                }

            }
            else if (choice == '3')
            {
                cout << "Выход из программы!\n";
                break;
            }
            else {
                cout << "Некорректный выбор, попробуйте снова!\n";
            }
        }
        else
        {
            if (role == "student")
            {
                cout << "\n1. Решать задания\n";
                cout << "2. Удалить аккаунт\n";
                cout << "3. Выйти из аккаунта\n";
                cout << "Выберите действие (1/2/3): ";

                cin >> choice;
                cin.ignore();

                if (choice == '1')
                {
                    while (true) {
                        cout << "\n1. Посмотреть задания\n";
                        cout << "2. Решить задание\n";
                        cout << "3. Выйти из режима заданий\n";
                        int taskChoice;
                        cin >> taskChoice;
                        cin.ignore();

                        if (taskChoice == 1) {
                            for (size_t i = 0; i < tasks.size(); ++i) {
                                cout << i + 1 << ". " << tasks[i].question << "\n";
                            }
                        }
                        else if (taskChoice == 2) {
                            int task_index;
                            cout << "Введите номер задания: ";
                            cin >> task_index;
                            cin.ignore();

                            if (task_index > 0 && task_index <= tasks.size()) {
                                const auto& task = tasks[task_index - 1];
                                cout << task.question << "\n";
                                for (size_t i = 0; i < task.options.size(); ++i) {
                                    cout << i + 1 << ". " << task.options[i] << "\n";
                                }
                                int answer;
                                cout << "Введите ваш ответ (1-" << task.options.size() << "): ";
                                cin >> answer;
                                cin.ignore();


                                bool isCorrect = false;
                                for (int correctAnswer : task.correctAnswers)
                                {
                                    if (answer - 1 == correctAnswer)
                                    {
                                        isCorrect = true;
                                        break;
                                    }
                                }
                                if (isCorrect)
                                {
                                    cout << "Правильно!\n";
                                }
                                else {
                                    cout << "Неправильно.\n";
                                }
                            }
                            else {
                                cout << "Неверный номер задания.\n";
                            }
                        }
                        else if (taskChoice == 3) {
                            break;
                        }
                    }
                }
                else if (choice == '2')
                {
                    char confirm;
                    cout << "Вы уверены, что хотите удалить аккаунт? (y/n): ";
                    cin >> confirm;
                    cin.ignore();
                    if (confirm == 'y' || confirm == 'Y') {
                        for (auto it = students.begin(); it != students.end(); ++it) {
                            if (it->username == user) {
                                students.erase(it);
                                break;
                            }
                        }
                        saveUsersToFile(students, usersFilenameStudents);
                        cout << "Аккаунт успешно удален.\n";
                        loggedIn = false;
                        break;
                    }
                    else
                    {
                        cout << "Удаление отменено!\n";
                    }

                }

                else if (choice == '3')
                {
                    loggedIn = false;
                    cout << "Вы вышли из аккаунта.\n";
                    break;
                }

                else {
                    cout << "Некорректный выбор, попробуйте снова!\n";
                }
            }
            else if (role == "admin")
            {
                cout << "\n1. Создать задание\n";
                cout << "2. Посмотреть задания\n";
                cout << "3. Посмотреть список обычных пользователей\n";
                cout << "4. Посмотреть список администраторов\n";
                cout << "5. Удалить аккаунт\n";
                cout << "6. Выйти из аккаунта\n";
                cout << "Выберите действие (1/2/3/4/5/6): ";

                cin >> choice;
                cin.ignore();


                if (choice == '1') {
                    while (true)
                    {
                        string question;
                        vector<string> options;
                        vector<int> correctAnswers;
                        int optionsCount, correctAnswersCount;

                        cout << "Введите вопрос: ";
                        getline(cin, question);

                        cout << "Введите количество вариантов ответа (не менее 2): ";
                        cin >> optionsCount;
                        cin.ignore();


                        if (optionsCount < 2)
                        {
                            cout << "Количество вариантов ответа должно быть не менее 2!\n";
                            continue;
                        }


                        for (int i = 0; i < optionsCount; ++i) {
                            string option;
                            cout << "Введите вариант ответа " << i + 1 << ": ";
                            getline(cin, option);
                            options.push_back(option);
                        }
                        cout << "Введите количество правильных ответов: ";
                        cin >> correctAnswersCount;
                        cin.ignore();



                        if (optionsCount < 3 && correctAnswersCount > 1)
                        {
                            cout << "Если вариантов ответа меньше 3, то правильный вариант ответа должен быть 1!\n";
                            continue;
                        }

                        for (int i = 0; i < correctAnswersCount; ++i) {
                            int correctAnswer;
                            cout << "Введите номер правильного ответа " << i + 1 << " (1-" << optionsCount << "): ";
                            cin >> correctAnswer;
                            cin.ignore();
                            if (correctAnswer > 0 && correctAnswer <= optionsCount)
                            {
                                correctAnswers.push_back(correctAnswer - 1);
                            }
                            else
                            {
                                cout << "Неверный номер ответа! попробуйте снова!" << endl;
                                i--;
                            }
                        }

                        tasks.push_back(Task(question, options, correctAnswers));

                        char cont;
                        cout << "Продолжить создание заданий? (y/n): ";
                        cin >> cont;
                        cin.ignore();
                        if (cont != 'y' && cont != 'Y')
                        {
                            break;
                        }
                    }
                }
                else if (choice == '2') {
                    for (size_t i = 0; i < tasks.size(); ++i) {
                        cout << i + 1 << ". " << tasks[i].question << "\n";
                    }
                }
                else if (choice == '3')
                {
                    cout << "Список пользователей:\n";
                    for (const auto& user : students)
                    {
                        cout << "- " << user.username << "\n";
                    }
                }
                else if (choice == '4') {
                    string pin;
                    cout << "Введите пинкод: ";
                    getline(cin, pin);
                    if (pin == "1234") {
                        cout << "Список администраторов:\n";
                        for (const auto& user : admins)
                        {
                            cout << "- " << user.username << "\n";
                        }
                    }
                    else {
                        cout << "Неправильный пинкод!\n";
                    }
                }
                else if (choice == '5')
                {
                    char confirm;
                    cout << "Вы уверены, что хотите удалить аккаунт? (y/n): ";
                    cin >> confirm;
                    cin.ignore();
                    if (confirm == 'y' || confirm == 'Y') {
                        for (auto it = admins.begin(); it != admins.end(); ++it) {
                            if (it->username == user) {
                                admins.erase(it);
                                break;
                            }
                        }
                        saveUsersToFile(admins, usersFilenameAdmins);
                        cout << "Аккаунт успешно удален.\n";
                        loggedIn = false;
                        break;
                    }
                    else
                    {
                        cout << "Удаление отменено!\n";
                    }
                }
                else if (choice == '6')
                {
                    loggedIn = false;
                    cout << "Вы вышли из аккаунта.\n";
                    break;
                }
                else {
                    cout << "Некорректный выбор, попробуйте снова!\n";
                }
            }
        }
    }
    saveTasksToFile(tasks, tasksFilename);
    return 0;
}