#ifndef __JSONHelper_H__
#define __JSONHelper_H__
//******************************************************************************
#include <fstream>
#include <jsoncpp/json/json.h>

class CJSONHelper {
public:
    static bool hasArray(const Json::Value &jv, const char *name) {
        if (!jv.isObject())
            return false;

        if (!jv.isMember(name))
            return false;

        return jv[name].isArray();
    }

    static bool hasObject(const Json::Value &jv, const char *name) {
        if (!jv.isObject())
            return false;

        if (!jv.isMember(name))
            return false;

        return jv[name].isObject();
    }

    static std::string getString(const Json::Value &jv, const char *name, const char *default_ = "") {
        if (!jv.isObject())
            return default_;

        if (!jv.isMember(name))
            return default_;

        if (jv[name].isString())
            return jv[name].asString();

        if (jv[name].isInt()) {
            char data[32] = {};
            snprintf(data, 32, "%d", jv[name].asInt());
            return data;
        }

        return default_;
    }

    static bool getBool(const Json::Value &jv, const char *name, bool default_ = false) {
        if (!jv.isObject())
            return default_;

        if (!jv.isMember(name))
            return default_;

        return jv[name].asBool();
    }

    static int getInt(const Json::Value &jv, const char *name, int default_ = 0) {
        if (!jv.isObject())
            return default_;

        if (!jv.isMember(name))
            return default_;

        if (!jv[name].isInt())
            return default_;

        return jv[name].asInt();
    }

    static unsigned int getUInt(const Json::Value &jv, const char *name, int default_ = 0) {
        if (!jv.isObject())
            return default_;

        if (!jv.isMember(name))
            return default_;

        if (!jv[name].isUInt())
            return default_;

        return jv[name].asUInt();
    }

    static bool parseJsonFromData(const char* data, size_t size, Json::Value& root, std::string& error) {
        Json::CharReaderBuilder builder;
        Json::CharReader* reader = builder.newCharReader();

        bool res = reader->parse(data, data + size, &root, &error);

        delete reader;

        return res;
    }

    static bool parseJsonFromFile(const char* path, Json::Value& root, std::string& error) {
        std::ifstream in(path);

        if (!in.is_open())
            return false;

        return Json::parseFromStream(Json::CharReaderBuilder(), in, &root, &error);
    }
};
//******************************************************************************
#endif