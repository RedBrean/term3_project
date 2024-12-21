#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>

class Message {
private:
    static constexpr char DELIMITER = '|';

    std::string text_;
    std::string date_;
    std::string sender_;
    std::string recipient_;
    std::string status_; // Переменная состояния сообщения

    // Метод для получения текущей даты
    static std::string get_current_date() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

public:
    // Конструктор по умолчанию
    Message() : text_(""), date_(get_current_date()), sender_(""), recipient_(""), status_("unsent") {}

    // Конструктор
    Message(const std::string& text, const std::string& sender, const std::string& recipient)
        : text_(text), date_(get_current_date()), sender_(sender), recipient_(recipient), status_("unsent") {}

    // Конструктор из строки
    Message(const std::string& str) {
        *this = from_string(str);
    }

    // Метод для сериализации (запись в строку через специальный символ DELIMITER)
    std::string to_string() const {
        std::ostringstream oss;
        oss << text_ << DELIMITER << date_ << DELIMITER << sender_ << DELIMITER << recipient_ << DELIMITER << status_;
        return oss.str();
    }

    // Метод для десериализации (восстановление объекта из строки)
    static Message from_string(const std::string& str) {
        std::istringstream iss(str);
        std::string text, date, sender, recipient, status;

        // Разделяем строку по символу DELIMITER
        if (std::getline(iss, text, DELIMITER) &&
            std::getline(iss, date, DELIMITER) &&
            std::getline(iss, sender, DELIMITER) &&
            std::getline(iss, recipient, DELIMITER) &&
            std::getline(iss, status, DELIMITER)) {
            return Message(text, sender, recipient, date, status);
        }

        throw std::invalid_argument("Некорректный формат строки");
    }

    // Конструктор для восстановления из строки
    Message(const std::string& text, const std::string& sender, const std::string& recipient, const std::string& date, const std::string& status)
        : text_(text), date_(date), sender_(sender), recipient_(recipient), status_(status) {}

    // Методы для доступа к полям (геттеры)
    const std::string& get_text() const { return text_; }
    const std::string& get_date() const { return date_; }
    const std::string& get_sender() const { return sender_; }
    const std::string& get_recipient() const { return recipient_; }
    const std::string& get_status() const { return status_; }

    // Метод для установки состояния
    void set_status(const std::string& status) { status_ = status; }
};
