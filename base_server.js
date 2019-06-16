var express = require('express');
var app = express();
const fs = require('fs');
var dateTime = require('node-datetime');
mysql = require('mysql');
var user = require('./inform');
var connection = mysql.createConnection({
        host: user.host,
        user: user.user,
        password: user.password,
        database: user.database,
})
connection.connect();


var seq=0;
var seq2=0;
function insert_sensor(ID,len,vib,state,time) {
	obj = {};
	obj.id = ID;
	obj.len = len;
	obj.vib = vib;
	obj.flag = state;
  var query = connection.query('insert into eval set ?', obj, function(err, rows, cols) {
    if (err) throw err;
    console.log("database table 'eval' insertion ok");
  });

}

app.get('/data',function(req,res){
		var ID = req.query.ID;
        var len = req.query.len;
        var vib = req.query.vib;
        var state = req.query.state;
        var moment = require('moment');
        require('moment-timezone');
		moment.tz.setDefault("Asia/Seoul");
        var date= moment().format('YYMMDDHHmmss');
        console.log("ID = %j, date = &j, len_payload =%j, vib_payload = %j, state = %j  ",ID, date,len,vib,state);
        fs.appendFile('data2.csv',ID+","+String(date)+","+String(len)+","+String(vib)+","+state+"\n",function (err){
        if (err) throw err
        });
        insert_sensor(ID,len,vib,state,date);
});

/*
 * 최근 테이블 및 좌석 현황 JSON 형태로 웹 출력 
 */
app.get('/result', function(req, res) {
	console.log("DB(cafe)의 TABLE(result)에서 마지막 ROW 호출");
	var moment = require('moment');
	var html = fs.readFile('./result.html', function (err, html) {
		html = ' ' + html;
		var sql = 'SELECT * FROM result ORDER BY time DESC limit 1';
		connection.query(sql, function(err, rows, cols) {
			if (err) throw err;
			var data = '';
			for (var i in rows) {
				//data += JSON.stringify(rows[i]);
				Date.prototype.toJSON = function() { return moment(this).format(); }
				data += JSON.stringify(rows[i]);
			}
			html = html.replace('<%DATA%>', data);
			res.writeHeader(200, {"Content-type": "text/html"});
			res.write(html);
			res.end();
		});
	});
});
var server = app.listen(4885, function () {
  var host = server.address().address
  var port = server.address().port
  console.log('listening at http://%s:%s', host, port)
});
