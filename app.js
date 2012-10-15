/**
  * Bonsai HTTP Server
  * Nathan Campos <nathanpc@dreamintech.net>
  */

// Includes
var net = require("net");

// Constants
var host = "127.0.0.1";
var port = 8484;
var debug = true;


function cleanSocketData(data) {
  if (data.toString().split("\r\n") > 1) {
    data = data.toString().split("\r\n").splice(this.length, 1).join("\n");
  } else {
    data = data.toString().split("\n").splice(this.length, 1).join("\n");
  }

  return data;
}

/*function command(data, socket) {
  switch (data[0]) {
    // A keyboard key was sent.
    case "KEY":
      if (debug) {
        console.log("Key: " + data[1]);
      }

      socket.write("OK");
      break;
    // Some shit was sent.
    default:
      if (debug) {
        console.log("Invalid Command: " + data);
      }

      socket.write("INV");
      break;
  }
}*/

var server = net.createServer(function (socket) {
  if (debug) {
    console.log("Client connected at " + socket.remoteAddress + ":" + socket.remotePort);
  }

  // Received Data
  socket.on("data", function (data) {
    //command(cleanSocketData(data).split(" "), socket);
    console.log(data.toString());
  });

  // Client Disconnected
  socket.on("close", function (data) {
    if (debug) {
      console.log("Client" + socket.remoteAddress + ":" + socket.remotePort + "disconnected");
    }
  });

  // Connection Ended
  socket.on("end", function (data) {
    if (debug) {
      console.log("Client" + socket.remoteAddress + ":" + socket.remotePort + "disconnected");
    }
  });

  // Error
  socket.on("error", function (data) {
    if (debug) {
      console.log("Socket ERROR: " + data);
    }
  });
});

server.listen(port, host);

console.log("Server running at " + host + ":" + port);