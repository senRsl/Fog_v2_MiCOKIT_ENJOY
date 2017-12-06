//显示反馈信息
function display_info_feedback(){
	var info = "要反馈啊?报批了没?";
	
	mui.confirm(info, "反馈", ["确定"], function(ret) {
	});
}

//显示版本信息
function display_info_version(){
	var info = "动员NMB V2.1";
	
	mui.confirm(info, "版本", ["确定"], function(ret) {
	});
}
