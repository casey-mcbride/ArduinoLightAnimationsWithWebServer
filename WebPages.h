static const char HTML_CONTENT_HOME[] PROGMEM = R""""(
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<link rel="stylesheet" type="text/css" href="style.css">
		<title>Home Page</title>
	</head>
	<body>
		<h1>Welcome to the Home Page</h1>
		<ul>
			<li><a href="/ledmessage.html">LED Message Page</a></li>
			<li><a href="/points.html">LED canvas</a></li>
		</ul>
	</body>
</html>
)"""";

static const char HTML_CONTENT_404[] PROGMEM = R""""(
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<link rel="stylesheet" type="text/css" href="style.css">
		<title>404 - Page Not Found</title>
	</head>
	<body>
		<h1 class="ErrorHeading">404</h1>
		<p>Oops! The page you are looking for could not be found on this Arduino!.</p>
		<p>Please check the URL or go back to the <a href="/">homepage</a>.</p>
	</body>
</html>
)"""";

static const char HTML_SET_MESSAGE_CONTENT[] = R"=====(
<!DOCTYPE html>
<html>
	<head>
		<title>Arduino LED Matrix Web</title>
		<link rel="stylesheet" type="text/css" href="style.css">
		<meta name="viewport" content="width=device-width, initial-scale=0.7">
	</head>

	<body>
		<h2>Arduino - LED Matrix via Web</h2>
		<form class="user-input" action="" method="GET">
			<input type="text" id="message" name="message" placeholder="Message to LED Matrix...">
			<input type="submit" value="Send">
		</form>
	</body>
</html>
)=====";

static const char HTML_SET_LEDS_CONTENT[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
	<head>
		<title>Arduino Pixel drawer</title>
		<script src="/script.js"></script>
		<link rel="stylesheet" type="text/css" href="style.css">
	</head>
	<body onload="handleBodyLoad()">
		<h1>Manual Setting</h1>
		<div id="innerContent"/>
		<script>
			function handleBodyLoad()
			{
				createMatrixButtons(document.getElementById("innerContent"));
			}
		</script>
	</body>
</html>
)rawliteral";

static const char STYLE_CSS[] PROGMEM = R"rawliteral(
body 
{
	font-size: 16px;
}

button 
{
	width: 16px;
	height: 16px;
	margin: 0px;
}

.ErrorHeading
{
	color: red;
}

.user-input
{
	margin-bottom: 20px;
}
.user-input input
{
	flex: 1;
	border: 1px solid #444;
	padding: 5px;
}
.user-input input[type="submit"]
{
	margin-left: 5px;
	background-color: #007bff;
	color: #fff;
	border: none;
	padding: 5px 10px;
	cursor: pointer;
}

/*Color picker style, put a color picker in a div with this class, and it will look much better*/
.colorpicker 
{
	position: relative;
	width: 10px;
	height: 10px;
	background-color: black;
	border: solid 1px black;
}

.colorpicker input[type="color"] 
{
	opacity: 0;
	position: absolute;
	width: 100%;
	height: 100%;
	cursor: pointer;
	width: 10px;
	height: 10px;
}

body
{
	color: blue;
}

.verticalLineDiv 
{
	border-left: 6px solid green;
	height: 500px;
}


)rawliteral";