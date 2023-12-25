#include "all_header.h"

void chunk_requests_from_leecher(vector <string> request_vector, int leecherSocket, string& res){

    string str_leecher_socker = to_string(leecherSocket);

    if(request_vector.empty()){

        res = "ERROR_FROM_SEEDER:  empty command sent by seeder!!!";
        error_Log("Seeder sent empty buffer  while leecher requested the download stuff request!!!");
        return;
    }

    if(request_vector[0] == "which_chucks_are_avialable"){

        if(request_vector.size() < 2){
            
            res = "ERROR_FROM_SEEDER: Less arguments !! \n correct format of this request:- which_chucks_are_avialable <file_name>\n";
            error_Log("Less agruments are provided!! in which_chucks_are_avialable request!!!");
            return;
        }

        if(request_vector.size() > 2){
            
            res = "ERROR_FROM_SEEDER: More arguments !! \n correct format of this request:- which_chucks_are_avialable <file_name>\n";
            error_Log("More agruments are provided!! in which_chucks_are_avialable request!!!");
            return;
        }

        string request_string = give_string_cmd(request_vector);
        request_Log(request_string);

        
        string requested_file_name = request_vector[1];
        auto file_map_it = all_files_meta_data.find(requested_file_name);

        // print_all_file_meta_data();

        if(file_map_it == all_files_meta_data.end()){
            
            res = "SUCCESS_FROM_SEEDER: ";
            success_Log("Succefully seeder  sent chunk info to :" + str_leecher_socker+"   leecher!!\n");
            return;
        }

        res = "SUCCESS_FROM_SEEDER: ";

        int chunk_number = 0;
        string str_chunk_number = "";


        for(auto chunk_it : file_map_it->second.which_chunk_present){

            str_chunk_number = to_string(chunk_number);

            if(chunk_it == true)
                res += str_chunk_number;
            
            else    
                continue;
            
            res += " ";
            chunk_number++;

        }

        res.pop_back();

        success_Log("Succesfully sent response of which chunk available to the leecher : " + str_leecher_socker );
        return;
    }

    else if(request_vector[0] == "send_me_this_chunk"){

        if(request_vector.size() < 3){
            
            res = "ERROR_FROM_SEEDER: Less arguments !! \n correct format of this request:- send_me_this_chunk <file_name> <chunk_ number>\n";
            error_Log("Less agruments are provided!! in send_me_this_chunk request!!!");
            return;
        }

        if(request_vector.size() > 3){
            
            res = "ERROR_FROM_SEEDER: More arguments !! \n correct format of this request:- send_me_this_chunk <file_name> <chunk_ number>\n";
            error_Log("More agruments are provided!! in send_me_this_chunk request!!!");
            return;
        }

        string file_name = request_vector[1];
        string str_requested_chunk_number = request_vector[2];

        int requested_chunk_number = stoi(str_requested_chunk_number);

        auto file_it = all_files_meta_data.find(file_name);

        if(file_it == all_files_meta_data.end()){
            res = "ERROR_FROM_SEEDER: File not found!!";
            error_Log("Request chunk related file is not present any more!!!");
            return;
        }

        vector<bool>request_file_chunk_status = file_it->second.which_chunk_present;

        string requested_file_path = file_it->second.file_path;
        const char* c_str_file_path = requested_file_path.c_str();

        

        int file_open_fd = open(c_str_file_path , O_RDONLY , S_IRWXU);

        if(file_open_fd < 0){
            cout << "There is an error in opening the file at seeder side"<<endl;
            res = "ERROR_FROM_SEEDER: error in opening the file!!";
            error_Log("There is an error in opening the file at seeder side by this leecher :  " + str_leecher_socker );
            return;
        }   

        int start_pointer_for_reading = get_start_pointer_for_chunk(requested_chunk_number);

        lseek(file_open_fd, start_pointer_for_reading, SEEK_SET);

        int file_size = file_it->second.file_size_bytes;
        
        int total_chunk = chunk_count(file_size);

        char read_chunk_buff[CHUNK_SIZE];

            int read_chunk_fd = read(file_open_fd, read_chunk_buff, CHUNK_SIZE);

            if( read_chunk_fd < 0){
                
                cout << "There is an error in reading the file at seeder side"<<endl;
                res = "ERROR_FROM_SEEDER: error in reading the chunk in the file!!";
                error_Log("There is an error in reading the chunk of the file at seeder side by this leecher :  " + str_leecher_socker );
                return;
            }

        string temp(read_chunk_buff ,CHUNK_SIZE );
        
        res = temp;

        int close_file_fd = close(file_open_fd);

        if(close_file_fd < 0){
            cout << "There is an error in closing the file at seeder side"<<endl;
            res = "ERROR_FROM_SEEDER: error in closing the chunk in the file!!";
            error_Log("There is an error in closing the chunk of the file at seeder side by this leecher :  " + str_leecher_socker );
            return;
        }

        return ;
    }

    else    
        res = "you sent not vaidate command !! so please send valid one!!!";
    
    return ;
}


