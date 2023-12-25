#include "all_header.h"

void start_download(string res_from_tracker, string file_name  , string destination_path , string &final_download_response){

    vector<string> download_command = custom_tokenisor(res_from_tracker , " ");

    string status = download_command[0];

    // if tracker side error are there
    if(status == "ERROR_AT_DOWNLOAD:"){
        final_download_response = "Download not possible because of there are ";
        final_download_response += res_from_tracker;
        return;
    }


    string SHA_file = download_command[0];

    int file_size = stoi(download_command[1]);

    download_command.erase(download_command.begin()); // sha
    download_command.erase(download_command.begin());  // file_size
 

    download_command.pop_back();// extra ip_port


    // tokenised all ip_port

    vector<pair<string, int>> ip_port;

    for(auto it: download_command){

        string temp_ip_port = it;

        vector<string> token_ip_port = custom_tokenisor(temp_ip_port, ":");

        string token_ip = token_ip_port[0];
        int token_port = stoi(token_ip_port[1]);


        pair<string,int> temp =make_pair(token_ip , token_port);

        ip_port.push_back(temp);
    }


    Client_Files download_file(file_name, destination_path ,SHA_file , file_size);

    int total_chunks_file = chunk_count(file_size);
    int last_chunk = last_chunk_size(file_size);   

    vector<bool> chunk_status_download_file(total_chunks_file , false);


    download_file.which_chunk_present = chunk_status_download_file;

    all_files_meta_data.insert({file_name , download_file});


    vector <unordered_set <string> > chuck_record_seeder_wise(total_chunks_file);

    for(auto seeder : ip_port){

        string seeder_ip = seeder.first;
        int seeder_port = seeder.second;

        string str_ip_port = seeder_ip + ":" + to_string(seeder_port);
        

        int seeder_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (seeder_socket == -1) {

            cout << "Error!!! creating the seeder socker for downloading chunk or getting chunk" << endl;
            error_Log("Error!!! creating the seeder socker for downloading chunk or getting chunk");
            exit(0);
        }

        success_Log("succefully seeder socket: " + to_string(seeder_socket) +  " created!!") ;

        struct sockaddr_in address_of_seeder;
        address_of_seeder.sin_family = AF_INET;
        address_of_seeder.sin_port = htons(seeder_port);

        const char* serverIP = seeder_ip.c_str();

        int inet_fd = inet_pton(AF_INET, serverIP, &address_of_seeder.sin_addr);

        if ( inet_fd<= 0) {
            cout <<" Error!!! converting seeder'IP for downloading chunk or getting chunk" << endl;
            error_Log(" Error!!! converting seeder'IP for downloading chunk or getting chunk");
            close(seeder_socket);
            exit(0);
        }

        success_Log("succefully converted seeder ip !!!");

        int connect_fd = connect(seeder_socket, (struct sockaddr*)&address_of_seeder, sizeof(address_of_seeder));

        if ( connect_fd == -1) {

            cout << " Error!!! connecting seeder'IP for downloading chunk or getting chunk" << endl;
            error_Log(" Error!!! converting seeder'IP for downloading chunk or getting chunk");
            close(seeder_socket);
            exit(0);
        }

        string seeder_command ="which_chucks_are_avialable";
        seeder_command.push_back(' ');
        seeder_command += file_name;

        int write_cmd_fd = write(seeder_socket, seeder_command.c_str(), seeder_command.size());

        if(write_cmd_fd < 0){
            // logger("Error at write to seeder at gettingChunkInfo\n");
            error_Log("ERROR!! at writing buffer to seeder when request was which_chucks_are_avialable ");
            final_download_response =  "ERROR!! at writing buffer to seeder when request was which_chucks_are_avialable \n";
            return;
        }

        char response_buffer[RES_BUFFER_SIZE];

        memset(response_buffer, '\0', sizeof(response_buffer));

        int red_res_fd = read(seeder_socket, response_buffer, sizeof(response_buffer));


        if(red_res_fd < 0){
            // logger("Error at read to seeder at gettingChunkInfo\n");
            error_Log("ERROR!! at reading buffer from seeder when request was which_chucks_are_avialable ");

            final_download_response = "ERROR!! at reading buffer from seeder when request was which_chucks_are_avialable \n";

            return;
        }


        string temp(response_buffer ,RES_BUFFER_SIZE );
        string final_download_response =temp;
        string str_space_delimeter = " ";

        vector <string> seeder_reponse_token = custom_tokenisor(final_download_response, str_space_delimeter );


        if(seeder_reponse_token[0] == "ERROR_FROM_SEEDER:"){
            cout << "Error at fetching chunk info from seeder side " + seeder_ip + ":" + to_string(seeder_port) + "\n" + final_download_response << endl;
            error_Log(final_download_response);
            return;
        }

        else if(seeder_reponse_token[0] == "SUCCESS_FROM_SEEDER:"){

            seeder_reponse_token.erase(seeder_reponse_token.begin());

            for(auto str_chunk_number: seeder_reponse_token ){

                cout<< str_chunk_number<<endl;
                int chunk_number = stoi(str_chunk_number);
                chuck_record_seeder_wise[chunk_number].insert(str_ip_port);
            }
            
        }
        close(close(seeder_socket));
    }

    vector< pair<int,int> > count_seeder_maps_chunk_number;

    int chunk_number = 0;
    bool is_whole_file_avaiable_in_network = true;

    for(auto chunk_it : chuck_record_seeder_wise){

        count_seeder_maps_chunk_number.push_back({ chunk_it.size(), chunk_number});
        
        if(chunk_it.empty()){
            is_whole_file_avaiable_in_network = false;
            break;
        }

        chunk_number++;

    }

    if(!is_whole_file_avaiable_in_network){
        cout << "The whole file is not avaialable in network so You can't download!!!"<<endl;
        error_Log("The whole file is not avaible in netwrok!!!");
        return;
    }

    if(is_whole_file_avaiable_in_network) {

        int dest_file_create_fd = open(destination_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRWXU); //changes 0777
     

        if(dest_file_create_fd < 0){

            final_download_response =  "Can't create the requested download file may be less spavce u have || file size is: " +  to_string(file_size) +  "   and file path is :   " +  destination_path + "\n\n";
            error_Log("Can't create the requested download file");
            return;
        }


        char fullBuff[CHUNK_SIZE] = {0};

        // creating file with nulll character!!
        for(int i = 0; i < total_chunks_file; i++){

            if(i == total_chunks_file - 1){
                error_Log("last_chunk");
                int last_size = last_chunk_size(file_size);
                char last_buffer[last_size];

                int write_fd = write(dest_file_create_fd, fullBuff, last_size) ;
                
                if(write_fd < 0){

                    final_download_response = "Error occured!! in creating NULL character file !! please check stroage || || file size is: " +  to_string(file_size) +  "   and file path is :   " +  destination_path + "\n\n";
                    error_Log("error in creating null character filled file in desti path!!1");
                    return;
                } 
            }
            else {
                // log
                error_Log("other_chunk");

                int write_fd = write(dest_file_create_fd, fullBuff, CHUNK_SIZE);
                if( write_fd < 0){
                    
                    final_download_response = "Error occured!! in creating NULL character file !! please check stroage || || file size is: " +  to_string(file_size) +  "   and file path is :   " +  destination_path + "\n\n";
                    error_Log("error in creating null character filled file in desti path!!1");
                    return;

                }
            }
        }

        int close_file_fd = close(dest_file_create_fd);

        if(close_file_fd < 0){
            final_download_response ="error in closing file of null character!\n";
            error_Log("error in closing file of null character!\n");
            return;
        }

        for(int i = 0; i < total_chunks_file; i++){
            pair <int, string> chunkAndSeeder = custom_piece_wise_selection_algorithm(chuck_record_seeder_wise, count_seeder_maps_chunk_number);

            vector <string> ipPort = custom_tokenisor(chunkAndSeeder.second, ":");
            string ip = ipPort[0];
            int port = stoi(ipPort[1]);

            int seederSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (seederSocket == -1) {
                cerr << "Error creating socket at seeder connect" << endl;
                exit(0);
            }

            struct sockaddr_in seederAddr;
            seederAddr.sin_family = AF_INET;
            seederAddr.sin_port = htons(port);  
            const char* serverIP = ip.c_str();
            if (inet_pton(AF_INET, serverIP, &seederAddr.sin_addr) <= 0) {
                cerr << "Error converting IP address of seeder" << endl;
                close(seederSocket);
                exit(0);
            }

            if (connect(seederSocket, (struct sockaddr*)&seederAddr, sizeof(seederAddr)) == -1) {
                cerr << "Error connecting to seeder" << endl;
                close(seederSocket);
                exit(0);
            }

            string command = "send_me_this_chunk " + file_name + " " + to_string(chunkAndSeeder.first);
             
            if(write(seederSocket, command.c_str(), command.size()) < 0){
                // logger("Error at write to seeder at gettingChunkInfo\n");
                final_download_response =  "Error at write to seeder at gettingChunkInfo\n";
                return;
            }
               
            char outputBuffer[CHUNK_SIZE];
            memset(outputBuffer, 0, sizeof(outputBuffer));
            if(read(seederSocket, outputBuffer, sizeof(outputBuffer)) < 0){
                // logger("Error at read to seeder at gettingChunkInfo\n");
                final_download_response =  "Error at read to seeder at gettingChunkInfo\n";
                return;
            }

            
            string response(outputBuffer,sizeof(outputBuffer));

            vector <string> resTokens = custom_tokenisor(response, " ");
            if(resTokens[0] == "SeederSideError:"){
                
                cout << "Error at fetching chunkInfo from " + ip + ":" + to_string(port) + "\n" + outputBuffer << endl;
            } 
            else {          

                int fd1 = open(destination_path.c_str(), O_RDWR, S_IRWXU);
                if(fd1 < 0){
                    final_download_response = "Error while opening the file " + destination_path + " at writing chunk of download\n";
                    return;
                }

                lseek(fd1, get_start_pointer_for_chunk(chunkAndSeeder.first), SEEK_SET);

                if(i == total_chunks_file-1){

                    // int last_chunk_size = last_chunk;

                    if(write(fd1, outputBuffer, last_chunk) < 0){
                        final_download_response = "Error while wrtie the file " + destination_path + " at writing chunk of download\n";
                        return;
                    }

                }

                else{
                    if(write(fd1, outputBuffer, sizeof(outputBuffer)) < 0){
                        final_download_response = "Error while wrtie the file " + destination_path + " at writing chunk of download\n";
                        return;
                    }
                }

                close(fd1);
                
                all_files_meta_data.at(file_name).which_chunk_present[chunkAndSeeder.first] = true;

                // write chunk in file
            }
            close(close(seederSocket));
        }
        

    }

    // printchunkStatusOfOthersVector(chuck_record_seeder_wise);

    return ;

}