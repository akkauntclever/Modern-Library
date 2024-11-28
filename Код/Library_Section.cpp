//
//  Library::Section.cpp
//  modern_library
//
//  Created by Иван Трифонов on 07.10.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include "Library.hpp"

Library::Section::Section(const std::string& section_title, const std::string& section_location_without_name) {
    _section_foundation_time = get_current_time();
    _section_title = section_title;
    _section_location = section_location_without_name + '/' + section_title;
}

std::string Library::Section::get_section_title() {
    return _section_title;
}

void Library::Section::fill_empty_section(const std::string& section_title, const std::string& section_foundation_time) {
    _section_location = _section_title = section_title;
    _section_foundation_time = section_foundation_time;
}

void Library::Section::set_section_location(const std::string& section_location) {
    _section_location = section_location;
}

void Library::Section::set_section_name(const std::string& section_name) {
    _section_title = section_name;
}

void Library::Section::set_section_foundation_time(const std::string& section_foundation_time) {
    _section_foundation_time = section_foundation_time;
}

void Library::Section::set_section_books(std::istream& stream) {
    size_t books_number = static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str()));
    for (size_t i = 0; i < books_number; ++i) {
        _books.push_back(static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str())));
    }
}

void Library::Section::push_back_empty_subsection() {
    _dependencies.push_back(Section());
}

Library::Section* Library::Section::get_last_subsection() {
    return &_dependencies.back();
}

bool Library::Section::add_new_subsection(const std::string& subsection_name) {
    for (Library::Section subsection : _dependencies) {
        if (subsection.get_section_title() == subsection_name) {
            return false;
        }
    }
    _dependencies.push_back(Section(subsection_name, _section_location));
    return true;
}

bool Library::Section::open_subsection(const std::string& subsection_name) {
    int subsection_number = get_subsection_number(subsection_name);
    if (subsection_number == -1) {
        return false;
    }
    return true;
}

int Library::Section::get_subsection_number(const std::string& subsection_title) {
    for (size_t i = 0; i < _dependencies.size(); ++i) {
        if (_dependencies[i].get_section_title() == subsection_title) {
            return (int)i;
        }    //  тут перебор секции на определение совпадения
    }
    return -1;
}

Library::Section* Library::Section::find_and_open_subsection(const std::string& short_section_title) {
    for (size_t i = 0; i < _dependencies.size(); ++i) {
        if (_dependencies[i].get_section_title() == short_section_title) {
            return &_dependencies[i];
        }
    }    //  тут поиск существующей секции
    throw (char*)"Внутренняя критическая ошибка: не найдено имя подраздела при поиске в разделе";    //  переделать в исключение класса
}

std::vector<size_t> Library::Section::get_all_books_numbers() {
    return _books;
}

bool Library::Section::check_book_exists(size_t book_num) {
    for (size_t books : _books) {
        if (books == book_num) {
            return true;
        }
    }
    return false;
}

bool Library::Section::add_new_book_to_current_section(size_t book_num) {
    if (check_book_exists(book_num)) {
        return false;
    }
    _books.push_back(book_num);
    return true;
}

void Library::Section::get_all_books(std::vector<size_t>& books) {
    for (size_t i = 0; i < _books.size(); ++i) {
        books.push_back(_books[i]);
    }
}

std::string Library::Section::get_all_books_in_string() {
    std::string return_str = std::to_string(_books.size());
    for (size_t i = 0; i < _books.size(); ++i) {
        return_str += '\n';
        return_str += std::to_string(_books[i]);
    }
    return return_str;
}

void Library::Section::change_subsection_title(const std::string& new_section_title, size_t section_location_number, std::vector<size_t>& books_to_change_location) {
    change_part_of_location_title(_section_location, _section_title, section_location_number);
    get_all_books(books_to_change_location);
    for (size_t i = 0; i < _dependencies.size(); ++i) {
        _dependencies[i].change_subsection_title(new_section_title, section_location_number, books_to_change_location);
    }
}

void Library::Section::change_section_title(const std::string& new_section_title, std::vector<size_t>& books_to_change_location) {
    _section_title = new_section_title;
    change_subsection_title(_section_title, get_number_current_section(_section_location), books_to_change_location);
}

std::string Library::Section::get_section_location() {
    return _section_location;
}

void Library::Section::print_subsections() {
    for (size_t i = 0; i < _dependencies.size(); ++i) {
        if (!_dependencies[i].empty()) {
            std::cout << "  \"" << _dependencies[i].get_section_title() << '\"' << std::endl;
        }
    }
}

void Library::Section::delete_section(std::vector<size_t>& books_for_delete) {
    get_all_books(books_for_delete);
    for (size_t i = 0; i < _dependencies.size(); ++i) {
        _dependencies[i].delete_section(books_for_delete);
    }
    _section_location.clear();
    _section_title.clear();
    _section_foundation_time.clear();
    _books.clear();
    _dependencies.clear();
}

void Library::Section::delete_book(size_t book_to_delete) {
    for (std::vector<size_t>::iterator iter = _books.begin(); iter != _books.end(); ++iter) {
        if (*(iter) == book_to_delete) {
            _books.erase(iter);
            return;
        }
    }
    throw (char*)"Внутренняя критическая ошибка: не найдена книга в разделе для удаления";
}

void Library::Section::clear_all_books() {
    _books.clear();
}

size_t Library::Section::subsections_size() {
    return _dependencies.size();
}

Library::Section* Library::Section::get_subsection_pointer(size_t subsection_number) {
    if (subsection_number >= _dependencies.size()) {
        throw (char*)"Внутренняя критическая ошибка: выход за пределы массива при получении указателя на подсекцию";
    }
    return &_dependencies[subsection_number];
}

bool Library::Section::empty() {
    if (_section_location.empty()) {
        return true;
    }
    return false;
}

void Library::Section::compress_dependencies() {
    std::vector<Library::Section> dependencies;
    for (size_t i = 0; i < _dependencies.size(); ++i) {
        if (!_dependencies[i].empty()) {
            dependencies.push_back(_dependencies[i]);
        }
    }
    for (size_t i = 0; i < dependencies.size(); ++i) {
        dependencies[i].compress_dependencies();
    }
    std::swap(_dependencies, dependencies);
}

std::string Library::Section::get_section_foundation_time() {
    return _section_foundation_time;
}

bool Library::Section::check_subsection_exists(const std::string& subsection_name) {
    for (size_t i = 0; i < _dependencies.size(); ++i) {
        if (_dependencies[i].get_section_title() == subsection_name) {
            return true;
        }
    }
    return false;
}
