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
#include <sstream>
#include <thread>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <fcntl.h>



using namespace std;

#define TRACKER_IP "127.0.0.1"
#define MAX_REQUEST 10

#define ll long long int
#define ld long double
#define ull unsinged long long int



// class

class User{

    public:
        string password;
        bool currentLogin;
        string ip;
        string port;
        string group_id;
        unordered_set<string>file_name;

        // login time
        User(string hash);

        void print_user_map_value();

};

class Files{

    public:
        string file_name;
        string SHA_hash;
        int size_of_file;;
        unordered_set<string>ip_port;    // string = ip:port
    
    Files(string hash, int size, string file_name , unordered_set<string>ip_port);


};

class Group{

    public:
        string group_id;    
        string admin;
        unordered_set<string> joining_requests;
        unordered_set<string> participants;
        unordered_map<string , Files> files_of_group; // string = file_name

    Group(string gid ,string admin);

    void print_group_details();
    
};

// variables

extern string logFileName;
extern string tracker1_ip;
extern string tracker2_ip;
extern string current_tracker_ip;
extern string seederFileName;
extern uint16_t tracker1_port;
extern uint16_t tracker2_port;
extern uint16_t current_tracker_port;
extern string client_ip;
extern uint16_t client_port;


// all data structure

extern unordered_map <string , User> user_meta_data;  // user_id  maps to user class
extern unordered_map <int , string> socket_user; // socketid  maps to the user_id
extern unordered_map <string , Group> group_map; // group id is mapped with group


// ds utility function

void print_user_meta_data();
void print_socket_user_map();
bool is_user_registered (string user_id);
bool is_user_login(int socket_id);
bool is_correct_userId (int socket_id, string user_id);
bool is_group_exist(string gid);
int is_user_part_of_group(int socket_id, string group_id);
bool is_admin(int socket_id , string group_id);
bool is_group_empty(string group_id);
int group_size(string group_id);
string ip_port_of_user(int socket_id);
int is_other_user_part_of_group(string user_id, string group_id);



// request handler per client
void request_handle_of_client(int client_socket);
void quit_handle_part(int tracker_socket);



// request exectuion function

void register_user(int socket_fd , vector<string>request , string &output );
void login( int client_socket , vector<string>request , string& output );
void logout( int client_socket, vector<string> request, string& output );
void create_group( int client_socket , vector<string> request , string& output);
void join_group( int client_socket , vector<string> request , string& output);
void leave_group( int client_socket , vector<string> request , string& output);
void list_requests( int client_socket , vector<string> request , string& output);
void accept_request( int client_socket , vector<string> request , string& output);
void list_groups(int client_socket, vector<string>req ,string &output);
void list_files(int client_socket ,vector<string> request , string &output);
void upload_file(int client_socket, vector<string> request,string &output );
void download_files(int client_socket, vector<string> request , string &output  );




// ultility function
vector<string> parse_command(char *client_request);
string give_string_cmd(vector<string> req_vec);
void print_request_vector(vector<string> req_vector);
void fetching_ip_port_from_file(int argc, char *argv[]);
vector<string> getTrackerInfo(char* path) ;



// Logger function
void clear_Error_Log();
void error_Log(const string &error);
void clear_success_Log();
void success_Log(const string &success);
void clear_request_Log();
void request_Log(const string &req);
