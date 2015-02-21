var MINI = require('minified');
var _=MINI._, $=MINI.$, $$=MINI.$$, EE=MINI.EE, HTML=MINI.HTML;
var downX = -1, downY = -1, color = {r:0,g:0,b:0},bgAnim=null,newMode=-1,downColor={h:-1,s:-1,l:-1},newColor={h:-1,s:-1,l:-1};
String.prototype.capitalize = function() { return this.charAt(0).toUpperCase() + this.slice(1); }
function run(code,success,error) {
	var content = '----BOUNDARY\r\nContent-Disposition: form-data; name="lua"\r\n\r\n'+code+"\r\n----BOUNDARY--\r\n";
	$.request('POST','/lua',content,{headers:{'Content-Type':"multipart/form-data; boundary=--BOUNDARY"}}).then(success, error);
}
function sendColor(color) {
	run('local r,g,b=hsl2rgb('+Math.floor(color.h)+','+Math.floor(color.s)+','+Math.floor(color.l)+');led.set(0,led.inited()-1,r,g,b,200)');
}
function clickProgram(event) {
	run('programs.dofile("'+event.target.id+'")',function(){ $('#'+event.target.id).set('className', 'loading'); });
}
function RGBToHSL(rgb,hsl) {
	var r=rgb.r/255, g=rgb.g/255, b=rgb.b/255, min = Math.min(r, g, b), max = Math.max(r, g, b), diff = max - min, h = 0, s = 0, l = (min + max) / 2;
	if (diff != 0) {
		s = l < 0.5 ? diff / (max + min) : diff / (2 - max - min);
		h = (r == max ? (g - b) / diff : g == max ? 2 + (b - r) / diff : 4 + (r - g) / diff) * 60;
	}
	hsl.h=h; hsl.s=s*255; hsl.l=l*510;
}
$(function() {
/*$("#color").spectrum({ color: "#ECC", showInput: true, containerClassName: "full-spectrum", showInitial: true, preferredFormat: "rgb", 
	move: function (color) {}, show: function () {}, beforeShow: function () {}, hide: function (color) { sendColor(color); }
});*/
run('=unpack(programs.list())', function(response){
	if (typeof(response) == 'undefined') return;
	var lines = response.split(/[\n> ]+/);
	var prgs = lines[0].split("\t").sort();
	for (prg in prgs) { var id=prgs[prg]; $('#toggle').addAfter(EE("li", {'id': id}, id.substring(2, id.lastIndexOf('.')).capitalize()).onClick(clickProgram)) };
});
setInterval(function() {run('=led.get(0,led.inited()-1)\n=programs.file', function(response) {
	if (typeof(response) == 'undefined') return;
	var lines = response.split(/[\n> ]+/);
	var cols = lines[0].split("\t");
	color.r=parseInt(cols[0]); color.g=parseInt(cols[1]); color.b=parseInt(cols[2]);
	newColor.h=-1;
	 var col = ((1 << 24) + (color.r << 16) + (color.g << 8) + color.b).toString(16).slice(1);
	 if (downX==-1&&downY==-1) bgAnim = $('html body').animate({$backgroundColor: "#" + col}, 5000);
	$('*', '#programs', true).filter(function(item){return item.id!=null&&item.id.indexOf("p_") == 0}).set('className', '');
	if (lines[1]!="") $('#'+lines[1]).set('className', 'active')
})}, 5000);
$('html').on('mousedown',function(event){
	downX=event.pageX;
	downY=event.pageY;
	if (newColor.h==-1) { RGBToHSL(color, downColor); }
	else { downColor.h=newColor.h; downColor.s=newColor.s; downColor.l=newColor.l; }
	if (downX < document.getElementById('scrollline').getBoundingClientRect().left) newMode=1;
	else newMode=2;
	return false;
}).on('mouseup',function(){
	downX=-1;
	downY=-1;
	newMode=0;
	sendColor(newColor);
}).on('mousemove',function(event){
	if (downX==-1||downY==-1) return true;
	if (bgAnim!=null) { bgAnim.stop(); bgAnim=null };
	if (newMode == 1) {
		newColor.l=Math.min(510,Math.max(0,(downColor.l + (downY-event.pageY))));
		newColor.h=downColor.h;
		newColor.s=downColor.s;
	} else if (newMode == 2) {
		newColor.h=(downColor.h + (event.pageX-downX)/2 + 3600000)%360;
		newColor.s=Math.min(255,Math.max(0,(downColor.s + (downY-event.pageY))));
		newColor.l=downColor.l;
	}
	$('html body').set('$background-color', "hsl(" + Math.floor(newColor.h) + "," + Math.floor(newColor.s/255*100) + "%," + Math.floor(newColor.l/510*100) + "%)");
	//bgAnim = $('html body').anim({$backgroundColor: "hsl(" + Math.floor(newColor.h) + "," + Math.floor(newColor.s/25*1005) + "%," + Math.floor(newColor.l/510*100) + "%)"}, 50);
	return false;
});
$('#programs').on('mousedown',function() {return false;});
});