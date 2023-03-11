#pragma once
#include <exception>
#include "vector"
#include "nlohmann/json.hpp"

struct Conf{
    std::string name;
    std::string version;
    int max_responses;
    std::vector<std::string> files;
    std::vector<std::string> requests;
};


class ConverterJSON {
public:
    ConverterJSON(std::string, std::string);

// returns docs from config.json
    std::vector<std::string> GetTextDocuments();

// returns max responses per 1 request
    int GetResponsesLimit();

// returns requests from requests.json
    std::vector<std::string> GetRequests();

protected:
    void readConfigFile(std::string);

private:
    Conf conf; // configuration structure
    std::vector<std::vector<std::pair<int, float>>> _answers; // result vector
};

