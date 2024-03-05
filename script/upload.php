#!/usr/bin/php-cgi
<?php
// header("Content-Type: text/html; charset=UTF-8");
// header("Content-Length: 4");

// Check if the form was submitted
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // Check if file was uploaded
    if (isset($_FILES["file"])) {
        $file = $_FILES["file"];

        // Check for errors
        if ($file["error"] === UPLOAD_ERR_OK) {
            $upload_dir = '../upload'; // Specify absolute upload directory
            $filename = basename($file["name"]);
            $target_path = $upload_dir . '/' . $filename;

            // Move uploaded file to the specified directory
            if (move_uploaded_file($file["tmp_name"], $target_path)) {
                echo "File '" . $filename . "' uploaded successfully!";
            } else {
                echo "Failed to move uploaded file.";
            }
        } else {
            echo "Error occurred during file upload: " . $file["error"];
        }
    } else {
        echo "No file uploaded.";
    }
} else {
    echo "No POST request received.";
}
?>
