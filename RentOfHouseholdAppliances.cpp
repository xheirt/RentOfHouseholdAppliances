#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//              ╔═════════════════════════════╗
//              ║      Класс для техники      ║
//              ╚═════════════════════════════╝

class RentableItem {
public:
    RentableItem(string name, string manufacturer, int year, string color, int inventoryNumber, double rentalPrice)
        : name(name), manufacturer(manufacturer), year(year), color(color), inventoryNumber(inventoryNumber), rentalPrice(rentalPrice){}

    virtual ~RentableItem(){}
    
    virtual double calculateRentalRevenue(int rentalPeriod) const {
        return rentalPrice * rentalPeriod;
    }

    virtual void displayInformation() const {
        cout << "Предмет БТ: " << name << "\nПроизводитель БТ: " << manufacturer << "\nГод выпуска: " << year
            << "\nЦвет предмета: " << color << "\nИнвентарный номер: " << inventoryNumber
            << "\nЦена аренды: " << rentalPrice << " за один день\n\n";
    }

    int getInventoryNumber() const {
        return inventoryNumber;
    }

private:
    string name, manufacturer, color;
    int year, inventoryNumber;
    double rentalPrice;
};

//              ╔═════════════════════════════╗
//              ║      Техника для дома       ║
//              ╚═════════════════════════════╝

class HomeAppliance : public RentableItem {
public:
    HomeAppliance(string name, string manufacturer, int year, string color, int inventoryNumber, double rentalPrice)
        : RentableItem(name, manufacturer, year, color, inventoryNumber, rentalPrice){}
    
    // Дополнительные характеристики и функциональность для бытовой техники для дома
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

class RentalCenter {
public:
    void rentItem(RentableItem* item, Client* client, int rentalPeriod) {
        // Проверка наличия предмета БТ в центре
        auto it = find_if(rentedItems.begin(), rentedItems.end(),
            [item](const pair<RentableItem*, int>& pair) {
                return pair.first->getInventoryNumber() == item->getInventoryNumber();
            });

        if (it == rentedItems.end()) {
            // Предмет не найден в списке арендованных, можно арендовать
            rentedItems.push_back(make_pair(item, rentalPeriod));
            clients.push_back(client);
            //cout << "Товар успешно сдан в аренду!\n";
        }
        else {
            cout << "Товар уже сдан в аренду!\n";
        }
    }

    void returnItem(RentableItem* item, int actualRentalPeriod) {
        auto it = find_if(rentedItems.begin(), rentedItems.end(),
            [item](const pair<RentableItem*, int>& pair) {
                return pair.first->getInventoryNumber() == item->getInventoryNumber();
            });

        if (it != rentedItems.end()) {
            double rentalRevenue = it->first->calculateRentalRevenue(it->second);
            double penalty = 0.0;

            // Проверка на превышение времени аренды
            if (actualRentalPeriod > it->second) {
                penalty = (actualRentalPeriod - it->second) * (2 * it->first->calculateRentalRevenue(1));
            }

            double totalRevenue = rentalRevenue + penalty;

            cout << "Товар успешно возвращен!\n";
            cout << "Доход от аренды: " << rentalRevenue << "\nШтраф: " << penalty << "\nОбщий доход: " << totalRevenue << "\n";

            // Удаляем предмет из списка арендованных
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

    vector<RentableItem*> insufficientItems() const {
        // Предположим, что это список предметов, которых не хватает в центре
        // Вам, возможно, нужно предоставить конкретную логику для этой функции
        std::vector<RentableItem*> insufficientItemsList;

        // Пример: добавим элементы, если количество арендованных элементов превышает определенное значение
        for (const auto& pair : rentedItems) {
            if (pair.second > 10) {
                insufficientItemsList.push_back(pair.first);
            }
        }

        return insufficientItemsList;
    }

private:
    vector<pair<RentableItem*, int>> rentedItems; // Предмет аренды и срок аренды
    vector<Client*> clients;                      // Список клиентов
};

enum MenuOption {
    SHOW_RENTED_ITEMS,
    SHOW_TOTAL_REVENUE,
    EXIT
};

void displayMenu() {
    cout << "Меню:\n"
         << "0. Показать арендованные предметы\n"
         << "1. Показать общую сумму\n"
         << "2. Выход\n";
}

void processMenuOption(RentalCenter& rentalCenter, MenuOption option) {
    switch (option) {
    case SHOW_RENTED_ITEMS: {
        // Показать арендованные предметы
        cout << "\nАрендованные предметы:\n" << "\n-------------------------\n";
        for (const auto& pair : rentalCenter.getRentedItems()) {
            pair.first->displayInformation();
            cout << "Срок аренды: " << pair.second << " дней\n";
            cout << "-------------------------\n";
        }
        break;
    }
    
    case SHOW_TOTAL_REVENUE: {
        // Показать общую выручку
        double totalRevenue = rentalCenter.calculateTotalRevenue();
        cout << "Общий доход: " << totalRevenue << "\n";
        break;
    }

    case EXIT:
        cout << "Exiting the program.\n";
        break;
    default:
        cout << "Invalid option.\n";
    }
}

int main()
{
    setlocale(LC_ALL, "ru");

    RentalCenter rentalC;

    HomeAppliance homeA("Стиральная машина", "Samsung", 2021, "Белый", 123456, 20.0);
    OrganizationAppliance orgA("Проектор", "Epson", 2020, "Черный", 789012, 30.0);
/*
    homeA.displayInformation();
    cout << "Выручка от аренды: " << homeA.calculateRentalRevenue(5) << "\n\n";

    orgA.displayInformation();
    cout << "Выручка от аренды: " << orgA.calculateRentalRevenue(3) << "\n\n";
    */
    IndividualClient individualClient("Вадим Жолудев");
    CorporateClient corporateClient("IV Company");

    rentalC.rentItem(&homeA, &individualClient, 7);
    rentalC.rentItem(&orgA, &corporateClient, 5);

    /*
    rentalC.returnItem(&homeA, 8);
    rentalC.returnItem(&orgA, 5);
    
    // Дополнительные примеры использования
    RentableItem* mostDemanded = rentalC.mostDemandedItem();
    if (mostDemanded) {
        std::cout << "Most demanded item: ";
        mostDemanded->displayInformation();
    }

    double totalRevenue = rentalC.calculateTotalRevenue();
    std::cout << "Total Revenue: " << totalRevenue << "\n";

    std::vector<RentableItem*> insufficientItemsList = rentalC.insufficientItems();
    std::cout << "Insufficient Items:\n";
    for (const auto& item : insufficientItemsList) {
        item->displayInformation();
    }*/

    MenuOption option = EXIT;
    do {
        displayMenu();
        cout << "Выберите опцию (0-2): ";
        int choice;
        cin >> choice;

        option = static_cast<MenuOption>(choice);

        processMenuOption(rentalC, option);

    } while (option != EXIT);

    system("pause");
    return 0;
}