void download_chunk_from_leecher (int leecher_socket_id){


    success_Log("Client connected successfully which socket number is  " + to_string(leecher_socket_id) + " !!!\n");

    char input_request_buffer[DOWNLOAD_BUFFER_SIZE];

    memset(input_request_buffer, 0, sizeof(input_request_buffer));

    int read_req_fd = read(leecher_socket_id, input_request_buffer, sizeof(input_request_buffer));
    
    if(read_req_fd < 0){

        cout << "There is an error in reading request buffer at leecher side "<<endl;
        error_Log("There is an error in reading request buffer at leecher side :  " + to_string(leecher_socket_id) + '\n');

        int close_leecher_socket_fd =  close(leecher_socket_id);

        if(close_leecher_socket_fd < 0){
        
            cout<<"There is an error in closing the leecher socket while leeacher download request!!"<<endl;
            error_Log("There is an error in closing the leecher socket while leeacher download request!!" + to_string(leecher_socket_id) + '\n');
            return;
        }

        return;
    }

    string string_input_req_buffer = (string)input_request_buffer;
    string space_str_delimeter = " ";
    vector <string> tokens = custom_tokenisor(string_input_req_buffer, space_str_delimeter);

    string response_buffer = "";

    chunk_requests_from_leecher(tokens, leecher_socket_id , response_buffer);

    if(response_buffer == "quit"){
        response_buffer = "see you!!.\n";
        success_Log("Socket closed as per request!!!");
        return;
    }

    int write_res_buff_fd = write(leecher_socket_id, response_buffer.c_str(), response_buffer.size());
    
    if(write_res_buff_fd < 0){
        // logger("Client socket number " + to_string(leecherSocket) + " : Write failed\n");
        error_Log("Error!! at writting the buffer !!!");

        close(leecher_socket_id);
        return;
    }

    int close_leecher_socket_fd  = close(leecher_socket_id);

    if(close_leecher_socket_fd < 0){
        cout<<"Error in seeder side to cosing leecher socket";
        error_Log("Error in seeder side to cosing leecher socket" + to_string(leecher_socket_id));
        return;
    }

}


