static const char SCRIPTS_JS[] PROGMEM = R"rawliteral(

const STANDARD_DELAY_MS = 100;
const LONG_OPERATION_DELAY_MS = 300;

function getRateLimitedCallback(actualFunction, delayMS)
{
	let timeout = null;
	let lastCall = 0;
	if (typeof myVar !== 'undefined')
		throw new Error("delayMS was not specified");

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

function createControlsFromControllerState(manualLedControlsDiv, repeatingColorControlsDiv)
{
	fetch('/ledControllerState.json')
	.then(response => 
	{
		if (!response.ok) {
			throw new Error("HTTP error " + response.status);
		}
		return response.json();
	})
	.then(ledControllerState => 
	{
		createManualLedControls(manualLedControlsDiv, ledControllerState);
		createRepeatingLedControls(repeatingColorControlsDiv, ledControllerState);
	})
	.catch(error => 
	{
		alert("Bulb info could not be parsed: " + error.message);
	})
}

function createManualLedControls(div, ledControllerState)
{
	div.innerHtml = "";

	let bulbInfo = ledControllerState.bulbInfo;
	
	var table = document.createElement("table");
	div.appendChild(table);

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
		colorPicker.oninput = getRateLimitedCallback(() => 
		{
			// Set all color pickers
			setAllLedsColor(hexToRgb(colorPicker.value));
			singleColorPickers.forEach(picker => 
			{
				picker.value = colorPicker.value;
				syncWrappingDivColor(picker);
			});
			syncWrappingDivColor(colorPicker);
		}, LONG_OPERATION_DELAY_MS);
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
		colorPicker.type = "color";
		colorPicker.oninput = getRateLimitedCallback(() => 
		{
			syncWrappingDivColor(colorPicker);
			setManualLedColor(bulbIndexToSet, hexToRgb(colorPicker.value));
		}, STANDARD_DELAY_MS);
		colorPicker.value = `rgb(${bulb.r},${bulb.g},${bulb.b})`;
		syncWrappingDivColor(colorPicker);
	}
}

function createRepeatingLedControls(div, ledControllerState)
{
	div.innerHtml = "";

	let repeatingColors = ledControllerState.repeatingColors;
	
	var table = document.createElement("table");
	div.appendChild(table);

	var row = document.createElement("tr");
	table.appendChild(row);

	for (let repeatingColorIndex = 0; repeatingColorIndex < repeatingColors.length; repeatingColorIndex++) 
	{
		const colorIndex = repeatingColorIndex;

		const colorItem = repeatingColors[repeatingColorIndex];
		const tableItem = document.createElement("td");
		row.appendChild(tableItem);

		const colorPickerContainer = document.createElement("div");
		colorPickerContainer.classList.add("colorpicker");
		tableItem.appendChild(colorPickerContainer);

		const colorPicker = document.createElement("input");
		colorPickerContainer.appendChild(colorPicker);
		colorPicker.type = "color";
		colorPicker.oninput = getRateLimitedCallback(() => 
		{
			syncWrappingDivColor(colorPicker);
			setRepeatingLedColor(colorIndex, hexToRgb(colorPicker.value));
		}, STANDARD_DELAY_MS);
		colorPicker.value = `rgb(${colorItem.r},${colorItem.g},${colorItem.b})`;
		syncWrappingDivColor(colorPicker);
	}

	row = document.createElement("tr");
	table.appendChild(row);

	var sliderTableItem = document.createElement("td");
	row.appendChild(sliderTableItem);
	sliderTableItem.colSpan = 5;

	const slider = document.createElement("input");
	sliderTableItem.appendChild(slider);
	slider.type = "range";
	slider.min = "1";
	slider.max = "5";
	slider.value = ledControllerState.repeatingColorsToUse; 
	slider.class = "slider";
	slider.margin = "2 2 10 2";
	slider.oninput = getRateLimitedCallback(() => setNumRepeatingColors(slider.value), STANDARD_DELAY_MS);
	slider.id = "repeatingColorSlider";
}

const COMMON_CONTENT_TYPE = "text/plain";

function setManualLedColor(bulbIndex, color) 
{
	fetch("/setManualLedColor.html",
	{
		method: "POST",
		headers: { "Content-Type": COMMON_CONTENT_TYPE },
		body: `${bulbIndex},${color.r},${color.g},${color.b}`
	});
}

function setRepeatingLedColor(repeatingColorIndex, color) 
{
	fetch("/setRepeatingLedColor.html",
	{
		method: "POST",
		headers: { "Content-Type": COMMON_CONTENT_TYPE },
		body: `${repeatingColorIndex},${color.r},${color.g},${color.b}`
	});
}

function setNumRepeatingColors(numRepeatingColors) 
{
	fetch("/setNumRepeatingColors.html",
	{
		method: "POST",
		headers: { "Content-Type": COMMON_CONTENT_TYPE },
		body: `${numRepeatingColors}`
	});
}

function setAllLedsColor(color) 
{
	fetch("/setAllLedsColor.html",
	{
		method: "POST",
		headers: { "Content-Type": COMMON_CONTENT_TYPE },
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