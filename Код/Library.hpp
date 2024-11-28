//
//  Library.hpp
//  modern_library
//
//  Created by Иван Трифонов on 17.09.2024.
//  Copyright © 2024 Иван Трифонов. All rights reserved.
//

#ifndef Library_hpp
#define Library_hpp

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

#if defined(_WIN32)
const std::string get_user_main_directory = (std::string)getenv("HOMEDRIVE") + (std::string)getenv("HOMEPATH");
const char slash = '\\';
const std::string error_message = "Например, допустимо \"C:\\users\\admin\\system 1.mdlb\", но не допустимо \"C:\\users\\admin\\system\\ 1.mdlb\", в случае, если файл системы называется \"system 1.mdlb\".";
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS
#else
const std::string get_user_main_directory = (std::string)getenv("HOME");
const char slash = '/';
const std::string error_message = "Например, допустимо \"/Users/admin/system 1.mdlb\", но не допустимо \"/Users/admin/system\\ 1.mdlb\", в случае, если файл системы называется \"system 1.mdlb\".";
#endif

    //  фции выводимых пользователю сообщений

void message_string(std::string output_string);
void message_leave_without_save();
void message_words_to_stay();
void message_words_for_exit();
void message_words_to_continue_last_task();
void message_words_to_save_leave();
void message_ask_system_name();
void message_earlier_opened_systems();
void message_choose_old_or_new_system();
void message_get_location_new_system();
void message_key_word_to_make_new_system();
void message_continue_make_previous_program();
void message_forbidden_system_name();
void message_not_valuable_existion_name();
void message_name_do_not_exist();
void message_system_saved();
void message_command_not_find();
void message_ask_leave_or_not();
void message_forbidden_section_title_asked_accepted_title();
void message_book_with_this_name_already_exists();
void union_message_get_and_choose_mode_opening_system();
void union_message_exit_or_stay_words();
void union_message_exit_read_user();
void commands();    //  печать команд, доступных для выполнения пользователю
void book_commands();    //  печать команд книги, доступных для выполнения пользователю
void find_commands();

    //  фции для чтения команд, введенных пользователем

bool find_forbidden_combinations(const std::string& string_for_check);    //  проверка на пустоту и содержание недопустимых комбинаций при вводе названия раздела
bool empty_or_with_slash(const std::string& string_for_check);
bool forbidden_name(const std::string& string_for_check);    //  проверка на пустоту и содержание символа '/' в предлагаемом пользователем названии (системы, книги)
std::string getline_answer();    //  получение строчки, вводимой пользователем
bool check_if_exit(const std::string& answer);    //  проверка ответа на соответствие командам выхода
bool check_if_cancel(const std::string& answer);    //  проверка ответа на соответствие командам отмены
bool check_if_save_leave(const std::string& answer);    //  проверка ответа на соответствие командам сохранения
std::string leave_library();    //  окончательный выход из программы по запросу пользователя
std::string leave_library_work();
std::string exit_read_user_work();    //  действия при первичном запросе на выход из программы без сохранения
std::string exit_read_user_setup();
std::string read_user(const std::string& answer);    //  чтение запросов пользователя без сохранения
std::string read_user_work();
std::string read_not_empty_user_work();
std::string read_user_setup();
std::string read_user_terminal();    //  получение команды при запросе из терминала
std::string read_user_section_name();
void exit_mode_read_user();    //  подтверждение несохраняемого выхода из системы при команде из терминала
void exit_mode_saved_read_user();    //  подтверждение сохраненного выхода из системы при команде из терминала
class CancelException {};    //  исключение для отменяемой команды
class SaveLeaveException {};    //  исключение для отменяемой команды

    //  общие системные фции

std::string read_string_from_stream(std::istream& file);    //  получение строки из файла
std::stringstream read_file_decrypted_stream(std::ifstream& file);
std::string get_current_time();    //  получение текущего времени
std::vector<std::string> get_section_names_from_path(const std::string& path_to_section);    //  разбиение входной строки на названия разделов
size_t get_first_part_symbol(const std::string& main_string, size_t part_number);    //  получение номера первого символа интересующего раздела
std::string collect_string_before_symbol(const std::string& main_string, size_t i);    //  получить строку до символа
std::string collect_string_after_symbol_with_symbol(const std::string& main_string, size_t i);    //  получить строку после символа
void change_part_of_location_title(std::string& string_to_change, const std::string& new_part, size_t number_part_to_change);    //  поменять подраздел строки в адресе
size_t get_number_current_section(const std::string& current_section_location);    //  получить номер текущей секции из адреса
void delete_last_location(std::string& location_to_change);    //  удалить наименование последнего расположения в адресе
void delete_string_from_vector(std::vector<std::string>& vector_to_change, size_t num_string_to_delete);
std::string AES_encryptString(const std::string& plaintext, const uint8_t key[16]);
std::string AES_decryptString(const std::string& ciphertext, const uint8_t key[16]);
static const uint8_t system_key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0xcf, 0x19, 0x24, 0x1b, 0xa6, 0x68};

