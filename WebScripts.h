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