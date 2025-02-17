#include "StringUtils.h"
#include <sstream> //included to work with stringstream

namespace StringUtils{

// Returns a substring of the string str, allows for negative values as in
// python last == 0 means to include last of string
std::string Slice(const std::string &str, ssize_t first, ssize_t last) noexcept{
    if (first < 0) {
        first = str.length() + first; //account for negative numbers
    }
    if (last < 0) {
        last = str.length() + last; //account for negative numbers
    }
    if (last == 0) { //last of string
        last = str.length();
    }
    return str.substr(first, last - first);
}

//returns capitalized string (first letter only capitalized)
std::string Capitalize(const std::string &str) noexcept{
    std::string first = str;
    first[0] = toupper(first[0]); //capitalize first letter

    //uncapitalize rest of string using loop (like python)
    for(std::size_t i = 1 ; i < first.length() ; ++i) {
        first[i] = tolower(first[i]);
    }
    return first;
}

std::string Upper(const std::string &str) noexcept{
    std::string all_str = str;
    for (size_t i = 0; i < all_str.size(); ++i) {
        all_str[i] = toupper(all_str[i]);
    }
    return all_str;
}

std::string Lower(const std::string &str) noexcept{
     std::string all_str = str;
    for (size_t i = 0; i < all_str.size(); ++i) {
        all_str[i] = tolower(all_str[i]);
    }
    return all_str;
}

//remove whitespace characters from left
std::string LStrip(const std::string &str) noexcept{
    if(str.length() == 0 || !isspace(str[0])) {
        return str;
    }
    return LStrip(str.substr(1)); //remove recursively
}

std::string RStrip(const std::string &str) noexcept{
    if(str.length() == 0 || !isspace(str[str.length() - 1])) {
        return str;
    }
    return RStrip(str.substr(0, str.length() - 1)); //remove recursively
}

std::string Strip(const std::string &str) noexcept{
    return LStrip(RStrip(str)); //remove last then beginning whitespaces
}

//center align string
std::string Center(const std::string &str, int width, char fill) noexcept{
    int padding = width - str.length();
    if(padding <= 0) { //already wide enough or 0
        return str;
    }
    int L_padding = padding / 2; //calculate middle point
    int R_padding = padding - L_padding; //calculate other half
    return std::string(L_padding, fill) + str + std::string(R_padding, fill); //add padding on L & R sides of string
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    int padding = width - str.length();
    if(padding <= 0) { //already wide enough or 0
        return str;
    }
    return str + std::string(padding, fill); //add padding on L side of string
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    int padding = width - str.length();
    if(padding <= 0) { //already wide enough or 0
        return str;
    }
    return std::string(padding, fill) + str; //add padding on R side of string
}

// Returns the string str with all instances of old replaced with rep
std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    if (str.find(old) == std::string::npos) { //if old not found/not a valid position
        return str;
    }
    std::string new_str = str; //create new string copy for replacment
    size_t first_repeat = new_str.find(old); //find first instance of old
    new_str.replace(first_repeat, old.length(), rep); //replace first instance of old with rep    
    return Replace(new_str, old, rep); //replace recursively
}

// Splits the string up into a vector of strings based on splt parameter, if
// splt parameter is empty string, then split on white space
std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    std::vector<std::string> split_str;    
    if (splt.empty()) {
        //read from string like a file
        std::istringstream ss(str); //intiliaze string stream (ss) with str
        std::string substring; //to hold each substring
        while (ss >> substring) { //use >> to extract each substring, skipping whitespace
            split_str.push_back(substring); //push what was extracted onto new var, substring
        }
    } else { //if splt parameter not empty
        size_t first = 0;
        size_t last = str.find(splt); //find first instance of splt
        while (last != std::string::npos) { //while splt exists
            std::string last_substring = str.substr(first, last - first); //extract substring that's left
            split_str.push_back(last_substring); //add extracted substring to string
            first = last + splt.length(); //update start
            last = str.find(splt, first); //find next instance of splt
        }
        split_str.push_back(str.substr(first)); //add the last substring bit after all splt's found
    }
    return split_str;
}

// Joins a vector of strings into a single string
std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    std::stringstream ss; //using string stream to join strings
    ss << vect[0]; //add first string to stream
    for (size_t i = 1; i < vect.size(); ++i) { //iterate through strings
        ss << str << vect[i]; //add vect and string to string stream
    }
    return ss.str(); //return stream as string
}

// Replaces tabs with spaces aligning at the tabstops
std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    std::string new_str = str; //create new string copy for replacment
    size_t first_tab = new_str.find('\t'); //find first index/position of tab
    size_t column = 0;
    while (first_tab != std::string::npos) { //while tab exists
        column = first_tab;
        int spaces = tabsize - (column % tabsize); //calculate spaces needed to replace tab
        new_str.replace(first_tab, 1, std::string(spaces, ' ')); //replace tab with calculated spaces
        first_tab = new_str.find('\t', first_tab + spaces); //find next tab
    }
    return new_str;
}

// Calculates the Levenshtein distance (edit distance) between the two
int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    std::string left_str = left;
    std::string right_str = right;
    if (ignorecase) { //if ignorecase is true
        left_str = Lower(left_str); //convert left string to lowercase
        right_str = Lower(right_str); //convert right string to lowercase
    }
    int left_len = left_str.length(); //get length of left string
    int right_len = right_str.length(); //get length of right string

    std::vector<std::vector<int>> matrix(left_len + 1, std::vector<int>(right_len + 1)); // matrix to store values

    for (int i = 0; i <= left_len; ++i) { //initialize matrix
        matrix[i][0] = i; //delete from left
    }
    for (int j = 0; j <= right_len; ++j) { //initialize matrix
        matrix[0][j] = j; //insert to match right
    }
    for (int i = 1; i <= left_len; ++i) { //iterate through left string
        for (int j = 1; j <= right_len; ++j) { //iterate through right string
            int cost; //initialize cost
            if (left_str[i - 1] == right_str[j - 1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            matrix[i][j] = std::min(std::min(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1),
                                    matrix[i - 1][j - 1] + cost);
        }
    }
    return matrix[left_len][right_len]; //return final cost
}

};