class Library {
    class Section {
        std::string _section_location = "";    //    местоположение текущего раздела (содержит название раздела)
        std::string _section_title = "";    //    наименование раздела
        std::string _section_foundation_time = "";    //  время создания раздела
        std::vector<size_t> _books;    //    книги раздела
        std::vector<Section> _dependencies;    //    подразделы раздела
    public:
        Section() {}
        Section(const std::string& section_title, const std::string& section_location_without_name);
        std::string get_section_title();    //  возвращает наименование раздела
        void fill_empty_section(const std::string& section_title, const std::string& section_foundation_time);    //  создание раздела в новой системе
        void set_section_location(const std::string& section_location);
        void set_section_name(const std::string& section_name);
        void set_section_foundation_time(const std::string& section_foundation_time);
        void set_section_books(std::istream& stream);
        void push_back_empty_subsection();
        Library::Section* get_last_subsection();
        bool add_new_subsection(const std::string& subsection_name);
        bool open_subsection(const std::string& subsection_name);    //  найти, существует ли подраздел с таким названием
        int get_subsection_number(const std::string& subsection_name);    //  определить номер подраздела
        Library::Section* find_and_open_subsection(const std::string& short_section_name);    //   установить текущий раздел на подраздел системы по названию
        std::vector<size_t> get_all_books_numbers();
        bool check_book_exists(size_t book_num);
        bool add_new_book_to_current_section(size_t book_num);
        void get_all_books(std::vector<size_t>& books);
        std::string get_all_books_in_string();
        void change_subsection_title(const std::string& new_section_title, size_t section_location_number, std::vector<size_t>& books_to_change_location);
        void change_section_title(const std::string& new_section_title, std::vector<size_t>& books_to_change_location);
        std::string get_section_location();
        void print_subsections();    //  вывести пользователю названия подразделов
        void delete_section(std::vector<size_t>& books_for_delete);
        void delete_book(size_t book_to_delete);
        void clear_all_books();
        size_t subsections_size();
        Library::Section* get_subsection_pointer(size_t subsection_number);
        bool empty();    //  true, если секция была удалена
        void compress_dependencies();
        std::string get_section_foundation_time();
        bool check_subsection_exists(const std::string& subsection_name);
    };
    
    class Book {
        class BookAssociations {
            std::string _association_title;
            std::string _value;
            bool _empty = true;
        public:
            BookAssociations() {}
            BookAssociations(const std::string& association_title, const std::string& value);
            std::string get_association_title();
            std::string get_value();
            void set_association(const std::string& association_title, const std::string& value);
            void set_value(const std::string& value);
            bool empty();
            bool empty_value();
            void delete_book_local_association();
        };
        std::string _book_title = "";    //  наименование книги
        std::string _book_section = "";    //  пользовательское местоположение книги
        std::string _book_foundation_time = "";
        std::vector<BookAssociations> _user_book_local_associations;    //  ассоциации пользователей по книге
    public:
        Book() {}
        Book(const std::string& book_title, const std::string& book_section, const std::vector<std::string>& global_associations);
        Book(const std::string& book_title, const std::string& book_section, const std::string& book_foundation_time);
        void add_from_file_book_associations(std::istream& stream);
        void add_empty_global_associations(const std::vector<std::string>& global_associations);
        void add_empty_global_association(const std::string& global_association);
        void set_book_title(const std::string& book_title);
        void add_user_book_association(const std::string& user_association, const std::string& value);  //  добавляет пользовательскую ассоциацию и значение
        void change_user_association(const std::string& user_association, const std::string& value);
        void print_book();
        void change_section(const std::string& section);
        std::string get_book_title();
        std::string get_book_section();
        void change_section_location(const std::string& new_subsection_title, size_t subsection_location_number);
        bool book_association_exists(const std::string& book_association);
        std::string get_book_association_title(size_t i);
        std::string get_book_association_field(size_t i);
        std::string get_book_association_field(const std::string& association_title);
        void change_association_field(const std::string& association_title, const std::string& association_field);
        void delete_book_local_association(const std::string& association_title);
        void delete_book(std::vector<std::string>& associations);
        bool empty_association(const std::string& association_title);
        bool empty();
        size_t size();
        void compress_user_book_local_associations();
        std::string get_book_foundation_time();
        std::string get_book_associations();
    };
    
