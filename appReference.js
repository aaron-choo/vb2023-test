// Requiring module
const express = require('express');

// Creating express object
const expressapp = express();

// Defining port number
const PORT = 4000;

// Function to serve all static files
// inside public directory.
expressapp.use(express.static('public'));
expressapp.use('/images', express.static('images'));

// Server setup
expressapp.listen(PORT, () => {
  console.log(`Running server on PORT ${PORT}...`);
})

var http = require('http');
var fs = require('fs');
const path = require("path");
const url = require("url");

var index = fs.readFileSync('index.html');

var SerialPort = require("serialport");

const parsers = SerialPort.parsers;
const parser = new parsers.Readline({
  delimiter: "\r\n",
});

var port = new SerialPort("/dev/cu.usbmodem2401", {
  baudRate: 9600,
  dataBits: 8,
  parity: "none",
  stopBits: 1,
  flowControl: false,
});

port.pipe(parser);


var app = http.createServer(function (req, res) {
  // Parsing the URL
  var request = url.parse(req.url, true);

  // Extracting the path of file
  var action = request.pathname;

  // Path Refinements
  var filePath = path.join(__dirname,
    action).split("%20").join(" ");

  // Checking if the path exists
  fs.exists(filePath, function (exists) {

    if (!exists) {
      res.writeHead(404, {
        "Content-Type": "text/plain"
      });
      res.end("404 Not Found");
      return;
    }

    // Extracting file extension
    var ext = path.extname(action);

    // Setting default Content-Type
    var contentType = "text/plain";

    // Checking if the extension of
    // image is '.png'
    if (ext === ".png") {
      contentType = "image/png";
    }

    // Setting the headers
    res.writeHead(200, {
      "Content-Type": contentType
    });

    // Reading the file
    fs.readFile(filePath,
      function (err, content) {
        // Serving the image
        res.end(content);
      });
  });
  res.writeHead(200, { 'Content-Type': 'text/html' });
  res.end(index);
});

var io = require('socket.io').listen(app);

io.on('connection', function (data) {
  console.log('Node.js is listening!');
})


parser.on("data", function (data) {
  console.log(data);
  io.emit('data', data)
});

app.listen(3000);