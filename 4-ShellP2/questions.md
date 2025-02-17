1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  _start here_ fork() creates a new process, which is a copy of the parent process. The fork generates new processes while simultaneously preserving its parent process. The execvp, on the other hand, creates new processes but does not preserve the parent process simultaneously. The created child process does not have to run the same program as the parent process does. The purpose of fork() is to create a new process, which becomes the child process of the caller.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  _start here_ Upon successful completion, folk() returns 0 to the child process and returns the process ID of the child process to the parent process. If the fork() system call fails, -1 is returned to the parent process, no child process is created and errno is set to indicate the error. The return value is set to handle this scenario.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  _start here_ The execvp() function needs two parameters: the file path or name of the program to execute and an array of  strings with the arguments for that program. The execvp() function replaces the current process image with a new process image specified by file. The new image is constructed from a regular, executable file called the new process image file. No return is made because the calling process image is replaced by the new process image. The PATH environment variable plays an important role. It specifies the directories to be searched to find a command. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  _start here_ The purpose of calling wait() in the parent process after forking is to block the calling process until one of its child processes exits or a signal is received. After the child process terminates, the parent continues its execution after the wait system call instruction. If we didn't call it, the parent process could simply die first.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  _start here_ The WEXITSTATUS() evaluates to the low-order 8 bits of the child's return status passed on the exit() or _exist() function. It returns the least significant byte of the return code of the process provided it terminated normally. It's important is because it returns the exit status of the child.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  _start here_ The implementation of build_cmd_buff() handles quoted arguments to account for quoted strings with spaces as a single argument. It is necessary is because quoted arguments are considered one single argument. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  _start here_ I parsed input to cmd_buff_t *cmd_buff for this assignment compared to using command_t and command_list structures. When I tried to refactor my old code, the structure is different so I would need to change around the code and set up.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  _start here_ Interprocess Communication (IPC) is a mechanism that allows processes to communicate. It helps processes synchronize their activities, share information, and avoid conflicts while accesssing shared resources. Signals are asynchromous notifications sent to a process or to a specific thread within the same process to notify it of an event. Each signal in a Linux system has a current disposition, which determines how the process behaves when it is delivered the signal. Signals in a Linux system differ from other IPC mechanisms by being a lightweight, typically used for simple, immediate actions like termination requests or error notifications. Other IPCs allow for more complex data exchange and communication between processes.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  _start here_ SIGKILL is used to kill a process. It cannot be caught, blocked, or ignored and typically is used as a last resort when a process becomes unresponsive or needs to be terminated without allowing it to perform any cleanup operations.

    SIGTERM is to terminate a process. It can be caught and handled by the program. It gives it a chance to save data and clean up resources before exiting. 

    SIGINT is a signal that the operating system sends to a process when the user presses Ctrl+C. It's a request for the process to interrupt what it's doing and terminate. It terminates immediately. A program can catch the SIGINT signal and handle it in a custom way though.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  _start here_ When a process receives SIGSTOP, it suspends the execution of a process temporarily. SIGSTOP cannot be caught or ignored by the process being terminated. SIGINT is a request for the process to interrupt what it's doing and terminate. By default, when a process receives SIGINT, it terminates immediately. The SIGSTOP signal is sent to a process by its controlling terminal to request it to stop temporarily. 
