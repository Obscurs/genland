//
// Created by arnito on 25/04/17.
//

#ifndef GENLAND_FUNCTIONS_H
#define GENLAND_FUNCTIONS_H
#include <dirent.h>
#include <cstring>

inline bool exists_file (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
int DeleteDirectory(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                DeleteDirectory(path);
            }

            printf("(not really) Deleting: %s\n", path);
            remove(path);

        }

    }
    closedir(dir);
    remove(dirname);
    printf("(not really) Deleting: %s\n", dirname);

    return 1;
}
#endif //GENLAND_FUNCTIONS_H

void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const string file_name = file_data.cFileName;
        const string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;


        out.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    DIR *dir;
    class dirent *ent;
    class stat st;
    const char * c = directory.c_str();
    dir = opendir(c);
    while ((ent = readdir(dir)) != NULL) {

        const std::string file_name = ent->d_name;
        const std::string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        //const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        //if (is_directory)
        //    continue;
        out.push_back(full_file_name);
    }
    closedir(dir);
#endif
    std::sort (out.begin(), out.end());
} // GetFilesInDirectory
