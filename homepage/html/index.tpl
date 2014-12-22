<!DOCTYPE html>
<html>
<head>
	<meta name="viewport" content="width=460,user-scalable=false" />
	<title>XMAS Star</title>
    	<link rel='stylesheet' href='style.css' type='text/css' media='screen' />    
    	<script src="script.js" type="text/javascript"></script>
</head>
<body>
    	<div id="page">
        	<div id="header">
			<h1> XMAS Star </h1>
            		Yeah... it's Christmas.
        	</div>
        	<div id="body">
			<select id="programSelect" onChange="programChange()">
				%programs%
			</select>
			<div id="programObj"></div>
        	</div>
		<div align="right"><a href="/wifi">Config</a></div>
	</div>
	<div id="debug"></div>
<script>
	programChange();
</script>
</body>
</html>