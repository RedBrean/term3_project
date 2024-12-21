class Message {
private:
    std::string text_;
    std::string date_;
    std::string sender_;
    std::string recipient_;

public:
    // Конструкторы
    Message() = default;
    Message(const std::string& text, const std::string& date, const std::string& sender, const std::string& recipient)
        : text_(text), date_(date), sender_(sender), recipient_(recipient) {}

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
            return Message(text, date, sender, recipient);
        }

        throw std::invalid_argument("Некорректный формат строки");
    }

    // Методы для доступа к полям (геттеры)
    const std::string& get_text() const { return text_; }
    const std::string& get_date() const { return date_; }
    const std::string& get_sender() const { return sender_; }
    const std::string& get_recipient() const { return recipient_; }
};