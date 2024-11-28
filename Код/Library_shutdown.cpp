//
//  Library_shutdown.cpp
//  modern_library
//
//  Created by Иван Трифонов on 07.10.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include "Library.hpp"

void Library::exit_mode() {
    if (_system_saved) {
        exit_mode_saved_read_user();
        return;
    }
    exit_mode_read_user();
}

void Library::save_as() {
    get_system_location();
    save_system();
}

void Library::save_system() {
    compress_system();
    if (write_to_location_fail()) {
        get_system_location();
    }
    _application_time = get_current_time();
    _system_saved = serialize_system();
    if (!_system_saved) {
        std::cout << "Ошибка сохранения системы. Повторите попытку или попробуйте \"сохранить как\"" << std::endl;
        return;
    }
    update_earlier_opened_systems_file();
    message_system_saved();
}

bool Library::write_to_location_fail() {
    std::ofstream file(_location);
    return file.fail();
}

void Library::get_system_location() {
    std::cout << "Введите местоположение папки для сохранения системы; через слэш введите выбираемое название файла системы:" << std::endl;
    while (!get_system_location(read_not_empty_user_work())) {
        std::cout << "Ошибка. Введите корректное местоположение существующей папки для сохранения системы; через слэш введите выбираемое новое непустое название файла системы:" << std::endl;
    }
}

bool Library::get_system_location(const std::string& location_to_save) {
    if (location_to_save.back() == '/') {
        return false;
    }
    std::ofstream file(location_to_save + ".mdlb");
    if (file.fail()) {
        return false;
    }
    _location = location_to_save + ".mdlb";
    return true;
}

bool Library::serialize_system() {
    std::ofstream file(_location, std::ios::out | std::ios::trunc);
    if (file.fail()) {
        return false;
    }
    std::string config_info = serialize_config_system_info();
    std::string content_info = "checkpoint: _storage\n" + serialize_storage() + "\ncheckpoint: _sections\n" + serialize_sections() + "\ncheckpoint: _associations\n" + serialize_user_book_associations();
    file << AES_encryptString(config_info + "\n" + content_info, system_key) << '\n';
    return true;
}

std::string Library::serialize_config_system_info() {
    return _name + '\n' + _foundation_time + '\n' + _application_time;
}

std::string Library::serialize_storage() {
    std::string return_str = std::to_string(_storage.size());    //  кол-во книг
    for (size_t i = 0; i < _storage.size(); ++i) {
        return_str += "\n";
        return_str += _storage[i].get_book_title();
        return_str += "\n";
        return_str += _storage[i].get_book_section();
        return_str += "\n";
        return_str += _storage[i].get_book_foundation_time();
        return_str += "\n";
        return_str += _storage[i].get_book_associations();
    }
    return return_str;
}

std::string Library::serialize_sections() {
    return serialize_sections(&_user_accosiations);
}

std::string Library::serialize_sections(Library::Section* section) {
    std::string return_str = section->get_section_location();
    return_str += "\n";
    return_str += section->get_section_title();
    return_str += "\n";
    return_str += section->get_section_foundation_time();
    return_str += "\n";
    return_str += section->get_all_books_in_string();
    return_str += "\n";
    return_str += std::to_string(section->subsections_size());
    for (size_t i = 0; i < section->subsections_size(); ++i) {
        return_str += "\n";
        return_str += serialize_sections(section->get_subsection_pointer(i));
    }
    return return_str;
}

std::string Library::serialize_user_book_associations() {
    std::string return_str = std::to_string(_user_book_associations.size());
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        return_str += "\n";
        return_str += _user_book_associations[i].get_association_name();
        return_str += "\n";
        return_str += std::to_string(_user_book_associations[i].check_global());
        return_str += "\n";
        return_str += _user_book_associations[i].get_all_books_number_string();
    }
    return return_str;
}

void Library::compress_system() {
    compress_earlier_opened_systems_locations();
    compress_user_book_associations();
    compress_user_book_local_associations();
    compress_storage();
    compress_dependencies();
}

void Library::compress_earlier_opened_systems_locations() {
    std::vector<std::string> earlier_opened_systems_locations;
    for (size_t i = 0; i < _earlier_opened_systems_locations.size(); ++i) {
        if (!_earlier_opened_systems_locations[i].empty()) {
            earlier_opened_systems_locations.push_back(_earlier_opened_systems_locations[i]);
        }
    }
    std::swap(_earlier_opened_systems_locations, earlier_opened_systems_locations);
}

