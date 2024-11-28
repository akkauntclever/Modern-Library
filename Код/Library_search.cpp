//
//  Library_search.cpp
//  modern_library
//
//  Created by Трифонов Иван on 23.11.2024.
//  Copyright © 2024 Трифонов Иван. All rights reserved.
//

#include "Library.hpp"

bool Library::find_terminal() {
    std::cout << "Для выведения команд поиска, введите \"команды\"." << std::endl;
    std::cout << "Для применения текущих параметров поиска введите \"найти\"." << std::endl;
    std::cout << "Чтобы завершить поиск книг по параметрам, введите \"завершить\"." << std::endl;
    std::cout << " $ ";
    std::string command = read_user_terminal();
    if (command == "команды") {
        find_commands();
        return true;
    }
    if (command == "завершить") {
        std::cout << "Работа в книжном терминале завершена." << std::endl;
        return false;
    }
    try {
        if (command == "текущие параметры") {
            current_parameters_search();
            return true;
        }
        if (command == "название книги") {
            set_book_name_search();
            return true;
        }
        if (command == "часть названия книги") {
            _book_full_name = false;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "полное название книги") {
            _book_full_name = true;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "параметры названия книги") {
            current_book_name_parameters_search();
            return true;
        }
        if (command == "сбросить название книги") {
            _book_name_to_search.clear();
            std::cout << "Название книги сброшено." << std::endl;
            return true;
        }
        if (command == "ассоциация") {
            set_field_search();
            return true;
        }
        if (command == "часть ассоциации") {
            _field_full_name = false;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "полная ассоциация") {
            _field_full_name = true;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "параметры ассоциаций") {
            current_field_parameters_search();
            return true;
        }
        if (command == "сбросить поиск ассоциации") {
            drop_field_search();
            return true;
        }
        if (command == "сбросить поиск всех ассоциаций") {
            _fields_to_search.clear();
            std::cout << "Все параметры ассоциаций сброшены." << std::endl;
            return true;
        }
        if (command == "содержимое ассоциации") {
            set_field_value_search();
            return true;
        }
        if (command == "часть содержимого ассоциации") {
            _field_value_full_name = false;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "полное содержимое ассоциации") {
            _field_value_full_name = true;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "параметры содержимого ассоциаций") {
            current_field_value_parameters_search();
            return true;
        }
        if (command == "сбросить поиск содержимого ассоциации") {
            drop_field_value_search();
            return true;
        }
        if (command == "сбросить поиск всех содержимых ассоциаций") {
            _fields_value_to_search.clear();
            std::cout << "Все параметры ассоциаций сброшены." << std::endl;
            return true;
        }
        if (command == "раздел") {
            set_section_search();
            return true;
        }
        if (command == "часть названия раздела") {
            _section_full_name = false;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "полное название раздела") {
            _section_full_name = true;
            std::cout << "Параметр установлен." << std::endl;
            return true;
        }
        if (command == "текущий раздел") {
            current_section_parameters_search();
            return true;
        }
        if (command == "сбросить поиск раздела") {
            _section_name_to_search.clear();
            std::cout << "Все параметры сброшены." << std::endl;
            return true;
        }
        if (command == "сбросить все параметры") {
            drop_all_search_parameters();
            return true;
        }
        if (command == "найти") {
            find_books();
            return true;
        }
        std::cout << "Команда не найдена." << std::endl;
    }
    catch (const CancelException& ex) {
        return true;
    }
    catch (const SaveLeaveException& ex) {
        save_system();
        throw (char*)"Осуществляется выход из системы для учета книг в библиотеке.";
    }
    return true;
}

void Library::current_parameters_search() {
    std::cout << "Параметры поиска книги: " << std::endl;
    current_book_name_parameters_search();
    current_field_parameters_search();
    current_field_value_parameters_search();
    current_section_parameters_search();
}

void Library::set_book_name_search() {
    std::cout << "Введите название книги для поиска:" << std::endl;
    std::string answer = read_user_work();
    if (answer.empty()) {
        set_book_name_search();
        return;
    }
    _book_name_to_search = answer;
}

void Library::current_book_name_parameters_search() {
    if (!_book_name_to_search.empty()) {
        std::cout << "Название книги для поиска: \"" << _book_name_to_search << '\"' << std::endl;
        if (_book_full_name) {
            std::cout << "Поиск книги осуществляется по её полному названию." << std::endl;
            return;
        }
        std::cout << "Поиск книги осуществляется по её частичному названию." << std::endl;
        return;
    }
    std::cout << "Параметр поиска по названию книги не установлен." << std::endl;
}

void Library::set_field_search() {
    std::cout << "Введите название поля книги для поиска:" << std::endl;
    std::string answer = read_user_work();
    if (answer.empty()) {
        set_field_search();
        return;
    }
    for (size_t i = 0; i < _fields_to_search.size(); ++i) {
        if (_fields_to_search[i] == answer) {
            std::cout << "Такой параметр поиска поля уже задан." << std::endl;
            set_field_search();
            return;
        }
    }
    _fields_to_search.push_back(answer);
}

