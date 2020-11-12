#ifndef __FileWalk_H__
#define __FileWalk_H__
//******************************************************************************
#include <dirent.h>
#include <string>
#include <stack>
#include "path.h"
//******************************************************************************
class CFileWalkIterator;

struct CDIRContext {
    DIR* d;
    std::string path;
    unsigned int deep;
};

class CFileItem {
public:
    bool operator!=(const CFileItem& it) const {
        return parent != it.parent || filename != it.filename;
    }

    bool operator==(const CFileItem& it) const {
        return parent == it.parent && filename == it.filename;
    }

public:
    std::string getAbsolutePath() const {
        return CPath::join(parent, filename);
    }

public:
    bool isDIR{};
    std::string parent;
    std::string filename;
};

class CFileWalkIterator {
public:
    CFileWalkIterator() {
        mDeep = 0;
    }

    explicit CFileWalkIterator(const std::string& path, unsigned int deep) {
        mDeep = deep;
        auto d = opendir(path.c_str());

        if (d) {
            mDIRStack.push({d, path, 0});
            next();
        }
    }

public:
    bool operator!=(const CFileWalkIterator& it) const {
        return mItem != it.mItem;
    }

    bool operator==(const CFileWalkIterator& it) const {
        return mItem == it.mItem;
    }

public:
    void next() {
        mItem = CFileItem();

        if (mDIRStack.empty())
            return;

        do {
            auto context = mDIRStack.top();
            auto entry = readdir(context.d);

            if (entry) {
                std::string parent = context.path;
                std::string filename = entry->d_name;

                if (filename == "." || filename == "..")
                    continue;

                bool isDIR = entry->d_type == DT_DIR;

                if (isDIR && mDeep > context.deep + 1) {
                    auto subPath = CPath::join(parent, filename);
                    auto subDIR = opendir(subPath.c_str());

                    if (subDIR)
                        mDIRStack.push({subDIR, subPath, context.deep + 1});
                }

                mItem.isDIR = isDIR;
                mItem.parent = parent;
                mItem.filename = filename;

                break;
            }

            mDIRStack.pop();
            closedir(context.d);
        } while (!mDIRStack.empty());
    }

public:
    CFileWalkIterator& operator++()  {
        next();
        return *this;
    }

public:
    CFileItem *operator->() {
        return &mItem;
    }

    CFileItem &operator*() {
        return mItem;
    }

private:
    CFileItem mItem;
    unsigned int mDeep;
    std::stack<CDIRContext> mDIRStack;
};

class CFileWalk {
    typedef CFileWalkIterator iterator;
public:
    explicit CFileWalk(const char *path, unsigned int deep = 1) {
        mPath = path;
        mDeep = deep;
    }

public:
    iterator begin() {
        return CFileWalk::iterator(mPath, mDeep);
    }

    iterator end() {
        return CFileWalk::iterator();
    }

private:
    std::string mPath;
    unsigned int mDeep;
};
//******************************************************************************
#endif
