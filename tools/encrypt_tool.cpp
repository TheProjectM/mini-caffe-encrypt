//
// Created by sreal on 7/30/19.
//

#include "caffe/encrypt_tool.hpp"
#include <string>
#include <iostream>

bool has_suffix(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./caffe_encrypt_tool  {prototxt|caffemodel} {password} " << std::endl;
        return -1;
    }

    std::string ori_file = argv[1];
    std::string pwd = argv[2];
    std::string encrypted_file = ori_file;
    encrypted_file = encrypted_file.insert(ori_file.rfind('.'), "_encrypted");

    if (has_suffix(ori_file, ".prototxt")) {
        caffe::encrypt_tool::encrypt_proto(ori_file, pwd, encrypted_file);
    } else if (has_suffix(ori_file, ".caffemodel")) {
        caffe::encrypt_tool::encrypt_binary(ori_file, pwd, encrypted_file);
    } else {
        fprintf(stderr, "please provide a .prototxt or a .caffemodel ");
    }

    std::cout << "encrypted file write to: " << encrypted_file << std::endl;
    return 0;
}