void Library::compress_storage() {
    std::vector<Library::Book> storage;
    for (size_t i = 0; i < _storage.size(); ++i) {
        if (!_storage[i].empty()) {
            storage.push_back(_storage[i]);
        }
    }
    change_user_book_associations(storage);
    change_user_book_sections(storage);
    std::swap(_storage, storage);
}

void Library::compress_user_book_associations() {
    std::vector<Library::Association> user_book_associations;
    for (size_t i = 0; i < _user_book_associations.size(); ++i) {
        if (!_user_book_associations[i].empty()) {
            user_book_associations.push_back(_user_book_associations[i]);
        }
    }
    std::swap(_user_book_associations, user_book_associations);
}

void Library::change_user_book_associations(std::vector<Library::Book>& storage) {
    delete_books_in_user_associations();
    fill_user_book_associations(storage);
}

void Library::delete_books_in_user_associations() {
    for (Library::Association association : _user_book_associations) {
        association.delete_all_user_books();
    }
}

void Library::fill_user_book_associations(std::vector<Library::Book>& storage) {
    for (size_t i = 0; i < storage.size(); ++i) {
        for (size_t j = 0; j < storage[i].size(); ++j) {
            if (!storage[i].get_book_association_field(j).empty()) {
                _user_book_associations[get_field_number(storage[i].get_book_association_title(j))].add_book(i);
            }
        }
    }
}

void Library::compress_user_book_local_associations() {
    for (size_t i = 0; i < _storage.size(); ++i) {
        _storage[i].compress_user_book_local_associations();
    }
}

void Library::change_user_book_sections(std::vector<Library::Book>& storage) {
    delete_books_in_user_sections();
    fill_user_book_sections(storage);
}

void Library::delete_books_in_user_sections() {
    delete_books_in_user_sections(&_user_accosiations);
}

void Library::delete_books_in_user_sections(Library::Section* current_section) {
    current_section->clear_all_books();
    for (size_t i = 0; i < current_section->subsections_size(); ++i) {
        delete_books_in_user_sections(current_section->get_subsection_pointer(i));
    }
}

void Library::fill_user_book_sections(std::vector<Library::Book>& storage) {
    for (size_t i = 0; i < storage.size(); ++i) {
        if (!get_section(storage[i].get_book_section())->add_new_book_to_current_section(i)) {
            throw (char*)"Внутренняя критическая ошибка: не добавлена книга в секцию при сжатии.";
        }
    }
}

void Library::compress_dependencies() {
    _user_accosiations.compress_dependencies();
}

void Library::update_earlier_opened_systems_file() {
    deserialize_earlier_opened_systems();
    _earlier_opened_systems_locations.push_back(_location);
    std::vector<std::string> earlier_opened_systems_locations;
    for (size_t i = 0; i < _earlier_opened_systems_locations.size(); ++i) {
        bool written_earlier = false;
        for (size_t j = 0; j < earlier_opened_systems_locations.size(); ++j) {
            if (earlier_opened_systems_locations[j] == _earlier_opened_systems_locations[i]) {
                written_earlier = true;
                break;
            }
        }
        if (!written_earlier) {
            earlier_opened_systems_locations.push_back(_earlier_opened_systems_locations[i]);
        }
    }
    std::swap(_earlier_opened_systems_locations, earlier_opened_systems_locations);
    serialize_earlier_opened_systems();
}

void Library::serialize_earlier_opened_systems() {
    std::ofstream file(get_user_main_directory + slash + ".modern_library_systems.mdlb", std::ios::out | std::ios::trunc);
    size_t existing_systems = 0;
    for (size_t i = 0; i < _earlier_opened_systems_locations.size(); ++i) {
        if (!_earlier_opened_systems_locations[i].empty()) {
            if (_earlier_opened_systems_locations[i][0] != '.') {
                ++existing_systems;
            }
        }
    }
    std::string output_string = std::to_string(existing_systems) + '\n';
    for (size_t i = 0; i < _earlier_opened_systems_locations.size(); ++i) {
        if (!_earlier_opened_systems_locations[i].empty()) {
            if (_earlier_opened_systems_locations[i][0] != '.') {
                output_string += _earlier_opened_systems_locations[i];
                output_string += '\n';
            }
        }
    }
    file << AES_encryptString(output_string, system_key) << '\n';
}

void Library::deserialize_earlier_opened_systems() {
    _earlier_opened_systems_locations.clear();
    std::vector<std::string> earlier_opened_systems = read_systems_list();
    for (size_t i = 0; i < earlier_opened_systems.size(); ++i) {
        std::ifstream read_file(earlier_opened_systems[i]);
        if (!read_file.fail()) {
            _earlier_opened_systems_locations.push_back(earlier_opened_systems[i]);
        }
        read_file.close();
    }
}
