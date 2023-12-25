#include "all_header.h"

// ip port
string tracker_ip, client_ip;
uint16_t client_port, tracker_port;

// ds
unordered_map <string, Client_Files> all_files_meta_data;


int main(int argc, char *argv[]){

    clear_Error_Log();
    clear_success_Log();
    clear_request_Log();

    if(argc < 3){
        cout << "Less agruments are given\n .//client <IP>:<port> <tracker_port>" << endl;
        error_Log("Less agruments are provided!!");
    }

    if(argc > 3){
        cout << "more agruments are given\n .//client <IP>:<port> <tracker_port>" << endl;
        error_Log("more agruments are provided!!");
    }

    success_Log("Arguments are corrected!!");

    fetching_ip_port_from_file(argc , argv);

    thread leacher_tread(create_n_connect_leacher);
    leacher_tread.detach();


    // seeder tread 

    int seeder_socket, client_socket;
    struct sockaddr_in address_of_seeder, client_address;
    socklen_t client_length;

    seeder_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(seeder_socket < 0) {
        
        cout << "There is an error in creating socket for seeder at client side" <<endl;
        error_Log("Error in creating socket for seeder in client in ip " + client_ip +  "  & port :  " + to_string(client_port));
        exit(0);
    }
    
    success_Log("Socket creating successfully!!");

    
    bzero((char *) &address_of_seeder, sizeof(address_of_seeder));

    address_of_seeder.sin_family = AF_INET;
    address_of_seeder.sin_addr.s_addr = INADDR_ANY;
    address_of_seeder.sin_port = htons(client_port);

    int opt = 1;

    if (setsockopt(seeder_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){

        cout << "There is an error in setsocketopt in seeder!!"<<endl;
        error_Log("Error!! in seeder thread in  setsocketset !! in ip " + client_ip +  "  & port :  " + to_string(client_port) );
    }

    success_Log("setsockopt done successfully!!");


    if (inet_pton(AF_INET, client_ip.c_str(), &address_of_seeder.sin_addr) <= 0){
        
        cout << "There is an error in inet_pton in seeder!!"<<endl;
        error_Log("Error!! in seeder thread in  inet_pton !! in ip " + client_ip +  "  & port :  " + to_string(client_port) );

    }

    success_Log("inet_pton done successfully!!");


    int bind_fd  = bind(seeder_socket, (struct sockaddr *) &address_of_seeder, sizeof(address_of_seeder));


    if ( bind_fd < 0) {

        cout<< "There is an error in binding the socket of seeder side" << endl;
        error_Log("Error!! in seeder thread in  binding the port !! in ip " + client_ip +  "  & port :  " + to_string(client_port) );
        close(seeder_socket);
    }

    success_Log("Successfully!! binding the socket");


    int listen_fd = listen(seeder_socket, MAX_REQUEST);

     if ( listen_fd == -1) {

        cout << "error at listening" << endl;
        error_Log("There is an error in listening the  socket so closing the socket!!");
        close(seeder_socket);
        return 1;
    }

    success_Log("Successfully!! listening the scoket!!");


    client_length = sizeof(client_address);
   
    for(;;){

        int client_socket = accept(seeder_socket, (struct sockaddr *) &client_address, &client_length);

        if (client_socket < 0) {
            
            cout<< "There is an error in accepting the socket of seeder side" << endl;
            error_Log("Error!! in seeder thread in  accepting the port !! in ip " + client_ip +  "  & port :  " + to_string(client_port) );

            exit(0);
        }

        success_Log("Sucessfully!! accepted ");
            
        thread downaload_chunk( download_chunk_from_leecher, client_socket);
        downaload_chunk.detach();

    }

    return 0;
}