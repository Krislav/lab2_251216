#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "..\IDictionary\IDictionary.hpp"

class Book {
private:
    IDictionary<std::string, std::vector<int>> words;
    std::vector<std::vector<std::string>> pages;

    std::string to_lower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), 
            [](unsigned char c){ return std::tolower(c); });
        return s;
    }

public:
    Book() = default;
    ~Book() = default;

    std::vector<std::string> GetPage(int pageNumber) {
        if (pageNumber < 1 || pageNumber > (int)pages.size()) {
            throw std::out_of_range("Invalid page number");
        }
        return pages[pageNumber - 1];
    }

    void ProcessText(const std::vector<std::string>& all_words, int page_size, bool is_size_in_words) {
        words = IDictionary<std::string, std::vector<int>>();
        pages.clear();

        if (all_words.empty() || page_size <= 0) return;

        int i = 0;
        int words_count = all_words.size();
        int current_page_idx = 1;

        while (i < words_count) {
            std::vector<std::string> current_page_content;
            int limit = 0;
            
            if (current_page_idx == 1) {
                limit = page_size / 2;
            } else if (current_page_idx % 10 == 0) {
                limit = (page_size * 2) / 3;
            } else {
                limit = page_size;
            }

            int current_usage = 0;
            while (i < words_count) {
                std::string raw_word = all_words[i];
                int cost = is_size_in_words ? 1 : (int)raw_word.length() + 1;

                if (current_usage + cost > limit && !current_page_content.empty()) {
                    break; 
                }

                current_page_content.push_back(raw_word);
                
                std::string key = to_lower(raw_word);
                std::vector<int> page_list;
                
                if (words.ContainsKey(key)) {
                    page_list = words.Get(key);
                    if (page_list.empty() || page_list.back() != current_page_idx) {
                        page_list.push_back(current_page_idx);
                    }
                } else {
                    page_list.push_back(current_page_idx);
                }
                words.Add(key, page_list);

                current_usage += cost;
                i++;
            }
            pages.push_back(current_page_content);
            current_page_idx++;
        }
    }

    std::vector<std::pair<std::string, std::vector<int>>> GetFullDictionary() {
        return words.GetFullDictionary();
    }

    std::vector<std::string> GetFullPages() {
        std::vector<std::string> all_words;
        for (const auto& page : pages) {
            all_words.insert(all_words.end(), page.begin(), page.end());
        }
        return all_words;
    }

    std::vector<int> Search(std::string word) {
        std::string key = to_lower(word);
        if (words.ContainsKey(key)) {
            return words.Get(key);
        }
        return {};
    }

    int GetTotalPages() const {
        return (int)pages.size();
    }
};