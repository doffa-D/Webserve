<?php
// to change session key name:
session_name("SID");

// to change path of sessions files
session_save_path('/Users/kchaouki/Desktop/___web/session');

header("Content-Type: text/html; charset=UTF-8");
session_start();

//database of users
$filename = 'users.txt';

// Check if form is submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve values from form
    $username = $_POST["username"];
    $password = $_POST["password"];

	$users = file($filename, FILE_IGNORE_NEW_LINES);
    foreach ($users as $user) {
        list($stored_username, $stored_password) = explode(':', $user);
        if ($username === $stored_username && $password === $stored_password) {
            // Login successful
			$_SESSION["username"] = $username;

			setcookie("username", $username, time() + 3600, "/");
            header("Location: welcome.php");
            exit;
        }
    }
	// Invalid username or password, set error message
	$_SESSION['error_message'] = "Invalid username or password.";
	header("Location: login.php");
}
?>