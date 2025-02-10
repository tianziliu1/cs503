1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  _start here_ The built-in fgets() C function is used for reading strings from a file or basic user inputs (like keyboard). It reads a text line and stores it in a string/character array. The function stops reading when it gets to a new line or the designated character limit (whichever comes earlier). fgets() reads in at most one less than size characters from stream and stores them into the buffer pointed to by s.  Reading stops after an EOF or a newline. If a newline is read, it is stored into the buffer.  A terminating null byte ('\0') is stored after the last character in the buffer.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  _start here_ The malloc() method in C is used to dynamically allocate a single large block of memory with the specified size. It returns a pointer of type void which can be cast into a pointer of any form. It does not initialize memory at execution time so that it has intialized each block with the default garbage value initially. malloc() is more flexible and efficient than allocating a fixed-size array.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  _start here_ Triming leading and trailing spaces from a command before storing it is crucial because extra spaces can lead to unexpected behavior when executing the command in a shell, potentially causing incorrect command parsing, unintended file selections, or even failing to execute the desired operation altogeter.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  _start here_ Here are 3 redirections we should implement in the custom shell: 1) implement redirection for output. The challenge would be handling file opening and writing errors. 2) Input redirction. It redircts the standard input of a command from a file. The challenge is to ensure the file exists and is readable. 3) Error redirection. The challenge is to ensure errors are correctly captured without mixing with STDOUT. For example, for ERR_TOO_MANY_COMMANDS, the output will be CMD_ERR_PIPE_LIMIT "error: piping limited to %d commands\n" STDOUT shows conventional output to this file handling while STDERR writes diagnostic output to this file handling. 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  _start here_ The key differencs between redirection and piping is that redirection sends a command's output directly to a file, while piping sends the output of one command to be used as the input of another command. Redirection controls where the output goes, while piping connects multiple commands to form a processing chain.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  _start here_ It is important to keep STDERR and STDOUT separate in a shell. There's an advantage to having error messages delivered by a dedicated stream. We can redirect a command's output (STDOUT) to a file and still see any error messages (STDERR) in the terminal window. We can react to the erros if needed, as they occur. It also stops the error message from contaminating the file that STDOUT has been redirected to. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  _start here_ To handle errors from commands fail when a command outputs both STDOUT and STDERR, we can redirect STDOUT AND STDERR to the same file. We can achieve this with the following command: "./error.sh > capture.txt 2>&1" 
    It redicts both STDOUT and STDERR to a text file to capture all output in one place.