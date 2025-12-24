#include "Interface.hpp"

std::string text;
int page_size = 0;
bool is_size_in_words = true;
IDictionary<std::string, int>* dictionary = new IDictionary<std::string, int>();

void Menu() {
    while (true) {
        std::cout << "\n=== Main Menu ==="
                  << "\n1. Enter page size"
                  << "\n2. Enter text"  
                  << "\n3. Exit"
                  << "\nSelect: ";
        
        int choice;
        std::cin.clear();
        std::cin >> choice;

        switch (choice) {
            case 1: EnterPageSize(); break;
            case 2: EnterText(); break;
            case 3: return;
            default: std::cout << "Invalid choice!\n";
        }
    }
}

void EnterPageSize() {
    std::cout << "Is page size in words(enter 1 if yes, 0 if no): ";
    std::cin >> is_size_in_words;
    if (is_size_in_words) {
        std::cout << "Enter page size in words: ";
        std::cin >> page_size;
    }else{
        std::cout << "Enter page size in symbols: ";
        std::cin >> page_size;
    }
}

void EnterText() {
    std::string input;
    int current_page = 1;
    
    std::cout << "Enter Text: ";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    if (!input.empty()) {
        input += ' ';
        std::vector<std::string> words = SplitStringToWords(input);
        if (page_size != 0) { // Размер в словах
            if (is_size_in_words) {
                int i = 0;
                int words_count = words.size();
                int words_on_current_page = 0;
                int two_thirds_page_size = (page_size * 2) / 3;

                while (words_on_current_page < page_size / 2 && i < words_count) {
                    std::string word_key = words[i];
                    while (dictionary->ContainsKey(word_key)) {
                        word_key += std::to_string(current_page);
                    }
                    dictionary->Add(word_key, current_page);
                    i++;
                    words_on_current_page++;
                }
                current_page++;
                while (i < words_count) {
                    words_on_current_page = 0;
                    if (current_page % 10 != 0) {
                        while (words_on_current_page < page_size && i < words_count) {
                            std::string word_key = words[i];
                            while (dictionary->ContainsKey(word_key)) {
                                word_key += std::to_string(current_page);
                            }
                            dictionary->Add(word_key, current_page);
                            i++;
                            words_on_current_page++;
                        }
                    }
                    else {
                        while (words_on_current_page < two_thirds_page_size && i < words_count) {
                            std::string word_key = words[i];
                            while (dictionary->ContainsKey(word_key)) {
                                word_key += std::to_string(current_page);
                            }
                            dictionary->Add(word_key, current_page);
                            i++;
                            words_on_current_page++;
                        }
                    }
                    current_page++;
                }
            }
            else { // Размер в символах
                int i = 0;
                int words_count = words.size();
                int symbols_on_current_page = -1;
                int two_thirds_page_size = (page_size * 2) / 3;

                while (symbols_on_current_page < page_size / 2 && i < words_count) {
                    std::string word_key = words[i];
                    while (dictionary->ContainsKey(word_key)) {
                        word_key += std::to_string(current_page);
                        symbols_on_current_page -= std::to_string(current_page).length();
                    }
                    dictionary->Add(word_key, current_page);
                    symbols_on_current_page += words[i].size() + 1;
                    i++;
                }
                current_page++;
                while (i < words_count) {
                    symbols_on_current_page = -1;
                    if (current_page % 10 != 0) {
                        while (symbols_on_current_page < page_size && i < words_count) {
                            std::string word_key = words[i];
                            while (dictionary->ContainsKey(word_key)) {
                                word_key += std::to_string(current_page);
                                symbols_on_current_page -= std::to_string(current_page).length();
                            }
                            dictionary->Add(word_key, current_page);
                            symbols_on_current_page += words[i].size() + 1;
                            i++;
                        }
                    }
                    else {
                        while (symbols_on_current_page < two_thirds_page_size && i < words_count) {
                            std::string word_key = words[i];
                            while (dictionary->ContainsKey(word_key)) {
                                word_key += std::to_string(current_page);
                                symbols_on_current_page -= std::to_string(current_page).length();
                            }
                            dictionary->Add(word_key, current_page);
                            symbols_on_current_page += words[i].size() + 1;
                            i++;
                        }
                    }
                    current_page++;
                }
            }
        }
        else {
            int words_count = words.size();
            for (int i = 0; i < words_count; i++) {
                std::string word_key = words[i];
                while (dictionary->ContainsKey(word_key)) {
                    word_key += std::to_string(current_page);
                }
                dictionary->Add(word_key, current_page);
            }
        }
        int s = dictionary->GetCount();
        std::vector<std::pair<std::string, int>> ready_for_output = dictionary->GetFullDictionary();
        for (int i = 0; i < s; i++) {
            std::cout << "Word: " << ready_for_output[i].first << " Page: " << ready_for_output[i].second << "\n";
        }
    }
    else {
        std::cout << "Text is empty";
    }

    return;
}

std::vector<std::string> SplitStringToWords(const std::string& str) {
    std::vector<std::string> words;
    
    size_t i = 0;
    while (i < str.length()) {
        while (i < str.length() && std::isspace(str[i])) {
            i++;
        }
        
        if (i >= str.length()) {
            break;
        }
        
        size_t start = i;
        while (i < str.length() && !std::isspace(str[i])) {
            i++;
        }
        
        words.push_back(str.substr(start, i - start));
    }
    
    return words;
}