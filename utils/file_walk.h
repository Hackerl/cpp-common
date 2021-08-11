#ifndef FILE_WALK_H
#define FILE_WALK_H

#include "path.h"
#include <dirent.h>
#include <string>
#include <stack>

class CFileEntry {
public:
    bool operator!=(const CFileEntry &entry) const {
        return parent != entry.parent || filename != entry.filename;
    }

    bool operator==(const CFileEntry& entry) const {
        return parent == entry.parent && filename == entry.filename;
    }

public:
    std::string getAbsolutePath() const {
        return CPath::join(parent, filename);
    }

public:
    void reset() {
        parent.clear();
        filename.clear();
    }

public:
    bool isDirectory{};

public:
    std::string parent;
    std::string filename;
};

struct CDIRContext {
    DIR* dir;
    std::string path;
    unsigned long deep;
};

class CFileIterator {
public:
    CFileIterator() = default;

    explicit CFileIterator(const std::string &directory, unsigned long deep) {
        mDeep = deep;
        DIR *dir = opendir(directory.c_str());

        if (dir) {
            mContexts.push({dir, CPath::getAbsolutePath(directory.c_str()), 0});
            this->operator++();
        }
    }

    CFileIterator(CFileIterator &&iterator) noexcept {
        mDeep = iterator.mDeep;
        mEntry = iterator.mEntry;
        mContexts = std::move(iterator.mContexts);
    }

    CFileIterator(CFileIterator &iterator) noexcept {
        mDeep = iterator.mDeep;
        mEntry = iterator.mEntry;
        mContexts = std::move(iterator.mContexts);
    }

    ~CFileIterator() {
        while(!mContexts.empty()) {
            closedir(mContexts.top().dir);
            mContexts.pop();
        }
    }

public:
    CFileIterator &operator++() {
        mEntry.reset();

        while (!mContexts.empty()) {
            CDIRContext &context = mContexts.top();
            dirent *entry = readdir(context.dir);

            if (entry) {
                std::string parent = context.path;
                std::string filename = entry->d_name;

                if (filename == "." || filename == "..")
                    continue;

                bool isDIR = entry->d_type == DT_DIR;

                if (isDIR && mDeep > context.deep + 1) {
                    std::string subPath = CPath::join(parent, filename);
                    DIR *subDIR = opendir(subPath.c_str());

                    if (subDIR) {
                        mContexts.push({subDIR, subPath, context.deep + 1});
                    }
                }

                mEntry = {isDIR, parent, filename};

                break;
            }

            closedir(context.dir);
            mContexts.pop();
        }

        return *this;
    }

public:
    bool operator!=(const CFileIterator& it) const {
        return mEntry != it.mEntry;
    }

    bool operator==(const CFileIterator& it) const {
        return mEntry == it.mEntry;
    }

    CFileEntry *operator->() {
        return &mEntry;
    }

    CFileEntry &operator*() {
        return mEntry;
    }

private:
    unsigned long mDeep{};

private:
    CFileEntry mEntry;
    std::stack<CDIRContext> mContexts;
};

class CFileWalker {
public:
    explicit CFileWalker(const std::string &directory, unsigned long deep = 1) {
        mDirectory = directory;
        mDeep = deep;
    }

public:
    CFileIterator begin() {
        return CFileIterator(mDirectory, mDeep);
    }

    CFileIterator end() {
        return mEndIterator;
    }

private:
    CFileIterator mEndIterator;

private:
    unsigned long mDeep;
    std::string mDirectory;
};

#endif
