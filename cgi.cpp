#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Replace with the path to your PHP-CGI executable
    const char* php_cgi_path = "/usr/bin/php-cgi";
    // Replace with the path to your PHP script
    const char* script_path = "/home/arash/Desktop/webserv/var/www/cgi-bin/ar.php";
	int fd = open("/home/arash/Desktop/webserv/var/www/cgi-bin/r.txt", O_CREAT | O_RDWR, 0666);
    // Prepare arguments for execve
    const char* argv[] = {
        php_cgi_path, // PHP-CGI executable
        script_path,  // Path to the PHP script
        nullptr      // Null terminator for argument list
    };

    // Create a new child process using fork
    pid_t child_pid = fork();

    if (child_pid == 0) {
		dup2(fd, 1);
        // Child process: Execute the PHP script using execve
        execve(php_cgi_path, const_cast<char**>(argv), nullptr);
        // If execve fails, print an error message
        perror("execve failed");
        return 1;
    } else {
        // Parent process: Wait for the child process to finish
        int status;
        waitpid(child_pid, &status, 0);

        // Handle the child process's exit status
        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status: " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << "Child process terminated abnormally" << std::endl;
        }
		close(fd);
    }

    return 0;
}
