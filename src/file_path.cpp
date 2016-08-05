#include "file_path.h"

FilePath::FilePath(const std::string& path) : _path(path) {}

const char* FilePath::str() {
    return _path.c_str();
}

TextFile::TextFile(const FilePath& path) : _path(path), _file(NULL) {}

TextFile::~TextFile() {
    close();
}

void TextFile::close() {
    if(is_open()){
        fclose(_file);
        _file = NULL;
    }
}

std::string TextFile::read_line() {
    if (!is_open()) {
        return "";
    }
    std::string result;
    char p;
    while (fread(&p, 1, 1, _file)) {
        result += p;
        if (p == '\n') {
            break;
        }
    }
    return result;
}

bool TextFile::is_open() const {
    return _file != NULL;
}

bool TextFile::open() {
    _file = fopen(_path.str(), "r");
    return is_open();
}
