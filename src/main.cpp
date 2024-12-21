#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

int main() {
    // Создаем объект JSON с параметрами
    json settings;
    settings["server_address"] = "94.131.15.248"; // IP-адрес сервера
    settings["server_port"] = "12345";          // Порт сервера

    // Записываем JSON в файл settings.json
    std::ofstream file("../settings.json");
    if (file.is_open()) {
        file << settings.dump(4); // Красивый вывод с отступами (4 пробела)
        std::cout << "Файл настроек settings.json успешно создан!" << std::endl;
    } else {
        std::cerr << "Ошибка открытия файла для записи." << std::endl;
    }

    return 0;
}