#include "all_header.h"

// all data structure
// unordered_map<int, string> socket_user; 
// unordered_map <string , User> user_meta_data;


void register_user(int client_socket , vector<string>request , string &output ){

    if(request.size () < 3){
        output = "You forgot to enter suffiecient argument!!!  \n   correct format was :- create_user <user_id> <password> !!!";
        error_Log("Less arguments for create_user command!!");
        return ;
    }

    if(request.size() > 3){
        output = "You entered more than suffiecient argument!!!  \n   correct format was :- create_user <user_id> <password> !!!";
        error_Log("More arguments for create_user command!!");
        return ;
    }
    
    string user_ID = request[1];
    string password  = request[2];

    if(is_user_registered(user_ID)){

        output = "User is already registered !!!";
        error_Log("ALready registered!!");
        return;
    }
    
    User new_user(password);

    user_meta_data.insert({user_ID, new_user});

    // print_user_meta_data();

    output = "Your registration is completed";
    success_Log("Successfully registred!!!");
    return;
}


void login( int client_socket , vector<string>request , string& output ){

    if(request.size () < 5){
        output = "You forgot to enter suffiecient argument!!!  \n   correct format was :- login <user_id> <password> !!!";
        error_Log("Less arguments for create_user command!!");
        return ;
    }

    if(request.size() > 5){
        output = "You entered more than suffiecient argument!!!  \n   correct format was :- login <user_id> <password> !!!";
        error_Log("More arguments for create_user command!!");
        return ;
    }

    string user_ID = request[1];
    string password  = request[2];
    string user_ip = request[3];
    string user_port = request[4];

    if(!(is_user_registered(user_ID))){

        output = "User is not registered or User ID is not correct!!!!!";
        error_Log("User is not registered or Wrong user id at login time!!");
        return;
    }

    if(is_user_login(client_socket)){

        output = "you are already logged in another device !!!";
        error_Log("Already Logged in same device!!!");
        return ;
    }
    

    auto user_it = user_meta_data.find(user_ID);

    if( password !=  user_it->second.password ){
        output = "Wrong Password!!!!";
        error_Log("Wrong password at Login time!!!");
        return;
    }

    user_it->second.currentLogin = true;
    user_it->second.ip = user_ip;
    user_it->second.port = user_port;
    socket_user[client_socket] =  user_ID;

    output = "Sucessfully logged in!!!";
    success_Log("Sucessfully logged in !!!!");
    // print_socket_user_map();

    return;

}


void logout( int client_socket , vector<string> request , string& output){

    if(request.size() > 1){
        output = "Too many arguments given!! \n Correct format is logout";
        error_Log("Too many arguments given in Logout command!!!");
        return;
    }

    if(!(is_user_login(client_socket))){
        output = "You not even Login!!!";
        error_Log("Attempt to logout without login ");
        return ;
    }

    auto socket_it = socket_user.find(client_socket);
    string user_ID = socket_it->second;

    auto user_it = user_meta_data.find(user_ID);

    user_it->second.currentLogin = false;
    user_it->second.ip.clear();
    user_it->second.port.clear();

    // set ip port as null string
    socket_user.erase(socket_it);

    success_Log("succesfully logout!!!");
    output = "succesfully logout!!!";

}


void create_group( int client_socket , vector<string> request , string& output){

    if(request.size() < 2){
        output = "Less agruments are provided\n Correct command is  create_group <group_id>";
        error_Log("Less agruments in create_group command!!");
        return;
    }

    if(request.size() > 2){
        output = "More agruments are provided\n Correct command is  create_group <group_id>";
        error_Log("More agruments in create_group command!!");
        return;
    }

    if(!is_user_login(client_socket)){
        output = "You can't create the group without login in the network!!!";
        error_Log("creating the group  without doing login!!");
        return ;
    }

    string group_id = request[1];

    if(is_group_exist(group_id)){
        output = "This group id  is already in use!!";
        error_Log("Group already exist!! while creating group!!");
        return;
    }

    // check first is it part of the another group ot not if it is part of another group then it cant create
    
    auto user_it  = socket_user.find(client_socket);
    string user_id = user_it->second;

    auto user_data_it = user_meta_data.find(user_id);
    string current_group_id = user_data_it->second.group_id;

    if(current_group_id.size() > 0){
        output = "You are part of another group so u cant create one!!";
        error_Log("Attemp tp create  group while it is part of another group!!");
        return;
    }

    Group new_group(group_id , user_id);
    new_group.participants.insert(user_id);

    group_map.insert({group_id , new_group});

    user_data_it->second.group_id = group_id;
    new_group.print_group_details();

    output = "Group is sucessfully created!";
    success_Log("Group with id:- " + group_id + "Is sucessfully created!");

}


