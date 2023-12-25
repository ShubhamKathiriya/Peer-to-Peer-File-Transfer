#include "all_header.h"


void request_handle_of_client(int client_socket){

    char client_request[1024] = {0};

    while (true){

        memset(client_request, 0, sizeof(client_request));

        int client_read_fd = read(client_socket, client_request, sizeof(client_request));

        if( client_read_fd<= 0){
            close(client_socket);
            error_Log("There is an  error in reading the buffer!!!");
            return;
        }

        success_Log("Successfully!! read the buffer!!");

        vector<string>request = parse_command(client_request);
        string command_string = give_string_cmd(request);

        request_Log("Client :-" + command_string);

        string command = request[0];
        string outputBuffer = "";

        if(command == "quit"){

            close(client_socket);
            outputBuffer = "Connection is terminated by ur request";

            success_Log("All requests are successfully completeddd");
        }

        else if(command =="create_user"){

            register_user( client_socket , request , outputBuffer );

        }

        else if(command == "Login"){
            
            login(client_socket , request , outputBuffer);

        }

        else if(command == "create_group"){
            create_group(client_socket, request , outputBuffer);
        }

        else if(command == "join_group"){
            join_group(client_socket, request , outputBuffer);
        }

        else if(command == "leave_group"){
            leave_group(client_socket, request , outputBuffer);

        }

        else if(command == "list_requests"){
            list_requests(client_socket, request , outputBuffer);
        }

        else if(command == "accept_request"){
            accept_request(client_socket, request , outputBuffer);
        }
        
        else if(command == "list_groups"){
            list_groups(client_socket , request , outputBuffer);
        }
        
        else if(command == "list_files"){
            list_files(client_socket, request , outputBuffer);
        }
      
        else if(command == "upload_file"){
            upload_file(client_socket, request , outputBuffer);
        }

        else if(command == "download_file"){
            download_files(client_socket, request , outputBuffer);
        }

        else if(command == "logout"){
            logout(client_socket, request , outputBuffer);
        }

        else if(command == "show_downloads"){
            
        }

        else if(command == "stop_share"){
            
        }

        else{
            outputBuffer = "You enter the wrong command !!!";
            error_Log(outputBuffer);
        } 

          
        
        
        
        
        
        
        outputBuffer =  outputBuffer + "\n\n";
        
        int response_fd = write(client_socket, outputBuffer.c_str(), outputBuffer.size());
        
        if(response_fd <= 0){
            close(client_socket);
            error_Log("Error in  the writing the buffer!!!");
            return;
        }

        success_Log("Succesfully write done!!!");

    }

}



void quit_handle_part(int tracker_socket){

    string input;

    for(;;){

        cin>>input;
        if(input == "quit"){

            close(tracker_socket);
            cout << "Tracker socket : " <<current_tracker_port << "  is closed!!" <<endl;
            success_Log("Tracker socket go down by request!!!");
            exit(0);
        }

    }

}