#include "all_header.h"

void clear_Error_Log(){
    ofstream out;
    out.open("client_error_log.txt");
    out.clear();
    out.close();
}

void error_Log(const string &error ){
    ofstream log_file("client_error_log.txt", ios_base::out | ios_base::app );
    log_file << error << endl;
}

void clear_success_Log(){
    ofstream out;
    out.open("client_success_log.txt");
    out.clear();
    out.close();
}

void success_Log(const string &success ){
    ofstream log_file("client_success_log.txt", ios_base::out | ios_base::app );
    log_file << success << endl;
}


void clear_request_Log(){
    ofstream out;
    out.open("client_updation_log.txt");
    out.clear();
    out.close();
}

void request_Log(const string &request ){
    ofstream log_file("client_updation_log.txt", ios_base::out | ios_base::app );
    log_file << request << endl;
}


