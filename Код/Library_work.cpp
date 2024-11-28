//
//  Library_work.cpp
//  modern_library
//
//  Created by Иван Трифонов on 07.10.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include "Library.hpp"

void Library::terminal() {
    std::cout << "Для выведения списка команд, введите \"команды\"." << std::endl;
    std::cout << _path_current_section << " $ ";
    std::string command = read_user_terminal();
    if (command == "команды") {
        commands();
        return;
    }
    try {
        if (command == "посмотреть сведения о системе") {
            show_system_parameters();
            return;
        }
        if (command == "создать книгу") {
            create_new_book();
            _system_saved = false;
            return;
        }
        if (command == "редактировать книгу") {
            change_book_fields();
            return;
        }
        if (command == "печать книги") {
            print_user_book();
            return;
        }
        if (command == "переименовать книгу") {
            change_book_title();
            _system_saved = false;
            return;
        }
        if (command == "удалить книгу") {
            delete_book();
            _system_saved = false;
            return;
        }
        if (command == "создать подраздел") {
            add_new_section();
            _system_saved = false;
            return;
        }
        if (command == "открыть подраздел") {
            switch_current_section_to_subsection();
            return;
        }
        if (command == "раздел выше") {
            switch_subsection_to_section();
            return;
        }
        if (command == "переименовать раздел") {
            change_section_title();
            _system_saved = false;
            return;
        }
        if (command == "показать подразделы") {
            show_subsections();
            return;
        }
        if (command == "удалить подраздел") {
            delete_section();
            _system_saved = false;
            return;
        }
        if (command == "показать книги") {
            show_books();
            return;
        }
        
        if (command == "установить глобальное поле") {
            set_global_field();
            _system_saved = false;
            return;
        }
        if (command == "показать глобальные поля") {
            show_global_fields();
            return;
        }
        if (command == "снять глобальное поле") {
            delete_global_field();
            _system_saved = false;
            return;
        }
        
        if (command == "поиск книги") {
            while(find_terminal()) {}
            return;
        }
        
        if (command == "выйти" || command == "выход" || command == "exit" || command == "leave") {
            exit_mode();
            return;
        }
        if (command == "сохранить" || command == "сохранение" || command == "save") {   //  сохранение системы
            save_system();
            return;
        }
        if (command == "сохранить как") {
            save_as();
            return;
        }
    }
    catch (const CancelException& ex) {
        return;
    }
    catch (const SaveLeaveException& ex) {
        save_system();
        throw (char*)"Осуществляется выход из системы для учета книг в библиотеке.";
    }
    std::cout << "Команда не найдена, повторите попытку." << std::endl;
}

void Library::add_new_section() {
    std::cout << "Введите название нового подраздела:" << std::endl;
    while (!add_new_sections(read_user_section_name())) {}    //  добавить возможность отмены!!! + запрет на пустую строку
}

bool Library::add_new_sections(const std::string& section_titles) {
    std::vector<std::string> sections_titles = get_section_names_from_path(section_titles);    //  удалил const
    for (size_t i = 0; i < sections_titles.size(); ++i) {
        if (!add_new_section(sections_titles[i])) {
            return false;
        }
        if (!switch_current_section_to_subsection(sections_titles[i])) {
            return false;
        }
    }
    for (size_t i = 0; i < sections_titles.size(); ++i) {
        delete_last_location(_path_current_section);
    }
    return true;
}

bool Library::add_new_section(const std::string& section_title) {
    Library::Section* _current_section = get_current_section();
    if (_current_section->add_new_subsection(section_title)) {    //  установить запрет на создание секции с одинаковым названием
        message_string("\"Создан подраздел: " + _path_current_section + "/" + section_title + "\"");
        return true;
    }
    message_not_valuable_existion_name();
    return false;
}

void Library::switch_current_section_to_subsection() {
    std::cout << "Введите название подраздела:" << std::endl;
    while (!switch_current_section_to_subsection(read_user_section_name())) {}
    std::cout << "Открыт подраздел: \"" << _path_current_section << '\"' << std::endl;
}

bool Library::switch_current_section_to_subsection(const std::string& subsection_title) {
    Library::Section* _current_section = get_current_section();
    std::vector<std::string> sections_titles = get_section_names_from_path(subsection_title);
    for (size_t i = 0; i < sections_titles.size(); ++i) {
        if(!_current_section->open_subsection(sections_titles[i])) {
            message_name_do_not_exist();
            return false;
        }
        _current_section = _current_section->find_and_open_subsection(sections_titles[i]);
    }
    _path_current_section += "/";
    _path_current_section += subsection_title;
    return true;
}

