// statistical type index
const STAT_TYPE_TOTAL_TIME = 0;
const STAT_TYPE_AVERAGE_TIME = 1;
const STAT_TYPE_CALL_NUMBER = 2;

// lsit count type index
const LIST_COUNT_30 = 0;
const LIST_COUNT_50 = 1;
const LIST_COUNT_100 = 2;
const LIST_COUNT_200 = 3;
const LIST_COUNT_ALL = 4;

function procCsvData(csv) {
	var allTextLines = csv.split(/\r\n|\n/);
	var objArr = [];

	var len =  allTextLines.length;
	if (len === 0) {
		alert("CSV data error!");
		return objArr;
	}

	// first line is title
	const SEP = ',';
	var keys = allTextLines[0].split(SEP);
	console.log('title:', keys)

	// data line
	for (var i = 1; i < allTextLines.length; i++) {
		var data = allTextLines[i].split(SEP);
		if (data.length !== keys.length) {
			console.warn('line text invalid, index: ' + i);
			console.warn(data);
			continue;
		}

		// check every element at data
		var err_tag = false;
		for (var j = 0; j < data.length; j++) {
			if (data[j] === '') {
				err_tag = true;
				break;
			}
		}

		if (err_tag) {
			console.warn('line text content invalid:');
			console.warn(data);
			continue;
		}
		else {
			var obj = {};
			for (var j = 0; j < data.length; j++) {
				obj[keys[j]] = data[j];
			}
			objArr.push(obj);
		}
	}

	return objArr;
}

function sortByType(data, type) {
	// sort by total time
	var totalTime = function(a, b) {
		return b.time - a.time;
	};

	// sort by average time
	var averageTime = function(a, b) {
		var avgA = a.time / a.number;
		var avgB = b.time / b.number;
		return avgB - avgA;
	}

	// sort by number of call
	var callNumber = function(a, b) {
		return b.number - a.number;
	}

	var sortFunc = undefined;
	if (type === STAT_TYPE_TOTAL_TIME) {
		sortFunc = totalTime;
	}
	else if (type === STAT_TYPE_AVERAGE_TIME) {
		sortFunc = averageTime;
	}
	else if (type === STAT_TYPE_CALL_NUMBER) {
		sortFunc = callNumber;
	}
	else {
		console.error('Unknown sort type: ' + type);
		return data;
	}

	return data.sort(sortFunc);
}

function dataListCount(data, listCount) {
	var l = 0;
	if (listCount === LIST_COUNT_30) {
		l = 30;
	}
	else if (listCount === LIST_COUNT_50) {
		l = 50;
	}
	else if (listCount === LIST_COUNT_100) {
		l = 100;
	}
	else if (listCount === LIST_COUNT_200) {
		l = 200;
	}
	else if (listCount === LIST_COUNT_ALL) {
		l = data.length;
	}
	else {
		console.error('Unknown list count type: ' + listCount);
		return data;
	}

	if (l > data.length) {
		l = data.length;
	}
	return data.slice(0, l);
}

function getObjData(obj, type) {
	// obj is object { function_name: ..., time: ..., number: ... }
	if (type === STAT_TYPE_TOTAL_TIME) {
		return obj.time;
	}
	else if (type === STAT_TYPE_AVERAGE_TIME) {
		return obj.time / obj.number;
	}
	else if (type === STAT_TYPE_CALL_NUMBER) {
		return obj.number;
	}
	else {
		console.error('Unknown type: ' + type);
		return 0;
	}
}

function getChartOption(csvData, type, listCount) {
	// csvData is array of object { function_name: ..., time: ..., number: ... }
	csvData = sortByType(csvData, type);
	csvData = dataListCount(csvData, listCount);

	console.log('echarHandle data len:' + csvData.length);
	var data = csvData.map((obj) => {
		return getObjData(obj, type);
	});

	var categories = csvData.map((obj) => {
		return obj.function_name;
	});

	var barH = 30;
	var charH = barH * csvData.length;

	var options = {
		chart: {
			height: charH,
			type: 'bar',
		},
		plotOptions: {
			bar: {
				horizontal: true,
				dataLabels: {
					position: 'top',
				},
			}
		},
		// show label on bar
		dataLabels: {
			enabled: true,
			// style: {
			// 	colors: ['#333']
			// },
			offsetX: 0,
			formatter: function(value, opt) {
				// var obj = csvData[opt.dataPointIndex]
				return parseInt(value);
			},
		},
		series: [{
			data: data
		}],
		xaxis: {
			categories: categories,
			position: 'top',
			labels: {
				// style: {
				// 	fontSize: '14px',
				// },
				offsetY: -12,
				formatter: function(value, timestamp, index) {
					var s = '';
					if (type === STAT_TYPE_TOTAL_TIME ||
						type === STAT_TYPE_AVERAGE_TIME) {
						s = 'μm';
					}
					return value + s;
				},
			},
		},
		yaxis: {
			// show: false,
			labels: {
				maxWidth: 220,
				formatter: function(value, index) {
					return value;
				},
			},
		},
		tooltip: {
			x: {
				show: true,
				formatter: function(value, opt) {
					return value;
				}
			},
			y: {
				formatter: function(value, opt) {
					var s = '';
					if (type === STAT_TYPE_TOTAL_TIME ||
						type === STAT_TYPE_AVERAGE_TIME) {
						s = 'μm';
					}
					return parseInt(value) + s;
				}
			},
		},
	}

	return options;
}