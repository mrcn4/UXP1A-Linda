#ifndef HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HPP

#include	<string>
#include	<vector>
#include 	<stdexcept>
#include 	<fcntl.h>

using std::string;
using std::vector;

namespace linda {

    /**
     * \brief Returns name for a named semafore.
     *
     * \param Pid PID of a process
     * \param SemNo number of semaphore
     *
     * \returns name of semaphore
     */
    string getSemName(int Pid,int SemNo);
    /**
     * \brief Check whether fd is a valid file descriptor
     *
     * \param fd file descriptor to check
     *
     * \returns 0 for invalid descriptor, !=0 otherwise
     */
    int is_valid_fd(int fd);

    void Tokenize(const string& str, std::vector<string>& tokens);
    void remove_quotations(string& str);
    //Tokenize helper:
    string unescaped_string(const string& str);
	string escape_string(const string& str);


}	// -----  end of namespace linda  -----

#endif