void join_group( int client_socket , vector<string> request , string& output){
    
    if(request.size() < 2){
        output = "Less agruments are provided\n Correct command is  join_group <group_id>";
        error_Log("Less agruments in join_group command!!");
        return;
    }

    if(request.size() > 2){
        output = "More agruments are provided\n Correct command is  join_group <group_id>";
        error_Log("More agruments in join_group command!!");
        return;
    }

    if(!is_user_login(client_socket)){
        output = "You can't join the group without login in the network!!!";
        error_Log("joining the group without doing login!!");
        return ;
    }

    string group_id = request[1];
    
    if(!is_group_exist(group_id)){
        output = "This group id  is not exist!!";
        error_Log("Group is not exist!! while joining group!!");
        return;
    }

    auto socket_it = socket_user.find(client_socket);
    string user_id = socket_it->second;

    int group_status = is_user_part_of_group(client_socket , group_id);

    if(group_status == 1){
        output = "You are already part of some another group not this group";
        error_Log("User is part of another group so it can't joing this group");
        return;
    }

    if(group_status == 2){
        output = "You are already part of this group!!!";
        error_Log("User is already part of join requested group!");
        return;
    }

    // user is not part of group

    auto group_it = group_map.find(group_id);

    group_it->second.joining_requests.insert(user_id);
    
    group_it->second.print_group_details();

    output = "Your joining request is sent to admin of the group";
    success_Log("Succesfully join request of sent to admin");
}



void leave_group( int client_socket , vector<string> request , string& output){

    if(request.size() < 2){
        output = "Less agruments are provided\n Correct command is  leave_group <group_id>";
        error_Log("Less agruments in leave_group command!!");
        return;
    }

    if(request.size() > 2){
        output = "More agruments are provided\n Correct command is  leave_group <group_id>";
        error_Log("More agruments in leave_group command!!");
        return;
    }

    if(!is_user_login(client_socket)){
        output = "You can't leave the group without login in the network!!!";
        error_Log("leaving the group without doing login!!");
        return ;
    }

    string group_id = request[1];
    
    if(!is_group_exist(group_id)){
        output = "This group id  is not exist!!";
        error_Log("Group is not exist!! while leaving group!!");
        return;
    }

    auto socket_it = socket_user.find(client_socket);
    string user_id = socket_it->second;

    int group_status = is_user_part_of_group(client_socket , group_id);

    if(group_status != 2){
        output = "You are not part of this group!!";
        error_Log("User is not part of this group so it can't leave this group");
        return;
    }

    auto user_it = user_meta_data.find(user_id);

    user_it->second.group_id = "";


    auto group_it = group_map.find(group_id);

    group_it->second.participants.erase(user_id);
    // reove filess from groups
    
    output = "You left the group!!";
    success_Log("User is successfully leave the group " + group_id);

}



void list_requests( int client_socket , vector<string> request , string& output){

    if(request.size() < 2){
        output = "Less agruments are provided\n Correct command is  list_requests <group_id>";
        error_Log("Less agruments in list_requests command!!");
        return;
    }

    if(request.size() > 2){
        output = "More agruments are provided\n Correct command is  list_requests <group_id>";
        error_Log("More agruments in list_requests command!!");
        return;
    }

    if(!is_user_login(client_socket)){
        output = "You can't list the request the group without login in the network!!!";
        error_Log("requesting the pending request the group without doing login!!");
        return ;
    }

    string group_id = request[1];
    
    if(!is_group_exist(group_id)){
        output = "This group id  is not exist!!";
        error_Log("Group is not exist!! while requesting the pending the request group!!");
        return;
    }

    int group_status = is_user_part_of_group(client_socket , group_id);

    if(group_status != 2){
        output = "You are not part of this group!!";
        error_Log("User is not part of this group so it can't leave this group");
        return;
    }


    if(!is_admin(client_socket , group_id)){
        output = "You are not the admin of this group!! sorry!!";
        error_Log("Attempting list of the request of pending request of the group!!");
        return;
    }

    auto group_it = group_map.find(group_id);

    unordered_set<string>pending_req = group_it->second.joining_requests;

    output = "================Group Pending Request==============\n\n";
    
    for(auto req: pending_req){

        output += req;
        output +='\n';
    }

    success_Log("Succesfully display the pending request to admin of the group!");
     
    // cout<<"======================================================="<<endl;


}


