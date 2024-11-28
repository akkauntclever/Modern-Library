//
//  System_functions.cpp
//  modern_library
//
//  Created by Иван Трифонов on 07.10.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include "Library.hpp"

//  фции чтения команд, введенных пользователем

bool find_forbidden_combinations(const std::string& string_for_check) {
    if (string_for_check.size() == 0) {
        return true;
    }
    for (size_t i = 0; i < string_for_check.size(); ++i) {
        if (string_for_check[i] == '/') {
            if (i == 0) {
                return true;
            }
            if (i == string_for_check.size() - 1) {
                return true;
            }
            if (string_for_check[i + 1] == '/') {
                return true;
            }
        }
    }
    return false;
}

bool empty_or_with_slash(const std::string& string_for_check) {
    if (string_for_check.empty()) {
        return true;
    }
    for (char symbol : string_for_check) {
        if (symbol == '/') {
            return true;
        }
    }
    return false;
}

bool forbidden_name(const std::string& string_for_check) {
    if (string_for_check.size() == 0) {
        return true;
    }
    for (size_t i = 0; i < string_for_check.size(); ++i) {
        if (string_for_check[i] == '/') {    //  расписать на несколько фций
            message_forbidden_system_name();
            return true;
        }
    }
    return false;
}

std::string getline_answer() {    //  получение строчки, вводимой пользователем
    std::string field_for_answer;
    std::getline(std::cin, field_for_answer);
    return field_for_answer;
}

bool check_if_exit(const std::string& answer) {    //  проверка ответа на соответствие командам выхода
    if (answer == "выйти" || answer == "выход" || answer == "exit" || answer == "leave") {
        return true;
    }
    return false;
}

bool check_if_cancel(const std::string& answer) {    //  проверка ответа на соответствие командам отмены
    if (answer == "отмена" || answer == "отменить" || answer == "cancel") {
        return true;
    }
    return false;
}

bool check_if_save_leave(const std::string& answer) {    //  проверка ответа на соответствие командам отмены
    if (answer == "сохранить и выйти" || answer == "save and leave" || answer == "save and exit") {
        return true;
    }
    return false;
}

std::string leave_library() {    //  окончательный выход из программы по запросу пользователя
    std::string answer = getline_answer();
    if (check_if_exit(answer)) {
        throw (char*)"Осуществляется выход из системы для учета книг в библиотеке.";
    }
    if (check_if_cancel(answer)) {
        throw CancelException{};
    }
    return answer;    //  если пользователь не выходит из системы, то ответ отправляется запрашиваемой фции
}

std::string leave_library_work() {
    std::string answer = leave_library();
    if (check_if_save_leave(answer)) {
        throw SaveLeaveException{};
    }
    return answer;    //  если пользователь не выходит из системы, то ответ отправляется запрашиваемой фции
}

std::string exit_read_user_work() {    //  действия при первичном запросе на выход из программы без сохранения
    union_message_exit_read_user();
    message_words_to_save_leave();
    message_words_to_continue_last_task();
    return leave_library_work();
}

std::string exit_read_user_setup() {    //  действия при первичном запросе на выход из программы без сохранения
    union_message_exit_read_user();
    return leave_library();
}

std::string read_user(const std::string& answer) {    //  чтение запросов пользователя без сохранения
    if (check_if_exit(answer)) {
        return exit_read_user_work();
    }    //  возможность сохранения предусматривается только при окончательном сохранении
    if (check_if_cancel(answer)) {
        throw CancelException{};
    }
    return answer;
}

std::string read_user_work() {
    return read_user(getline_answer());
}

std::string read_not_empty_user_work() {
    std::string answer = read_user_work();
    if (answer.empty()) {
        return read_not_empty_user_work();
    }
    return answer;
}

std::string read_user_setup() {
    std::string answer = getline_answer();
    if (check_if_exit(answer)) {
        return exit_read_user_setup();
    }
    return answer;
}

std::string read_user_terminal() {    //  фции реализации выхода с сохранением + отмены + обработки неправильного ввода прописываются в терминале
    std::string answer = getline_answer();
    return answer;
}

std::string read_user_section_name() {
    std::string answer = read_user(getline_answer());
    if (find_forbidden_combinations(answer)) {
        message_forbidden_section_title_asked_accepted_title();
        return read_user_section_name();
    }
    return answer;
}

void exit_mode_read_user() {
    while(true) {
        exit_read_user_setup();
        message_command_not_find();
    }
}