void Library::current_field_parameters_search() {
    if (!_fields_to_search.empty()) {
        for (size_t i = 0; i < _fields_to_search.size(); ++i) {
            std::cout << "Название поля для поиска: \"" << _fields_to_search[i] << '\"' << std::endl;
        }
        if (_field_full_name) {
            std::cout << "Поиск поля книги осуществляется по его полному названию." << std::endl;
            return;
        }
        std::cout << "Поиск поля книги осуществляется по его частичному названию." << std::endl;
        return;
    }
    std::cout << "Параметры поиска по полям книги не установлены." << std::endl;
}

void Library::drop_field_search() {
    std::cout << "Введите название параметра поля для удаления:" << std::endl;
    while (!drop_field_search(read_user_work()));
}

bool Library::drop_field_search(const std::string& field_name) {
    for (size_t i = 0; i < _fields_to_search.size(); ++i) {
        if (_fields_to_search[i] == field_name) {
            delete_string_from_vector(_fields_to_search, i);
            std::cout << "Параметр успешно сброшен." << std::endl;
            return true;
        }
    }
    std::cout << "Поле для удаления не найдено." << std::endl;
    std::cout << "Введите название параметра поля для удаления:" << std::endl;
    return false;
}

void Library::set_field_value_search() {
    std::cout << "Введите содержимое поля книги для поиска:" << std::endl;
    std::string answer = read_user_work();
    if (answer.empty()) {
        set_field_value_search();
        return;
    }
    for (size_t i = 0; i < _fields_value_to_search.size(); ++i) {
        if (_fields_value_to_search[i] == answer) {
            std::cout << "Такой параметр поиска содержимого поля уже задан." << std::endl;
            set_field_value_search();
            return;
        }
    }
    _fields_value_to_search.push_back(answer);
}

void Library::current_field_value_parameters_search() {
    if (!_fields_value_to_search.empty()) {
        for (size_t i = 0; i < _fields_value_to_search.size(); ++i) {
            std::cout << "Содержимое поля книги для поиска: \"" << _fields_value_to_search[i] << '\"' << std::endl;
        }
        if (_field_value_full_name) {
            std::cout << "Поиск содержимого поля книги осуществляется по его полному названию." << std::endl;
            return;
        }
        std::cout << "Поиск содержимого поля книги осуществляется по его частичному названию." << std::endl;
        return;
    }
    std::cout << "Параметры поиска по содержимому полей книги не установлены." << std::endl;
}

void Library::drop_field_value_search() {
    std::cout << "Введите название параметра содержимого поля для удаления:" << std::endl;
    while (!drop_field_value_search(read_user_work()));
}

bool Library::drop_field_value_search(const std::string& field_value_name) {
    for (size_t i = 0; i < _fields_value_to_search.size(); ++i) {
        if (_fields_value_to_search[i] == field_value_name) {
            delete_string_from_vector(_fields_value_to_search, i);
            std::cout << "Параметр успешно сброшен." << std::endl;
            return true;
        }
    }
    std::cout << "Содержимое поля для удаления не найдено." << std::endl;
    std::cout << "Введите название параметра содержимого поля для удаления:" << std::endl;
    return false;
}

void Library::set_section_search() {
    std::cout << "Введите название раздела для поиска:" << std::endl;
    std::string answer = read_user_work();
    if (empty_or_with_slash(answer)) {
        std::cout << "Недопустимое название секции (нельзя пустое и с символом \"\\\")." << std::endl;
        set_section_search();
        return;
    }
    _section_name_to_search = answer;
}

void Library::current_section_parameters_search() {
    if (!_section_name_to_search.empty()) {
        std::cout << "Название раздела книги для поиска: \"" << _section_name_to_search << '\"' << std::endl;
        if (_section_full_name) {
            std::cout << "Поиск раздела книги осуществляется по его полному названию." << std::endl;
            return;
        }
        std::cout << "Поиск раздела книги осуществляется по его частичному названию." << std::endl;
        return;
    }
    std::cout << "Параметр поиска по названию раздела книги не установлен." << std::endl;
}

void Library::drop_all_search_parameters() {
    _book_name_to_search.clear();
    _fields_to_search.clear();
    _fields_value_to_search.clear();
    _section_name_to_search.clear();
    _book_full_name = false;
    _field_full_name = false;
    _field_value_full_name = false;
    _section_full_name = false;
}

void Library::find_books() {
    std::vector<size_t> found_book_nums = find_books_by_name();
    for (size_t i = 0; i < _fields_to_search.size(); ++i) {
        find_books_by_field(found_book_nums, i);
    }
    for (size_t i = 0; i < _fields_value_to_search.size(); ++i) {
        find_books_by_field_value(found_book_nums, i);
    }
    find_books_by_section_name(found_book_nums);
    if (found_book_nums.empty()) {
        std::cout << "Не найдено книг, соответствующим данным параметрам поиска." << std::endl;
        return;
    }
    std::cout << "Найденные книги:" << std::endl;
    for (size_t i = 0; i < found_book_nums.size(); ++i) {
        _storage[found_book_nums[i]].print_book();
        std::cout << std::endl;
    }
}

