// 探针
//const _APPID = "db456b4a-17fc-11e6-a739-00163e0204c0";
// 烤箱
const _APPID = "f0835e20-d990-11e7-9baf-00163e120d98";
const _PRODUCTID = "9de12c3a-bd37-11e7-9baf-00163e120d98";

const _TOKEN = 'fogtoken';
const _CLIENTID = 'fogclientid';

function toast(msg) {
    api.toast({
        msg: msg,
        duration: 2000,
        location: 'bottom'
    });
}

function getMetaMessage(ret){
	return ret.meta.message;
}

function getMetaCode(ret){
	return ret.meta.code;
}

function listenKeyback() {
    var backSecond = 0;
    api.addEventListener({
        name: 'keyback'
    }, function(ret, err) {
        var curSecond = new Date().getSeconds();
        if (Math.abs(curSecond - backSecond) > 2) {
            backSecond = curSecond;
            api.toast({
                msg: '再按一次退出『店老大』',
                duration: 2000,
                location: 'bottom'
            });
        } else {
            api.closeWidget({
                id: api.appId,
                retData: {
                    name: 'closeWidget'
                },
                silent: true,
                animation: {
                    type: 'flip',
                    subType: 'from_bottom',
                    duration: 500
                }
            });
        }
    });
}

function _HTTPGET(host, type, callback){
    var retm, errm;
     $.ajax({
        url: host,
        type: type,
        headers: {
            "Content-Type": "application/json",
        },
        success: function(ret) {
            callback(ret, errm);
        },
        error: function(err) {
            callback(retm, err);
        }
    });
}

function _HTTPPOST(host, type, data, callback){
    var retm, errm;
     $.ajax({
        url: host,
        type: type,
        data : data,
        headers: {
            "Content-Type": "application/json"
        },
        success: function(ret) {
            callback(ret, errm);
        },
        error: function(err) {
            callback(retm, err);
        }
    });
}

