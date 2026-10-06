// oop1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <clocale>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

struct Train {
    int train_number;
    char destination[30];
    int departure_hour;
    int departure_min;
    int travel_hour;
    int travel_min;
};

void save_trains_to_file(const Train trains[], int count, const char* filename = "INFO.DAT") {
    std::ofstream fout(filename, std::ios::binary);
    if (!fout.is_open()) {
        std::cerr << "Помилка відкриття файлу для запису!\n";
        return;
    }

    fout.write(reinterpret_cast<const char*>(trains), count * sizeof(Train));
}

int load_trains_from_file(Train trains[], int max_count, const char* filename = "INFO.DAT") {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        std::cerr << "Помилка відкриття файлу для читання!\n";
        return 0;
    }

    int count = 0;
    while (count < max_count && fin.read(reinterpret_cast<char*>(&trains[count]), sizeof(Train))) {
        count++;
    }

    return count;
}

void print_train(const Train& train) {
    std::cout << std::left << std::setw(6) << train.train_number << " "
        << std::setw(20) << train.destination << " "
        << std::right << std::setfill('0')
        << std::setw(2) << train.departure_hour << ":"
        << std::setw(2) << train.departure_min << "  "
        << std::setfill(' ')
        << std::setw(2) << train.travel_hour << "г "
        << std::setfill('0') << std::setw(2) << train.travel_min << "хв\n"
        << std::setfill(' ');
}

void print_trains(const Train trains[], int count) {
    for (int i = 0; i < count; ++i) {
        print_train(trains[i]);
    }
}

int main() {
    // Налаштування української локалізації та кодування консолі
    std::system("chcp 65001 > nul");
    std::setlocale(LC_ALL, "");

    constexpr int MAX_TRAINS = 100;
    constexpr int CUTOFF_HOUR = 21;
    constexpr int CUTOFF_MIN = 0;

    Train trains[MAX_TRAINS];
    int train_count = 0;

    std::cout << "Кількість поїздів: ";
    if (!(std::cin >> train_count) || train_count <= 0 || train_count > MAX_TRAINS) {
        std::cerr << "Некоректна кількість поїздів або перевищено ліміт (" << MAX_TRAINS << ").\n";
        return 1;
    }

    for (int i = 0; i < train_count; ++i) {
        std::cout << "\nПоїзд " << i + 1 << "\n";

        std::cout << "Номер: ";
        std::cin >> trains[i].train_number;

        std::cout << "Станція призначення: ";
        std::cin >> trains[i].destination;

        std::cout << "Час відправлення (год хв): ";
        std::cin >> trains[i].departure_hour >> trains[i].departure_min;

        std::cout << "Час у дорозі (год хв): ";
        std::cin >> trains[i].travel_hour >> trains[i].travel_min;
    }

    save_trains_to_file(trains, train_count);

    Train trains_from_file[MAX_TRAINS];
    int loaded_count = load_trains_from_file(trains_from_file, MAX_TRAINS);

    std::cout << "\n=== Усі поїзди з файлу ===\n";
    print_trains(trains_from_file, loaded_count);

    std::cout << "\n=== Поїзди не пізніше 21:00 ===\n";
    for (int i = 0; i < loaded_count; ++i) {
        int dep_minutes = trains_from_file[i].departure_hour * 60 + trains_from_file[i].departure_min;
        int cutoff_minutes = CUTOFF_HOUR * 60 + CUTOFF_MIN;

        if (dep_minutes <= cutoff_minutes) {
            print_train(trains_from_file[i]);
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
