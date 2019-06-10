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
    //host: 'localhost',
    //user: 'me2',
    //password: '',
    //database: 'mytemp'
})
connection.connect();


var seq=0;
var seq2=0;

function insert_sensor(value,time) {
  obj = {};
  obj.value = value;
        obj.time =time;

  var query = connection.query('insert into sensors set ?', obj, function(err, rows, cols) {
    if (err) throw err;
    console.log("database insertion ok= %j", obj);
  });
}

app.get('/data',function(req,res){

        var C_id = req.query.C_id;
        var T_id = req.query.T_id;
        var S_id = req.query.S_id;

        var len = req.query.len;
        var vib = req.query.vib;
        var state = req.query.state;
        var moment = require('moment');
        require('moment-timezone');
        moment.tz.setDefault("Asia/Seoul");
        var date= moment().format('YYYY-MM-DD HH:mm:ss');

        console.log("c_id = %j , t_id = %j s_id = %j len_payload =%j vib_payload = %j presence = %j ",C_id,T_id,S_id,len,vib,state);

        fs.appendFile('data.csv',C_id+","+T_id+","+S_id+","+String(len)+","+String(vib)+","+state+"\n",function (err){
        if (err) throw err
        });

});


var server = app.listen(3000, function () {
  var host = server.address().address
  var port = server.address().port
  console.log('listening at http://%s:%s', host, port)
});
