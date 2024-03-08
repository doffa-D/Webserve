<?php
header("Content-Type: text/html; charset=UTF-8");
// to change session key name:
session_name("SID");

// to change path of sessions files
session_save_path('../../session');
session_start();


// Check if user is logged in
if (!isset($_SESSION["username"])) {
    // If not logged in, check for cookie
    if (isset($_COOKIE["username"])) {
        $_SESSION["username"] = $_COOKIE["username"];
    } else {
        // If no session or cookie, redirect to login page
        header("Location: login.php");
        exit;
    }
}
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Home Page</title>
		<link rel="stylesheet" href="web_style.css">
	</head>
	<body>
		<div class="bgimg">
			<div class="topleft">
			</div>
			<div class="middle">
			<h1>Welcome <?php echo $_SESSION["username"]; ?> to the our Webserve</h1>
			<hr>
			</div>
			<div class="bottomright">
				<p>made by: hdagdagu rrhinzar kchaouki </p>
			</div>
		</div>
	</body>
	
</html>	