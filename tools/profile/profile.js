var fileInput = document.getElementById('csv');
var typeSelect = document.getElementById('type_select');
var countSelect = document.getElementById('count_select');

var recordArr = [];		// parse from csv file
var chart = undefined;	// ApexCharts object

var typeIndex = 0;		// default is 0
var countIndex = 0;		// default is 0

function updateLabel() {
	// update input texfiled label
	var labelElm = document.getElementById('file_label');
	var f = fileInput.files[0];
	if (f) {
		labelElm.innerHTML = f.name;
	}
}

function updateChart(options) {
	if (chart === undefined) {
		chart = new ApexCharts(
			document.querySelector("#chart"),
			options
		);
		chart.render();
	}
	else {
		console.log('update chart options');
		chart.updateOptions(options);
	}

	updateLabel();
}

function readFile() {
	// read the file data and save it as array to recordArr
	var reader = new FileReader();

	reader.onload = function() {
		var csvData = reader.result;
		recordArr = procCsvData(csvData);
		if (recordArr.length === 0) {
			alert('file data error!');
			return;
		}

		var options = getChartOption(recordArr, typeIndex, countIndex);
		updateChart(options);
	}

	reader.onerror = function() {
		alert("Can't read file");
	}

	reader.readAsText(fileInput.files[0], 'utf8');
};

function selectChange(e) {
	index = e.srcElement.selectedIndex
	console.log('selectChange: ' + typeIndex);

	if (e.srcElement === typeSelect) {
		typeIndex = index;
	}
	else if (e.srcElement === countSelect) {
		countIndex = index;
	}

	if (recordArr.length === 0) {
		return;
	}

	var options = getChartOption(recordArr, typeIndex, countIndex);
	updateChart(options);
};

// set file input event
if (fileInput) {
	fileInput.addEventListener('change', readFile);
}
else {
	console.error('fileInput is null');
}

// set statistical type select event
if (typeSelect) {
	typeSelect.addEventListener('change', selectChange);
}
else {
	console.error('typeSelect is null');
}

// set list count select event
if (countSelect) {
	countSelect.addEventListener('change', selectChange);
}
else {
	console.error('typeSelect is null');
}
