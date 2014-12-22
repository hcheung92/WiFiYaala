<table style="width:100%">
  <tr align="center">
    <td>Color 1</td>
    <td>Color 2</td>
    <td>Color 3</td>
    <td>Current Color</td>
  </tr>
  <tr>
    <td><input type="color" name="color1" style="width:100%; height:6em" value="#FF0000"></td>
    <td><input type="color" name="color2" style="width:100%; height:6em" value="#00FF00"></td>
    <td><input type="color" name="color3" style="width:100%; height:6em" value="#0000FF"></td>
    <td id="viewCurrentColor"></td>
  </tr>
</table>

<table style="width:100%">
	<tr>
    		<td>Random order</td>
    		<td><input type="checkbox" name="rndColor" value="rndColor"></td>
  	</tr>
  	<tr><td></td></tr>
  	<tr>
		<td>Color 1</td>
  	</tr>
  	<tr>
    		<td width="32%">On time</td>
    		<td width="48%"><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "onTime1Show")' id="onTime1Input" style="width:100%;"/></td>
		<td width="20%" style="padding-left:15px;" id="onTime1Show"></td>
  	</tr>
  	<tr>
    		<td>Off time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "offTime1Show")' onChange="" id="offTime1Input" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="offTime1Show"></td>
  	</tr>
  	<tr>
    		<td>Fade time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "fadeTime1Show")' onChange="fadeTime1Input" id="" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="fadeTime1Show"></td>		
  	</tr>
  	<tr>
    		<td>Cross Fade</td>
		<td><input type="checkbox" name="crossFade1" value="crossFade1"></td>
  	</tr>
	<tr><td></td></tr>
	<tr>
		<td>Color 2</td>
  	</tr>
  	<tr>
    		<td>On time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "onTime2Show")' onChange="" id="onTime2Input" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="onTime2Show"></td>
  	</tr>
  	<tr>
    		<td>Off time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "offTime2Show")' onChange="" id="offTime2Input" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="offTime2Show"></td>
  	</tr>
  	<tr>
    		<td>Fade time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "fadeTime2Show")' onChange="fadeTime2Input" id="" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="fadeTime2Show"></td>		
  	</tr>
  	<tr>
    		<td>Cross Fade</td>
		<td><input type="checkbox" name="crossFade2" value="crossFade2"></td>
  	</tr>
	<tr><td></td></tr>
	<tr>
		<td>Color 3</td>
  	</tr>
  	<tr>
    		<td>On time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "onTime3Show")' onChange="" id="onTime3Input" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="onTime3Show"></td>
  	</tr>
  	<tr>
    		<td>Off time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "offTime3Show")' onChange="" id="offTime3Input" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="offTime3Show"></td>
  	</tr>
  	<tr>
    		<td>Fade time</td>
    		<td><input class="slider" type="range" min="0" max="100" value="0" step="0.5" onInput='adjustTime(this.value, "fadeTime3Show")' onChange="fadeTime3Input" id="" style="width:100%;"/></td>
		<td style="padding-left:15px;" id="fadeTime3Show"></td>		
  	</tr>
  	<tr>
    		<td>Cross Fade</td>
		<td><input type="checkbox" name="crossFade3" value="crossFade3"></td>
  	</tr>
</table>
