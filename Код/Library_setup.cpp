//
//  Library.cpp
//  modern_library
//
//  Created by Иван Трифонов on 17.09.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#include "Library.hpp"

void Library::one_argument(const std::vector<std::string>& list_of_systems) {
    try {
        if (list_of_systems.size() != 0) {  //  если существуют ранее открывавшиеся файлы
            write_info_about_earlier_opened_systems(list_of_systems);    //  ! желательно сохранять, прежде чем заново выводить
        }
        std::string answer = user_opening_choise();
        if (answer == "новая") {  //  создание новой системы учета книг в библиотеке
            make_library();
            return;  //  система для учета книг создана
        }
        read_system(answer);  //  открытие системы для учета книг в библиотеке по введенному местоположению файла
    } catch (const CancelException& ex) {
        one_argument(list_of_systems);
    }
}

void Library::one_argument() {  //  обычный запуск программы (без входных аргументов)
    update_earlier_opened_systems_file();
    std::vector<std::string> list_of_systems = read_systems_list();
    one_argument(list_of_systems);
}

void Library::two_arguments(std::string position) {
    read_system(position);
}

std::vector<std::string> Library::read_systems_list() {    //  вернуть спискок адресов ранее открывавшихся систем
    std::ifstream file_systems_list(get_user_main_directory + slash + ".modern_library_systems.mdlb");
    if (file_systems_list.fail()) {    //  не найден файл со списком ранее открывавшихся систем
        return std::vector<std::string>();
    }
    std::stringstream stream = read_file_decrypted_stream(file_systems_list);
    std::vector<std::string> list_location_systems = read_systems_file_systems_list(stream, static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str())));
    return list_location_systems;
}

std::vector<std::string> Library::read_systems_file_systems_list(std::istream& file, size_t count_systems) {    //  прочитать строчки с адресами ранее открывавшихся систем
    std::vector<std::string> list_location_systems(count_systems);
    for (size_t i = 0; i < count_systems; ++i) {
        list_location_systems[i] = read_string_from_stream(file);
    }
    return list_location_systems;
}

void Library::write_info_about_earlier_opened_systems(const std::vector<std::string>& list_of_systems) {
    message_earlier_opened_systems();
    for (size_t i = 0; i < list_of_systems.size(); ++i) {
        push_library_configuration_info(list_of_systems[i]);
    }
    serialize_earlier_opened_systems();
}

void Library::push_library_configuration_info(const std::string& system_file_location) {    //  печать исключтельно конфигурационных сведений о системе
    std::ifstream system_file(system_file_location);
    if (system_file.fail()) {
        return;
    }
    _earlier_opened_systems_locations.push_back(system_file_location);
    std::stringstream stream = read_file_decrypted_stream(system_file);
    print_config_library_info(stream, system_file_location);  //  вывод конфигурационных данных о системе
    system_file.close();
}

void Library::print_config_library_info(std::istream& stream, const std::string& system_location) {
    message_string("\"" + read_string_from_stream(stream) + "\"");    //  печать названия системы    //  !!!добавить криптографию
    message_string(system_location);    //  печать местоположения системы
    message_string("Время создания: " + read_string_from_stream(stream));    //  печать времени создания системы
    message_string("Время последнего сохранения: " + read_string_from_stream(stream));    //  печать времени последнего сохранения системы
}

std::string Library::user_opening_choise() {
    union_message_get_and_choose_mode_opening_system();
    return read_user_setup();
}

void Library::make_library() {    //  базовая инициализация новой системы для учета книг
    do {
        try {
            message_ask_system_name();
            _path_current_section = _name = read_user_setup();
        } catch (const CancelException& ex) {
            _name = "";
        }
    } while (forbidden_name(_name));
    _foundation_time = get_current_time();
    _user_accosiations.fill_empty_section(_name, _foundation_time);
}

void Library::read_system(const std::string& location) {
    std::ifstream file_system(location);
    if (file_system.fail()) {
        std::cerr << "\nОшибка открытия системы для учета книг в библиотеке.\n" << std::endl;
        std::cerr << "\nУбедитесь, что в самом названии системы или папок отсутствуют дополнительные символы, такие как \\." << std::endl;
        std::cerr << error_message << std::endl;
        one_argument();    //  открыть режим выбора системы для учета книг
        return;
    }
    _location = location;
    std::stringstream stream = read_file_decrypted_stream(file_system);
    file_system.close();
    if (!download_system(stream)) {
        clear_system();
        std::cerr << "\nОшибка: файл поврежден. Попробуйте другой файл или создайте новую систему.\n" << std::endl;
        one_argument();
    };
}

bool Library::download_system(std::istream& stream) {  //  функция загрузки найденной и выбранной системы для учета книг //  !!!добавить криптографию
    add_config_system_info(stream);
    if (read_string_from_stream(stream) != "checkpoint: _storage") {
        return false;
    }
    deserialize_storage(stream);
    if (read_string_from_stream(stream) != "checkpoint: _sections") {
        return false;
    }
    deserialize_sections(stream);
    if (read_string_from_stream(stream) != "checkpoint: _associations") {
        return false;
    }
    deserialize_associations(stream);
    _system_saved = true;
    return true;
}

void Library::add_config_system_info(std::istream& stream) {
    _name = read_string_from_stream(stream);
    _foundation_time = read_string_from_stream(stream);
    _application_time = read_string_from_stream(stream);
    _path_current_section = _name;
}

void Library::deserialize_storage(std::istream& stream) {
    size_t storage_size = static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str()));
    for (size_t i = 0; i < storage_size; ++i) {    //  !!!добавить криптографию
        std::string book_title = read_string_from_stream(stream);
        std::string book_section = read_string_from_stream(stream);
        std::string book_foundation_time = read_string_from_stream(stream);
        _storage.push_back(Book(book_title, book_section, book_foundation_time));
        _storage[i].add_from_file_book_associations(stream);
    }
}

void Library::deserialize_sections(std::istream& stream) {
    deserialize_sections(&_user_accosiations, stream);
    
}

void Library::deserialize_sections(Library::Section* section, std::istream& stream) {
    section->set_section_location(read_string_from_stream(stream));
    section->set_section_name(read_string_from_stream(stream));
    section->set_section_foundation_time(read_string_from_stream(stream));
    section->set_section_books(stream);
    size_t subsections_number = static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str()));
    for (size_t i = 0; i < subsections_number; ++i) {
        section->push_back_empty_subsection();
        deserialize_sections(section->get_last_subsection(), stream);
    }
}

void Library::deserialize_associations(std::istream& stream) {
    size_t associations_number = static_cast<size_t>(std::atoi(read_string_from_stream(stream).c_str()));
    for (size_t i = 0; i < associations_number; ++i) {
        _user_book_associations.push_back(Association(read_string_from_stream(stream)));
        _user_book_associations.back().set_global_setup(static_cast<bool>(std::atoi(read_string_from_stream(stream).c_str())));
        _user_book_associations.back().add_book_associations_from_file(stream);
    }
}

void Library::clear_system() {
    _name.clear();
    _location.clear();
    _foundation_time.clear();
    _application_time.clear();
    _earlier_opened_systems_locations.clear();
    _storage.clear();
    std::vector<size_t> books_for_delete;
    _user_accosiations.delete_section(books_for_delete);
    books_for_delete.clear();
    _user_book_associations.clear();
    _path_current_section.clear();
    _system_saved = false;
}
