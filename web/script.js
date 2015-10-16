oled = function() {
	var canvas, ctx, x, y, fill, clear, SCALE = 8;

	function initCanvas() {
		canvas = document.getElementById('canvas');
		ctx = canvas.getContext('2d');
		ctx.scale(SCALE, SCALE);
	}

	function initListeners() {
		canvas.addEventListener("mousemove", function(event) {
			x = parseInt(event.layerX / SCALE),
			y = parseInt(event.layerY / SCALE);
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

		canvas.addEventListener('contextmenu', function(event) {
			event.preventDefault();
		});
	}

	function initTimer() {}

	function draw() {
		if (fill) {
			ctx.fillRect(x, y, 1, 1);
		} else if (clear) {
			ctx.clearRect(x, y, 1, 1);
		}
	}

	function init() {
		initCanvas();
		initListeners();
		initTimer();
	}

	return {
		init: init
	};

}();
