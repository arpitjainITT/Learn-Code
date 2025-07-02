#ifndef NEWSFETCHERMANAGER_HPP
#define NEWSFETCHERMANAGER_HPP

#include "../adapters/INewsApiAdapter.hpp"
#include <vector>
#include <memory>

class NewsFetcherManager {
public:
    void registerAdapter(std::shared_ptr<INewsApiAdapter> adapter);
    void fetchAll();

private:
    std::vector<std::shared_ptr<INewsApiAdapter>> adapters;
};

#endif
