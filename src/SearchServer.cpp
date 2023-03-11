#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include "ConverterJSON.h"
#include "SearchServer.h"


template <typename T>
std::string NumberToString ( T Number, int n )
{
    std::ostringstream ss;
    ss.precision(n);
    ss << std::fixed;
    ss << Number;
    return ss.str();
}

SearchServer::SearchServer(InvertedIndex & idx) : _index(idx){}

std::map<std::string, int> SearchServer::wordsSplit(std::string str) {
    std::map<std::string, int> words;
    int start = 0;
    if(str.back() != ' ')
        str +=" ";
    int end = str.find(" ");
    while(end != -1){
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

std::vector<std::vector<RelativeIndex>> SearchServer::search(ConverterJSON converterJson) {
    std::vector  queries_input = converterJson.GetRequests();
    // requests like {"milk water banana milk water water", "sugar"};
    // will be like this vector < {(banana,1)(milk 2)(water 3)}, {sugar, 1}>
    for (int i = 0; i < queries_input.size(); ++i) {
        _requests.push_back(wordsSplit(queries_input[i]));
    }
    calcResult();
    return _result;
}


void SearchServer::calcResult() {
    // create vector to collect frequency word per doc <DocID, Frequency> per request
    std::vector<std::map<int, int>> vMapDocs;
    std::vector<int> absMax;

    for (int nRequest = 0; nRequest < _requests.size(); ++nRequest) {
        absMax.push_back(0); // by default
        // per each request sorting map-key - > map-value
        std::set<std::pair<std::string, int>, comp> set(_requests[nRequest].begin(), _requests[nRequest].end());
        // now its sorted in set per value

        // creates map <DocID, Frequency>
        std::map<int, int> mapDocCount;

        for (auto const &pair: set) {
            // pair - word from request and how many
            // _index.GetWordCount(pair.first) - vеctor of docs <map(docid,count)>

            for (int i = 0; i < _index.GetWordCount(pair.first).size(); ++i) {
                // key -> doc number, value ->  counting how many
                mapDocCount[_index.GetWordCount(pair.first)[i].doc_id] += _index.GetWordCount(pair.first)[i].count;
                // 1 request, Next word ALL docs
            }
            // 1 request, ALL word ALL docs
        }
        // and collect for this request
        vMapDocs.push_back(mapDocCount);
        //NEXT request ALL words ALL Docs,

        for (auto it = mapDocCount.begin(); it != mapDocCount.end(); ++it) {
            if (it->second > absMax[nRequest])
                absMax[nRequest] = it->second;
        }
    }
    //finally ALL requests ALL words ALL Docs collected in vMapDocs

// answer.json creation
    nlohmann::json j;
    for (int nRequest = 0; nRequest < vMapDocs.size(); ++nRequest) {

        std::string iString = std::to_string(nRequest);
        unsigned int number_of_zeros = 3 - iString.length();
        iString.insert(0, number_of_zeros, '0');

        if (absMax[nRequest] > 0){ // means result exists
            j["answers"]["request" + iString]["result"] = "true";

            // create vector RelativeIndex to keep result <doc_id, rel> for word from request
            std::vector<RelativeIndex> v_RelativeIndex;

            // running mapDocs for nRequest and calculation relevances (abs & rel)
            for (auto it1 = vMapDocs[nRequest].begin(); it1 != vMapDocs[nRequest].end(); ++it1) {

                RelativeIndex relativeIndex;
                float flRel = (it1->second / static_cast<float> (absMax[nRequest]));
                std::string strDocId    = NumberToString(it1->first, 0);
                std::string strRel      = NumberToString(flRel, 9);
                std::map<std::string, std::string> c_map{
                        {"docid", strDocId},
                        {"rank",  strRel}
                };

                relativeIndex.doc_id = it1->first;
                relativeIndex.rank = flRel;

                j["answers"]["request" + iString]["relevance"] += c_map;
                v_RelativeIndex.push_back(relativeIndex);
            }
            // result per request
            _result.push_back(v_RelativeIndex);
        } else{
            j["answers"]["request" + iString]["result"] = "false";
            // do nothing here if false
            _result.push_back({}); // means request with no result
        }
    }
    // for nice printing
    std::ofstream os;
    os.open("answers.json");
    os << j.dump(3) << std::endl;
    std::cout << j.dump(3);
    os.close();
}



