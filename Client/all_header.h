#include <bits/stdc++.h>
#include <netinet/in.h> 
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <openssl/sha.h>
#include <sstream>
#include <pthread.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <fcntl.h>

using namespace std;

// ALL BUFFER SIZE
#define ARG_BUFFER_SIZE 32768
#define RES_BUFFER_SIZE 70000 
#define CHUNK_SIZE 32768
#define DOWNLOAD_BUFFER_SIZE 32768
#define HASH_BUFFER_SIZE 8192


// at request at one time
#define MAX_REQUEST 1050




// all string delimeter

#define space_delimeter " "
#define colon_delimeter ":"
#define comma_delimeter ","
 

// data types
#define ll long long int 
#define ld long double
#define ull unsigned long long int
#define uc unsigned char


// ip port
extern string tracker_ip, client_ip;
extern uint16_t tracker_port, client_port;


// data structure

class Client_Files {
public:
    
    string file_path;
    string file_name;
    string SHA_hash;
    int file_size_bytes;
    vector <bool> which_chunk_present;

    void print_client_file_structure();

    Client_Files(string file_name ,string file_path , string SHA_hash, int file_size_bytes);

};

extern unordered_map <string, Client_Files > all_files_meta_data;
void print_all_file_meta_data();



// leacher function
void create_n_connect_leacher();
void handle_leacher_request(int client_socket  , string command , string& res);
void download_chunk_from_leecher (int leecher_socket_id);
void chunk_requests_from_leecher(vector <string> request_vector, int leecherSocket, string& res);



// peer to peer
void start_download(string res_from_tracker, string file_name  , string destination_path , string &final_download_response);



// Algorithm function

string calc_SHA_hash_of_file(const char* file_path);
pair <int, string> custom_piece_wise_selection_algorithm(vector <unordered_set <string> > &chunkStatusOfOthers, vector <pair<int,int> > &countOfClientForEachChunk);



// utility function 
vector<string> parse_command(char *client_request);
void print_request_vector(vector<string> req_vector);
string give_string_cmd(vector<string> req_vec);
void fetching_ip_port_from_file(int argc, char *argv[]);
vector<string> fetching_tracker_info_from_file(char* file_path)  ;
vector<string> spliting_string_by_delimeter(string address, string delim );
vector<string> custom_tokenisor(string command, string delimiter);
int get_file_size_in_bytes(const char *file_path);
string extract_filename_from_filepath(const char* file_path_c_str);
int chunk_count( int file_size );
int last_chunk_size(int file_size);
int get_start_pointer_for_chunk(int chunk_number);
void printchunkStatusOfOthersVector(vector <unordered_set <string> > chunk_record_status);



// Logger function
void clear_Error_Log();
void error_Log(const string &error);
void clear_success_Log();
void success_Log(const string &success);
void clear_request_Log();
void request_Log(const string &request );

