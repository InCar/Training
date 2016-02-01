var Http = require("http");
var Static = require("node-static");
var moment = require("moment");

var files = new Static.Server(".", { cache: 0 });
Http.createServer((req, res) => {
    req.addListener("end", () => {
        var tmNow = new Date();
        console.info("%s HTTP %s %s", moment().format("HH:mm:ss"), req.method, req.url);
        files.serve(req, res);
    }).resume();
}).listen(8080, ()=>{
    console.log("http start in port 8080...");
});