void handle_leacher_request(int client_socket  , string command , string& res){

        vector<string> commandVector = custom_tokenisor(command, " ");

        if(commandVector[0] == "create_user") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {

                cout<<"There is some error in while sending the command to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];

            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {

                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;

            }

            res = string(tracker_response);
        } 

        else if(commandVector[0] == "Login") {
            
            command.push_back(' ');
            command += client_ip;

            command.push_back(' ');
            string str_client_port = to_string(client_port); 

            command += str_client_port;

            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        } 
        

        else if(commandVector[0] == "logout") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        }         


        else if(commandVector[0] == "create_group") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        } 

        else if(commandVector[0] == "join_group") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        }

        else if(commandVector[0] == "leave_group") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        } 


        else if(commandVector[0] == "list_requests") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        }


        else if(commandVector[0] == "accept_request") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        }       

        else if(commandVector[0] == "list_files") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        } 
        
        else if(commandVector[0] == "list_groups") {
            
            int command_write_fd = write(client_socket, command.c_str(), command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);
        } 



        else if(commandVector[0] == "upload_file") {

            const char* file_path_c_str = commandVector[1].c_str();

            string file_SHA_hash =  calc_SHA_hash_of_file(file_path_c_str);
            int file_size_bytes = get_file_size_in_bytes(file_path_c_str);
            string file_name = extract_filename_from_filepath(file_path_c_str);

            string tracker_command = "";
            // upload_file <group id><sha><file size><fileName>


            tracker_command += commandVector[0];
            tracker_command.push_back(' ');

            tracker_command += commandVector[2];
            tracker_command.push_back(' ');

            tracker_command += file_SHA_hash;
            tracker_command.push_back(' ');

            tracker_command += to_string(file_size_bytes);
            tracker_command.push_back(' ');

            tracker_command += file_name;
            tracker_command.push_back(' ');
            
            int command_write_fd = write(client_socket, tracker_command.c_str(), tracker_command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }


            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            Client_Files my_files(file_name , commandVector[1] , file_SHA_hash , file_size_bytes);

            int chunk_number = chunk_count(file_size_bytes);
        
            vector<bool>v(chunk_number , true);
            my_files.which_chunk_present = v;
            
            all_files_meta_data.insert({file_name , my_files});
            
            res = string(tracker_response);
        } 


        // incomplete
        else if(commandVector[0] == "download_file") {

            const char* file_path_c_str = commandVector[2].c_str();

            string file_name = extract_filename_from_filepath(file_path_c_str);

            string tracker_command = "";
            // download_file <group id><fileName> <dest_path>  terminal
            // download_file  <group_id><filename>    for tracker


            tracker_command += commandVector[0];
            tracker_command.push_back(' ');

            tracker_command += commandVector[1];      // group id
            tracker_command.push_back(' ');

            tracker_command += file_name;
            tracker_command.push_back(' ');

            string destination_path = commandVector.back();

            int command_write_fd = write(client_socket, tracker_command.c_str(), tracker_command.size());

            if (command_write_fd < 0) {
                cout<<"There is some error in while sending the cpmmand to tracker!!"<<endl;
                error_Log("Error in while sending the file tp tracker!!");
                return ;
            }
            
            char tracker_response[RES_BUFFER_SIZE];
            memset(tracker_response , '\0', sizeof(tracker_response) );

            int read_res_fd = read(client_socket, &tracker_response, RES_BUFFER_SIZE);
            
            if (read_res_fd < 0) {
                cout<< "There is some error in reading the tracker_response of tracker!!"<<endl;
                error_Log("Error in while reading the responce of tracker!!");
                return ;
            }

            
            res = string(tracker_response);

            string final_download_response = "";

            start_download(res, file_name, destination_path , final_download_response);
            cout << "SHA is matched!! So download successfully!!"<<endl;
            success_Log("Succesfully downloaded!!!");
            sleep(15);
            // res = final_dwon_res
            return ;
        } 


        else{

            res = "You entered Invalid command please check again :)  !!!\n\n";  
            return ;
        }   

    }


void create_n_connect_leacher() {

    int leacher_socket;

    struct sockaddr_in address_of_tracker;
    struct hostent *tracker;
    
    leacher_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(leacher_socket < 0) {
        cout <<"There is an error in opening the tracjer socket \n Please make sure Your tracke is on!!!"<<endl;
        error_Log("Can't open the tracker socket at leacher side!!!");

        return;
    }

    success_Log("Successfully opened tracker socket at leacher side!!");


    tracker = gethostbyname(tracker_ip.c_str());

    if(!tracker) {

        cout << "Your tracker is not avaible please check it again tracker_info_file.txt!!!!" <<endl;
        error_Log("no such tracker exist in leacher side!!!");
        return;
    }

    success_Log("Tracker found successfully !!!");

    address_of_tracker.sin_family = AF_INET;
    
    bcopy((char *) tracker->h_addr, (char *) & address_of_tracker.sin_addr.s_addr, tracker->h_length);

    address_of_tracker.sin_port = htons(tracker_port);

    int connect_fd = connect(leacher_socket,(struct sockaddr *) &address_of_tracker, sizeof(address_of_tracker));
   

    if(connect_fd < 0) {

        cout << "cant accept the connection please check the tracker once!!!" << endl;
        error_Log("Connection failed at leacher side!!!");
        close(leacher_socket);
        return;
    }

    cout << "successfully connection established!!!" << endl;

    success_Log("Connection established success at leacher side!!!");

    for(;;){

        string tracker_response = "";
        string leacher_command = "";
        // cin.ignore();
        cin.clear();
        getline(cin, leacher_command);
        
        request_Log(leacher_command);
        // infi loop
        if(!leacher_command.size()){
            
            continue;
        }
        
        if(leacher_command == "quit"){

            cout << "closing leacher socket by request!!" << endl;
            success_Log("Closed the leacher socket y request!!");
            close(leacher_socket);
            exit(0);
        }

        handle_leacher_request(leacher_socket, leacher_command , tracker_response);
        
        request_Log("client:-  " + leacher_command);

        cout << tracker_response << endl;
    }
}