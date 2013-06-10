#include "HelperFunctions.hpp"
#include <fcntl.h>

namespace linda {

    string getSemName(int Pid,int SemNo)
    {
        string ChildPidString = std::to_string(Pid);
        const string NamedSemPrefix = "Linda_";
        const string SemSuffix = "_s";
        string SemName;

        SemName.append(NamedSemPrefix).append(ChildPidString).
            append(SemSuffix).append(std::to_string(SemNo));
        return SemName; 
    }

    // c-style-string-inside aware function
    // for example: alfa beta "gamma \"citation\""
    // is decomposed to 3 tokens (last one: gamma "citation")
    void Tokenize(const string& str, std::vector<string>& tokens) {
    	bool in_string = false;
    	string::size_type pos = 0;
    	string token;

    	while(pos < str.size()) {

    		if(in_string) {
    			//pos > 0
    			if(str[pos] == '"') {

    				if(str[pos-1] != '\\') {
    					//end of string
    					in_string = false;
    					tokens.push_back(token);
    					token = "";
    				}
    				else {
    					//escaped "
    					token.back() = str[pos]; //replace \ with "
    				}
    			} else
    				token += str[pos];

    		} else {

    			if(str[pos] == '"') {
    				in_string = true;
    			}
    			else if(str[pos] == ' ') {
    				if(token.size() > 0) {
    					tokens.push_back(token);
    					token="";
    				}
    			}
    			else {
    				token += str[pos];
    			}
    		}

    		++pos;
    	}

    	if(in_string)
    		throw std::logic_error("Parse query error, missing '\"' !");

    	//last token
    	if(token.size() > 0)
            tokens.push_back(token);
    }

    int is_valid_fd(int fd)
    {
        return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
    }


}		// -----  end of namespace linda  -----
