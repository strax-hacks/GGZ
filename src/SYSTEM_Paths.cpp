#include "SYSTEM_Paths.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>

#ifndef OPENGGS_DATADIR
#define OPENGGS_DATADIR "/usr/share/openggs/data"
#endif

static bool FileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

static bool DirExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool EnsureDirectoryExists(const std::string& path) {
    if (path.empty() || DirExists(path)) {
        return true;
    }
    
    // Create parent directories recursively
    size_t pos = 0;
    while ((pos = path.find('/', pos + 1)) != std::string::npos) {
        std::string sub = path.substr(0, pos);
        if (!sub.empty() && !DirExists(sub)) {
            mkdir(sub.c_str(), 0755);
        }
    }
    return (mkdir(path.c_str(), 0755) == 0 || DirExists(path));
}

std::string GetConfigDir() {
    const char* xdgConfig = getenv("XDG_CONFIG_HOME");
    std::string dir;
    if (xdgConfig && strlen(xdgConfig) > 0) {
        dir = std::string(xdgConfig) + "/openggs";
    } else {
        const char* home = getenv("HOME");
        if (home && strlen(home) > 0) {
            dir = std::string(home) + "/.config/openggs";
        } else {
            dir = "./.config/openggs";
        }
    }
    EnsureDirectoryExists(dir);
    return dir;
}

std::string GetConfigPath(const std::string& filename) {
    return GetConfigDir() + "/" + filename;
}

std::string GetDataDir() {
    const char* xdgData = getenv("XDG_DATA_HOME");
    std::string dir;
    if (xdgData && strlen(xdgData) > 0) {
        dir = std::string(xdgData) + "/openggs";
    } else {
        const char* home = getenv("HOME");
        if (home && strlen(home) > 0) {
            dir = std::string(home) + "/.local/share/openggs";
        } else {
            dir = "./.local/share/openggs";
        }
    }
    EnsureDirectoryExists(dir);
    EnsureDirectoryExists(dir + "/saves");
    return dir;
}

std::string GetSavePath(const std::string& filename) {
    return GetDataDir() + "/saves/" + filename;
}

std::string GetAssetPath(const std::string& relativePath) {
    // If the path is already valid as-is
    if (FileExists(relativePath) || DirExists(relativePath)) {
        return relativePath;
    }

    // Check prefixed with base/
    std::string baseRelative = "base/" + relativePath;
    if (FileExists(baseRelative) || DirExists(baseRelative)) {
        return baseRelative;
    }

    // Check parent directories (e.g., when running tests in build/ or build/desktop/)
    const std::vector<std::string> prefixes = {"../", "../../", "../../../", "../../../../"};
    for (const auto& p : prefixes) {
        if (FileExists(p + relativePath) || DirExists(p + relativePath)) {
            return p + relativePath;
        }
        if (FileExists(p + baseRelative) || DirExists(p + baseRelative)) {
            return p + baseRelative;
        }
    }

    // Check system data directory
    std::string sysData = std::string(OPENGGS_DATADIR) + "/" + relativePath;
    if (FileExists(sysData) || DirExists(sysData)) {
        return sysData;
    }

    // Check system data directory with base subpath
    std::string sysDataBase = std::string(OPENGGS_DATADIR) + "/base/" + relativePath;
    if (FileExists(sysDataBase) || DirExists(sysDataBase)) {
        return sysDataBase;
    }

    // Check if relativePath starts with "base/"
    if (relativePath.rfind("base/", 0) == 0) {
        std::string sub = relativePath.substr(5);
        for (const auto& p : prefixes) {
            if (FileExists(p + sub) || DirExists(p + sub)) {
                return p + sub;
            }
        }
        std::string sysSub = std::string(OPENGGS_DATADIR) + "/" + sub;
        if (FileExists(sysSub) || DirExists(sysSub)) {
            return sysSub;
        }
    }

    // Fallback to original path
    return relativePath;
}
