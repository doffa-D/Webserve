#!/bin/bash

# Function to recursively list files and cat their content
list_files_and_cat() {
    local dir="$1"

    # Loop through files and directories in the current directory
    for item in "$dir"/*; do
        if [ -f "$item" ]; then
            # If it's a file, print its name and content
            echo "File: $item"
            cat "$item"
            echo -e "\n"  # Add an empty line for separation
        elif [ -d "$item" ]; then
            # If it's a directory, call the function recursively
            list_files_and_cat "$item"
        fi
    done
}

# Check if an argument (directory path) is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory_path>"
    exit 1
fi

# Get the directory path from the command line argument
directory_path="$1"

# Check if the provided path is a directory
if [ ! -d "$directory_path" ]; then
    echo "Error: '$directory_path' is not a directory."
    exit 1
fi

# Call the list_files_and_cat function with the provided directory path
list_files_and_cat "$directory_path"