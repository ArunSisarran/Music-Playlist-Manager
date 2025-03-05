#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class DotEnv {
private:
    std::unordered_map<std::string, std::string> env_vars;

public:
    DotEnv(const std::string& path = ".env") {
        std::ifstream file(path);
        if (!file.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#') continue;

            // Split on first '='
            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) continue;

            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Remove quotes if present
            if ((value.front() == '"' && value.back() == '"') ||
                (value.front() == '\'' && value.back() == '\'')) {
                value = value.substr(1, value.length() - 2);
            }

            env_vars[key] = value;
        }
    }

    std::string get(const std::string& key, const std::string& defaultValue = "") {
        // First check if it's in our loaded env vars
        auto it = env_vars.find(key);
        if (it != env_vars.end()) {
            return it->second;
        }

        // Then check system environment variables
        const char* systemEnv = std::getenv(key.c_str());
        if (systemEnv) {
            return std::string(systemEnv);
        }

        // Return default if nothing found
        return defaultValue;
    }
};
