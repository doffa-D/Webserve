#!/bin/bash

# Set the root directory
root_directory="."  # Use the root directory or update it to the desired directory

# List all files in all subdirectories
echo "Listing all files in all subdirectories within $root_directory:"
find "$root_directory" -type f
