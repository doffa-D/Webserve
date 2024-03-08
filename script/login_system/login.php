<?php
header("Content-Type: text/html; charset=UTF-8");

// to change session key name:
session_name("SID");

// to change path of sessions files
session_save_path('../../session');;
session_start();
// Check if user is already logged in
if(isset($_SESSION["username"])) {
	// If logged in, redirect to welcome page
    header("Location: welcome.php");
    exit;
}

$error = '';

// Check if error message is set in session (from login_process.php)
if(isset($_SESSION['error_message'])) {
    $error = $_SESSION['error_message'];
    // Clear the session variable
    unset($_SESSION['error_message']);
}

// If not logged in, display login form
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Login Page</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
	<div id="login-form-wrap">
		<h2>Login</h2>
		<?php if(!empty($error)): ?>
		<script>
			alert("<?php echo $error; ?>");
		</script>
		<?php endif; ?>
		<form id="login-form" method="post" action="login_process.php">
		  <p>
		  	<input type="text" id="username" name="username" placeholder="Username" required>
		  </p>
		  <p>
		  	<input type="password" id="password" name="password" placeholder="Password" required>
		  </p>
		  <p>
		  	<input type="submit" id="login" value="Login">
		  </p>
		</form>
	</div>
</body>
</html>