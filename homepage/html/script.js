function dbg(str, add)
{
	add = typeof add !== 'undefined' ? add : false;
	var debug = document.getElementById("debug");
	
	str += "<br />";
	if(add)
		debug.innerHTML += str;
	else
		debug.innerHTML = str;
}

function programChange()
{
	//dbg("jep");
	var sel = document.getElementById("programSelect");
	loadPage(sel.options[sel.selectedIndex].value, "programObj");
}

function refresh()
{
	var hr = new XMLHttpRequest(); 
	hr.open("GET", "refresh.cgi", true); 
	hr.onreadystatechange = function() 
	{
		if(hr.readyState == 4 && hr.status == 200) 
		{
			//dbg("refresh: " + hr.responseText, true);
			var resp=JSON.parse(hr.responseText);
			//dbg("json: " + resp.program, true); 
			
			if(document.getElementById("programSelect").selectedIndex != parseInt(resp.program))
			{
				//dbg("wrong program selected", true);
				document.getElementById("programSelect").selectedIndex = parseInt(resp.program);
				programChange();
				return;
			}
			if(resp.program == "0")
			{
				//prevent feedback loop
				if(updateSteady.timer != undefined)
					clearTimeout(updateSteady.timer);
				updateSteady.f = undefined;
				updateSteady.timer = setTimeout(function(){updateSteady.timer = undefined}, 550);
				
				document.getElementById("steadyColor").value = resp.data.value;	
			}			
		}
	} 
	hr.send();
}

function loadPage(href, pos)
{
	if(loadPage.timer != undefined)
		clearInterval(loadPage.timer);
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.open("GET", href, true);
	xmlhttp.onreadystatechange = function() 
    	{
        	if(xmlhttp.readyState == 4 && xmlhttp.status == 200) 
        	{
			document.getElementById(pos).innerHTML = xmlhttp.responseText;
			setTimeout(refresh,100);
			loadPage.timer = setInterval(refresh, 4000);
		}
	}
	xmlhttp.send();
}

function logslider(position) 
{
	if(position == 0)
		return 0;

	// position will be between 0 and 100
	var minp = 0;
	var maxp = 100;

	// The result should be between 100 an 10000000
	var minv = Math.log(10);
	var maxv = Math.log(10000000);

	// calculate adjustment factor
	var scale = (maxv-minv) / (maxp-minp);

	return Math.exp(minv + scale*(position-minp));
}

function time2human(value)
{
	if(value < 1000)
		return value.toFixed(0) + "ms";
	value /= 1000;
	if(value < 60)
		return value.toFixed(1) + "s";
	value /= 60;
	if(value < 60)
		return value.toFixed(1) + "s";
	value /= 60;
	return value.toFixed(1) + "h";
	
}

function adjustTime(value, lab)
{
	var l = document.getElementById(lab);

	l.innerHTML = time2human(logslider(value));
}
	

//function update(f, value)
//{
//	if(update.timer != undefined)
//	{
//		if(f != update.f)
//		{
//			setTimeout(function(){update(f, value)}, 100);	//postpone different request
//			return;
//		}
//		else	//same field. we just update the value
//		{
//			update.value = value;
//			return;
//		}
//	}
//	
//	// currently no timer running
//	update.f = f;
//	update.value = value;
//	update.timer = setTimeout(function()
//	{
//		update.timer = undefined;
//
//	},500);	
//}

function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}

function rgbToHex(r, g, b) {
    return "#" + ((1 << 24) + (r << 16) + (g << 8) + (b<<0)).toString(16).slice(1);
}

function adjustValue(value, lab, ch)
{
	var l = document.getElementById(lab);
	var color = document.getElementById("steadyColor");

	col = hexToRgb(color.value);
	if(ch=="r")
		col.r = value;
	if(ch=="g")
		col.g = value;
	if(ch=="b")
		col.b = value;
	
	color.value = rgbToHex(col.r, col.g, col.b);
	l.innerHTML = value;
	
	updateSteady("steadyColor", document.getElementById("steadyColor").value);
}



//hack: prevent event overflows
function updateSteady(f, value)
{
	var col = hexToRgb(value);
	document.getElementById("steadyRedShow").innerHTML = col.r;
	document.getElementById("steadyGreenShow").innerHTML = col.g;
	document.getElementById("steadyBlueShow").innerHTML = col.b;
	document.getElementById("steadyRed").value = col.r;
	document.getElementById("steadyGreen").value = col.g;
	document.getElementById("steadyBlue").value = col.b;

	
	if(updateSteady.timer == undefined)
	{
		var hr = new XMLHttpRequest(); 
		hr.open("POST", "steady.cgi", true); 
		hr.onreadystatechange = function() 
		{
			if(hr.readyState == 4 && hr.status == 200) 
        		{
			}
   		} 
    		hr.send(f + "=" + value);
//		dbg(f + "=" + value, true);
		updateSteady.f = undefined;
		updateSteady.timer = setTimeout(function(){updateSteady.timer = undefined; if(updateSteady.f != undefined) updateSteady(updateSteady.f, updateSteady.value);}, 200);
		return;
	}
	else
	{
		if(updateSteady.f != undefined)
		{
			if(updateSteady.f != f)
			{
				setTimeout(function(){update(f, value)},250);	//postpone different request.
				return;
			}
		}
		else
			updateSteady.f = f;
		updateSteady.value = value;
	}
}