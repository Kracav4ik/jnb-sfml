#ifndef JNB_SFML_FILE_PATH_H
#define JNB_SFML_FILE_PATH_H

#include <string>

struct FilePath {
    std::string _path;

    FilePath(const std::string& path);

    const char* str();
};

struct TextFile {
    FilePath _path;
    FILE* _file;

    TextFile(const FilePath& path);

    bool open();

    bool is_open() const;

    std::string read_line();

    void close();

    ~TextFile();
};

#endif //JNB_SFML_FILE_PATH_H