void Library::switch_subsection_to_section() {
    if (_path_current_section == _name) {
        std::cout << "Текущий подраздел уже является верхним." << std::endl;
        return;
    }
    delete_last_location(_path_current_section);
    std::cout << "Открыт раздел: \"" << _path_current_section << '\"' << std::endl;
}

void Library::show_subsections() {
    Library::Section* _current_section = get_current_section();
    std::cout << "Подразделы раздела \"" << _path_current_section << "\":" << std::endl;
    _current_section->print_subsections();
}

Library::Section* Library::get_current_section() {
    return get_section(_path_current_section);
}

Library::Section* Library::get_section(const std::string& section_location) {
    Library::Section* section = &_user_accosiations;
    std::vector<std::string> sections_titles = get_section_names_from_path(section_location);
    for (size_t i = 0; i < sections_titles.size(); ++i) {
        if (i != 0) {
            section = section->find_and_open_subsection(sections_titles[i]);
        }
    }
    return section;
}

void Library::create_new_book() {
    std::cout << "Введите название книги:" << std::endl;
    while (!add_new_book(read_not_empty_user_work())) {}
}

bool Library::add_new_book(const std::string& book_title) {
    Library::Section* section = get_current_section();
    if (check_same_name_book_exists(section->get_all_books_numbers(), book_title)) {
        message_book_with_this_name_already_exists();
        std::cout << "Введите название книги:" << std::endl;
        return false;
    }
    _storage.push_back(Book(book_title, _path_current_section, get_library_global_associations()));
    section->add_new_book_to_current_section(_storage.size() - 1);
    std::cout << "Книга \"" << book_title << "\" успешно создана и сохранена в разделе: \"" << _path_current_section << "\"." << std::endl;
    return true;
}

std::string Library::get_book_title_by_individual_number(size_t individual_number) {
    if (individual_number >= _storage.size()) {
        throw (char*)"Запрошена книга по адресу, выходящему за пределы хранилища книг.";
    }
    return _storage[individual_number].get_book_title();
}