void exit_mode_saved_read_user() {
    while(true) {
        message_ask_leave_or_not();
        union_message_exit_or_stay_words();
        leave_library();
        message_command_not_find();
    }
}

//  общие системные фции

std::string read_string_from_stream(std::istream& file) {    //  прочитать строку из файла
    std::string answer = "";
    std::getline(file, answer);
    return answer;
}

std::stringstream read_file_decrypted_stream(std::ifstream& file) {    //  получить все содержимое файла
    std::string read_string = "";
    std::string result_string = "";
    std::stringstream result_stream;
    while (std::getline(file, read_string)) {
        result_string += read_string;
        result_string += '\n';
        read_string.clear();
    }
    result_string.pop_back();
    result_stream << AES_decryptString(result_string, system_key);
    return result_stream;
}

std::string get_current_time() {    //  получение текущей даты + времени в виде строки std::string
    std::stringstream answer;
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    answer << std::ctime(&time);
    std::string result;
    std::getline(answer, result);
    return result;
}

std::vector<std::string> get_section_names_from_path(const std::string& path_to_section) {
    std::vector<std::string> short_section_name(1);
    for (char symbol : path_to_section) {
        if (symbol == '/') {
            short_section_name.push_back("");
        }
        if (symbol != '/') {
            short_section_name[short_section_name.size() - 1] += symbol;
        }
    }
    return short_section_name;
}

size_t get_first_part_symbol(const std::string& main_string, size_t part_number) {
    if (part_number == 0) {
        throw (char*)"Внутренняя критическая ошибка: получен неправильный номер для переименования адреса.";
    }
    if (part_number == 1) {
        return 0;
    }
    size_t counter = 2;
    for (size_t i = 0; i < main_string.size(); ++i) {
        if (main_string[i] == '/') {
            if (counter == part_number) {
                return i + 1;
            }
            ++counter;
        }
    }
    throw (char*)"Внутренняя критическая ошибка: не найдена запрашиваемая часть адреса.";
}

std::string collect_string_before_symbol(const std::string& main_string, size_t i) {
    std::string result_string = "";
    for (size_t j = 0; j < i; ++j) {
        result_string += main_string[j];
    }
    return result_string;
}

std::string collect_string_after_symbol_with_symbol(const std::string& main_string, size_t i) {
    std::string result_string = "";
    for (size_t j = i; j < main_string.size(); ++j) {
        result_string += main_string[j];
    }
    return result_string;
}

void change_part_of_location_title(std::string& string_to_change, const std::string& new_part, size_t number_part_to_change) {
    if (number_part_to_change == 0) {
        throw (char*)"Внутренняя критическая ошибка: получен неправильный номер для переименования адреса.";
    }
    size_t first_change_part_symbol = get_first_part_symbol(string_to_change, number_part_to_change);
    std::string first_temporary_string = collect_string_before_symbol(string_to_change, first_change_part_symbol);
    std::string second_temporary_string = "";
    for (size_t i = first_change_part_symbol; i < string_to_change.size(); ++i) {
        if (string_to_change[i] == '/') {
            second_temporary_string = collect_string_after_symbol_with_symbol(string_to_change, i);
            break;
        }
    }
    string_to_change.clear();
    string_to_change = first_temporary_string + new_part + second_temporary_string;
}

size_t get_number_current_section(const std::string& current_section_location) {
    size_t counter = 1;
    for (size_t i = 0; i < current_section_location.size(); ++i) {
        if (current_section_location[i] == '/') {
            ++counter;
        }
    }
    return counter;
}

void delete_last_location(std::string& location_to_change) {
    std::string temporary_location = "";
    for (size_t i = location_to_change.size(); i > 0; --i) {
        if (location_to_change[i - 1] == '/') {
            for (size_t j = 0; j < i - 1; ++j) {
                temporary_location += location_to_change[j];
            }
            break;
        }
    }
    location_to_change.clear();
    location_to_change = temporary_location;
}

void delete_string_from_vector(std::vector<std::string>& vector_to_change, size_t num_string_to_delete) {
    if (num_string_to_delete >= vector_to_change.size()) {
        throw (char*)"Внутренняя критическая ошибка: выход за пределы массива вектора строк при обращении.";
    }
    std::vector<std::string> temp_vector;
    for (size_t i = 0; i < num_string_to_delete; ++i) {
        temp_vector.push_back(vector_to_change[i]);
    }
    for (size_t i = num_string_to_delete + 1; i < vector_to_change.size(); ++i) {
        temp_vector.push_back(vector_to_change[i]);
    }
    std::swap(vector_to_change, temp_vector);
}
