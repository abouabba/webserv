#include "CgiHandler.hpp"
#include <unistd.h>     // fork, execve, pipe, dup2
#include <sys/wait.h>   // waitpid
#include <sstream>      // stringstream
#include <cstring>      // strcpy
#include <stdexcept>    // runtime_error

CgiHandler::CgiHandler(const HttpRequest& request, const std::string& scriptPath)
    : request(request), scriptPath(scriptPath) {}


void CgiHandler::freeEnv(char** envp) const {
    if (!envp)
        return;

    for (size_t i = 0; envp[i]; i++)
        delete[] envp[i];

    delete[] envp;
}

void CgiHandler::buildEnv() {
    env["REQUEST_METHOD"] = request.method;

    env["SCRIPT_FILENAME"] = scriptPath;

    env["SERVER_PROTOCOL"] = "HTTP/1.0";

    env["GATEWAY_INTERFACE"] = "CGI/1.1";

    size_t pos = request.uri.find('?');
    if (pos != std::string::npos)
        env["QUERY_STRING"] = request.uri.substr(pos + 1);
    else
        env["QUERY_STRING"] = "";

    if (request.headers.count("Content-Length"))
        env["CONTENT_LENGTH"] = request.headers.at("Content-Length");
    else
        env["CONTENT_LENGTH"] = "";

    if (request.headers.count("Content-Type"))
        env["CONTENT_TYPE"] = request.headers.at("Content-Type");
    else
        env["CONTENT_TYPE"] = "";
}

char** CgiHandler::envToCharArray() const {
    char** envp = new char*[env.size() + 1];

    size_t i = 0;
    for (std::map<std::string, std::string>::const_iterator it = env.begin();
         it != env.end(); ++it) {

        std::string line = it->first + "=" + it->second;
        envp[i] = new char[line.size() + 1];
        std::strcpy(envp[i], line.c_str());
        i++;
    }
    envp[i] = NULL;
    return envp;
}

std::string CgiHandler::execute() {
    buildEnv();

    int inPipe[2];
    int outPipe[2];

    if (pipe(inPipe) == -1 || pipe(outPipe) == -1)
        throw std::runtime_error("pipe failed");

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("fork failed");

    if (pid == 0) {
        // ===== CHILD =====
        dup2(inPipe[0], STDIN_FILENO);
        dup2(outPipe[1], STDOUT_FILENO);

        close(inPipe[1]);
        close(outPipe[0]);

        char* argv[2];
        argv[0] = const_cast<char*>(scriptPath.c_str());
        argv[1] = NULL;

        char** envp = envToCharArray();

        execve(argv[0], argv, envp);

        // perror("execve failed");

        freeEnv(envp);  
        // exit(1);
    }

    // ===== PARENT =====
    close(inPipe[0]);
    close(outPipe[1]);

    if (request.method == "POST" && !request.body.empty()) {
        write(inPipe[1], request.body.c_str(), request.body.size());
    }
    close(inPipe[1]);

    std::stringstream output;
    char buffer[1024];
    ssize_t bytes;

    while ((bytes = read(outPipe[0], buffer, sizeof(buffer))) > 0)
        output.write(buffer, bytes);

    close(outPipe[0]);
    waitpid(pid, NULL, 0);

    return output.str();
}