#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <cassert>
#include <regex>
#include <thread>
#include <omp.h>


std::string trim(std::string str) {
    assert(!str.empty());
    int first = str.find_first_not_of(" \n\t");
    int last = str.find_last_not_of(" \n\t");
    return std::string(str.begin()+first, str.begin()+last+1);
}

std::string getContentInTag(std::string str, std::string tag, int startingFrom=0){
    assert(str.find("<"+tag+">", startingFrom) != -1 and str.find("</"+tag+">", startingFrom) != -1);
    return std::string(str.begin()+str.find("<"+tag+">", startingFrom)+4, str.begin()+str.find("</"+tag+">", startingFrom));
}

int getLocationAtTagBegin(std::string str, std::string tag, int startingFrom=0){
    assert(str.find("<"+tag+">", startingFrom) != -1);

    return str.find("<"+tag+">", startingFrom)+4;
}

int getLocationAtTagEnd(std::string str, std::string tag, int startingFrom=0){
    assert(str.find("</"+tag+">", startingFrom) != -1);

    return str.find("</"+tag+">", startingFrom)+4;
}

int getLocationAtRegex(std::string str, std::regex regx, int startingFrom=0){
    std::smatch m;
    str.erase(0, startingFrom);
    std::regex_search(str, m, regx);
    return m.position();
}

std::vector<std::string> getAnagramsFromHTML(std::string text){
    std::string anagramRawData = trim(getContentInTag(text, "blockquote"));

    //std::cout << getLocationAtRegex(anagramRawData, std::regex("<\\/script><b>[a-zA-Z \\n]*<\\/b>")) << " " << anagramRawData.size() << std::endl;
    int anagramStatusLoc = getLocationAtRegex(anagramRawData, std::regex("<\\/script><b>[\\s\\S]*<\\/b>"));
    if(anagramStatusLoc==anagramRawData.size()){
        std::cout << "no anagrams listed for name, " << text << std::endl;
        return {};
    }
    std::string anagramStatus = getContentInTag(anagramRawData, "b", anagramStatusLoc);
    std::optional<std::string> anagramList = std::nullopt;

    int anagramListStart = getLocationAtRegex(anagramRawData, std::regex("<br>[a-zA-Z\\s]+<br>"));
    int anagramListEnd = getLocationAtRegex(anagramRawData, std::regex("<br>[a-zA-Z\\s]*<script>"));
    anagramList = std::string(anagramRawData.begin()+anagramListStart+4,
                              anagramRawData.begin()+anagramListEnd);


    std::string filtered;

    anagramList = std::regex_replace(anagramList.value(), std::regex("<br>\\n"), "\n");

    std::vector<std::string> anagrams;
    std::string line; std::stringstream ss(anagramList.value());
    while(std::getline(ss, line)){
        anagrams.push_back(line);
    }

    return anagrams;
}


int main() {

    std::vector<cpr::AsyncResponse> responses;
    std::vector<std::string> names;

    for(int i = 0; i < 437; i++){
        responses.push_back(cpr::GetAsync(cpr::Url{"https://www.southampton.ac.uk/people"}, cpr::Parameters{{"page",std::to_string(i)}}));
    }

    #pragma omp parallel for
    for(int i = 0; i < 437; i++){
        cpr::Response r = responses[i].get();
        std::string res = r.text;
        res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
        res = std::regex_replace(res, std::regex("\\n"), "");

        //std::smatch m;
        //std::regex_search(res, m, std::regex(">[a-zA-Z.]+<\\/a><\\/h4>"));
        //std::cout << m.str(0) << std::endl;

        std::vector<std::string> matches;
        std::smatch match;
        std::regex regx(">[a-zA-Z.]+<\\/a><\\/h4>");
        int j = 1;
        while (std::regex_search(res, match, regx)) {
            matches.push_back(match.str(0));
            //std::cout << "\nMatched string is " << match.str(0) << std::endl << "and it is found at position " << match.position(0)<< std::endl;
            j++;

            // suffix to find the rest of the string.
            res = match.suffix().str();
        }

        for(auto &j : matches){
            std::string name = "";
            bool toggle = false;
            for(int k = 2; k < j.size(); k++){
                if(j[k]=='<') break;
                if(isupper(j[k])) toggle = true;
                if(toggle && isalpha(j[k])) name += j[k];
            }


            names.push_back(name);
            cpr::Response anagram = cpr::Get(cpr::Url{"https://new.wordsmith.org/anagram/anagram.cgi"}
            , cpr::Parameters{{"anagram", name},{"t","500"},{"a","n"}});

            std::vector<std::string> nameAnagrams = getAnagramsFromHTML(anagram.text);

            std::cout << "gotten anagrams for: " << name << std::endl;

        }



        //int start = getLocationAtRegex(res, std::regex(">[a-zA-Z.]+<\\/a><\\/h4>"));
        //std::cout << res.substr(start,100) << std::endl;

        //std::cout << r.text.substr(0,100) << std::endl;
    }



    cpr::Response r = cpr::Get(cpr::Url{"https://new.wordsmith.org/anagram/anagram.cgi?anagram=...&t=500&a=n"});
                               //cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                               //cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    //std::cout << r.text;                         // JSON text string


    std::vector<std::string> out = getAnagramsFromHTML(r.text);
}
