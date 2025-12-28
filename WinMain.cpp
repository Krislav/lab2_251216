#define UNICODE
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <fstream>
#include "Interface.hpp"
#include "Tests\Tests.hpp"

// std::string text;
// int page_size = 0;
// bool is_size_in_words = true;
// IDictionary<std::string, int>* dictionary = new IDictionary<std::string, int>();
std::vector<std::wstring> g_words_storage;

#pragma comment(lib, "comctl32.lib")

#define ID_TEXTBOX      101
#define ID_PAGESIZE     102
#define ID_WORDS_RADIO  103
#define ID_CHARS_RADIO  104
#define ID_PROCESS_BTN  105
#define ID_LOAD_BTN     106
#define ID_LISTVIEW     107
#define ID_TESTS_BTN    108

HWND hTextBox, hPageSize, hListView;
HWND hWordsRadio, hCharsRadio;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    AllocConsole();
    
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    INITCOMMONCONTROLSEX icc{ sizeof(icc), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icc);

    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"WordIndexerApp";

    RegisterClassW(&wc);


    HWND hwnd = CreateWindowW(
        wc.lpszClassName,
        L"Word Page Indexer",
        WS_OVERLAPPEDWINDOW,
        100, 100, 900, 600,
        nullptr, nullptr, hInst, nullptr
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
void CreateListView(HWND hwnd) {
    INITCOMMONCONTROLSEX icc{};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icc);

    hListView = CreateWindowW(
        WC_LISTVIEWW,
        nullptr,
        WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
        10, 10, 550, 540,
        hwnd,
        (HMENU)1,
        GetModuleHandle(nullptr),
        nullptr
    );

    ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    LVCOLUMNW col{};
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

    col.iSubItem = 0;
    col.cx = 350;
    col.pszText = (LPWSTR)L"Word";
    ListView_InsertColumn(hListView, 0, &col);

    col.iSubItem = 1;
    col.cx = 150;
    col.pszText = (LPWSTR)L"Page";
    ListView_InsertColumn(hListView, 1, &col);
}
std::string LoadTextFromFile(HWND hwnd) {
    OPENFILENAMEW ofn{};
    wchar_t fileName[MAX_PATH]{};

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0";
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;

    if (!GetOpenFileNameW(&ofn)) return "";

    std::ifstream file(fileName);
    std::string text;
    std::getline(file, text);
    return text;
}
void FillListView(HWND hListView) {
    ListView_DeleteAllItems(hListView);
    g_words_storage.clear();

    auto data = dictionary->GetFullDictionary();

    for (int i = 0; i < (int)data.size(); i++) {
        LVITEMW item{};
        item.mask = LVIF_TEXT;
        item.iItem = i;

        std::wstring word(data[i].first.begin(), data[i].first.end());
        std::wstring page = std::to_wstring(data[i].second);

        g_words_storage.push_back(word);
        g_words_storage.push_back(page);

        item.pszText = (LPWSTR)g_words_storage[g_words_storage.size() - 2].c_str();
        ListView_InsertItem(hListView, &item);

        LVITEMW subItem{};
        subItem.iSubItem = 1;
        subItem.pszText = (LPWSTR)g_words_storage[g_words_storage.size() - 1].c_str();
        SendMessageW(hListView, LVM_SETITEMTEXTW, i, (LPARAM)&subItem);
    }
}

void ProcessText(HWND hwnd) {
    dictionary = new IDictionary<std::string, int>();

    wchar_t buffer[8192];
    GetWindowTextW(hTextBox, buffer, 8192);
    std::wstring wtext(buffer);
    text.assign(wtext.begin(), wtext.end());

    page_size = GetDlgItemInt(hwnd, ID_PAGESIZE, nullptr, FALSE);
    is_size_in_words = SendMessage(hWordsRadio, BM_GETCHECK, 0, 0) == BST_CHECKED;

    std::vector<std::string> words = SplitStringToWords(text + " ");
    int current_page = 1;
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
    FillListView(hListView);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateListView(hwnd);
        
        CreateWindowW(L"STATIC", L"Page size (words or symbols):",
            WS_VISIBLE | WS_CHILD,
            570, 155, 200, 20,
            hwnd, nullptr, nullptr, nullptr);

        hTextBox = CreateWindowW(L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE,
            570, 10, 300, 150,
            hwnd, (HMENU)ID_TEXTBOX, nullptr, nullptr);

        hPageSize = CreateWindowW(L"EDIT", L"10",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            570, 180, 100, 25,
            hwnd, (HMENU)ID_PAGESIZE, nullptr, nullptr);

        hWordsRadio = CreateWindowW(L"BUTTON", L"Words",
            WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
            570, 220, 100, 25,
            hwnd, (HMENU)ID_WORDS_RADIO, nullptr, nullptr);

        hCharsRadio = CreateWindowW(L"BUTTON", L"Symbols",
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
            680, 220, 100, 25,
            hwnd, (HMENU)ID_CHARS_RADIO, nullptr, nullptr);

        SendMessage(hWordsRadio, BM_SETCHECK, BST_CHECKED, 0);

        CreateWindowW(L"BUTTON", L"Process",
            WS_VISIBLE | WS_CHILD,
            570, 260, 120, 30,
            hwnd, (HMENU)ID_PROCESS_BTN, nullptr, nullptr);

        CreateWindowW(L"BUTTON", L"Load file",
            WS_VISIBLE | WS_CHILD,
            700, 260, 120, 30,
            hwnd, (HMENU)ID_LOAD_BTN, nullptr, nullptr);

        CreateWindowW(L"BUTTON", L"Run Tests",
            WS_VISIBLE | WS_CHILD,
            570, 300, 250, 30,
            hwnd, (HMENU)ID_TESTS_BTN, nullptr, nullptr);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_PROCESS_BTN:
            ProcessText(hwnd);
            break;

        case ID_LOAD_BTN: {
            std::string fileText = LoadTextFromFile(hwnd);
            std::wstring w(fileText.begin(), fileText.end());
            SetWindowTextW(hTextBox, w.c_str());
            break;
        }

        case ID_TESTS_BTN:
            TestAll(); 
            MessageBoxW(hwnd, L"Tests completed! Check console output.", L"Tests", MB_OK | MB_ICONINFORMATION);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}