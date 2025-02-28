#include <iostream>
#include <cassert>
#include <cstring>

class MedalRow {
    char country[4];
    int medals[3];

public:
    static const int GOLD{ 0 };
    static const int SILVER{ 1 };
    static const int BRONZE{ 2 };

    MedalRow(const char* countryP = nullptr, const int* medalsP = nullptr) {
        strcpy(country, countryP ? countryP : "NON");
        for (int i{ 0 }; i < 3; ++i) {
            medals[i] = medalsP ? medalsP[i] : 0;
        }
    }

    MedalRow& setCountry(const char* countryP) {
        if (countryP) {
            strcpy(country, countryP);
        }
        return *this;
    }

    const char* getCountry() const { return country; }

    int& operator[](int idx) {
        assert((idx >= 0 && idx < 3) && "Index out of range!");
        return medals[idx];
    }

    int operator[](int idx) const {
        assert((idx >= 0 && idx < 3) && "Index out of range!");
        return medals[idx];
    }

    // Дружественная функция для перегрузки оператора <<
    friend std::ostream& operator<<(std::ostream& os, const MedalRow& row);
};

// Перегрузка оператора << для MedalRow
std::ostream& operator<<(std::ostream& os, const MedalRow& row) {
    os << '[' << row.country << "]-( ";
    for (int i{ 0 }; i < 3; ++i) {
        os << row.medals[i];
        if (i < 2) { os << '\t'; }
    }
    os << " )\n";
    return os;
}

class MedalsTable {
    MedalRow* medalRows;
    int size;
    int capacity;

    int findCountry(const char* country) const {
        for (int i{ 0 }; i < size; ++i) {
            if (strcmp(medalRows[i].getCountry(), country) == 0) {
                return i;
            }
        }
        return -1;
    }

public:
    MedalsTable(int capacity = 10) : capacity{ capacity }, size{ 0 } {
        medalRows = new MedalRow[capacity];
    }

 
    MedalsTable(const MedalsTable& other) : capacity{ other.capacity }, size{ other.size } {
        medalRows = new MedalRow[capacity];
        for (int i = 0; i < size; ++i) {
            medalRows[i] = other.medalRows[i];
        }
    }

    // Оператор присваивания копированием
    MedalsTable& operator=(const MedalsTable& other) {
        if (this != &other) {
            delete[] medalRows;
            capacity = other.capacity;
            size = other.size;
            medalRows = new MedalRow[capacity];
            for (int i = 0; i < size; ++i) {
                medalRows[i] = other.medalRows[i];
            }
        }
        return *this;
    }

    // Конструктор перемещения
    MedalsTable(MedalsTable&& other) noexcept : capacity{ other.capacity }, size{ other.size }, medalRows{ other.medalRows } {
        other.medalRows = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // Оператор присваивания перемещением
    MedalsTable& operator=(MedalsTable&& other) noexcept {
        if (this != &other) {
            delete[] medalRows;
            capacity = other.capacity;
            size = other.size;
            medalRows = other.medalRows;
            other.medalRows = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

  
    ~MedalsTable() {
        delete[] medalRows;
    }

    MedalRow& operator[](const char* country) {
        int idx{ findCountry(country) };
        if (idx == -1) {
            assert(size < capacity && "Table is FULL!");
            idx = size++;
            medalRows[idx].setCountry(country);
        }
        return medalRows[idx];
    }

    const MedalRow& operator[](const char* country) const {
        int idx{ findCountry(country) };
        assert(idx != -1 && "Country not found on const table");
        return medalRows[idx];
    }

    // Дружественная функция для перегрузки оператора <<
    friend std::ostream& operator<<(std::ostream& os, const MedalsTable& table);
};

// Перегрузка оператора << для MedalsTable
std::ostream& operator<<(std::ostream& os, const MedalsTable& table) {
    for (int i{ 0 }; i < table.size; ++i) {
        os << table.medalRows[i];
    }
    return os;
}

int main() {
    MedalsTable mt1;
    std::cout << "Medals table #1:\n";
    mt1["UKR"][MedalRow::GOLD] = 14;
    mt1["UKR"][MedalRow::SILVER] = 5;
    mt1["HUN"][MedalRow::BRONZE] = 9;
    mt1["HUN"][MedalRow::GOLD] = 7;
    mt1["POL"][MedalRow::GOLD] = 4;
    mt1["POL"][MedalRow::SILVER] = 2;
    std::cout << mt1;

  
    std::cout << "\nMedals table #2 (copy of table #1):\n";
    MedalsTable mt2{ mt1 };
    std::cout << mt2;

    
    std::cout << "\nMedals table #3 (move from table #1):\n";
    MedalsTable mt3{ std::move(mt1) };
    std::cout << mt3;

   
    std::cout << "\nMedals table #1 after move:\n";
    std::cout << mt1;

    return 0;
}
