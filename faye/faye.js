var http = require('http'),
    faye = require('faye'),
    npid = require('npid');

var secret = process.env["FAYE_SECRET"] || '',
    port   = process.env["FAYE_PORT"] || 9292,
    server = http.createServer(),
    bayeux = new faye.NodeAdapter({mount: '/', timeout: 25}),
    pid    = npid.create(process.env["HOME"] + "/faye.pid");

pid.removeOnExit();

bayeux.addExtension({
  incoming: function(message, callback) {
    if (!message.channel.match(/^\/meta\//)) {
      var password = message.ext && message.ext.secret;

      if (password != secret && secret != '') {
        console.log(`Client ${message.clientId}: 403 Unauthorized, wrong password '${password}'`);
        message.error = "403::Password required";
      }
    }
    callback(message);
  },

  outgoing: function(message, callback) {
    if (message.ext)
      delete message.ext.password;
    callback(message);
  }
});

console.log("Faye server listening on", port);
bayeux.attach(server);
server.listen(port);
