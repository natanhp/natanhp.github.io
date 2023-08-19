#include "util.h"

std::string Util::ReadHTMLFile(const std::string& path) {

    std::string content;

    std::ifstream inputFile("resources/pages/" + path);

    if (inputFile.is_open()) {
         content.assign((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

         inputFile.close();
    } else {
            content = "<p>Resource not found</p>";
    }

    return content;
}
