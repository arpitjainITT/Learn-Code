#ifndef PAGINATOR_H
#define PAGINATOR_H

#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
#include <stdexcept>

template <typename T>
class Paginator {
private:
    const std::vector<T>& items;
    size_t currentPage;
    const size_t pageSize;

public:
    Paginator(const std::vector<T>& items, size_t pageSize = 5)
        : items(items), currentPage(0), pageSize(pageSize) {}

    void display(const std::function<void(const T&)>& renderItem) {
        size_t totalPages = (items.size() + pageSize - 1) / pageSize;
        while (true) {
            system("clear");
            std::cout << "Page " << currentPage + 1 << " of " << totalPages << "\n";
            size_t start = currentPage * pageSize;
            size_t end = std::min(start + pageSize, items.size());

            for (size_t i = start; i < end; ++i)
                renderItem(items[i]);

            std::cout << "\n[N]ext | [P]revious | [B]ack: ";
            char cmd;
            std::cin >> cmd;
            if (cmd == 'N' || cmd == 'n') {
                if (currentPage < totalPages - 1) currentPage++;
            } else if (cmd == 'P' || cmd == 'p') {
                if (currentPage > 0) currentPage--;
            }
            else if (cmd == 'P' || cmd == 'p'){
                break;
            } else {
                std::cout << "Invalid command. Use N, P or B.\n";
                continue;
            }
        }
    }
};

#endif
