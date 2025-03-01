1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_answer here_ We can use waitpid() to ensure that all child processes complete before the shell continues accepting user input. The waitpid() funciton is called in a loop to handle multiple child processes. 
If I forgot to call waitpid() on all child processes, the child processes may turn into zombie processes. It can lead to resource leakage and eventually casue the system to run out of process slots. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_answer here_ It's necessary to close unused pipe ends after calling dup2() because of the following reasons: it would waste resources and hit system limits on the number of open file descriptors. 
It might interfere with the expected communication behavior. Unused pipes can lead to deadlocks.
If we leave pipes open, processes might hang or wait indefinitely due to improper handling of EOF signals. It can lead to resource exhaustion and the inability to open new files or pipes. 

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_answer here_ The reason cd is implemented as a built-in command rather than an external command is that cd affects the shell's current working directory, which is a property of the shell process itself. 
Running cd as a built-in command avoids the overhead of forking a new process. It makes it more efficient in terms of system resources and execution speed. 
If cd were implemented as an external process, it would only change the current directory of the child process running the external command and not the shell's working directory.  
It's ineffective. Once the external cd process terminates, control returns to the parent shell process, whihc remains in its original directory. It would involve forking a new process and executing a new program, 
leading to unnecessary performanc overhead and slower shell operations.


4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_answer here_ To modify my implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently, I would use dynamic data structures and allocate memory as needed. Using a dynamically allocated arrary to store commands and allocate pipes dynamically as needed.
1) Initialize dynamic arrays; 2) resize arrays as needed; 3) free the allocated memory at the end of the shell's execution.
Some trade-offs to consider: Dynamic allocation may introduce some overhead due to frequent allocations and reallocations. It increases the complexity of the code. It can impact performance.
