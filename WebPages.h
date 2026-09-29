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
	</style>
	</head>
	<body onload="handleBodyLoad()">
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

input[type="color" i] 
{
	border-radius: 0%;
	inline-size: 20px;
	block-size: 20px;
	border-width: 1px;
	border-style: none;
/* border-color: rgba( 0, 255, 153, 0.5); */
	width: 15px;
}
button 
{
	margin: 0;
	padding: 10px 20px; /* Adjust padding as needed */
}
.button-container 
{
	display: flex;
	gap: 0; /* Set gap to zero to remove space between child elements */
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

static const char SCRIPTS_JS[] PROGMEM = R"rawliteral(

function getRateLimitedCallback(actualFunction, delayMS)
{
	let timeout = null;
	let lastCall = 0;

	let rateLimitedCallback = () =>
	{
		if(timeout != null)
		{
			clearTimeout(timeout);
			timeout = null;
		}

		let msSinceLastCall = Date.now() - lastCall;

		if( msSinceLastCall > delayMS )
		{
			actualFunction();
			lastCall = Date.now();
		}
		else
		{
			timeout = setTimeout(rateLimitedCallback, delayMS - msSinceLastCall);
		}
	}

	return rateLimitedCallback;
}

function createMatrixButtons(element)
{
	fetch('/bulbInfo.json')
	.then(response => 
	{
		if (!response.ok) {
			throw new Error("HTTP error " + response.status);
		}
		return response.json();
	})
	.then(json => 
	{
		element.innerHtml = "";

		let bulbInfo = json.bulbInfo;
		
		var table = document.createElement("table");
		var lowerRow = document.createElement("tr");
		var isChangingAllLeds = false;
		var singleColorPickers = [];

		{
			const colorPickerContainer = document.createElement("td");
			const colorPicker = document.createElement("input");
			colorPicker.type = "color";
			colorPicker.addEventListener('input', getRateLimitedCallback(() => 
			{
				isChangingAllLeds = true;

				// Set all color pickers
				setAllLedsColor(hexToRgb(colorPicker.value));
				singleColorPickers.forEach(picker => picker.value = colorPicker.value);

				isChangingAllLeds = false;
			}, 300));
			colorPicker.value = 'rgb(255, 255, 255)';
			colorPickerContainer.appendChild(colorPicker);
			lowerRow.appendChild(colorPickerContainer);
		}

		// Add a spacer
		var spacerContainer = document.createElement("td");
		var spacerDiv = document.createElement("div");
		spacerDiv.classList.add("verticalLineDiv");
		
		spacerContainer.appendChild(spacerDiv);
		lowerRow.appendChild(spacerContainer);

		for (let bulbIndex = 0; bulbIndex < bulbInfo.length; bulbIndex++) 
		{
			const bulb = bulbInfo[bulbIndex];
			const bulbIndexToSet = bulbIndex;

			const colorPickerContainer = document.createElement("td");
			const colorPicker = document.createElement("input");
			colorPicker.classList.add("SingleLedColorPickers");
			colorPicker.type = "color";
			colorPicker.addEventListener('input', getRateLimitedCallback(() => 
			{
				if(!isChangingAllLeds)
					setLedColor(bulbIndexToSet, hexToRgb(colorPicker.value));
			}, 100));
			colorPicker.value = `rgb(${bulb.r},${bulb.g},${bulb.b})`;
			singleColorPickers.push(colorPicker);
			colorPickerContainer.appendChild(colorPicker);
			lowerRow.appendChild(colorPickerContainer);
		}

		table.appendChild(lowerRow);
		element.appendChild(table);
	})
	.catch(error => 
	{
		alert("Bulb info could not be parsed: " + error.message);
	})
}

function setLedColor(bulbIndex, color) 
{
	fetch("/setLedColor.html",
	{
		method: "POST",
		headers: { "Content-Type": "application/x-www-form-urlencoded" },
		body: `${bulbIndex},${color.r},${color.g},${color.b}`
	});
}

function setAllLedsColor(color) 
{
	fetch("/setAllLedsColor.html",
	{
		method: "POST",
		headers: { "Content-Type": "application/x-www-form-urlencoded" },
		body: `${color.r},${color.g},${color.b}`
	});
}

function hexToRgb(hex) {
	var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
	return result ? 
	{
		r: parseInt(result[1], 16),
		g: parseInt(result[2], 16),
		b: parseInt(result[3], 16)
	} : null;
}
	
)rawliteral";