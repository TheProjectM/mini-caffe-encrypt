//
// Created by sreal on 7/29/19.
//

#ifndef CAFFE_ENCRYPT_H
#define CAFFE_ENCRYPT_H

#include <string>
#include <google/protobuf/text_format.h>
#include "../src/proto/caffe.pb.h"
#include "net.hpp"


namespace caffe {
    class NetParameter;

    class Net;

    class encrypt_tool {
    public:
        static void decrypt_proto(const std::string &file_name, const std::string &passwd, std::shared_ptr<caffe::Net> &net);

        static void decrypt_binary(const std::string &file_name, const std::string &passwd, std::shared_ptr<caffe::Net> &net);


        static void encrypt_proto(const std::string &file_name, const std::string &passwd, const std::string &output_file);

        static void encrypt_binary(const std::string &file_name, const std::string &passwd, const std::string &output_file);

    private:
        static void decrypt_proto(const std::string &file_name, const std::string &passwd, caffe::NetParameter &params);

        static void decrypt_binary(const std::string &file_name, const std::string &passwd, caffe::NetParameter &params);

    };

}

#endif //CAFFE_ENCRYPT_H
