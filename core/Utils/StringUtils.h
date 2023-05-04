#pragma once
#include <string>
#include <sstream>

namespace HexEngine {
    class StringUtils {
        public:
        template<typename T>
        static std::string ToString(T cont) {
            std::stringstream ss;
            ss << cont;
            return ss.str();
        }
        template<typename To, typename From>
        static To To(From cont) {
            std::stringstream ss;
            ss << cont;
            To ret;
            ss >> ret;
            return ret;
        }
        static std::string& Replace(std::string& cont, std::string_view from, std::string_view to) {
            for (int pos = 0; pos < cont.length(); pos += to.length()) {
                int rep = cont.find(from, pos);
                if (rep == std::string::npos) {
                    return cont;
                }
                cont.replace(rep, from.length(), to);
            }
            return cont;
        }
        static std::string Path(std::string_view orig) {
            std::string ret = (std::string)orig;
#if defined(_MSC_VER)
            StringUtils::Replace(ret, "/", "\\");
#endif
            return ret;
        }
    };
}
