//
//  Library_Associations.cpp
//  modern_library
//
//  Created by Дина Трифонова on 16.10.2024.
//  Copyright © 2024 Трифонов Иван. All rights reserved.
//

#include "Library.hpp"

Library::Association::Association(const std::string& association_name) {
    _association_name = association_name;
}

void Library::Association::add_book_associations_from_file(std::istream& stream) {
    size_t books_number = static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str()));
    for (size_t i = 0; i < books_number; ++i) {
        add_book(static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str())));
    }
}

void Library::Association::set_association_name(const std::string& association_name) {
    _association_name = association_name;
}

std::string Library::Association::get_association_name() {
    return _association_name;
}

void Library::Association::add_book(size_t book_number) {
    _books.insert(book_number);
}

void Library::Association::delete_book(size_t book_number) {
    _books.erase(book_number);
    if (_books.empty() && !_global) {
        delete_user_book_association();
    }
}

std::vector<size_t> Library::Association::get_all_books_number() {
    std::vector<size_t> result;
    for (std::set<size_t>::iterator iter = _books.begin(); iter != _books.end(); ++iter) {
        result.push_back(*iter);
    }
    return result;
}

std::string Library::Association::get_all_books_number_string() {
    std::string return_str = std::to_string(_books.size());
    for (std::set<size_t>::iterator iter = _books.begin(); iter != _books.end(); ++iter) {
        return_str += "\n";
        return_str += std::to_string(*iter);
    }
    return return_str;
}

bool Library::Association::check_global() {
    return _global;
}

void Library::Association::set_global_setup(bool global) {
    _global = global;
}

void Library::Association::set_global(bool global) {
    _global = global;
    if (!global && _books.empty()) {
        delete_user_book_association();
    }
}

bool Library::Association::empty() {
    return _association_name.empty();
}

void Library::Association::delete_user_book_association() {
    _association_name.clear();
    _books.clear();
}

void Library::Association::delete_all_user_books() {
    _books.clear();
}
