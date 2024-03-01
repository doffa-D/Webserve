#!/usr/bin/python3

import cgi
import cgitb
import os
cgitb.enable()

form = cgi.FieldStorage()
uploaded_file = form["file"]
if uploaded_file.filename:
    upload_dir = os.getenv("UPLOAD_DIR", "./")
    os.makedirs(upload_dir, exist_ok=True)
    filepath = os.path.join(upload_dir, os.path.basename(uploaded_file.filename))
    with open(filepath, 'wb') as file:
        file.write(uploaded_file.file.read())
    print(f'<h1 style="text-align:center; background-color:blue; color:yellow">The file was uploaded successfully to {filepath}</h1>')