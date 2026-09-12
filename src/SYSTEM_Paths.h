#ifndef SYSTEM_PATHS_H
#define SYSTEM_PATHS_H

#include <string>

// Resolves a relative asset path by searching:
// 1. Local ./base/<subpath> or ./<relativePath>
// 2. Install DATADIR (/usr/share/openggs/data/<subpath>)
// 3. Executable-relative data paths
std::string GetAssetPath(const std::string& relativePath);

// Returns XDG-compliant config directory (~/.config/openggs/)
// Creates the directory if it does not exist.
std::string GetConfigDir();

// Returns full path to a configuration file in config directory
std::string GetConfigPath(const std::string& filename);

// Returns XDG-compliant data/save directory (~/.local/share/openggs/)
// Creates the directory if it does not exist.
std::string GetDataDir();

// Returns full path to a save file in data directory
std::string GetSavePath(const std::string& filename);

// Utility to create directory recursively if not present
bool EnsureDirectoryExists(const std::string& path);

#endif // SYSTEM_PATHS_H
