#define UNICODE
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include "Book\Book.hpp"
#include "IDictionary\IDictionary.hpp"
#include "Tests\Tests.hpp"

std::string text;
int page_size = 0;
bool is_size_in_words = true;
std::vector<std::wstring> g_words_storage;
Book myBook;
int current_view_page = 1;
bool sort_by_page = false;

#pragma comment(lib, "comctl32.lib")

#define ID_TEXTBOX      101
#define ID_PAGESIZE     102
#define ID_WORDS_RADIO  103
#define ID_CHARS_RADIO  104
#define ID_PROCESS_BTN  105
#define ID_LOAD_BTN     106
#define ID_LISTVIEW     107
#define ID_TESTS_BTN    108

#define ID_LEFT_PAGE    201
#define ID_RIGHT_PAGE   202

HWND hTextBox, hPageSize, hListView;
HWND hWordsRadio, hCharsRadio;
HWND hPageCounter, hPrevBtn, hNextBtn;

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

void AddRowToListView(int index, const std::wstring& word, const std::wstring& pages) {
    LVITEMW item{};
    item.mask = LVIF_TEXT;
    item.iItem = index;
    item.pszText = const_cast<LPWSTR>(word.c_str());
    ListView_InsertItem(hListView, &item);

    LVITEMW subItem{};
    subItem.iSubItem = 1;
    subItem.pszText = const_cast<LPWSTR>(pages.c_str());
    SendMessageW(hListView, LVM_SETITEMTEXTW, index, (LPARAM)&subItem);
}

void FillListView(HWND hListView) {
    SendMessage(hListView, WM_SETREDRAW, FALSE, 0); // Отключаем мерцание
    ListView_DeleteAllItems(hListView);
    g_words_storage.clear();

    if (!sort_by_page) {
        auto data = myBook.GetFullDictionary();
        for (int i = 0; i < (int)data.size(); i++) {
            std::wstring word(data[i].first.begin(), data[i].first.end());
            
            std::string p_list = "";
            for (size_t j = 0; j < data[i].second.size(); ++j) {
                p_list += std::to_string(data[i].second[j]) + (j == data[i].second.size() - 1 ? "" : ", ");
            }
            std::wstring pages_w(p_list.begin(), p_list.end());
            
            g_words_storage.push_back(word);
            g_words_storage.push_back(pages_w);
            
            AddRowToListView(i, g_words_storage[g_words_storage.size() - 2], g_words_storage.back());
        }
    } else {
        int total_pages = myBook.GetTotalPages();
        int item_index = 0;

        for (int p = 1; p <= total_pages; ++p) {
            std::vector<std::string> page_words = myBook.GetPage(p);
            
            std::sort(page_words.begin(), page_words.end());

            for (const auto& w : page_words) {
                std::wstring word(w.begin(), w.end());
                std::wstring page_num = std::to_wstring(p);

                g_words_storage.push_back(word);
                g_words_storage.push_back(page_num);

                AddRowToListView(item_index++, g_words_storage[g_words_storage.size() - 2], g_words_storage.back());
            }
        }
    }

    SendMessage(hListView, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hListView, NULL, TRUE);
}

