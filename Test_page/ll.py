#!/usr/bin/env python3
import cgi
import os

# Set content type header
print("Content-Type: text/html; charset=UTF-8\n")

# Check if the form was submitted
form = cgi.FieldStorage()
if form:
    # Check if file was uploaded
    if "file" in form:
        file_item = form["file"]

        # Check for errors
        if file_item.file:
            upload_dir = './upload'  # Specify absolute upload directory
            filename = os.path.basename(file_item.filename)
            target_path = os.path.join(upload_dir, filename)

            # Move uploaded file to the specified directory
            with open(target_path, 'wb') as fout:
                while True:
                    chunk = file_item.file.read(1024)
                    if not chunk:
                        break
                    fout.write(chunk)
            print("File '{}' uploaded successfully!".format(filename))
        else:
            print("Failed to move uploaded file.")
    else:
        print("No file uploaded.")
else:
    print("No POST request received.")

    # while True:
    #     print("This will run indefinitely. Press Ctrl+C to stop.")