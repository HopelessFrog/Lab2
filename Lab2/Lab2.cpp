#include <Windows.h>
#include "PurchaseLogAnalyzer.h"
#include <iomanip>
#include <iostream>
#define EXIT 0x1B
#define CONSOLE_INPUT 0x31
#define SAVE_TO_FILE 0x31
#define FILE_INPUT 0x32

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BOLD "\033[1m"


void PrintMenu() {
    std::cout << BOLD << YELLOW << "\n===========================\n";
    std::cout << "  GAS PURCHASE ANALYZER" << std::endl;
    std::cout << "===========================\n";
    std::cout << " [1] Analyze manually inputted logs" << std::endl;
    std::cout << " [2] Analyze logs from a file" << std::endl;
    std::cout << " [ESC] Exit the program" << std::endl;
    std::cout << "===========================\n";
    std::cout << " Select an option: ";
}

void PrintAbbreviations() {
    std::cout << BOLD << YELLOW << "\n===============================================\n";
    std::cout << BOLD << YELLOW << " Column Headers and Their Full Names" << std::endl;
    std::cout << "===============================================\n";
    InitAbbreviations(std::cout);
    std::cout << BOLD << YELLOW << "===============================================\n\n";
}

void PrintResults(const TextTable& analyzed, const TextTable& analyzedByBrand) {
    std::cout << BOLD << GREEN << "\n=================== ANALYSIS RESULT ===================\n";
    std::cout << BOLD << "\nAnalyzed by each log entry:\n";
    std::cout << analyzed << std::endl;

    std::cout << BOLD <<  "\nAnalyzed by brand:\n";
    std::cout << analyzedByBrand << std::endl;
    std::cout << "=======================================================\n\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    while (true) {
        PrintMenu();
        int option = _getch();
        std::vector<PurchaseLog*> logs;

        if (option == CONSOLE_INPUT) {
            std::cout << "\n>> Manual Input Selected\n";
            logs = ReadLogFromConsole();
        }
        else if (option == FILE_INPUT) {
            std::cout << "\n>> File Input Selected\n";
            std::cout << " Please input the path to the file: ";
            std::string path = GetString();
            logs = ReadLogFromFile(path);
        }
        else if (option == EXIT) {
            std::cout << RED <<  "\nExiting the program... Goodbye!\n";
            break;
        }
        else {
            std::cout << BOLD << RED << "\nInvalid option. Please try again.\n";
            continue;
        }

        if (logs.empty()) {
            std::cout << BOLD << RED << "\nNo logs found. Returning to the main menu.\n";
            continue;
        }

        PrintAbbreviations();

        auto analyzed = GetAnalyzedLogs(logs);
        auto analyzedByBrand = GetAnalyzedLogsByBrands(logs);

        PrintResults(analyzed, analyzedByBrand);

        std::cout << YELLOW << "Do you want to save the results to a file? [1 - Yes / Any other key - No]: ";
        option = _getch();
        if (option == SAVE_TO_FILE) {
            SaveToFile(analyzed, analyzedByBrand);
            std::cout << BOLD << GREEN << "\nResults successfully saved!\n";
        }

        for (auto& log : logs) {
            delete log;
        }
        logs.clear();
    }
}
