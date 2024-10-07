#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <cassert>
#include <regex>


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




int main() {
    cpr::Response r = cpr::Get(cpr::Url{"https://new.wordsmith.org/anagram/anagram.cgi?anagram=tarapore&t=500&a=n"});
                               //cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                               //cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    //std::cout << r.text;                         // JSON text string


    std::string anagramRawData = trim(getContentInTag(r.text, "blockquote"));

    //std::cout << getLocationAtRegex(anagramRawData, std::regex("<\\/script><b>[a-zA-Z \\n]*<\\/b>")) << " " << anagramRawData.size() << std::endl;
    int anagramStatusLoc = getLocationAtRegex(anagramRawData, std::regex("<\\/script><b>[\\s\\S]*<\\/b>"));
    if(anagramStatusLoc==anagramRawData.size()){
        std::cout << "no anagrams listed for name X" << std::endl;
        return 0;
    }
    std::string anagramStatus = getContentInTag(anagramRawData, "b", anagramStatusLoc);
    std::optional<std::string> anagramList = std::nullopt;

    int anagramListStart = getLocationAtRegex(anagramRawData, std::regex("<br>[a-zA-Z\\s]+<br>"));
    int anagramListEnd = getLocationAtRegex(anagramRawData, std::regex("<br>[a-zA-Z\\s]*<script>"));
    anagramList = std::string(anagramRawData.begin()+anagramListStart+4,
                              anagramRawData.begin()+anagramListEnd);


    std::cout << anagramList.value() << std::endl;




    return 0;
}
