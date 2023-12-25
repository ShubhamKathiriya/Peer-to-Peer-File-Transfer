#include "all_header.h"

using namespace std;

string calc_SHA_hash_of_file(const char* file_path) {
    // Replace 'your_file_path' with the path to the file you want to hash

    int file_open_fd = open(file_path, O_RDONLY);

    if (file_open_fd < 0) {
        
        cerr << "Erro in opening the file for hashing";
        error_Log("Error occured in opeing the file for calculating the  SHA hash of the file!!!");
        
        return "";
    }


    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    char buffer[HASH_BUFFER_SIZE];
    ssize_t bytes_read;


    while ((bytes_read = read(file_open_fd, buffer, sizeof(buffer))) > 0) 
        SHA256_Update(&sha256, buffer, bytes_read);
    

    int file_close_fd = close(file_open_fd);

    if(file_close_fd < 0){
        cout << "Error!! in closing the file for calculating the hash of the file";
        error_Log("Error occurred in closing the file for calculating the hash of the file ");
        return "";
    }


    uc hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    char hex_hash[2 * SHA256_DIGEST_LENGTH + 1];

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
        sprintf(hex_hash + 2 * i, "%02x", hash[i]);
    
    string str_file_name = string(file_path);

    success_Log( str_file_name + "   calculated hash successfully!!!!");


    return hex_hash;

}


pair <int, string> custom_piece_wise_selection_algorithm(vector <unordered_set <string> > &chunkStatusOfOthers, vector <pair<int,int> > &countOfClientForEachChunk){


    sort(countOfClientForEachChunk.begin(), countOfClientForEachChunk.end());

    auto it = countOfClientForEachChunk.begin();

    int chunkNo = it->second;
    int totalSeeders = it->first;

    auto r = rand() % totalSeeders;

    auto it3 = chunkStatusOfOthers[chunkNo].size();

    auto it2 = chunkStatusOfOthers[chunkNo].begin();
    advance(it2, r);

    string ipPortToReturn = *it2;

    countOfClientForEachChunk.erase(it);
    return {chunkNo, ipPortToReturn};

}

