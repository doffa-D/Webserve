#!/usr/bin/env python3

import os
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class CodeChangeHandler(FileSystemEventHandler):
    def on_any_event(self, event):
        if event.is_directory or not event.src_path.endswith(".cpp"):
            return
        print("Changes detected in {}".format(event.src_path))
        compile_and_run()

def compile_and_run():
    print("Compiling and running...")
    subprocess.run(["make", "run"])

def watch_directory(directory):
    event_handler = CodeChangeHandler()
    observer = Observer()
    observer.schedule(event_handler, path=directory, recursive=True)
    observer.start()
    print("Watching directory: {}".format(directory))
    try:
        while True:
            pass
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == "__main__":
    # Specify the directory to watch
    watched_directory = "."  # You can change this to your project's root directory
    user_installation_path = os.path.expanduser("~/.local/bin/")
    os.environ["PATH"] = "{}:{}".format(user_installation_path, os.environ["PATH"])
    watch_directory(watched_directory)
