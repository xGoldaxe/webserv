#pragma once

#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

#define IS_FILE_FOLDER 0
#define IS_FILE_NOT_FOLDER 1
#define IS_FILE_ERROR -1 // doesnt exist

bool file_exist(const std::string &name);
int is_file(std::string dir);
bool usable_file(const std::string &name);
bool file_readable(const std::string &name);
