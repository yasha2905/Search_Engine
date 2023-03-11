#pragma once
#include "nlohmann/json.hpp"
#include "ConverterJSON.h"
#include <thread>

struct Entry {
    size_t doc_id, count;
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex();
    void UpdateDocumentBase(ConverterJSON);
    std::vector<Entry> GetWordCount(const std::string& word);

protected:
    void threadExec(int);
    std::map<std::string, int> wordsSplit(std::string);
    void Update_docs();


private:
    std::vector<std::string> docs;
    std::vector<int> countDocs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};