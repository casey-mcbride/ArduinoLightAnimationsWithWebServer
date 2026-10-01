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

function syncWrappingDivColor(embeddedColorPicker) 
{
	embeddedColorPicker.parentElement.style.backgroundColor = embeddedColorPicker.value;
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
		element.appendChild(table);

		var lowerRow = document.createElement("tr");
		table.appendChild(lowerRow);

		var singleColorPickers = [];

		{
			const changeAllColorsTableData = document.createElement("td");
			lowerRow.appendChild(changeAllColorsTableData);

			const pickerDiv = document.createElement("div");
			changeAllColorsTableData.appendChild(pickerDiv);
			pickerDiv.classList.add("colorpicker");
			
			const colorPicker = document.createElement("input");
			pickerDiv.appendChild(colorPicker);

			colorPicker.type = "color";
			colorPicker.addEventListener('input', getRateLimitedCallback(() => 
			{
				// Set all color pickers
				setAllLedsColor(hexToRgb(colorPicker.value));
				singleColorPickers.forEach(picker => 
				{
					picker.value = colorPicker.value;
					syncWrappingDivColor(picker);
				});
				syncWrappingDivColor(colorPicker);
			}, 300));
			colorPicker.value = 'rgb(255, 255, 255)';

			syncWrappingDivColor(colorPicker);
		}

		var ledTableContainer = document.createElement("td");
		lowerRow.appendChild(ledTableContainer);

		var ledTable = document.createElement("table");
		ledTableContainer.appendChild(ledTable);

		// Make multiple rows of the LEDs, so they're compact
		var currentRow = null;
		for (let bulbIndex = 0; bulbIndex < bulbInfo.length; bulbIndex++) 
		{
			// Every 10 bulbs make a new row
			if(bulbIndex % 10 == 0)
			{
				currentRow = document.createElement("tr");
				ledTable.appendChild(currentRow);
			}

			const bulb = bulbInfo[bulbIndex];
			const bulbIndexToSet = bulbIndex;

			const tableData = document.createElement("td");
			currentRow.appendChild(tableData);

			const pickerDiv = document.createElement("div");
			tableData.appendChild(pickerDiv);
			pickerDiv.classList.add("colorpicker");

			const colorPicker = document.createElement("input");
			pickerDiv.appendChild(colorPicker);
			singleColorPickers.push(colorPicker);
			colorPicker.classList.add("SingleLedColorPickers");
			colorPicker.type = "color";
			colorPicker.addEventListener('input', getRateLimitedCallback(() => 
			{
				syncWrappingDivColor(colorPicker);
				setLedColor(bulbIndexToSet, hexToRgb(colorPicker.value));
			}, 100));
			colorPicker.value = `rgb(${bulb.r},${bulb.g},${bulb.b})`;
			syncWrappingDivColor(colorPicker);
		}

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