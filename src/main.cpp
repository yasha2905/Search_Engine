#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    ConverterJSON converterJson("config.json","requests.json"); // reading json configuration
    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(converterJson); // calculate inverted index
    SearchServer searchServer(invertedIndex);
    searchServer.search(converterJson); // searching result and creating answers json
    return 0;
}


