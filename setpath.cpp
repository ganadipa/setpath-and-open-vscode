// C++ Program to get the current working directory
#include <filesystem>
#include <iostream>
#include <stdlib.h>
#include <unistd.h> 
#include <map>

using namespace std;

/**
 * the varpath.txt contains string like this
 * '''file
 * CPP_PATH=/home/username/Projects/cpp\n
 * 
 *
 * 
 * '''
*/

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: setpath '<var>=<path>'. \ne.g. setpath 'CPP_PATH=/home/username/Projects/cpp'\n";
        return 1;
    }

    // If not using sudo then print message
    if (getuid() != 0)
    {
        cout << "Permission denied: use sudo\n";
        return 1;
    }

    // OK get the var and the path
    string arg = argv[1];
    int pos = arg.find("=");
    string var = arg.substr(0, pos);
    string path = arg.substr(pos + 1, arg.length());

    // parse path
    // if path is like "~/Projects/cpp" then replace it with the full path
    if (path.find("~") != string::npos)
    {
        string home = getenv("HOME");
        path.replace(path.find("~"), 1, home);
    }

    // if path is like "." then replace it with the full path
    if (path.find(".") != string::npos)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        path.replace(path.find("."), 1, cwd);
    }
    cout << path << endl;

    chdir("/bin");
    // If the varpath.txt does not exist, create it
    FILE *file = fopen("varpath.txt", "r");
    if (file == NULL)
    {
        file = fopen("varpath.txt", "w");
        fclose(file);
    }

    // Read the file and store the content in a map
    // Also strip the newline character if exist only
    map<string, string> varpath;
    file = fopen("varpath.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Also strip the newline character if exist only
        string str = line;
        str.erase(str.find_last_not_of("\n") + 1);
        int pos = str.find("=");
        string var = str.substr(0, pos);
        string path = str.substr(pos + 1, str.length());
        varpath[var] = path;
    }

    // Update the map with the new var and path
    bool update = false;
    if (varpath.find(var) != varpath.end()) {
        update = true;
    }
    string from = varpath[var];
    varpath[var] = path;

    file = fopen("varpath.txt", "w");
    for (auto &[k, v]: varpath) {
        if (!k.empty() && !v.empty()) {
            fprintf(file, "%s=%s\n", k.c_str(), v.c_str());
        }
    }
    fclose(file);

    if (update) {
        cout << "Updated " << var << " from " << from << " to " << path << endl;
    } else {
        cout << "Added " << var << " with " << path << endl;
    }



    return 0;
}
