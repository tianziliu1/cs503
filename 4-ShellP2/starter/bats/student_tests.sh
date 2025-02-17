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

@test "Check pwd runs without errors" {
    run ./dsh <<EOF                
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Check echo with quoted string" {
    run ./dsh <<EOF                
echo "hello, world"
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"hello, world"* ]]
}

@test "Check cd command changes directory" {
    mkdir -p test_dir
    run ./dsh <<EOF                
cd test_dir
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"test_dir"* ]]
}

@test "Check cd without arguments" {
    initial_dir=$(pwd)
    run ./dsh <<EOF                
cd
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"$initial_dir"* ]]
}

