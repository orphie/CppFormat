#pragma once

#include <memory>   // for std::unique_ptr
#include <vector>   
class Config;
class FileDb {
public:
    FileDb();
    ~FileDb();
public:
    using Paragraph  = std::vector<std::string>;
    using Paragraphs = std::vector<Paragraph>;
    class FileDiff {
    public:
        std::string _fileName;
        Paragraphs  _parags;
    };
    void addFileDiff(const FileDiff&);
private:
    class FileDbImpl;
    std::unique_ptr<FileDbImpl> _pImpl;
};

