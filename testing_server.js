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

function insert_sensor(c_id,t_id,s_id,len,vib,state,time) {
  obj = {};
  obj.C_id = c_id;
  obj.S_id = s_id;
  obj.T_id = t_id;
  obj.len = len;
  obj.vib = vib;
  obj.flag=state;
  obj.time = time;

        obj2 = {};
        obj2.C_id=c_id;
        obj2.S_id=s_id;
        obj2.T_id=t_id;
        obj2.flag=state;
        obj2.time=time;

  var query = connection.query('insert into save set ?', obj, function(err, rows, cols) {
    if (err) throw err;
    console.log("database table 'save' insertion ok");
  });

  var query2 = connection.query('insert into app set ?', obj2, function(err, rows, cols) {
    if (err) throw err;
console.log("database table 'app' insertion ok");
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
        var date = moment().format('YYMMDDHHmmss');
        console.log("ID = %j , date = %j  len_payload =%j, vib_payload = %j, state = %j  ",ID,date, len, vib, state);
        fs.appendFile('data2.csv',ID+","+String(date)+","+String(len)+","+String(vib)+","+state+"\n",function (err){
        if (err) throw err
        });
});

var server = app.listen(3000, function () {
  var host = server.address().address
  var port = server.address().port
  console.log('listening at http://%s:%s', host, port)
});
            