void accept_request( int client_socket , vector<string> request , string& output){

    if(request.size() < 3){
        output = "Less agruments are provided\n Correct command is  accept_request <group_id> <user_id>";
        error_Log("Less agruments in accept_request command!!");
        return;
    }

    if(request.size() > 3){
        output = "More agruments are provided\n Correct command is  accept_request <group_id> <user_id>";
        error_Log("More agruments in accept_request command!!");
        return;
    }

    if(!is_user_login(client_socket)){
        output = "You can't accept the request of the group without login in the network!!!";
        error_Log("accept the request the pending request the group without doing login!!");
        return ;
    }

    string group_id = request[1];
    string req_id = request[2];
    
    if(!is_group_exist(group_id)){
        output = "This group id  is not exist!!";
        error_Log("Group is not exist!! while requesting the pending the request group!!");
        return;
    }


    int group_status = is_other_user_part_of_group(req_id , group_id);

    if(group_status == 2){
        output = "You are  part of this group!!";
        error_Log("User is  part of this group so while accepting the request!!");
        return;
    }


    if(!is_admin(client_socket , group_id)){
        output = "You are not the admin of this group!! sorry!!";
        error_Log("Attempting list of the request of pending request of the group!!");
        return;
    }
    
    // check wherether user is part of another group or not if yes return ele go on

    auto user_data_it = user_meta_data.find(req_id);
    string current_group_id = user_data_it->second.group_id;


    if(current_group_id.size()  && current_group_id != group_id){
        output = "user is already part of the some other group";
        error_Log("user is already part of the some other group");
        return;
    }

    if(current_group_id == group_id){
        output = "user is already part of the same other group";
        error_Log("user is already part of the same other group");
        return;
    }

    auto group_it = group_map.find(group_id);

    unordered_set<string>pending_req = group_it->second.joining_requests;

    //check the user id is in pending request or not

    if(pending_req.find(req_id) == pending_req.end()){
        output = "This user request is not pending!!";
        error_Log("This user request is not pending!!");
        return;
    }

    // remove the req from pending 
    // add in participant

    pending_req.erase(req_id);
    group_it->second.participants.insert(req_id);

    group_it->second.joining_requests.erase(req_id);

    // set the user group id this one
    user_data_it->second.group_id = group_id;

    output = "Succesfully accepted user request";
    success_Log("Succesfully accepted user request");

}


void list_files(int client_socket ,vector<string> request , string &output){

    if(request.size() < 2){
        output = "Less agruments are provided\n Correct command is  list_files <group_id>";
        error_Log("Less agruments in list_files command!!");
        return;
    }

    if(request.size() > 2){
        output = "More agruments are provided\n Correct command is  list_files <group_id>";
        error_Log("More agruments in list_files command!!");
        return;
    }


    if(!is_user_login(client_socket)){
        output = "You can't list the files of the group without login in the network!!!";
        error_Log("attempt the request the listing the files  the group without doing login!!");
        return ;
    }
    
    string group_id = request[1];

    if(!is_group_exist(group_id)){
        output = "This group id  is not exist!!";
        error_Log("Group is not exist!! while requesting the pending the request group!!");
        return;
    }


    int group_status = is_user_part_of_group(client_socket , group_id);

    if(group_status != 2){
        output = "You are not part of this group!!";
        error_Log("User is not part of this group so it can't leave this group");
        return;
    }

    auto group_it = group_map.find(group_id);

    unordered_map<string , Files> mapping =  group_it->second.files_of_group;

    string all_files = "";

    for(auto file_name : mapping){
        
        all_files += file_name.first;
        all_files += "\n";
    }

    output = all_files;
    success_Log("All files display of group!!");
}


void list_groups(int client_socket, vector<string>request ,string &output){

    if(request.size() > 1){
        output = "MOre agrumens are provided!! correct command is  list_groups ";
        error_Log("MOre agruments for the list groups command");
        return;
    }

    if(!is_user_login(client_socket)){
        output = "You can't accept the listing of the group without login in the network!!!";
        error_Log("attemp the request the  listing the group without doing login!!");
        return ;
    }

    output = "";

    for(auto it: group_map){

        output += it.first;
        output += '\n';
    }

    success_Log("all group within newtwork displayedd!!!");
}