void UpdatePageDisplay() {
    if (myBook.GetTotalPages() == 0) return;

    std::vector<std::string> page_words = myBook.GetPage(current_view_page);
    std::string page_text = "";
    for (const auto& w : page_words) page_text += w + " ";

    std::wstring w_page_text(page_text.begin(), page_text.end());
    SetWindowTextW(hTextBox, w_page_text.c_str());

    std::wstring counter = L"Page: " + std::to_wstring(current_view_page) + 
                           L" / " + std::to_wstring(myBook.GetTotalPages());
    SetWindowTextW(hPageCounter, counter.c_str());
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

void ProcessText(HWND hwnd) {
    if (text.empty()) {
        wchar_t buffer[8192];
        GetWindowTextW(hTextBox, buffer, 8192);
        std::wstring wtext(buffer);
        text = std::string(wtext.begin(), wtext.end());
    }

    int page_size_val = GetDlgItemInt(hwnd, ID_PAGESIZE, nullptr, FALSE);
    if (page_size_val <= 0) page_size_val = 10;

    bool is_words = SendMessage(hWordsRadio, BM_GETCHECK, 0, 0) == BST_CHECKED;

    std::vector<std::string> words_vec = SplitStringToWords(text);
    
    myBook.ProcessText(words_vec, page_size_val, is_words);

    current_view_page = 1;
    UpdatePageDisplay();
    FillListView(hListView);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateListView(hwnd);

        hTextBox = CreateWindowW(L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
            570, 10, 300, 150,
            hwnd, (HMENU)ID_TEXTBOX, nullptr, nullptr);

        hPrevBtn = CreateWindowW(L"BUTTON", L"<", 
            WS_VISIBLE | WS_CHILD, 
            570, 170, 40, 25, 
            hwnd, (HMENU)ID_LEFT_PAGE, nullptr, nullptr);

        hPageCounter = CreateWindowW(L"STATIC", L"Page: 0/0", 
            WS_VISIBLE | WS_CHILD | SS_CENTER, 
            615, 175, 100, 20, 
            hwnd, nullptr, nullptr, nullptr);

        hNextBtn = CreateWindowW(L"BUTTON", L">", 
            WS_VISIBLE | WS_CHILD, 
            720, 170, 40, 25, 
            hwnd, (HMENU)ID_RIGHT_PAGE, nullptr, nullptr);
        
        CreateWindowW(L"STATIC", L"Page size (words or symbols):",
            WS_VISIBLE | WS_CHILD,
            570, 210, 250, 20,
            hwnd, nullptr, nullptr, nullptr);

        hPageSize = CreateWindowW(L"EDIT", L"10",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            570, 235, 100, 25,
            hwnd, (HMENU)ID_PAGESIZE, nullptr, nullptr);

        hWordsRadio = CreateWindowW(L"BUTTON", L"Words",
            WS_VISIBLE | WS_CHILD | WS_GROUP | BS_AUTORADIOBUTTON,
            570, 270, 100, 25,
            hwnd, (HMENU)ID_WORDS_RADIO, nullptr, nullptr);

        hCharsRadio = CreateWindowW(L"BUTTON", L"Symbols",
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
            680, 270, 100, 25,
            hwnd, (HMENU)ID_CHARS_RADIO, nullptr, nullptr);

        SendMessage(hWordsRadio, BM_SETCHECK, BST_CHECKED, 0);

        CreateWindowW(L"BUTTON", L"Process",
            WS_VISIBLE | WS_CHILD,
            570, 310, 120, 35,
            hwnd, (HMENU)ID_PROCESS_BTN, nullptr, nullptr);

        CreateWindowW(L"BUTTON", L"Load file",
            WS_VISIBLE | WS_CHILD,
            700, 310, 120, 35,
            hwnd, (HMENU)ID_LOAD_BTN, nullptr, nullptr);

        CreateWindowW(L"BUTTON", L"Run Tests",
            WS_VISIBLE | WS_CHILD,
            570, 355, 250, 35,
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
            
        case ID_LEFT_PAGE:
            if (current_view_page > 1) { current_view_page--; UpdatePageDisplay(); }
            break;

        case ID_RIGHT_PAGE:
            if (current_view_page < myBook.GetTotalPages()) { current_view_page++; UpdatePageDisplay(); }
            break;
        }
        break;
    case WM_NOTIFY: {
            LPNMHDR nmhdr = (LPNMHDR)lParam;
            if (nmhdr->idFrom == 1 && nmhdr->code == LVN_COLUMNCLICK) {
                LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lParam;
                sort_by_page = (pnmv->iSubItem == 1);
                FillListView(hListView);
            }
            break;
        }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}