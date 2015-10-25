oled = function() {
  var canvas, ctx, x, y, fill, clear, data,
    device, token,
    buffer = new Array(),
    SCALE = 8,
    WIDTH = 128,
    HEIGHT = 64;

  function initCanvas() {
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
    ctx.scale(SCALE, SCALE);
    ctx.fillRect(0, 0, WIDTH, HEIGHT);
  }

  function initListeners() {
    canvas.addEventListener("mousemove", function(event) {
      x = parseInt(event.layerX / SCALE);
      y = parseInt(event.layerY / SCALE);
      if (!fill && !clear) {
        return;
      }
      draw();
    });

    canvas.addEventListener("mousedown", function(event) {
      if (event.button === 0) {
        fill = true;
      } else if (event.button === 2) {
        clear = true;
      }
      draw();
    });

    canvas.addEventListener("mouseup", function(event) {
      fill = false;
      clear = false;
    });

    canvas.addEventListener("mouseout", function(event) {
      fill = false;
      clear = false;
    });

    canvas.addEventListener('contextmenu', function(event) {
      event.preventDefault();
    });
  }

  function initData() {
    var x, y;
    data = new Array(WIDTH);
    for (x = 0; x < WIDTH; x++) {
      data[x] = new Array(HEIGHT);
      for (y = 0; y < HEIGHT; y++) {
        data[x][y] = {
          changed: false,
          value: false
        };
      }
    }
  }

  function initScanTimer() {
    setInterval(function() {
      scanData();
    }, 1000);
  }

  function draw() {
    if (fill) {
      ctx.clearRect(x, y, 1, 1);
    } else if (clear) {
      ctx.fillRect(x, y, 1, 1);
    }
    updateMatrix(fill);
  }

  function updateMatrix(value) {
    var obj = data[x][y];
    if (obj.value === value) {
      return;
    }
    obj.changed = true;
    obj.value = fill;
  }

  // Maximum data size is 63 characters.
  function scanData() {
    var x, y,
      nbrPixels = 0,
      change = '';
    for (x = 0; x < WIDTH; x++) {
      for (y = 0; y < HEIGHT; y++) {
        if (data[x][y].changed) {
          change += encodePixel(x, y, data[x][y].value);
          data[x][y].changed = false;
          nbrPixels++;
          if (nbrPixels === 10) {
            buffer.push(change);
            change = '';
            nbrPixels = 0;
          }
        }
      }
    }
    if (change) {
      buffer.push(change);
    }
  }

  function encodePixel(x, y, value) {
    var result, tmp, start;
    tmp = '00' + x;
    start = tmp.length - 3
    result = tmp.substring(start, start + 3);
    tmp = '0' + y;
    start = tmp.length - 2;
    result += tmp.substring(start, start + 2);
    result += value ? 1 : 0;
    return result;
  }

  function sendData() {
    if (buffer[0]) {
      var myRequest = new XMLHttpRequest();
      myRequest.open("POST", "https://api.particle.io/v1/devices/" + device + "/draw", true);
      myRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
      myRequest.send("access_token=" + token + "&args=" + buffer[0]);
      buffer.shift();
    }

    setTimeout(function() {
      sendData()
    }, 100);
  }

  function clearCanvas() {
    ctx.fillRect(0, 0, WIDTH, HEIGHT);
    initData();
    var myRequest = new XMLHttpRequest();
    myRequest.open("POST", "https://api.particle.io/v1/devices/" + device + "/clear", true);
    myRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    myRequest.send("access_token=" + token);
  }

  function init(deviceId, tokenId) {
    device = deviceId;
    token = tokenId;
    initCanvas();
    initData();
    initListeners();
    initScanTimer();
    sendData();
  }

  return {
    init: init,
    clear: clearCanvas
  };

}();
