#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ApiUtils/tools/httplib.h>

enum HttpRequestType {
    HTTP_REQUEST_SEND = 0,  //发送文件到服务端
    HTTP_REQUEST_REQUEST    //请求服务端的文件
};

class HttpClientHelper {
   private:
    const std::string host;
    const int host_port;
    httplib::Client client;

   private:
    httplib::MultipartFormDataItems makeRequestPack(HttpRequestType type, int msgid, const std::string& content, const std::string& filename);
    void parseReturnPack(const httplib::Result& res, std::string& msgid, std::string& content);

   public:
    HttpClientHelper(const std::string _host,const int _host_port);
    ~HttpClientHelper();
    bool sendFileToServer(int& msgid, std::string filepath);
    bool askFileFromServer(int msgid, std::string& filename);
};

inline HttpClientHelper::HttpClientHelper(const std::string _host, const int _host_port)
    : host(_host), host_port(_host_port), client(host,host_port) {
}

inline HttpClientHelper::~HttpClientHelper() {
}

inline httplib::MultipartFormDataItems HttpClientHelper::makeRequestPack(HttpRequestType type, int msgid, const std::string& content, const std::string& filename) {
    std::string type_str = type == HttpRequestType::HTTP_REQUEST_SEND ? "SEND" : "REQUEST";
    httplib::MultipartFormDataItems items = {
        {"request", type_str, "", "application/octet-stream"},
        {"msgid", std::to_string(msgid), "", "application/octet-stream"},
        {"file", content, "", "application/octet-stream"},
        {"filename", filename, "", "application/octet-stream"}};
    return items;
}

inline void HttpClientHelper::parseReturnPack(const httplib::Result& res, std::string& msgid, std::string& content) {
    //如果msgid全部为空格，就说明失败
    //小于256按照协议必然不对
    if (res->body.size() < 256) {
        std::cout << "min than 256" << std::endl;
        content = std::string();
        return;
    }

    // msgid获取, 需要返回去除左右两边空格的数据
    msgid = res->body.substr(0, 256);
    if (msgid.empty() == false)
        msgid.erase(0, msgid.find_first_not_of(" "));
    if (msgid.empty() == false)
        msgid.erase(msgid.find_last_not_of(" ") + 1);

    //尾部的不定长string获取
    content = res->body.substr(256);
    return;
}

// msgid为服务器存储的文件的id，文件id. 错误情况下不改变msgid
inline bool HttpClientHelper::sendFileToServer(int& msgid, std::string filepath) {
    std::ifstream ifs(filepath, std::ios::binary);

    if (ifs.is_open() == false)
        return false;

    int slashindex = filepath.find_last_of('/');
    if (slashindex == std::string::npos)
        slashindex = -1;

    //准备的填充数据,0. msgid已经准备就绪
    std::string filename = filepath.substr(slashindex + 1);  // 1. filename
    std::ostringstream oss;
    oss << ifs.rdbuf();
    std::string content = oss.str();  // 2. content
    ifs.close();
    HttpRequestType type = HTTP_REQUEST_SEND;  // 3. request-type

    //拼包
    httplib::MultipartFormDataItems items = makeRequestPack(type, msgid, content, filename);

    //获取结果
    auto res = client.Post("/post", items);
    std::string file_msgid;
    parseReturnPack(res, file_msgid, content);

    // filename 上传成功，服务器将msgid返回。否则置为空
    if (file_msgid.empty() == true) {
        return false;
    } else {
        msgid = stoi(file_msgid);
        return true;
    }
}

// file_download_dir : 文件下载的文件夹，返回实际安装的文件路径. msgid为对方发过来的文件id
inline bool HttpClientHelper::askFileFromServer(int msgid, std::string& file_download_dir) {
    //准备数据 msgid已经准备就绪
    HttpRequestType type = HttpRequestType::HTTP_REQUEST_REQUEST;
    //文件名和文件体空着就行
    std::string file_send = "", file_name_send = "";

    //打包
    httplib::MultipartFormDataItems items = makeRequestPack(HttpRequestType::HTTP_REQUEST_REQUEST, msgid, file_send, file_name_send);

    //获取结果
    auto res = client.Post("/post", items);
    //文件名称和文件内容
    std::string filename, content;
    parseReturnPack(res, filename, content);

    //按照filename来判断成功没有
    if (filename.empty() == true) {
        return false;
    } else {
        //分开文件后缀和前缀，方便文件名重复的时候生成(1).cpp这种
        int dotindex = filename.find_last_of('.');
        std::string filename_prefix, filename_suffix;
        if (dotindex == std::string::npos) {
            filename_prefix = filename;
        } else {
            filename_prefix = filename.substr(0, dotindex);
            filename_suffix = filename.substr(dotindex);
        }

        //判断用户路径加没加/
        file_download_dir = *file_download_dir.rbegin() == '/' ? file_download_dir : file_download_dir + '/';
        //绝对路径生成
        filename = file_download_dir + filename;
        std::ifstream ifs(filename);
//        if (ifs.good() == true) {
//            ifs.close();
//            int test_file_count = 0;
//            bool is_exist = true;
////            while (is_exist == true) {
////                test_file_count++;
////                std::ifstream ifss(file_download_dir + filename_prefix + "(" + std::to_string(test_file_count) + ")" + filename_suffix);
////                is_exist = ifss.good();
////            }
//            std::string final_file_path = file_download_dir + filename_prefix + "(" + std::to_string(test_file_count) + ")" + filename_suffix;
//            //确定文件不存在，写入
//            std::ofstream ofs(final_file_path, std::ios::binary);
//            ofs << content;
//            ofs.close();
//            file_download_dir = final_file_path;
//        } else {
            ifs.close();
            std::string final_file_path = filename;
            //确定文件不存在，写入
            std::ofstream ofs(final_file_path, std::ios::binary);
            ofs << content;
            ofs.close();
            file_download_dir = final_file_path;
//        }
    }
    return true;
}

#endif  // HTTP_UTILS_H
