#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>

int main(int ac, char **av, char **env)
{
    char **ptr = new char *[3];
    std::string jo = "/usr/bin/php";
    ptr[0] = const_cast<char*>(jo.c_str());
    jo = "hhhh.php";
    ptr[1] = const_cast<char*>(jo.c_str());
    ptr[2] = NULL;
    execve(ptr[0], ptr, env);
    perror("execve"); // Print any errors that occurred during execve
    delete[] ptr; // Free the memory allocated for ptr
    return 0;
}
