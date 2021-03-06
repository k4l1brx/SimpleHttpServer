#include "HttpGenerator.h"

map<int, string> statusCode = {
    {200, "OK"},
    {404, "Not Found"},
};

string HttpGenerator::header(int code, size_t contentSize, string type, bool chunked) {
    string ans = "HTTP/1.1 " + to_string(code) + ' ' + statusCode[code] + "\r\n";
    if (code == 404) {
        ans.append("Location: /404.html\r\n");
    }
    /* ans.append("X-Content-Type-Options: nosniff\n"); */
    ans.append("X-Content-Type-Options: nosniff\r\n");
    ans.append("Content-Type: " + type + "\r\n");
    if (! chunked)
        ans.append("Content-Length: " + to_string(contentSize) + "\r\n");
    else ans.append("Transfer-Encoding: chunked\r\n");
    ans.append("\r\n");
    return ans;
}

string HttpGenerator::htmlString(int code, string fileDir, string type) {
    ifstream ifs;
    ifs.open(fileDir);
    string ans;
    string htmlContent;
    if (! ifs) {
        code = 404;
        ifs.open("./pages/404.html", ios::in);
    }
    ifs.seekg(0, ifs.end);
    htmlContent.resize(ifs.tellg());
    ifs.seekg(0, ifs.beg);
    ifs.read(&htmlContent[0], htmlContent.size());
    ifs.close();
    ans = header(code, htmlContent.size(), type) + htmlContent;
    return ans;
}

string HttpGenerator::redirection(string location, string fileDir) {
    ifstream ifs(fileDir);
    string tmp, ans;
    string htmlContent;
    while (getline(ifs, tmp)) {
        htmlContent += tmp + '\n';
    }
    ifs.close();
    return "HTTP/1.1 303 See Other\nLocation: " + location + '\n' + htmlContent + '\n';
}