    class Association {
        std::string _association_name = "";
        std::set<size_t> _books;
        bool _global = false;
    public:
        Association(const std::string& association_name);
        void add_book_associations_from_file(std::istream& stream);
        void set_association_name(const std::string& association_name);
        std::string get_association_name();
        void add_book(size_t book_number);
        void delete_book(size_t book_number);
        std::vector<size_t> get_all_books_number();
        std::string get_all_books_number_string();
        bool check_global();
        void set_global_setup(bool global);
        void set_global(bool global);
        bool empty();
        void delete_user_book_association();
        void delete_all_user_books();
    };
    //  общие сведения о системе
    std::string _name = "";    //  имя системы для учета книг
    std::string _location = "";    //  местоположение файла системы для создания и учета книг
    std::string _foundation_time = "";    //  дата и время создания системы для учета книг
    std::string _application_time = "";    //  дата и время последнего обращения к системе для учета книг
    std::vector<std::string> _earlier_opened_systems_locations;    //  список местоположений ранее открывавшихся систем
    // содержимое системы
    std::vector<Library::Book> _storage;    //  хранилище книг в системе для учета книг
    Section _user_accosiations;    // самая большая секция - вся система для учета книг
    std::vector<Association> _user_book_associations;    //  все существуюшие поля книг
    std::string _path_current_section = "";    //  текущий раздел библиотеки
    bool _system_saved = false;    //  информация о том, сохранена ли текущая система в файле
    //  параметры поиска
    bool _book_full_name = false;
    bool _field_full_name = false;
    bool _field_value_full_name = false;
    bool _section_full_name = false;
    std::string _book_name_to_search = "";
    std::vector<std::string> _fields_to_search;
    std::vector<std::string> _fields_value_to_search;
    std::string _section_name_to_search = "";
public:
    //  фции запуска системы
    void one_argument();    //  запуск системы в режиме "по умолчанию"
    void one_argument(const std::vector<std::string>& list_of_systems);    //  запуск системы со списком открывавшихся ранее систем
    void two_arguments(std::string position);    //  запуск системы в режиме открытия системы по заданному местоположению
    std::vector<std::string> read_systems_list();    //  вернуть спискок адресов ранее открывавшихся систем
    std::vector<std::string> read_systems_file_systems_list(std::istream& file, size_t count_systems);    //  вернуть строчки с адресами ранее открывавшихся систем
    void write_info_about_earlier_opened_systems(const std::vector<std::string>& list_of_systems);    //  печать информации о каждой найденной системе
    void push_library_configuration_info(const std::string& system_file_location);
    void print_config_library_info(std::istream& stream, const std::string& system_location);
    std::string user_opening_choise();    //  запрос и получение информации о выборе пользователем системы для работы
    void make_library();    //  создание пустой системы
    void read_system(const std::string& location);    //  открыть сохраненную библиотеку
    bool download_system(std::istream& stream);    //  скачать открытую библиотеку в программу
    void add_config_system_info(std::istream& stream);    //  добавление конфигурационных сведений о системе при создании системы
    void deserialize_storage(std::istream& stream);
    void deserialize_sections(std::istream& stream);
    void deserialize_sections(Library::Section* section, std::istream& stream);
    void deserialize_associations(std::istream& stream);
    void clear_system();
    
