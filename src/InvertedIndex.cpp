#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include "InvertedIndex.h"

// mutex creation
std::mutex entry_access;
std::mutex vEntry_access;
std::mutex freq_dictionary_access;
std::mutex countDocs_access;

// space separation and counting repeats
std::map<std::string, int> InvertedIndex::wordsSplit(std::string str) {
    std::map<std::string, int> words;
    if(str.back() != ' ')
        str +=" ";
    int start = 0;
    int end = str.find(" ");
    while (end != -1) {
        std::string word = str.substr(start, end - start);
        if (words.find(word) == words.end()) {
            words[word] = 1;
        } else {
            words[word] = ++words[word];
        }
        start = end + 1;
        end = str.find(" ", start);
    }
    return words;
}
InvertedIndex::InvertedIndex() {}

void InvertedIndex::threadExec(int iDoc) {
    Entry entry;
    //std::cout << "Thread number " << n << " is running ..." << std::endl;
    std::map<std::string, int> wordsCount;
    // fill up Entry structure  - creating map <word ->  {doc_id, count}>;
    std::map<std::string, Entry> entryCount;
    wordsCount = wordsSplit(docs[iDoc]);
    entry_access.lock();
    entry.doc_id = iDoc;
    entry_access.unlock();
    int tmpCount=0;
    for (auto it = wordsCount.begin(); it != wordsCount.end(); ++it){
        entry_access.lock();
        entry.count = it->second;
        entry_access.unlock();
        entryCount[it->first] = entry;
        tmpCount += it->second;
    }
    countDocs_access.lock();
    countDocs.push_back(tmpCount);
    countDocs_access.unlock();
    // so here per iDoc created map<std::string, Entry> entryCount

    // now we have to add it to frequency dictionary
    // map<string, vector<Entry>> freq_dictionary for iDoc entryCount
    for (auto it1 = entryCount.begin(); it1 != entryCount.end(); ++it1) {
        std::vector<Entry> vEntry;
        vEntry_access.lock();
        vEntry.push_back(it1->second);
        vEntry_access.unlock();
        freq_dictionary_access.lock();
        freq_dictionary[it1->first].push_back(it1->second);
        freq_dictionary_access.unlock();
    }
}

void InvertedIndex::Update_docs() {
    Entry entry;
    std::vector<Entry> vEntry;
    std::vector<std::thread> threads;
    for(int iDoc = 0; iDoc < docs.size(); ++iDoc) {
        threads.push_back(std::thread (&InvertedIndex::threadExec, this, iDoc));
    }
    std::cout << "synchronizing all threads...\n";
    for (auto& th : threads) th.join();
}

void InvertedIndex::UpdateDocumentBase(ConverterJSON converterJson)
{
    std::vector<std::string> vDocs = converterJson.GetTextDocuments();
    for(int i = 0; i < vDocs.size(); ++i){
        std::ifstream  file;
        file.open(vDocs[i], std::ios::in);
        std::string str, strNew;
        while(getline(file, strNew)) {
            str += strNew + " ";
        }
        docs.push_back(str);
    }
    Update_docs(); // to calculate freq_dictionary
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    return freq_dictionary[word];
}