void upload_file(int client_socket, vector<string> request,string &output ){

    if(request.size() < 5){
        output = "Less agruments are provided\n Correct command is  upload_file <file_path> <group id>";
        error_Log("Less agruments in upload_file command!!");
        return;
    }

    if(request.size() > 5){
        output = "More agruments are provided\n Correct command is  upload_file <file_path> <group id>";
        error_Log("More agruments in upload_file command!!");
        return;
    }

    string group_id = request[1];
    string SHA  = request[2];
    int file_size = stoi(request[3]);
    string file_name = request[4];

    if(!is_user_login(client_socket)){
        output = "You can't upload the file in the group without login in the network!!!";
        error_Log("attempt the upload the file in the group without doing login!!");
        return ;
    }

    if(!is_group_exist(group_id)){
        output = "This group id  is not exist!!";
        error_Log("Group is not exist!! while requesting the pending the request group!!");
        return;
    }

    int group_status = is_user_part_of_group(client_socket , group_id);

    if(group_status != 2){
        output = "You are not part of this group!!";
        error_Log("User is not part of this group so it can't leave this group");
        return;
    }

    string ip_Port = ip_port_of_user(client_socket);
    
    auto group_it = group_map.find(group_id);

    unordered_map<string , Files> file_map = group_it->second.files_of_group;

    if( file_map.find(file_name) != file_map.end()){

        auto file_it  = file_map.find(file_name);

        file_it->second.ip_port.insert(" ");
        file_it->second.ip_port.insert(ip_Port);

        auto user_it = socket_user.find(client_socket);
        string user_id = user_it->second;

        auto user_data  = user_meta_data.find(user_id);
        user_data->second.file_name.insert(file_name);
        cout<<"updated!!!"<<endl;
    }

    else{

        unordered_set<string> ip_port_files;

        ip_port_files.insert(ip_Port);

        Files newfile (SHA,  file_size,file_name, ip_port_files);

        group_it->second.files_of_group.insert({file_name,newfile});

        auto user_it = socket_user.find(client_socket);
        string user_id = user_it->second;

        auto user_data  = user_meta_data.find(user_id);
        user_data->second.file_name.insert(file_name);

        group_it->second.print_group_details();
        cout<<endl;
    }


    output = file_name +  "     file uploaded";
    // group_it->second.print_group_details();
    success_Log("file uploaded!");
}


void download_files(int client_socket, vector<string> request , string &output  ){

    if(request.size() < 3){
        output = "ERROR_AT_DOWNLOAD: Less agruments are provided\n Correct command is  download_file <group id><fileName><dest_path>";
        error_Log("Less agruments in upload_file command!!");
        return;
    }

    if(request.size() > 3){
        output = "ERROR_AT_DOWNLOAD: More agruments are provided\n Correct command is  download_file <group id><fileName><dest_path>";
        error_Log("More agruments in upload_file command!!");
        return;
    }

    string group_id = request[1];
    string file_name = request[2];

    if(!is_user_login(client_socket)){
        output = "ERROR_AT_DOWNLOAD: You can't dowload the file without login in the network!!!";
        error_Log("attempt download file of the group without doing login!!");
        return ;
    }


    if(!is_group_exist(group_id)){
        output = "ERROR_AT_DOWNLOAD: This group id  is not exist!!";
        error_Log("Group is not exist!! while downloadingthe file!!");
        return;
    }

    int group_status = is_user_part_of_group(client_socket , group_id);

    if(group_status != 2){
        output = "ERROR_AT_DOWNLOAD: You are not part of this group!!";
        error_Log("User is not part of this group so it can't download from this group");
        return;
    }

    string ip_port = ip_port_of_user(client_socket);

    auto group_it = group_map.find(group_id);

    unordered_map<string , Files> file_map = group_it->second.files_of_group;

    auto file_it = file_map.find(file_name);

    if(file_it == file_map.end()){
        error_Log("ERROR_AT_DOWNLOAD: File not Found!!");
        output = "File not  found!!";
        return ;
    }

    string file_ip_port = "";

    file_ip_port += file_it->second.SHA_hash;
    file_ip_port += " ";
    file_ip_port += to_string(file_it->second.size_of_file);
    file_ip_port += " ";

    unordered_set<string>details =  file_it->second.ip_port;

    for(auto data: details){
        file_ip_port += data;
        file_ip_port += " ";
    }

    output = file_ip_port;

    success_Log("file downloaded deatils delivered!!!");
}
