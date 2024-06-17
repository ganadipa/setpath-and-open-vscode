// C++ Program to get the current working directory
#include <filesystem>
#include <iostream>
#include <stdlib.h>
#include <unistd.h> 
#include <map>

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: vscp <var>. \ne.g. vscp cp\n";
        return 1;
    }

    chdir("/bin");
    // read all the variables and paths from the file
    map<string, string> varpath;
    FILE *file = fopen("varpath.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Also strip the newline character if exist only
        string str = line;
        str.erase(str.find_last_not_of("\n") + 1);
        int pos = str.find("=");
        varpath[str.substr(0, pos)] = str.substr(pos + 1, str.length());
    }

    // get the path of the variable
    string var = argv[1];
    string path = varpath[var];
    if (path.empty())
    {
        std::cout << "Variable not found\n";
        return 1;
    }

    // change the directory
    cout << path.c_str() << endl;
    chdir(path.c_str());
    system("code .");

    return 0;
}
