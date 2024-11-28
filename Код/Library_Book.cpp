//
//  Library::Book.cpp
//  modern_library
//
//  Created by Иван Трифонов on 07.10.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include "Library.hpp"

Library::Book::Book(const std::string& book_title, const std::string& book_section, const std::vector<std::string>& global_associations) {
    _book_title = book_title;
    _book_section = book_section;
    _book_foundation_time = get_current_time();
    add_empty_global_associations(global_associations);
}

Library::Book::Book(const std::string& book_title, const std::string& book_section, const std::string& book_foundation_time) {
    _book_title = book_title;
    _book_section = book_section;
    _book_foundation_time = book_foundation_time;
}

void Library::Book::add_from_file_book_associations(std::istream& stream) {
    size_t book_associations_size = static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str()));
    for (size_t i = 0; i < book_associations_size; ++i) {
        std::string association_title = read_string_from_stream(stream);
        std::string value = read_string_from_stream(stream);
        _user_book_local_associations.push_back(BookAssociations(association_title, value));
    }
}

void Library::Book::add_empty_global_associations(const std::vector<std::string>& global_associations) {
    for (std::string global_association : global_associations) {
        _user_book_local_associations.push_back(BookAssociations(global_association, ""));
    }
}

void Library::Book::add_empty_global_association(const std::string& global_association) {
    _user_book_local_associations.push_back(BookAssociations(global_association, ""));
}

void Library::Book::set_book_title(const std::string& book_title) {
    _book_title = book_title;
}

void Library::Book::add_user_book_association(const std::string& user_association, const std::string& value) {
    _user_book_local_associations.push_back(BookAssociations(user_association, value));    //  посмотреть, как правильно добавлять
}

void Library::Book::change_user_association(const std::string& user_association, const std::string& value) {
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (_user_book_local_associations[i].get_association_title() == user_association) {
            _user_book_local_associations[i].set_value(value);
            return;
        }
    }
}

void Library::Book::print_book() {
    std::cout << "Наименование книги: " << _book_title << ";" << std::endl;
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (!_user_book_local_associations[i].empty()) {
            std::cout << _user_book_local_associations[i].get_association_title() << ": " << _user_book_local_associations[i].get_value() << ";" << std::endl;
        }
    }
    std::cout << "Местоположение книги: \"" << _book_section << "\";" << std::endl;
    std::cout << "Время создания книги: " << _book_foundation_time << "." << std::endl;
}

void Library::Book::change_section(const std::string& section) {
    _book_section = section;
}

std::string Library::Book::get_book_title() {
    return _book_title;
}

std::string Library::Book::get_book_section() {
    return _book_section;
}

void Library::Book::change_section_location(const std::string& new_subsection_title, size_t subsection_location_number) {
    change_part_of_location_title(_book_section, new_subsection_title, subsection_location_number);
}

bool Library::Book::book_association_exists(const std::string& book_association) {
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (_user_book_local_associations[i].get_association_title() == book_association) {
            return true;
        }
    }
    return false;
}

std::string Library::Book::get_book_association_title(size_t i) {
    if (i >= _user_book_local_associations.size()) {
        throw (char*)"Внутренняя критическая ошибка: обращение за границу массива при получении по номеру названия ассоциации";
    }
    return _user_book_local_associations[i].get_association_title();
}

std::string Library::Book::get_book_association_field(size_t i) {
    if (i >= _user_book_local_associations.size()) {
        throw (char*)"Внутренняя критическая ошибка: обращение за границу массива при получении по номеру поля ассоциации";
    }
    return _user_book_local_associations[i].get_value();
}

std::string Library::Book::get_book_association_field(const std::string& association_title) {
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (_user_book_local_associations[i].get_association_title() == association_title) {
            return _user_book_local_associations[i].get_value();
        }
    }
    throw (char*)"Внутренняя критическая ошибка: не найдено поле ассоциации у книги.";
}

void Library::Book::change_association_field(const std::string& association_title, const std::string& association_field) {
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (_user_book_local_associations[i].get_association_title() == association_title) {
            _user_book_local_associations[i].set_value(association_field);
            return;
        }
    }
    throw (char*)"Внутренняя критическая ошибка: не найдено поле ассоциации у книги (для изменения поля).";
}

void Library::Book::delete_book_local_association(const std::string& association_title) {
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (_user_book_local_associations[i].get_association_title() == association_title) {
            _user_book_local_associations[i].delete_book_local_association();
            return;
        }
    }
    throw (char*)"Внутренняя критическая ошибка: не найдено поле ассоциации у книги (для удаления).";
}

void Library::Book::delete_book(std::vector<std::string>& associations) {
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        associations.push_back(_user_book_local_associations[i].get_association_title());
    }
    _book_title.clear();
    _book_section.clear();
    _book_foundation_time.clear();
    _user_book_local_associations.clear();
}

bool Library::Book::empty_association(const std::string& association_title) {
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (_user_book_local_associations[i].get_association_title() == association_title) {
            return _user_book_local_associations[i].empty_value();
        }
    }
    throw (char*)"Внутренняя критическая ошибка: не найдено поле ассоциации у книги (для проверки на пустоту).";
}

bool Library::Book::empty() {
    if (_book_section.empty()) {
        return true;
    }
    return false;
}

size_t Library::Book::size() {
    return _user_book_local_associations.size();
}

void Library::Book::compress_user_book_local_associations() {
    std::vector<Library::Book::BookAssociations> user_book_local_associations;
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        if (!_user_book_local_associations[i].empty()) {
            user_book_local_associations.push_back(_user_book_local_associations[i]);
        }
    }
    std::swap(_user_book_local_associations, user_book_local_associations);
}

std::string Library::Book::get_book_foundation_time() {
    return _book_foundation_time;
}

std::string Library::Book::get_book_associations() {
    std::string return_str = std::to_string(_user_book_local_associations.size());
    for (size_t i = 0; i < _user_book_local_associations.size(); ++i) {
        return_str += '\n';
        return_str += _user_book_local_associations[i].get_association_title();
        return_str += '\n';
        return_str += _user_book_local_associations[i].get_value();
    }
    return return_str;
}

Library::Book::BookAssociations::BookAssociations(const std::string& association_title, const std::string& value) {
    _association_title = association_title;
    _value = value;
    _empty = false;
}

std::string Library::Book::BookAssociations::get_association_title() {
    return _association_title;
}

std::string Library::Book::BookAssociations::get_value() {
    return _value;
}

void Library::Book::BookAssociations::set_association(const std::string& association_title, const std::string& value) {
    _association_title = association_title;
    _value = value;
    _empty = false;
}

void Library::Book::BookAssociations::set_value(const std::string& value) {
    _value = value;
}

bool Library::Book::BookAssociations::empty() {
    return _empty;
}

bool Library::Book::BookAssociations::empty_value() {
    return _value == "";
}

void Library::Book::BookAssociations::delete_book_local_association() {
    _association_title.clear();
    _value.clear();
    _empty = true;
}
