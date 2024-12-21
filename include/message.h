#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>

class Message {
private:
    std::string text_;
    std::string date_;
    std::string sender_;
    std::string recipient_;

    // Метод для получения текущей даты
    static std::string get_current_date() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

public:
    // Конструктор
    Message(const std::string& text, const std::string& sender, const std::string& recipient)
        : text_(text), date_(get_current_date()), sender_(sender), recipient_(recipient) {}

    // Метод для сериализации (запись в строку через запятую)
    std::string to_string() const {
        std::ostringstream oss;
        oss << text_ << "," << date_ << "," << sender_ << "," << recipient_;
        return oss.str();
    }

    // Метод для десериализации (восстановление объекта из строки)
    static Message from_string(const std::string& str) {
        std::istringstream iss(str);
        std::string text, date, sender, recipient;

        // Разделяем строку по запятой
        if (std::getline(iss, text, ',') &&
            std::getline(iss, date, ',') &&
            std::getline(iss, sender, ',') &&
            std::getline(iss, recipient, ',')) {
            return Message(text, sender, recipient, date);
        }

        throw std::invalid_argument("Некорректный формат строки");
    }

    // Конструктор для восстановления из строки
    Message(const std::string& text, const std::string& sender, const std::string& recipient, const std::string& date)
        : text_(text), date_(date), sender_(sender), recipient_(recipient) {}

    // Методы для доступа к полям (геттеры)
    const std::string& get_text() const { return text_; }
    const std::string& get_date() const { return date_; }
    const std::string& get_sender() const { return sender_; }
    const std::string& get_recipient() const { return recipient_; }
};
