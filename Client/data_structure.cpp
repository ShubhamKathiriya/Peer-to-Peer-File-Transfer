#include "all_header.h"


Client_Files::Client_Files(string file_name, string file_path, string file_SHA_hash, int file_size){

    this->file_name = file_name;
    this->file_path = file_path;
    this->SHA_hash = file_SHA_hash;
    this->file_size_bytes = file_size;
    this->which_chunk_present = {};
    
}

void Client_Files::print_client_file_structure(){

    cout << "The name of file  : "  << file_name << "," << endl 
        << "SHA of the file   : "   << SHA_hash << endl
        << "The size of the files in bytes : "  << file_size_bytes << endl
        << "The path of file : "  << file_path << endl
        << "chunk records(0/1) : ";
    
    int chunk_nummber = 0;

    for(auto it: which_chunk_present)
        cout << chunk_nummber++ << ":" << boolalpha << it << " ";
    
    cout << endl;
    return;
}



void print_all_file_meta_data(){

    cout<<"==============ALL Files DATA==========================="<<endl<<endl;

    for(auto file_it : all_files_meta_data){

        cout<<"File name:-  "<<endl;

        cout <<file_it.first<<endl;
        
        file_it.second.print_client_file_structure();

        cout<<"-------------------------"<<endl;
    }

    cout<<"======================================================================"<<endl<<endl;

}