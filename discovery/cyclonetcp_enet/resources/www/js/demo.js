//Global variables
var adcData = [
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
];

var accX = [
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
];

var accY = [
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
];

var accZ = [
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
];

function createXmlHttpRequestObject() {
	if(window.XMLHttpRequest) {
		return new XMLHttpRequest();
	}
	else if(window.ActiveXObject) {
		//Available MSXML object IDs
		var progId = ['Msxml2.XMLHTTP.6.0', 'Msxml2.XMLHTTP.3.0', 'Msxml2.XMLHTTP', 'Microsoft.XMLHTTP'];
		//Iterate through MSXML object IDs
		for(var i in progId)
		{
			try
			{
				//Instantiate the highest version of MSXML
				return new ActiveXObject(progId[i]);
			}
			catch(e)
			{
			}
		}
	}
	
	return null;	
}

function initEvent() {
	//Refresh page contents
	refreshPage(null);
	//Start timer
	setTimeout('timerEvent()', 200);
};

function timerEvent() {
	//Create XMLHttpRequest object
	var xhr = createXmlHttpRequestObject();
	//Failed to created the object?
	if(xhr == null) {
		window.alert('XMLHttpRequest object is not supported!');
		return;
	}
	
	//Set callback function
	xhr.onreadystatechange = function() {
		if(xhr.readyState == 4) {
			if(xhr.status == 200) {
				refreshPage(xhr);
				setTimeout('timerEvent()', 200);
			}
		}
	};
	
	//Send request
	xhr.open('GET', 'data.xml', true);
	xhr.send(null);
}

function refreshPage(xhr) {
	var i = 0;
	var ax = 0;
	var ay = 0;
	var az = 0;
	var adcValue = 0;
	
	if(xhr != null) {
		ax = parseInt(xhr.responseXML.getElementsByTagName('ax')[0].childNodes[0].nodeValue);
		ay = parseInt(xhr.responseXML.getElementsByTagName('ay')[0].childNodes[0].nodeValue);
		az = parseInt(xhr.responseXML.getElementsByTagName('az')[0].childNodes[0].nodeValue);
		adcValue = parseInt(xhr.responseXML.getElementsByTagName('adc')[0].childNodes[0].nodeValue);
	}
	
	//Make room for the new values
	for(i = 0; i < (adcData.length - 1); i++)
		adcData[i] = adcData[i + 1];
	for(i = 0; i < (accX.length - 1); i++)
		accX[i] = accX[i + 1];
	for(i = 0; i < (accY.length - 1); i++)
		accY[i] = accY[i + 1];
	for(i = 0; i < (accZ.length - 1); i++)
		accZ[i] = accZ[i + 1];
	
	//Store the new values
	adcData[adcData.length - 1] = adcValue;
	accX[accX.length - 1] = ax;
	accY[accY.length - 1] = ay;
	accZ[accZ.length - 1] = az;
	
	try {
		//Refresh chart
		drawChart();
	}
	catch(e) {
	}
}

function drawChart() {
	var ctx = document.getElementById('canvas1').getContext('2d');
	
	//Store the current transformation matrix
	ctx.save();
	//Use the identity matrix while clearing the canvas
	ctx.setTransform(1, 0, 0, 1, 0, 0);
	ctx.clearRect(0, 0, canvas1.width, canvas1.height);
	//Restore the transform
	ctx.restore();
	
	//Draw outline
	ctx.save();
	ctx.fillStyle = '#FFFFFF';
	ctx.shadowColor='#888';
	ctx.shadowBlur = 5;
	ctx.shadowOffsetX = 2;
	ctx.shadowOffsetY = 2;
	ctx.fillRect(10, 10, canvas1.width - 20, canvas1.height - 20);
	ctx.restore();
	
	//Draw grid
	ctx.save();
	ctx.strokeStyle = '#DDD';
	ctx.lineWidth = 1;
	ctx.beginPath();
	
	for(var i = 1; i < 6; i++) {
		ctx.moveTo(10, 10 + i * (canvas1.height - 20) / 6);
		ctx.lineTo(canvas1.width - 10, 10 + i * (canvas1.height - 20) / 6);
	}
	
	for(var i = 1; i < 12; i++) {
		ctx.moveTo(10 + i * (canvas1.width - 20) / 12, 10);
		ctx.lineTo(10 + i * (canvas1.width - 20) / 12, canvas1.height - 10);
	}
	
	ctx.stroke();
	ctx.restore();
	
	//Draw curves
	ctx.save();
	ctx.lineWidth = 3;
	ctx.shadowColor = '#888';
	ctx.shadowBlur = 5;
	ctx.shadowOffsetX = 2;
	ctx.shadowOffsetY = 2;
	
	//ADC data
	ctx.strokeStyle = '#FCB400';
	ctx.beginPath();
	
	for(var i = 0; i < adcData.length; i++) {
		var x = 10 + i * (canvas1.width - 20) / (adcData.length - 1);
		var y = (canvas1.height - 10) - adcData[i] * (canvas1.height - 20) / 4096;
		y = Math.max(y, 12);
		y = Math.min(y, canvas1.height - 12);
		
		if(!i)
			ctx.moveTo(x, y);
		else
			ctx.lineTo(x, y);
	}
	
	ctx.stroke();
	
	//X-axis acceleration
	ctx.strokeStyle = '#D62408';
	ctx.beginPath();
	
	for(var i = 0; i < accX.length; i++) {
		var x = 10 + i * (canvas1.width - 20) / (accX.length - 1);
		var y = (canvas1.height / 2) - accX[i] * (canvas1.height - 20) / 128;
		y = Math.max(y, 12);
		y = Math.min(y, canvas1.height - 12);

		if(!i)
			ctx.moveTo(x, y);
		else
			ctx.lineTo(x, y);
	}
	
	ctx.stroke();
	
	//Y-axis acceleration
	ctx.strokeStyle = '#21AA29';
	ctx.beginPath();
	
	for(var i = 0; i < accY.length; i++) {
		var x = 10 + i * (canvas1.width - 20) / (accY.length - 1);
		var y = (canvas1.height / 2) - accY[i] * (canvas1.height - 20) / 128;
		y = Math.max(y, 12);
		y = Math.min(y, canvas1.height - 12);

		if(!i)
			ctx.moveTo(x, y);
		else
			ctx.lineTo(x, y);
	}
	
	ctx.stroke();
	
	//Z-axis acceleration
	ctx.strokeStyle = '#2159D6';
	ctx.beginPath();
	
	for(var i = 0; i < accZ.length; i++) {
		var x = 10 + i * (canvas1.width - 20) / (accZ.length - 1);
		var y = (canvas1.height / 2) - accZ[i] * (canvas1.height - 20) / 128;
		y = Math.max(y, 12);
		y = Math.min(y, canvas1.height - 12);

		if(!i)
			ctx.moveTo(x, y);
		else
			ctx.lineTo(x, y);
	}
	
	ctx.stroke();
	ctx.restore();
}
