//
// Created by sreal on 7/29/19.
//

#include "caffe/encrypt_tool.hpp"
#include <fstream>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <glog/logging.h>
#include <fcntl.h>
#include <sstream>

using google::protobuf::io::FileInputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::Message;

void caffe::encrypt_tool::encrypt_binary(const std::string &file_name, const std::string &passwd,
                                         const std::string &output_file) {
    if (passwd.empty()) {
        fprintf(stderr, "no empty password!!! \n");
        return;
    }

    std::ifstream in_file(file_name, std::ios::binary);

    if (!in_file) {
        fprintf(stderr, "can't read file: %s \n", file_name.c_str());
        return;
    }

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
    fprintf(stdout, "encrypt binary file size: %d bytes\n", int(size));
    in_file.seekg(0, std::ios::beg);

    std::vector<unsigned char> engine_data(size);
    in_file.read((char *) engine_data.data(), size);
    in_file.close();

    std::ofstream out_proto(output_file, std::ios::out | std::ios::binary);
    if (!out_proto.is_open()) {
        fprintf(stderr, "fail to open file to write: %s\n", output_file.c_str());
        return;
    }

    //encode
    if (not passwd.empty()) {
        int password_len = passwd.size();
        for (int i = 0; i < engine_data.size(); ++i) {
            unsigned char before_encode = engine_data[i];
            unsigned char encode_word = (before_encode) ^(unsigned char) passwd[i % password_len];
            out_proto.put(encode_word);
            //outfile.write((char*)&encode_word, sizeof(char));
        }
    }

    out_proto.close();
}

void caffe::encrypt_tool::encrypt_proto(const std::string &file_name, const std::string &passwd,
                                        const std::string &output_file) {
    if (passwd.empty()) {
        fprintf(stderr, "no empty password!!! \n");
        return;
    }


    std::ifstream in_file(file_name);

    if (!in_file) {
        fprintf(stderr, "can't read file: %s \n", file_name.c_str());
        return;
    }

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
    fprintf(stdout, "encrypt proto file size: %d bytes\n", int(size));
    in_file.seekg(0, std::ios::beg);

    std::vector<unsigned char> engine_data(size);
    in_file.read((char *) engine_data.data(), size);
    in_file.close();

    std::ofstream out_file(output_file, std::ios::binary);

    //encode
    if (not passwd.empty()) {
        int password_len = passwd.size();
        for (int i = 0; i < engine_data.size(); ++i) {
            unsigned char before_encode = engine_data[i];
            unsigned char encode_word = (before_encode) ^(unsigned char) passwd[i % password_len];
            out_file.put(encode_word);
            //outfile.write((char*)&encode_word, sizeof(char));
        }
    }

    out_file.close();
}


void caffe::encrypt_tool::decrypt_binary(const std::string &file_name, const std::string &passwd, caffe::NetParameter &params) {

    std::ifstream in_file(file_name, std::ios::binary);

    if (!in_file) {
        fprintf(stderr, "can't read file: %s \n", file_name.c_str());
        return;
    }

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
//    fprintf(stdout, "decrypt binary file size: %d bytes\n", int(size));
    in_file.seekg(0, std::ios::beg);

    std::string engine_data(size, 'c');
    in_file.read((char *) engine_data.data(), size);
    in_file.close();

    int pwd_len = passwd.size();

    if (pwd_len > 0) {
        //decrypt
        for (size_t i = 0; i < engine_data.size(); ++i) {
            engine_data[i] = (((unsigned char) engine_data[i]) ^ ((unsigned char) passwd[i % pwd_len]));
        }
    } else {
        fprintf(stdout, "no password, no decryption, load binary file directly.");
    }

    std::istringstream net_model(engine_data);
    auto *model_input = new google::protobuf::io::IstreamInputStream((std::istream *) (&net_model));
    auto *coded_input_model = new google::protobuf::io::CodedInputStream(model_input);
    coded_input_model->SetTotalBytesLimit(INT_MAX, 536870912);
    params.ParseFromCodedStream(coded_input_model);

    delete model_input;
    delete coded_input_model;
}

void caffe::encrypt_tool::decrypt_proto(const std::string &file_name, const std::string &passwd, caffe::NetParameter &params) {
    std::ifstream in_file(file_name, std::ios::binary);
    if (!in_file)
        return;

    std::streampos begin, end;
    begin = in_file.tellg();
    in_file.seekg(0, std::ios::end);
    end = in_file.tellg();
    std::size_t size = end - begin;
//    fprintf(stdout, "decrypt proto file size: %d bytes\n", int(size));
    in_file.seekg(0, std::ios::beg);

    std::string engine_data(size, 'c');
    in_file.read((char *) engine_data.data(), size);
    in_file.close();

    int pwd_len = passwd.size();

    if (pwd_len > 0) {
        //decrypt
        for (size_t i = 0; i < engine_data.size(); ++i) {
            engine_data[i] = (((unsigned char) engine_data[i]) ^ ((unsigned char) passwd[i % pwd_len]));
        }
    } else {
        fprintf(stdout, "no password, no decryption, load proto file directly.");
    }

    std::istringstream net_proto(engine_data);

    auto *proto_input = new google::protobuf::io::IstreamInputStream((std::istream *) (&net_proto));
    google::protobuf::io::ZeroCopyInputStream *coded_input_proto = proto_input;
    google::protobuf::TextFormat::Parse(coded_input_proto, &params);

    delete proto_input;
}

void caffe::encrypt_tool::decrypt_proto(const std::string &file_name, const std::string &passwd, std::shared_ptr<caffe::Net> &net) {
    NetParameter proto;
    decrypt_proto(file_name, passwd, proto);
    net.reset(new caffe::Net(proto));
}

void caffe::encrypt_tool::decrypt_binary(const std::string &file_name, const std::string &passwd, std::shared_ptr<caffe::Net> &net) {
    NetParameter parameter;
    decrypt_binary(file_name, passwd, parameter);
    net->CopyTrainedLayersFrom(parameter);
}
