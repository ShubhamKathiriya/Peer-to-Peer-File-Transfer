#include "all_header.h"

User::User(string hash = "a" ){

    this->password = hash;
    this->currentLogin = false;
}

void User::print_user_map_value(){

    cout << "user password hash:   "<< password<<endl;
    cout << "user current login status:   " << boolalpha << currentLogin << endl;
    cout << "user ip:   "  << ip <<endl;
    cout << "user port:   " << port << endl;
    cout << "user socket:   " << socket << endl;
    cout << "group  id :    " << group_id << endl;

    unordered_set<string> user_files = file_name;

    cout<<"Files of user:-    "<<endl;
    for(auto it : user_files){
        cout << it <<endl;
    }

    cout<<"========"<<endl;
}

void print_user_meta_data(){

    for(auto user_it : user_meta_data){

        cout << "User id:   " << user_it.first <<endl;
        user_it.second.print_user_map_value();
        cout<<"======"<<endl;
    }
    cout<<"=================================================XXXXXXXXXXXXXXXXXx================================="<<endl;
}

void print_socket_user_map(){

    cout<<socket_user.size()<<endl;;

    for(auto socket : socket_user){
        cout<< " socket :- " << socket.first<<"      =>      ";
        cout<< " user id :-" << socket.second <<endl;
    }

}


bool is_user_login(int socket_id){

    string user_id = socket_user[socket_id];

    User user_data = user_meta_data[user_id];

    return user_data.currentLogin;

}


bool is_user_registered (string user_id){

    // string user_id = socket_user[socket_id];

    if(user_meta_data.find(user_id) != user_meta_data.end())
        return true;
    
    return false;

}

bool is_correct_userId (int socket_id, string user_id){

    auto socket_it = socket_user.find(socket_id);

    if(user_id == socket_it->second)
        return true;

    return false;
}

string ip_port_of_user(int socket_id){

    auto socket_it = socket_user.find(socket_id);

    string user_id = socket_it->second;

    auto user_it = user_meta_data.find(user_id);

    string ip = user_it->second.ip;
    string port  = user_it->second.port;

    return (ip + ":" + port);
}

Group:: Group(string g_id ="-" , string user_admin = "-"){

    this->group_id = g_id;
    this->admin =  user_admin;

}

void Group::print_group_details(){

    cout<<"===Group id: " <<group_id<<"========="<<endl;

    cout << "Admin:-  "<< admin<<endl;

    cout << "Pending Joining Request: "<<endl;
    cout<<joining_requests.size()<<endl;
    for(auto it: joining_requests)
        cout<<it<<endl;
    
    cout<<"----------------------"<<endl;


    cout << "Group member" << endl;
    cout<<participants.size()<<endl;
    for(auto it : participants)
        cout << it<<endl;

    cout<<"----------------------"<<endl;
    
    cout<< "Files:   "<<endl;

    for(auto it2 : files_of_group){
        cout <<"File name:  " << it2.first <<"    =>    "<<endl ;

        unordered_set<string> ipPOrt = it2.second.ip_port;

        for(auto it3 : ipPOrt)
            cout<< "ip POrt =>  "<<it3<<endl;

        cout<<"file Hash :- "<< it2.second.SHA_hash<<endl;

        cout<<"file size :-  "<< it2.second.size_of_file << endl;

    }


}


bool is_group_exist(string gid){

    if(group_map.find(gid) != group_map.end())
        return true;
    
    return false;
}

/*
    0 -> is not part of any group
    1 -> is part of another of group but not that agrument  group id
    2 - > is part of agrument group id
*/

int is_user_part_of_group(int socket_id, string group_id){

    auto socket_it = socket_user.find(socket_id);
    string user_id = socket_it->second;

    auto user_it = user_meta_data.find(user_id);
    string user_group_id = user_it->second.group_id;


    if(user_group_id.size() == 0)
        return 0;
    
    else if(user_group_id != group_id)
        return 1;
    
    return 2;
    
}

/*
    0 -> is not part of any group
    1 -> is part of another of group but not that agrument  group id
    2 - > is part of agrument group id
*/

int is_other_user_part_of_group(string user_id, string group_id){


    auto user_it = user_meta_data.find(user_id);
    string user_group_id = user_it->second.group_id;


    if(user_group_id.size() == 0)
        return 0;
    
    else if(user_group_id != group_id)
        return 1;
    
    return 2;
    
}

bool is_admin(int socket_id , string group_id){

    auto socket_it = socket_user.find(socket_id);
    string user_id = socket_it->second;

    auto group_it = group_map.find(group_id);
    string admin_id = group_it->second.admin;

    if(user_id == admin_id)
        return true;
    
    return false;

}

int group_size(string group_id){

    auto group_it = group_map.find(group_id);

    return group_it->second.participants.size();
}

bool is_group_empty(string group_id){

    if(group_size(group_id) == 0)
        return true;

    return false;
}



Files:: Files(string hash ="a", int size=0, string file_name="z", unordered_set<string>ip_port={}){

    this->SHA_hash = hash;
    this->size_of_file = size;
    this->file_name = file_name;
    this->ip_port =ip_port;
}