std::vector<size_t> Library::find_books_by_name() {
    if (_book_full_name) {
        return find_books_by_full_name();
    }
    std::vector<size_t> books_matches;
    const std::regex regex(_book_name_to_search);
    for (size_t i = 0; i < _storage.size(); ++i) {
        std::smatch match;
        std::string book_title = _storage[i].get_book_title();
        if (std::regex_search(book_title, match, regex)) {
            books_matches.push_back(i);
        }
    }
    return books_matches;
}

std::vector<size_t> Library::find_books_by_full_name() {
    std::vector<size_t> books_matches;
    for (size_t i = 0; i < _storage.size(); ++i) {
        if (_storage[i].get_book_title() == _book_name_to_search) {
            books_matches.push_back(i);
        }
    }
    return books_matches;
}

void Library::find_books_by_field(std::vector<size_t>& book_nums, size_t field_num) {
    if (field_num >= _fields_to_search.size()) {
        throw (char*)"Внутренняя критическая ошибка: field_num >= _fields_to_search";
    }
    if (_field_full_name) {
        find_books_by_full_field(book_nums, field_num);
        return;
    }
    std::vector<size_t> books_matches;
    const std::regex regex(_fields_to_search[field_num]);
    for (size_t i = 0; i < book_nums.size(); ++i) {
        for (size_t j = 0; j < _storage[book_nums[i]].size(); ++j) {
            std::smatch match;
            std::string book_field = _storage[book_nums[i]].get_book_association_title(j);
            if (std::regex_search(book_field, match, regex)) {
                books_matches.push_back(i);
                break;
            }
        }
    }
    std::swap(books_matches, book_nums);
}

void Library::find_books_by_full_field(std::vector<size_t>& book_nums, size_t field_num) {
    std::vector<size_t> books_matches;
    for (size_t i = 0; i < book_nums.size(); ++i) {
        for (size_t j = 0; j < _storage[book_nums[i]].size(); ++j) {
            if (_storage[book_nums[i]].get_book_association_title(j) == _fields_to_search[field_num]) {
                books_matches.push_back(i);
                break;
            }
        }
    }
    std::swap(books_matches, book_nums);
}

void Library::find_books_by_field_value(std::vector<size_t>& book_nums, size_t field_value_num) {
    if (field_value_num >= _fields_value_to_search.size()) {
        throw (char*)"Внутренняя критическая ошибка: field_value_num >= _fields_value_to_search";
    }
    if (_field_value_full_name) {
        find_books_by_full_field_value(book_nums, field_value_num);
        return;
    }
    std::vector<size_t> books_matches;
    const std::regex regex(_fields_value_to_search[field_value_num]);
    for (size_t i = 0; i < book_nums.size(); ++i) {
        for (size_t j = 0; j < _storage[book_nums[i]].size(); ++j) {
            std::smatch match;
            std::string book_field_value = _storage[book_nums[i]].get_book_association_field(j);
            if (std::regex_search(book_field_value, match, regex)) {
                books_matches.push_back(i);
                break;
            }
        }
    }
    std::swap(books_matches, book_nums);
}

void Library::find_books_by_full_field_value(std::vector<size_t>& book_nums, size_t field_value_num) {
    std::vector<size_t> books_matches;
    for (size_t i = 0; i < book_nums.size(); ++i) {
        for (size_t j = 0; j < _storage[book_nums[i]].size(); ++j) {
            if (_storage[book_nums[i]].get_book_association_field(j) == _fields_value_to_search[field_value_num]) {
                books_matches.push_back(i);
                break;
            }
        }
    }
    std::swap(books_matches, book_nums);
}

void Library::find_books_by_section_name(std::vector<size_t>& book_nums) {
    if (_section_full_name) {
        find_books_by_full_section_name(book_nums);
        return;
    }
    std::vector<size_t> books_matches;
    const std::regex regex(_section_name_to_search);
    for (size_t i = 0; i < book_nums.size(); ++i) {
        std::smatch match;
        std::string book_section = get_section_names_from_path(_storage[book_nums[i]].get_book_section()).back();
        if (std::regex_search(book_section, match, regex)) {
            books_matches.push_back(book_nums[i]);
        }
    }
    std::swap(books_matches, book_nums);
}

void Library::find_books_by_full_section_name(std::vector<size_t>& book_nums) {
    std::vector<size_t> books_matches;
    for (size_t i = 0; i < book_nums.size(); ++i) {
        if (_section_name_to_search == get_section_names_from_path(_storage[book_nums[i]].get_book_section()).back()) {
            books_matches.push_back(i);
        }
    }
    std::swap(books_matches, book_nums);
}
