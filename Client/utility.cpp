#include "all_header.h"

vector<string> parse_command(char *client_request){

    string s, in = string(client_request);
    stringstream ss(in);
    vector<string> request_vector;

    while(ss >> s)
        request_vector.push_back(s);
    
    return request_vector;

}

void print_request_vector(vector<string> req_vector){

    cout << "toal_arguments:  "<< req_vector.size() << endl;

    for(auto req_token : req_vector)
        cout << req_token <<"  "; 

    cout<<endl;
}

string give_string_cmd(vector<string> req_vec){

    string cmd = "";

    for(int i=0; i<(int)req_vec.size() ; i++){
        cmd += req_vec[i];
        cmd += " ";
    }

    cmd = cmd.substr(0, cmd.length()-1);

    return cmd;
}

vector<string> spliting_string_by_delimeter(string address, string delimeter = ":"){
    
    vector<string> spilted_response;

    size_t position = 0;

    while ((position = address.find(delimeter)) != string::npos) {

        string t = address.substr(0, position);
        spilted_response.push_back(t);
        address.erase(0, position + delimeter.length());

    }

    spilted_response.push_back(address);

    return spilted_response;
}

vector<string> fetching_tracker_info_from_file(char* file_path) {

    // cout<<"filePath :     ";
    // cout<<file_path<<endl;
    int tracker_read_fd = open(file_path, O_RDONLY);

    vector<string> tracker_info;
    
    if (tracker_read_fd != -1) {
        
        string info;
        
        char read_buffer[1];

        for(;;){
            
            ssize_t read_done_bytes = read(tracker_read_fd, read_buffer, sizeof(read_buffer));
            
            if(read_done_bytes <= 0)
                break;

            if(read_buffer[0] == '\n'){

                tracker_info .push_back(info);
                info.clear();
            }
            
            else{
                info += read_buffer[0];
            }
        }

        if (!info.empty()) {
            tracker_info.push_back(info); // Add the last line if it didn't end with a newline
        }

        close(tracker_read_fd);
    }

    else{

        cout << "tracker_info_file is not " << endl;
        exit(-1);
    }

    return tracker_info;
}

void fetching_ip_port_from_file(int argc, char *argv[]){

    string peerInfo = argv[1];  
    string trackerInfoFilename = argv[2]; 

    vector<string> peeraddress = spliting_string_by_delimeter(peerInfo); 
    client_ip = peeraddress[0]; 
    client_port = stoi(peeraddress[1]);

    vector<string> all_tracker_info = fetching_tracker_info_from_file(argv[2]);

    tracker_ip = all_tracker_info[0];
    tracker_port = stoi(all_tracker_info[1]);
    
}


vector<string> custom_tokenisor(string command, string delimiter) {

    vector <string> command_tokens;

    string token(command);
    
    size_t start = token.find_first_not_of(" ");
    size_t end = token.find_last_not_of(" ");

    token = token.substr(start,end+1-start);

    int position = 0;

    while ((position = token.find(delimiter)) != string::npos) {

        command_tokens.push_back(token.substr(0, position));
        token.erase(0, position + 1);
    }
    command_tokens.push_back(token);

    return command_tokens;
}


int get_file_size_in_bytes(const char *path_of_file){

    int open_file_fd = open(path_of_file, O_RDONLY);

    if (open_file_fd < 0) {

        cout << "Error  in opening file while calculating the file size!! " << endl;
        error_Log("Error  in opening file while calculating the file size!!");
        return -1;
    }

    off_t size_bytes = lseek(open_file_fd, 0, SEEK_END);

    if (size_bytes < 0) {

        cout << "Error  in calculating the size of  file while calculating the file size!! "<< endl;
        error_Log("Error  in calculating the size of  file while calculating the file size!! ");
        return -1;
    }


    int close_file_fd = close(open_file_fd);

    if(close_file_fd < 0){

        cout << "Error  in closing file while calculating the file size!! "<< endl;
        error_Log("Error  in closing file while calculating the file size!! ");
        return -1;

    }

    success_Log("succesfully file size calculated!!!");

    return size_bytes;
}


string extract_filename_from_filepath(const char* file_path_c_str){

    string file_delimeter = "/";

    vector<string> file_tokens = custom_tokenisor(file_path_c_str , file_delimeter );

    string file_name = file_tokens.back();


    return file_name;
}


int chunk_count( int file_size ){

    double double_file_size = (double)file_size;

    double double_chunk_size = (double)CHUNK_SIZE;

    return ceil( double_file_size / double_chunk_size );

}

int last_chunk_size(int file_size){

    return (file_size % CHUNK_SIZE);

}


int get_start_pointer_for_chunk(int chunk_number){

    return CHUNK_SIZE*chunk_number;
}


// comment karvanu
void printchunkStatusOfOthersVector(vector <unordered_set <string> > chunk_record_status){
    int count = 0;
    cout << endl;
    cout << "CHUNK STATUS OF OTHERS........................." << endl;
    for(auto it: chunk_record_status){
        cout << "Chunk No " << count << " : ";
        for(auto ipPort: it){
            cout << ipPort << " ";
        }
        count++;
    }
    
}
