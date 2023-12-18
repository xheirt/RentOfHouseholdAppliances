#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <locale>
#include <iomanip>

using namespace std;

//              ╔═════════════════════════════╗
//              ║      Класс для техники      ║
//              ╚═════════════════════════════╝

class RentableItem {
public:
    RentableItem(string name, string manufacturer, int year, string color, int inventoryNumber, double rentalPrice)
        : name(name), manufacturer(manufacturer), year(year), color(color), inventoryNumber(inventoryNumber), rentalPrice(rentalPrice){}

    RentableItem() = default;

    virtual ~RentableItem(){}
    
    virtual double calculateRentalRevenue(int rentalPeriod) const {
        return rentalPrice * rentalPeriod;
    }

    virtual void displayInformation() const {
        cout << "|" << std::setw(25) << name << "|"
            << std::setw(20) << manufacturer << "|"
            << std::setw(8) << year << "|"
            << std::setw(15) << color << "|"
            << std::setw(15) << inventoryNumber << "|"
            << std::setw(10) << std::fixed << std::setprecision(2) << rentalPrice << "|"
            << "\n|-------------------------|--------------------|--------|---------------|---------------|----------|";
    }

    int getInventoryNumber() const {
        return inventoryNumber;
    }

    int getRentalPeriod() const {
        return rentalPeriod;
    }
    // Добавим перегрузку оператора '<' для сравнения предметов БТ по сроку аренды
    bool operator<(const RentableItem& other) const {
        return rentalPeriod < other.rentalPeriod;
    }   

private:
    string name, manufacturer, color;
    int year, inventoryNumber, rentalPeriod;
    double rentalPrice;
};

//              ╔═════════════════════════════╗
//              ║      Техника для дома       ║
//              ╚═════════════════════════════╝

class HomeAppliance : public RentableItem {
public:
    HomeAppliance(string name, string manufacturer, int year, string color, int inventoryNumber, double rentalPrice)
        : RentableItem(name, manufacturer, year, color, inventoryNumber, rentalPrice){}
    
};

//              ╔═════════════════════════════╗
//              ║   Техника для организации   ║
//              ╚═════════════════════════════╝

class OrganizationAppliance : public RentableItem {
public:
    OrganizationAppliance(string name, string manufacturer, int year, string color, int inventoryNumber, double rentalPrice)
        : RentableItem(name, manufacturer, year, color, inventoryNumber, rentalPrice) {}

    // Дополнительные характеристики и функциональность для бытовой техники для организации
};

//              ╔═════════════════════════════╗
//              ║        Класс Клиента        ║
//              ╚═════════════════════════════╝

class Client {
public:
    Client(string name, string type) : name(name), type(type) {}

    virtual ~Client() {}

    virtual void displayInformation() const {
        cout << "Имя клиента: " << name << "\nТип клиента: " << type << "\n";
    }

private:
    string name, type;
};

//              ╔═════════════════════════════╗
//              ║          Физ лица           ║
//              ╚═════════════════════════════╝

class IndividualClient : public Client {
public:
    IndividualClient(string name) : Client(name, "Individual") {}

    // Дополнительные характеристики и функциональность для физического лица
};

//              ╔═════════════════════════════╗
//              ║           Юр лица           ║
//              ╚═════════════════════════════╝

class CorporateClient : public Client {
public:
    CorporateClient(string name) : Client(name, "Corporate") {}

    // Дополнительные характеристики и функциональность для юридического лица
};

//              ╔═════════════════════════════╗
//              ║        Центр проката        ║
//              ╚═════════════════════════════╝

class RentalCenter {
public:

//-----------Проверка наличия предмета БТ в центре-----------\\

    void rentItem(RentableItem* item, Client* client, int rentalPeriod) {
        auto it = find_if(rentedItems.begin(), rentedItems.end(),
            [item](const pair<RentableItem*, int>& pair) {
                return pair.first->getInventoryNumber() == item->getInventoryNumber();
            });

        if (it == rentedItems.end()) {
            // если предмет не арендован
            rentedItems.push_back(make_pair(item, rentalPeriod));
            clients.push_back(client);
            //cout << "Товар успешно сдан в аренду!\n";
        }
        else {
            cout << "Товар уже сдан в аренду!\n";
        }
    }

    void returnItem(RentableItem* item, int actualRentalPeriod) {

//---------Ищем арендованный предмет по инвентарному номеру---------\\

        auto it = find_if(rentedItems.begin(), rentedItems.end(),
            [item](const pair<RentableItem*, int>& pair) {
                return pair.first->getInventoryNumber() == item->getInventoryNumber();
            });

        //если предмет найден в списке аренды
        if (it != rentedItems.end()) {
            //рассчитываем доход от аренды без штрафа
            double rentalRevenue = it->first->calculateRentalRevenue(it->second);
            double penalty = 0.0;

            // Проверка на превышение времени аренды

            if (actualRentalPeriod > it->second) {
                // Рассчитываем штраф как удвоенную стоимость аренды за каждый лишний день
                penalty = (actualRentalPeriod - it->second) * (2 * it->first->calculateRentalRevenue(1));
            }

            // Рассчитываем общий доход
            double totalRevenue = rentalRevenue + penalty;

            cout << "Товар успешно возвращен!\n";
            cout << "Доход от аренды: " << rentalRevenue << "\nШтраф: " << penalty << "\nОбщий доход: " << totalRevenue << "\n";

            // Здесь можно добавить возможность продления аренды
            char extendRent;
            cout << "Хотите продлить аренду? (y/n): ";
            cin >> extendRent;

            if (extendRent == 'y' || extendRent == 'Y') {
                int additionalDays;
                cout << "Введите количество дополнительных дней аренды: ";
                cin >> additionalDays;
                it->second += additionalDays;
                cout << "Аренда успешно продлена на " << additionalDays << " дней.\n";
            }

            rentedItems.erase(it);
        }
        else {
            cout << "Товара нет в списке аренды!\n";
        }
    }