bool Library::check_same_name_book_exists(const std::vector<size_t>& books_individual_numbers, const std::string& title_for_check) {
    for (size_t book_number : books_individual_numbers) {
        if (get_book_title_by_individual_number(book_number) == title_for_check) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> Library::get_library_global_associations() {
    std::vector<std::string> global_associations;
    for (Association association : _user_book_associations) {
        if (association.check_global()) {
            global_associations.push_back(association.get_association_name());
        }
    }
    return global_associations;
}

void Library::print_user_book() {
    std::cout << "Введите название книги:" << std::endl;
    while (!print_user_book(read_not_empty_user_work())) {}
}

bool Library::print_user_book(const std::string& book_title) {
    Library::Section* section = get_current_section();
    int bookNumber = get_book_number(section->get_all_books_numbers(), book_title);
    if (bookNumber == -1) {
        message_string("Не найдена книга для печати.");
        std::cout << "Введите название книги:" << std::endl;
        return false;
    }
    _storage[bookNumber].print_book();
    return true;
}
    
int Library::get_book_number(const std::vector<size_t>& books_individual_numbers, const std::string& title_for_check) {
    for (size_t book_number : books_individual_numbers) {
        if (get_book_title_by_individual_number(book_number) == title_for_check) {
            return (int)book_number;
        }
    }
    return -1;
}

void Library::change_section_title() {
    std::cout << "Введите новое название текущего раздела:" << std::endl;
    while (!change_section_title(read_not_empty_user_work())) {}
}

bool Library::change_section_title(const std::string& section_new_name) {
    if (empty_or_with_slash(section_new_name) || section_above_exists(section_new_name)) {
        std::cout << "Некорректное название для раздела." << std::endl;
        std::cout << "Введите новое название текущего раздела (запрещено использовать \"/\" и \" \"):" << std::endl;
        return false;
    }
    Library::Section* section = get_current_section();
    std::vector<size_t> books_for_change_location;
    section->change_section_title(section_new_name, books_for_change_location);
    for (size_t i = 0; i < books_for_change_location.size(); ++i) {
        _storage[books_for_change_location[i]].change_section_location(section_new_name, get_number_current_section(section->get_section_location()));
    }
    if (_path_current_section == _name) {
        _name = section->get_section_location();
    }
    _path_current_section = section->get_section_location();
    return true;
}

bool Library::section_above_exists(const std::string& section_new_name) {
    if (_path_current_section == _name) {
        return false;
    }
    std::string current_path = _path_current_section;
    delete_last_location(current_path);
    Library::Section* section = get_section(current_path);
    return section->check_subsection_exists(section_new_name);
}

void Library::show_books() {
    Library::Section* _current_section = get_current_section();
    std::cout << "Книги раздела \"" << _path_current_section << "\":" << std::endl;
    std::vector<size_t> books = _current_section->get_all_books_numbers();
    for (size_t i = 0; i < books.size(); ++i) {
        std::cout << "  \"" << _storage[books[i]].get_book_title() << '\"' << std::endl;
    }
};

bool Library::check_book_exists(const std::vector<size_t>& books_for_check, const std::string& book_name) {
    if (get_book_number(books_for_check, book_name) == -1) {
        return false;
    }
    return true;
}

void Library::change_book_title() {
    std::cout << "Введите старое название книги:" << std::endl;
    while (!change_book_title(read_not_empty_user_work())) {}
}

bool Library::change_book_title(const std::string& old_book_title) {
    Library::Section* section = get_current_section();
    if (!check_book_exists(section->get_all_books_numbers(), old_book_title)) {
        std::cout << "Книга с указанным названием не найдена." << std::endl;
        std::cout << "Введите старое название книги:" << std::endl;
        return false;
    }
    while (!change_book_title(old_book_title, read_not_empty_user_work())) {}
    return true;
}

bool Library::change_book_title(const std::string& old_book_title, const std::string& new_book_title) {
    Library::Section* section = get_current_section();
    if (check_book_exists(section->get_all_books_numbers(), new_book_title)) {
        std::cout << "Книга с новым названием уже существует." << std::endl;
        std::cout << "Введите другое новое название книги:" << std::endl;
        return false;
    }
    _storage[get_book_number(section->get_all_books_numbers(), old_book_title)].set_book_title(new_book_title);
    return true;
}

void Library::change_book_fields() {
    std::cout << "Введите название изменяемой книги:" << std::endl;
    while (!change_book_fields(read_not_empty_user_work())) {}
}

bool Library::change_book_fields(const std::string& book_name) {
    Library::Section* section = get_current_section();
    if (!check_book_exists(section->get_all_books_numbers(), book_name)) {
        std::cout << "Книга с указанным названием не найдена." << std::endl;
        std::cout << "Введите название изменяемой книги:" << std::endl;
        return false;
    }
    while(book_terminal((size_t)get_book_number(section->get_all_books_numbers(), book_name))) {}
    return true;
}

bool Library::book_terminal(size_t book_num) {
    std::cout << "Для выведения списка команд книжного терминала, введите \"команды\"." << std::endl;
    std::cout << "Чтобы завершить работу с книгой, введите \"завершить\"." << std::endl;
    std::cout << _storage[book_num].get_book_title() << " $ ";
    std::string command = read_user_terminal();
    if (command == "команды") {
        book_commands();
        return true;
    }
    if (command == "завершить") {
        std::cout << "Работа в книжном терминале завершена." << std::endl;
        return false;
    }
    
    try {
        if (command == "создать поле") {
            add_book_field(book_num);
            _system_saved = false;
            return true;
        }
        if (command == "редактировать поле") {
            edit_book_field(book_num);
            _system_saved = false;
            return true;
        }
        if (command == "удалить поле") {
            delete_book_field(book_num);
            _system_saved = false;
            return true;
        }
        if (command == "печать книги") {
            _storage[book_num].print_book();
            return true;
        }
        if (command == "установить глобальное поле") {
            set_global_field();
            _system_saved = false;
            return true;
        }
        if (command == "показать глобальные поля") {
            show_global_fields();
            return true;
        }
        if (command == "снять глобальное поле") {
            delete_global_field();
            _system_saved = false;
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
    return true;    //  поддерживает работу книжного терминала
}

void Library::add_book_field(size_t book_num) {
    std::cout << "Введите наименование добавляемого поля:" << std::endl;
    while (!add_book_field(book_num, read_not_empty_user_work())) {}
}

bool Library::add_book_field(size_t book_num, const std::string& field_title) {
    if (_storage[book_num].book_association_exists(field_title)) {
        std::cout << "Поле \"" << field_title << "\" уже существует." << std::endl;
        std::cout << "Введите наименование добавляемого поля:" << std::endl;
        return false;
    }
    std::cout << "Введите содержимое поля:" << std::endl;
    _storage[book_num].add_user_book_association(field_title, read_not_empty_user_work());
    if (!field_exists(field_title)) {
        _user_book_associations.push_back(Association(field_title));
    }
    _user_book_associations[get_field_number(field_title)].add_book(book_num);
    return true;
}

bool Library::field_exists(const std::string& field_title) {
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].get_association_name() == field_title) {
            return true;
        }
    }
    return false;
}

size_t Library::get_field_number(const std::string& field_title) {
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].get_association_name() == field_title) {
            return i;
        }
    }
    std::cerr << field_title << ' ';
    throw (char*)"Внутренняя критическая ошибка: не найдена ассоциация";
}

