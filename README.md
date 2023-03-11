# SEARCH ENGINE realization
## Short description 
Console multi thread application represents searching service using JSON configuration 
files for future web implementation

The following principals are realized:
1. User creates file config.json with general information and set of file names for searching  
2. The program creates inverted index for future searching
3. User creates file requests.json with request information (subject for searching)
4. The program creates answers.json with the searching result

## Example
### config.json
{
    "config":\
&emsp;{\
&emsp;&emsp;"name": "SearchEngine",\
&emsp;&emsp;"version": "0.1",\
&emsp;&emsp;"max_responses": 5\
&emsp;&emsp;},\
    "files":\
&emsp;&emsp;[\
&emsp;&emsp;"../resources/file001.txt",\
&emsp;&emsp;"../resources/file002.txt",\
&emsp;&emsp;"../resources/file003.txt",\
&emsp;&emsp;"../resources/file004.txt",\
&emsp;]\
}

config — general information\
&emsp;name — name of Search Engine\
&emsp;version — version number\
&emsp;max_responses — maximum responses per 1 request.\
&emsp;files - path to files with request.\


### example requests.json.
requests.json:\
{\
&emsp;"requests": [\
&emsp;&emsp;"some words..",\
&emsp;&emsp;"some words..",\
&emsp;&emsp;"some words..",\
&emsp;&emsp;"some words..",\
&emsp;&emsp;…\
&emsp;]\
}\

## Installation instruction 
1. Make SearchEngine directory
2. Put in SearchEngine.zip and unpack it
3. Do make and link processes according to your operating system.
4. Create config.json and requests.json files.
5. Run SearchEngine and see the answer.json result. 

