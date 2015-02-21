var MINI = require('minified');
var _=MINI._, $=MINI.$, $$=MINI.$$, EE=MINI.EE, HTML=MINI.HTML;
function run(code,success,error) {
var content = '----BOUNDARY\r\nContent-Disposition: form-data; name="lua"\r\n\r\n'+code+"\r\n----BOUNDARY--\r\n";
$.request('POST','/lua',content,{headers:{'Content-Type':"multipart/form-data; boundary=--BOUNDARY"}}).then(success, error);
}
function sendColor(color) {
	var rgb=color.toRgb();
	run('led.set(0,led.inited()-1,'+rgb.r+','+rgb.g+','+rgb.b+',200)');
}
function clickProgram(event) {
	run('programs.dofile("'+event.target.id+'")',function(){ $('#'+event.target.id).set('className', 'loading'); });
}
$(function() {
/*$("#color").spectrum({ color: "#ECC", showInput: true, containerClassName: "full-spectrum", showInitial: true, preferredFormat: "rgb", 
	move: function (color) {}, show: function () {}, beforeShow: function () {}, hide: function (color) { sendColor(color); }
});*/
run('=unpack(programs.list())', function(response){
	if (typeof(response) == 'undefined') return;
	var lines = response.split(/[\n> ]+/);
	var prgs = lines[0].split("\t");
	for (prg in prgs) { $('#toggle').addAfter(EE("li", {'id': prgs[prg]}, prgs[prg]).onClick(clickProgram)) };
});
setInterval(function() {run('=led.get(0,led.inited()-1)\n=programs.file', function(response) {
	if (typeof(response) == 'undefined') return;
	var lines = response.split(/[\n> ]+/);
	var cols = lines[0].split("\t");
	 var col = ((1 << 24) + (parseInt(cols[0]) << 16) + (parseInt(cols[1]) << 8) + parseInt(cols[2])).toString(16).slice(1);
	 $('html body').animate({$backgroundColor: "#" + col}, 5000);
	$('*', '#programs', true).filter(function(item){return item.id!=null&&item.id.indexOf("p_") == 0}).set('className', '');
	if (lines[1]!="") $('#'+lines[1]).set('className', 'active')
})}, 5000);
});