void Library::edit_book_field(size_t book_num) {
    std::cout << "Введите наименование изменяемого поля:" << std::endl;
    while (!edit_book_field(book_num, read_not_empty_user_work())) {}
}

bool Library::edit_book_field(size_t book_num, const std::string& field_title) {
    if (!_storage[book_num].book_association_exists(field_title)) {
        std::cout << "Поле \"" << field_title << "\" не существует." << std::endl;
        std::cout << "Введите наименование изменяемого поля:" << std::endl;
        return false;
    }
    std::cout << "Старое содержимое поля: " << _storage[book_num].get_book_association_field(field_title) << std::endl;
    std::cout << "Введите новое содержимое поля:" << std::endl;
    std::string answer = read_not_empty_user_work();
    if (_storage[book_num].empty_association(field_title)) {
        _user_book_associations[get_field_number(field_title)].add_book(book_num);
    }
    _storage[book_num].change_association_field(field_title, answer);
    return true;
}

void Library::delete_book_field(size_t book_num) {
    std::cout << "Введите наименование удаляемого поля:" << std::endl;
    while (!delete_book_field(book_num, read_not_empty_user_work())) {}
}

bool Library::delete_book_field(size_t book_num, const std::string& field_title) {
    if (!_storage[book_num].book_association_exists(field_title)) {
        std::cout << "Поле \"" << field_title << "\" не существует." << std::endl;
        std::cout << "Введите наименование удаляемого поля:" << std::endl;
        return false;
    }
    _storage[book_num].delete_book_local_association(field_title);
    delete_book_in_user_association(book_num, field_title);
    if (field_is_global(field_title)) {
        _storage[book_num].add_user_book_association(field_title, "");
        std::cout << "Глобальное поле \"" << field_title << "\" очищено." << std::endl;
        return true;
    }
    std::cout << "Поле \"" << field_title << "\" удалено из книги." << std::endl;
    return true;
}

bool Library::field_is_global(const std::string& field_title) {
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].get_association_name() == field_title) {
            return _user_book_associations[i].check_global();
        }
    }
    return false;
}


void Library::delete_book_in_user_association(size_t book_num, const std::string& field_title) {
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].get_association_name() == field_title) {
            _user_book_associations[i].delete_book(book_num);
            if (_user_book_associations[i].empty() && !_user_book_associations[i].check_global()) {
                _user_book_associations[i].delete_user_book_association();
            }
            return;
        }
    }
    throw (char*)"Внутренняя критическая ошибка: не найдена глобальная ассоциация (при проверке на пустоту)";
}
        
void Library::set_global_field() {
    std::cout << "Введите название поля для установления глобальным:" << std::endl;
    while (!set_global_field(read_not_empty_user_work())) {}
}

bool Library::set_global_field(const std::string& global_field_name) {
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].get_association_name() == global_field_name) {
            if (_user_book_associations[i].check_global()) {
                std::cout << "Данное поле уже установлено глобальным." << std::endl;
                return false;
            }
            set_global_field(get_field_number(global_field_name));
            return true;
        }
    }
    _user_book_associations.push_back(Association(global_field_name));
    return set_global_field(_user_book_associations.size() - 1);
}

bool Library::set_global_field(size_t _user_book_associations_number) {
    if (_user_book_associations_number >= _user_book_associations.size()) {
        throw (char*)"Внутренняя критическая ошибка: номер ассоциации превышает допустимый.";
    }
    for (size_t i = 0; i < _storage.size(); ++i) {
        if (!_storage[i].book_association_exists(_user_book_associations[_user_book_associations_number]
                                                 .get_association_name())) {
            _storage[i].add_empty_global_association(_user_book_associations[_user_book_associations_number]
                                                     .get_association_name());
        }
    }
    _user_book_associations[_user_book_associations_number].set_global(true);
    std::cout << "Поле \"" << _user_book_associations[_user_book_associations_number].get_association_name() << "\" установлено глобальным." << std::endl;
    return true;
}

