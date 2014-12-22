<p>
	<input type="color" id="steadyColor" style="width:100%; height:6em;" value="#FFCC00" onInput='updateSteady(this.id, this.value)'>

	<table style="width:100%">
		<tr>
			<td width="15%">Red</td>
			<td width="69%"><input class="slider" type="range" min="0" max="255" value="255" step="1" onInput='adjustValue(this.value, "steadyRedShow", "r")' id="steadyRed" style="width:100%;"/></td>
			<td width="15%" style="padding-left:15px;" id="steadyRedShow">255</td>
		</tr>
		<tr>
			<td width="15%">Green</td>
			<td width="69%"><input class="slider" type="range" min="0" max="255" value="204" step="1" onInput='adjustValue(this.value, "steadyGreenShow", "g")' id="steadyGreen" style="width:100%;"/></td>
			<td width="15%" style="padding-left:15px;" id="steadyGreenShow">204</td>
		</tr>
		<tr>
			<td width="15%">Blue</td>
			<td width="69%"><input class="slider" type="range" min="0" max="255" value="0" step="1" onInput='adjustValue(this.value, "steadyBlueShow", "b")' id="steadyBlue" style="width:100%;"/></td>
			<td width="15%" style="padding-left:15px;" id="steadyBlueShow">0</td>
		</tr>
	</table>
</p>