    //  фции работы с Library
    void terminal();    //  обработчик команд, посылаемых пользователем
    void add_new_section();    //  создать новый раздел
    bool add_new_sections(const std::string& sections_title);
    bool add_new_section(const std::string& section_title);    //  создать новый раздел с именованием
    void switch_current_section_to_subsection();    //  поменять текущий раздел на подраздел
    bool switch_current_section_to_subsection(const std::string& subsection_title);    //  поменять текущий раздел на заданный подраздел
    void switch_subsection_to_section();
    void show_subsections();    //  показать пользователю все текущие подразделы раздела
    Library::Section* get_current_section();    //  получает доступ к интересующему разделу для внесения изменений
    Library::Section* get_section(const std::string& section_location);
    void create_new_book();
    bool add_new_book(const std::string& book_title);
    std::string get_book_title_by_individual_number(size_t individual_number);
    bool check_same_name_book_exists(const std::vector<size_t>& books_individual_numbers, const std::string& title_for_check);
    std::vector<std::string> get_library_global_associations();
    void print_user_book();
    bool print_user_book(const std::string& book_title);
    int get_book_number(const std::vector<size_t>& books_individual_numbers, const std::string& title_for_check);
    void change_section_title();
    bool change_section_title(const std::string& section_new_name);
    bool section_above_exists(const std::string& section_new_name);
    void show_books();
    bool check_book_exists(const std::vector<size_t>& books_for_check, const std::string& book_name);
    void change_book_title();
    bool change_book_title(const std::string& old_book_title);
    bool change_book_title(const std::string& old_book_title, const std::string& new_book_title);
    void change_book_fields();
    bool change_book_fields(const std::string& book_name);
    bool book_terminal(size_t book_num);
    void add_book_field(size_t book_num);
    bool add_book_field(size_t book_num, const std::string& field_title);
    bool field_exists(const std::string& field_title);
    size_t get_field_number(const std::string& field_title);
    void edit_book_field(size_t book_num);
    bool edit_book_field(size_t book_num, const std::string& field_title);
    void delete_book_field(size_t book_num);
    bool delete_book_field(size_t book_num, const std::string& field_title);
    bool field_is_global(const std::string& field_title);
    void delete_book_in_user_association(size_t book_num, const std::string& field_title);
    void set_global_field();
    bool set_global_field(const std::string& global_field_name);
    bool set_global_field(size_t _user_book_associations_number);
    bool check_field_is_global(const std::string& global_field_name);
    void show_global_fields();
    void delete_global_field();
    bool delete_global_field(const std::string& global_field_name);
    void delete_book();
    bool delete_book(const std::string& book_for_delete);
    void delete_section();
    bool delete_section(const std::string& section_for_delete);
    void delete_book_association(const std::string& association_title, size_t book_number);
    void show_system_parameters();
    bool find_terminal();
    void find_commands();
    void current_parameters_search();
    void set_book_name_search();
    void current_book_name_parameters_search();
    void set_field_search();
    void current_field_parameters_search();
    void drop_field_search();
    bool drop_field_search(const std::string& field_name);
    void set_field_value_search();
    void current_field_value_parameters_search();
    void drop_field_value_search();
    bool drop_field_value_search(const std::string& field_value_name);
    void set_section_search();
    void current_section_parameters_search();
    void drop_all_search_parameters();
    void find_books();
    std::vector<size_t> find_books_by_name();
    std::vector<size_t> find_books_by_full_name();
    void find_books_by_field(std::vector<size_t>& book_nums, size_t field_num);
    void find_books_by_full_field(std::vector<size_t>& book_nums, size_t field_num);
    void find_books_by_field_value(std::vector<size_t>& book_nums, size_t field_value_num);
    void find_books_by_full_field_value(std::vector<size_t>& book_nums, size_t field_value_num);
    void find_books_by_section_name(std::vector<size_t>& book_nums);
    void find_books_by_full_section_name(std::vector<size_t>& book_nums);
    
    //  фции сохранения и выхода из системы
    void exit_mode();
    void save_as();
    void save_system();
    bool write_to_location_fail();
    void get_system_location();
    bool get_system_location(const std::string& location_to_save);
    bool serialize_system();
    std::string serialize_config_system_info();
    std::string serialize_storage();
    std::string serialize_sections();
    std::string serialize_sections(Library::Section* section);
    std::string serialize_user_book_associations();
    void compress_system();
    void compress_earlier_opened_systems_locations();
    void compress_storage();
    void compress_user_book_associations();
    void change_user_book_associations(std::vector<Library::Book>& storage);
    void delete_books_in_user_associations();
    void fill_user_book_associations(std::vector<Library::Book>& storage);
    void compress_user_book_local_associations();
    void change_user_book_sections(std::vector<Library::Book>& storage);
    void delete_books_in_user_sections();
    void delete_books_in_user_sections(Library::Section* current_section);
    void fill_user_book_sections(std::vector<Library::Book>& storage);
    void compress_dependencies();
    void update_earlier_opened_systems_file();
    void serialize_earlier_opened_systems();
    void deserialize_earlier_opened_systems();
};

#endif /* Library_hpp */