    const vector<pair<RentableItem*, int>>& getRentedItems() const {
        return rentedItems;
    }

    double calculateTotalRevenue() const {
        double totalRevenue = 0.0;

        for (const auto& pair : rentedItems) {
            totalRevenue += pair.first->calculateRentalRevenue(pair.second);
        }

        return totalRevenue;
    }

private:
    vector<pair<RentableItem*, int>> rentedItems; // Предмет аренды и срок аренды
    vector<Client*> clients;                      // Список клиентов
};






// Пример сохранения и загрузки данных
template <typename T>
void saveData(const std::vector<T>& data, const std::string& filename) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        for (const auto& item : data) {
            file << item;
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

template <typename T>
void loadData(std::vector<T>& data, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        while (!file.eof()) {
            T item;
            file >> item;
            data.push_back(item);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}


//ищет предмет по инвентарному номеру в списке арендованных предметов
RentableItem* findItemByInventoryNumber(RentalCenter& rentalCenter, int inventoryNumber) {
    for (const auto& pair : rentalCenter.getRentedItems()) {
        if (pair.first->getInventoryNumber() == inventoryNumber) {
            return pair.first;
        }
    }
    return nullptr;
}

RentableItem* findItemByInventoryNumberForRent(RentalCenter& rentalCenter, int inventoryNumber) {
    // Поиск предмета в центре БТ, который не находится в аренде
    // Здесь вам нужно реализовать логику поиска предмета для аренды
    // Может быть, у вас есть вектор всех предметов в центре, и вы исключаете из него те, которые уже арендованы
    // Это зависит от структуры вашего приложения
    // Пример:
    for (const auto& pair : rentalCenter.getRentedItems()) {
        if (pair.first->getInventoryNumber() == inventoryNumber) {
            return nullptr; // Предмет уже в аренде
        }
    }

    // Возвращает предмет, который можно арендовать
    return findItemByInventoryNumber(rentalCenter, inventoryNumber);
}


int main()
{
    setlocale(LC_ALL, "ru");

    RentalCenter rentalC;

    vector<RentableItem> rentableItems;

    ofstream outputFile("data.txt");
    if (outputFile.is_open()) {
        outputFile << "WashingMachine Samsung 2021 white 123456 20.0\n";
        outputFile << "Holodilnik LG 2022 gray 789012 25.0\n";
        outputFile << "Posydamoishnayamachine Bosch 2020 super_gray 654321 18.0\n";
        outputFile << "Mikrovolnovka Panasonic 2023 black 987654 15.0\n";

        outputFile.close();
    }
    else {
        cout << "Не удалось открыть файл для записи" << endl;
    }

    
    ifstream inputFile("data.txt");
    if (inputFile.is_open()) {
        string name, manufacturer, color;
        int year, inventoryNumber;
        double rentalPrice;

        cout << "Предмет БТ |Производитель БТ|Год|Цвет предмета|Инвентарный номер|Цена за 1 день|" << endl;
        while (inputFile >> name >> manufacturer >> year >> color >> inventoryNumber >> rentalPrice) {
            HomeAppliance appliance(name, manufacturer, year, color, inventoryNumber, rentalPrice);
            appliance.displayInformation();
            cout << endl;  // добавьте переход на новую строку между объектами
        }

        inputFile.close();
    }
    else {
        cout << "Не удалось открыть файл для чтения" << endl;
    }
   

    //rentableItems.emplace_back("Washing Machine", "Samsung", 2022, "White", 123456, 20.0);
    //rentableItems.emplace_back("Projector", "Epson", 2021, "Black", 789012, 30.0);
    //saveData(rentableItems, "Товары.txt");
    /*
    HomeAppliance homeA("Стиральная машина", "Samsung", 2021, "Белый", 123456, 20.0);
    HomeAppliance homeB("Холодильник", "LG", 2022, "Серый", 789012, 25.0);
    HomeAppliance homeC("Посудомоечная машина", "Bosch", 2020, "Серебристый", 654321, 18.0);
    HomeAppliance homeD("Микроволновая печь", "Panasonic", 2023, "Черный", 987654, 15.0);

    OrganizationAppliance orgA("Проектор", "Epson", 2020, "Черный", 789012, 30.0);

    IndividualClient individualClient("Вадим Жолудев");
    IndividualClient individualClient1("Кирилл Лазарев");
    CorporateClient corporateClient("IV Company");

    rentalC.rentItem(&homeA, &individualClient, 7);
    rentalC.rentItem(&orgA, &corporateClient, 5);
    rentalC.rentItem(&homeD, &individualClient, 22);
    */
    system("pause");
    return 0;
}
