#include "all_header.h"


unordered_map <string , User> user_meta_data;  // user_id  maps to user class
unordered_map <int , string> socket_user; // socketid  maps to the user_id
unordered_map <string , Group> group_map;  // group id is mapped to the group

string logFileName, tracker1_ip, tracker2_ip, current_tracker_ip,seederFileName;
uint16_t tracker1_port, tracker2_port, current_tracker_port;
string client_ip;
uint16_t client_port;

int main(int argc, char *argv[]){

    clear_Error_Log();
    clear_success_Log();
    clear_request_Log();

    if(argc < 3){
        cout << "Less agruments" << endl;
        error_Log("Less agrument!! PLease provide the port for the tracker");
        return  0;
    }

    if(argc > 3){
        cout <<"More arguments"<< endl;
        error_Log("More than required agrument!! You entered some not required agruments");
        return  0;
    }

    
    fetching_ip_port_from_file(argc , argv);
    
    // cout<<current_tracker_port<<endl;
    
    success_Log("Successfully received port number :-  "  + to_string(current_tracker_port) );

    int tracker_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (tracker_socket == -1) {
        
        cout << "error at socket creating" << endl;
        error_Log("There is an error in creating the socket !!");
        return 0;
    }

    success_Log("Socket creating successfully!!");

    struct sockaddr_in address_server;
    address_server.sin_family = AF_INET;
    address_server.sin_port = htons(current_tracker_port);
    address_server.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;

    string trackerIP = TRACKER_IP;

    if (setsockopt(tracker_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        cerr << "setsockopt";
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET, trackerIP.c_str(), &address_server.sin_addr) <= 0){
        cerr << "\nInvalid address/ Address not supported \n";
        return -1;
    }


    int bind_fd = bind(tracker_socket, (struct sockaddr*)&address_server, sizeof(address_server)) ;

    if (bind_fd == -1) {
        cout << "error at binding" << endl;
        error_Log("There is an error in binding the  socket so closing the socket !!");
        close(tracker_socket);
        return 1;
    }

    success_Log("Successfully!! binding the socket");

    int listen_fd = listen(tracker_socket, MAX_REQUEST);

    if ( listen_fd == -1) {
        cout << "error at listening" << endl;
        error_Log("There is an error in listening the  socket so closing the socket!!");
        close(tracker_socket);
        return 1;
    }

    success_Log("Successfully!! listening the scoket!!");
    
    thread quitThread(quit_handle_part, current_tracker_port);
    quitThread.detach();

    vector <thread> client_threads;

    while(true){

        struct sockaddr_in address_client;
        socklen_t address_clientSize = sizeof(address_client);

        int clientSocket = accept(tracker_socket, (struct sockaddr*)&address_client, &address_clientSize);
        
        if (clientSocket == -1) {
            cout << "Error at accepting!!" << endl;
            error_Log("There is an error at accepting!!");
            return 1;
        }

        // char clientIP[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(address_client.sin_addr), clientIP, INET_ADDRSTRLEN);

        // client_ip = (string)clientIP;
        // client_port = ntohs(address_client.sin_port);

        // cout << client_ip<<"    "<<client_port<<endl;


        success_Log("Sucessfully!! accepted ");

        client_threads.push_back(thread(request_handle_of_client, clientSocket));
    }

    for(auto it=client_threads.begin(); it!=client_threads.end();it++){
        if(it->joinable()) it->join();
    }
    
    success_Log("All requested are completed!!!");
    
    int close_fd = close(tracker_socket);

    if(close_fd == -1){
        cout<<"Error at closing the sockets!!"<<endl;
        error_Log("There is an error in closing the socket!!");
    }

    success_Log("Socket is closed!!");

    return 0;
}