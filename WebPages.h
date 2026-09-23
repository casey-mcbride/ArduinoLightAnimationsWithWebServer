static const char* HTML_CONTENT_HOME = R""""(
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

static const char* HTML_CONTENT_404 = R""""(
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
	</style>
	</head>
	<body onload="handleBodyLoad()">
		<div id="innerContent"/>
		<script>
			function handleBodyLoad()
			{
				createMatrixButtons(document.getElementById("innerContent"), %JAVASCRIPT_MATRIX%);
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

body
{
	color: blue;
}

.on
{
	background-color: red;
}

.off
{
	background-color: white;
}
)rawliteral";

static const char SCRIPTS_JS[] PROGMEM = R"rawliteral(
function createMatrixButtons(element, flags)
{
	element.innerHtml = "";

	var table = document.createElement("table");
	for (let row = 0; row < flags.length; row++) 
	{
		var tableRow = document.createElement("tr");
		for (let column = 0; column < flags[0].length; column++) 
		{
			const tableElement = document.createElement("td");
			const button = document.createElement("button");
			if(flags[row][column])
				button.classList.add("on");
			else
				button.classList.add("off");
			const x = column;
			const y = row;
			button.onclick = (event) => { toggleLED(button, x, y); };
			tableElement.appendChild(button);
			tableRow.appendChild(tableElement);
		}

		table.appendChild(tableRow);
	}

	element.appendChild(table);
}

function toggleLED(button, x, y) 
{
	fetch("/points.html",
	{
		method: "POST",
		headers: { "Content-Type": "application/x-www-form-urlencoded" },
		body: x + "," + y
	});

	if(button.classList.contains("off"))
	{
		button.classList.remove("off");
		button.classList.add("on");
	}
	else
	{
		button.classList.remove("on");
		button.classList.add("off");
	}
}
)rawliteral";