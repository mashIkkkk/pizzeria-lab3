// Лабораторная работа 3, Вариант 7
// Пиццерия: процесс приготовления заказа

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <chrono>
#include <windows.h>

using namespace std;

// Класс "Заказ"
class PizzaOrder {
public:
    string pizzaType;
    string size;
    vector<string> toppings;
    bool isReady;
    bool isPacked;

    PizzaOrder(string type, string s, vector<string> top) {
        pizzaType = type;
        size = s;
        toppings = top;
        isReady = false;
        isPacked = false;
    }
};

// Класс "Пиццерия"
class Pizzeria {
private:
    map<string, vector<string>> recipes;
    map<string, int> stock;

public:
    Pizzeria() {
        recipes["Маргарита"] = { "тесто", "томатный соус", "моцарелла", "базилик" };
        recipes["Пепперони"] = { "тесто", "томатный соус", "пепперони", "моцарелла" };
        recipes["Гавайская"] = { "тесто", "томатный соус", "ветчина", "ананас", "моцарелла" };

        stock["тесто"] = 10;
        stock["томатный соус"] = 5;
        stock["моцарелла"] = 8;
        stock["пепперони"] = 4;
        stock["базилик"] = 3;
        stock["ветчина"] = 5;
        stock["ананас"] = 3;
    }

    bool checkIngredients(PizzaOrder& order) {
        vector<string> needed = recipes[order.pizzaType];
        vector<string> missing;

        for (string ing : needed) {
            if (stock[ing] <= 0) {
                missing.push_back(ing);
            }
        }

        if (!missing.empty()) {
            cout << "❌ Отказ в заказе! Нет ингредиентов: ";
            for (string m : missing) cout << m << " ";
            cout << endl;
            return false;
        }
        return true;
    }

    bool preparePizza(PizzaOrder& order) {
        cout << "\n🍕 Начинаем готовить " << order.pizzaType << " (" << order.size << ")" << endl;
        cout << "   Добавки: " << (order.toppings.empty() ? "нет" : "");
        for (string t : order.toppings) cout << t << " ";
        cout << endl;

        cout << "   - Замешиваем тесто..." << endl;
        cout << "   - Добавляем соус..." << endl;
        cout << "   - Выкладываем начинку: ";
        for (string ing : recipes[order.pizzaType]) cout << ing << " ";
        cout << endl;

        cout << "   - 🔥 Выпекаем в печи 15 минут..." << endl;
        this_thread::sleep_for(chrono::seconds(2));

        order.isReady = true;
        cout << "   ✅ Пицца готова!" << endl;
        return true;
    }

    void packPizza(PizzaOrder& order) {
        cout << "   📦 Упаковываем пиццу в коробку..." << endl;
        order.isPacked = true;
        cout << "   ✅ Пицца упакована" << endl;
    }

    void deliverToCourier(PizzaOrder& order) {
        cout << "   🛵 Передаём заказ курьеру" << endl;
        cout << "   🧾 Чек: Пицца " << order.pizzaType << ", " << order.size;
        if (!order.toppings.empty()) {
            cout << ", добавки: ";
            for (string t : order.toppings) cout << t << " ";
        }
        else {
            cout << ", без добавок";
        }
        cout << endl;
    }

    bool processOrder(PizzaOrder& order) {
        cout << "\n================================================" << endl;
        cout << "ПИЦЦЕРИЯ: НОВЫЙ ЗАКАЗ" << endl;
        cout << "================================================" << endl;

        if (!checkIngredients(order)) return false;
        if (!preparePizza(order)) return false;
        packPizza(order);
        deliverToCourier(order);

        for (string ing : recipes[order.pizzaType]) {
            stock[ing]--;
        }

        cout << "\n✅ Заказ выполнен успешно!" << endl;
        return true;
    }

    void showMenu() {
        cout << "\n📋 МЕНЮ ПИЦЦЕРИИ:" << endl;
        cout << "   1. Маргарита" << endl;
        cout << "   2. Пепперони" << endl;
        cout << "   3. Гавайская" << endl;
    }
};

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Pizzeria pizzeria;

    cout << "========================================" << endl;
    cout << "   ДОБРО ПОЖАЛОВАТЬ В ПИЦЦЕРИЮ!" << endl;
    cout << "========================================" << endl;

    pizzeria.showMenu();

    string pizzaType, size, toppingsInput;

    cout << "\nВыберите пиццу: ";
    getline(cin, pizzaType);

    string pizzaTypeLower = toLower(pizzaType);
    string actualPizzaType = "";

    if (pizzaTypeLower == "маргарита" || pizzaTypeLower == "margarita")
        actualPizzaType = "Маргарита";
    else if (pizzaTypeLower == "пепперони" || pizzaTypeLower == "pepperoni")
        actualPizzaType = "Пепперони";
    else if (pizzaTypeLower == "гавайская" || pizzaTypeLower == "hawaiian")
        actualPizzaType = "Гавайская";
    else {
        cout << "❌ Извините, пиццы \"" << pizzaType << "\" нет в меню" << endl;
        return 1;
    }

    cout << "Выберите размер (Маленькая/Средняя/Большая): ";
    getline(cin, size);

    cout << "Дополнительные начинки (через запятую или Enter): ";
    getline(cin, toppingsInput);

    vector<string> toppings;
    if (!toppingsInput.empty()) {
        string temp;
        for (char c : toppingsInput) {
            if (c == ',') {
                if (!temp.empty()) {
                    toppings.push_back(temp);
                    temp.clear();
                }
            }
            else if (c != ' ') {
                temp += c;
            }
        }
        if (!temp.empty()) toppings.push_back(temp);
    }

    PizzaOrder order(actualPizzaType, size, toppings);
    pizzeria.processOrder(order);

    cout << "\nНажмите Enter для выхода...";
    cin.get();

    return 0;
}