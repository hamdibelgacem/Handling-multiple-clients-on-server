#include "split.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::vector<std::string> &split(const std::string &s, const std::string& delims, std::vector<std::string> &elems) {
    char* tok;
    char cchars [s.size()+1];
    char* cstr = &cchars[0];
    strcpy(cstr, s.c_str());
    tok = strtok(cstr, delims.c_str());
    while (tok != NULL) {
        elems.push_back(tok);
        tok = strtok(NULL, delims.c_str());
    }   
    return elems;
}

std::vector<std::string> split(const std::string &s, const std::string& delims) {
	stringstream ss;
	ss << s;
	string input_string = ss.str();
	input_string.erase( std::remove(input_string.begin(), input_string.end(), '\n'), input_string.end() );
	input_string.erase( std::remove(input_string.begin(), input_string.end(), '\r'), input_string.end() );
    std::vector<std::string> elems;
    return split(input_string, delims, elems);
}