bool Library::check_field_is_global(const std::string& global_field_name) {
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].get_association_name() == global_field_name) {
            if (_user_book_associations[i].check_global()) {
                return true;
            }
            return false;
        }
    }
    std::cout << "Данное поле не найдено." << std::endl;
    return false;
}

void Library::show_global_fields() {
    std::cout << "Все установленные глобальные поля:" << std::endl;
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].check_global()) {
            std::cout << "  \"" << _user_book_associations[i].get_association_name() << "\"" << std::endl;
        }
    }
}

void Library::delete_global_field() {
    std::cout << "Введите название поля со снимаемым глобальным значением:" << std::endl;
    while (!delete_global_field(read_not_empty_user_work())) {}
}

bool Library::delete_global_field(const std::string& global_field_name) {
    if (!field_exists(global_field_name)) {
        std::cout << "Данное поле не существует." << std::endl;
        std::cout << "Введите название поля для снятия значения глобального:" << std::endl;
        return false;
    }
    if (!check_field_is_global(global_field_name)) {
        std::cout << "Данное поле не является глобальным." << std::endl;
        std::cout << "Введите название поля для снятия значения глобального:" << std::endl;
        return false;
    }
    for (size_t i = 0; i < _storage.size(); ++i) {
        if (_storage[i].get_book_association_field(global_field_name).empty()) {
            _storage[i].delete_book_local_association(global_field_name);
        }    //  если есть пустые книги - то я очев эти поля сотру, а если поля не пустые - я их не трогаю
    }
    _user_book_associations[get_field_number(global_field_name)].set_global(false);
    return true;
}

void Library::delete_book() {
    std::cout << "Введите название удаляемой книги:" << std::endl;
    while (!delete_book(read_not_empty_user_work())) {}
}

bool Library::delete_book(const std::string& book_for_delete) {
    Library::Section* section = get_current_section();
    if (!check_book_exists(section->get_all_books_numbers(), book_for_delete)) {
        std::cout << "Книга с указанным названием не найдена." << std::endl;
        std::cout << "Введите название удаляемой книги:" << std::endl;
        return false;
    }
    size_t book_for_delete_number = (size_t)get_book_number(section->get_all_books_numbers(), book_for_delete);
    std::vector<std::string> associations;
    _storage[book_for_delete_number].delete_book(associations);
    for (size_t i = 0; i < associations.size(); ++i) {
        delete_book_association(associations[i], book_for_delete_number);
    }
    section->delete_book(book_for_delete_number);
    std::cout << "Книга \"" << book_for_delete << "\" удалена." << std::endl;
    return true;
}

void Library::delete_section() {    //  затестить на неправильное название
    std::cout << "Введите название удаляемого подраздела:" << std::endl;
    while (!delete_section(read_not_empty_user_work())) {}
}

bool Library::delete_section(const std::string& section_for_delete) {
    Library::Section* section = get_current_section();
    if (!section->open_subsection(section_for_delete)) {
        std::cout << "Подраздел с указанным названием не найден." << std::endl;
        std::cout << "Введите название удаляемого раздела:" << std::endl;
        return false;
    }
    std::vector<size_t> books_for_delete;
    section->find_and_open_subsection(section_for_delete)->delete_section(books_for_delete);
    for (size_t i = 0; i < books_for_delete.size(); ++i) {
        std::vector<std::string> deleted_book_associations;
        _storage[books_for_delete[i]].delete_book(deleted_book_associations);
        for (size_t j = 0; j < deleted_book_associations.size(); ++j) {
            delete_book_association(deleted_book_associations[j], i);
        }
    }
    std::cout << "Подраздел \"" << section_for_delete << "\" со всем его содержимым удален." << std::endl;
    return true;
}

void Library::delete_book_association(const std::string& association_title, size_t book_number) {
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (_user_book_associations[i].get_association_name() == association_title) {
            _user_book_associations[i].delete_book(book_number);
        }
    }
}

void  Library::show_system_parameters() {
    std::cout << "Название системы: " << _name << std::endl;
    std::cout << "Дата создания системы: " << _foundation_time << std::endl;
    std::cout << "Время последнего сохранения системы: " << _application_time << std::endl;
    if (_location == "") {
        std::cout << "Местоположение файла для сохранения системы не задано." << std::endl;
        return;
    }
    std::cout << "Местоположение файла сохранения системы: " << _location << std::endl;
}
