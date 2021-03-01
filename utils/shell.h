#ifndef __Shell_H__
#define __Shell_H__
//******************************************************************************
#include <list>
#include <string>
#include <wordexp.h>
//******************************************************************************
class CShellAPI {
public:
    static bool expansion(const std::string &str, std::list<std::string> &words) {
        wordexp_t p = {};

        if (wordexp(str.c_str(), &p, WRDE_NOCMD) != 0)
            return false;

        for (int i = 0; i < p.we_wordc; i++)
            words.emplace_back(p.we_wordv[i]);

        wordfree(&p);

        return true;
    }
};
//******************************************************************************
#endif