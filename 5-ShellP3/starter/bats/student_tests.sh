#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Change directory" {
    current=$(pwd)

    cd /tmp
    mkdir -p dsh-test

    run "${current}/dsh" <<EOF                
cd dsh-test
pwd
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="/tmp/dsh-testdsh2>dsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Change directory - no args" {
    current=$(pwd)

    cd /tmp
    mkdir -p dsh-test

    run "${current}/dsh" <<EOF                
cd
pwd
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="/tmpdsh2>dsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}


@test "Which which ... which?" {
    run "./dsh" <<EOF                
which which
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="/usr/bin/whichdsh2>dsh2>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "It handles quoted spaces" {
    run "./dsh" <<EOF                
   echo " hello     world     " 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output=" hello     world     dsh2> dsh2> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]
}

@test "Test single pipe: ls | wc -l" {
    run ./dsh <<< "ls | wc -l"
    [[ $status -eq 0 ]]
}

@test "Test multiple pipes: cat file.txt | grep 'word' | wc -l" {
    run ./dsh <<< "cat file.txt | grep 'word' | wc -l"
    [[ $status -eq 0 ]]
}

@test "Test empty pipeline: | grep" {
    run ./dsh <<< "| grep"
    [[ $output =~ "error: invalid command" ]]
    [[ $status -ne 0 ]]
}

@test "Test too many pipes" {
    run ./dsh <<< "cmd1 | cmd2 | cmd3 | cmd4 | cmd5 | cmd6 | cmd7 | cmd8 | cmd9"
    [[ $output =~ "error: piping limited to 8 commands" ]]
}

@test "Check SH_PROMPT" {
  [ "$SH_PROMPT" = "dsh3> " ]
}

@test "Check EXIT_CMD" {
  [ "$EXIT_CMD" = "exit" ]
}

@test "Check build_cmd_buff function" {
  cmd_line='ls -l'
  cmd_buff_alloc
  build_cmd_buff "$cmd_line" "$_cmd_buff"
  [ "${argv[0]}" = "ls" ]
  [ "${argv[1]}" = "-l" ]
  free_cmd_buff "$_cmd_buff"
}

@test "Check exec_built_in_cmd function" {
  cmd_line='exit'
  cmd_buff_alloc
  build_cmd_buff "$cmd_line" "$_cmd_buff"
  run exec_built_in_cmd "$_cmd_buff"
  [ "$status" -eq 0 ]
  free_cmd_buff 
}  