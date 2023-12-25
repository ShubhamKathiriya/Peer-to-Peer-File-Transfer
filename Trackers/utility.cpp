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

    for(int i=0; i<req_vec.size() ; i++){
        cmd += req_vec[i];
        cmd += " ";
    }

    cmd = cmd.substr(0, cmd.length()-1);

    return cmd;
}


vector<string> getTrackerInfo(char* file_path) {

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

    vector<string> all_tracker_info = getTrackerInfo(argv[1]);

    if(string(argv[2]) == "1"){

        tracker1_ip = all_tracker_info[0];
        tracker1_port = stoi(all_tracker_info[1]);
        current_tracker_ip = tracker1_ip;
        current_tracker_port = tracker1_port;
    
    }

    else{

        tracker2_ip = all_tracker_info[2];
        tracker2_port = stoi(all_tracker_info[3]);
        current_tracker_ip = tracker2_ip;
        current_tracker_port = tracker2_port;

    }

}



