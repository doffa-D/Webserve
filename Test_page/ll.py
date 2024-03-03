#!/usr/bin/python3
print("Content-Type: text/html")
# print("Content-Lenght: 50")
print()

import cgi
import cgitb
import os
import sys
cgitb.enable()

# Current working directory
print("in python", file=sys.stderr)

print("stuck", file=sys.stderr)
form = cgi.FieldStorage()

if 'file' not in form:
    print("Failed to upload file!")
    exit()

fileitem = form['file']

# Check if file was uploaded
if fileitem.filename:
    # Strip leading path from file name to avoid directory traversal attacks
    filename = os.path.basename(fileitem.filename)
    upload_dir = './upload'  # Make sure this directory exists and has write permissions
    
    # Clean filename
    clean_filename = ''.join(e for e in filename if e.isalnum() or e in ['.', '-']).rstrip()
    file_path = os.path.join(upload_dir, clean_filename)

    # Avoid overwriting existing files
    counter = 1
    while os.path.exists(file_path):
        filename, file_extension = os.path.splitext(clean_filename)
        file_path = os.path.join(upload_dir, f"{filename}_{counter}{file_extension}")
        counter += 1

    # Retrieve file data and save
    file_data = fileitem.file.read()
    with open(file_path, 'xb') as f:
        f.write(file_data)

    # print("File '" + filename + "' uploaded and saved as '" + os.path.basename(file_path) + "'!")
else:
    print("Failed to upload file!")
# !/usr/bin/python3

# while True:
#     print("This will run indefinitely. Press Ctrl+C to stop.")