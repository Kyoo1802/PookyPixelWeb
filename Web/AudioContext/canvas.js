class CanvasInfo {
    constructor(ledXCanvas, w, h) {
        this.myCanvas = ledXCanvas;
        this.myCtx = ledXCanvas.getContext('2d', { willReadFrequently: true });
        this.smallCanvas = document.createElement('canvas');
        this.smallCtx = this.smallCanvas.getContext('2d', { willReadFrequently: true });
        this.smallCtx.width = 1;
        this.smallCtx.height = 1;
        this.setSize(w, h);
        this.lastTick = 0;
        this.framesPassed = 1;
        this.display = {
            x: 0,
            y: 0,
            w: 100,
            h: 100
        };
        this.matrixW = 1;
        this.matrixH = 1;
        this.showData = [];
    }
    tick(newTick, framesPassed) {
        this.lastTick = newTick;
        this.framesPassed = framesPassed;
    }
    setSize(w, h) {
        this.w = w;
        this.h = h;
        this.myCanvas.width = w;
        this.myCanvas.height = h;
        this.myCtx.width = w;
        this.myCtx.height = h;
    }
    setMatrixSize(w, h) {
        this.matrixW = w;
        this.matrixH = h;
        this.smallCtx.width = w;
        this.smallCtx.height = h;
        this.smallCanvas.width = w;
        this.smallCanvas.height = h;
        this.smallCtx.scale(this.matrixW / this.w, this.matrixH / this.h);
    }
    setDisplay(x, y, w, h) {
        this.display.x = x;
        this.display.y = y;
        this.display.w = w;
        this.display.h = h;
    }
    computeScaleWidth() {
        return this.w / this.matrixW;
    }
    computeScaleHeight() {
        return this.h / this.matrixH;
    }
    copyCanvas() {
        if (this.matrixH && this.matrixW && this.matrixH > 0 && this.matrixW > 0) {
            // let a = performance.now();
            this.smallCtx.drawImage(this.myCanvas, 0, 0);
            // console.log("drawImage: `"+(performance.now()-a));
            // let a = performance.now();
            this.showData = this.smallCtx.getImageData(0, 0, this.matrixW, this.matrixH).data;
            // console.log("getImageData: " + (performance.now() - a));
        }
    }
}

class ViewInfo {
    constructor() {
        this.posX = 0;
        this.posY = 0;
        this.size = 100;
    }
    setPosX(x) {
        this.posX = x;
    }
    setPosY(y) {
        this.posY = y;
    }
    setSize(s) {
        this.size = s;
    }
}

class BkgPainter {
    constructor(canvasInfo) {
        this.type = "COLOR";
        this.colorState = 'SOLID';
        this.startColor = '#000000';
        this.endColor = '#000000';
        this.animate = false;
        this.cameraState = 'STOP';
        this.videoScreen = null;
        this.screenState = 'STOP';
        this.audioStream = null;
        this.audioState = 'STOP'
        this.viewInfo = new ViewInfo();
        this.animatePercentage = 0;
    }
    setType(type) {
        this.type = type;
    }
    setColorState(state) {
        this.colorState = state;
    }
    setAnimate(state) {
        this.animate = state;
        this.animatePercentage = 0;
        this.gradientTransition = 0;
    }
    setStartColor(color) {
        this.startColor = color;
    }
    setEndColor(color) {
        this.endColor = color;
    }
    setLocalVideoScreen(videoScreen) {
        this.type = 'LOCAL_VIDEO';
        this.videoScreen = videoScreen;
    }
    setCameraScreen(videoScreen) {
        this.type = 'CAMERA';
        this.videoScreen = videoScreen;
    }
    setVideoScreen(videoScreen) {
        this.type = 'SCREEN';
        this.videoScreen = videoScreen;
    }
    setScreenState(state) {
        this.audioState = 'STOP';
        this.screenState = state;
    }
    setAudioStream(type, stream, analyzer) {
        this.type = type;
        this.audioStream = stream;
        this.audioAnalyzer = analyzer;
        this.audioBuffer = new Uint8Array(this.audioAnalyzer.frequencyBinCount);
        this.validAudioBuffer = this.type === 'MIC' ? this.audioBuffer.length * .28 : this.audioBuffer.length * .8;
    }
    setAudioState(state) {
        this.screenState = 'STOP';
        this.audioState = state;
    }
    stopStream() {
        this.screenState = 'STOP';
        this.audioState = 'STOP';
        this.audioStream = null;
        this.audioAnalyzer = null;
        this.type = 'COLOR';
    }
    sendCommand(command, key, value, secondValue, thirdValue) {
        if (command === "BkgPainter") {
            if (key === "setType") {
                this.setType(value);
            } else if (key === "setStartColor") {
                this.setStartColor(value);
            } else if (key === "setEndColor") {
                this.setEndColor(value);
            } else if (key === "setAnimate") {
                this.setAnimate(value);
            } else if (key === "setColorState") {
                this.setColorState(value);
            } else if (key === "setCameraScreen") {
                this.setCameraScreen(value);
            } else if (key === "setLocalVideoScreen") {
                this.setLocalVideoScreen(value);
            } else if (key === "setVideoScreen") {
                this.setVideoScreen(value);
            } else if (key === "setScreenState") {
                this.setScreenState(value);
            } else if (key === "setAudioStream") {
                this.setAudioStream(value, secondValue, thirdValue);
            } else if (key === "setAudioState") {
                this.setAudioState(value);
            } else if (key === "stopStream") {
                this.stopStream();
            } else if (key === "setViewX") {
                this.viewInfo.setPosX(value);
            } else if (key === "setViewY") {
                this.viewInfo.setPosY(value);
            } else if (key === "setViewSize") {
                this.viewInfo.setSize(value);
            }
        }
    }
    init() { }
    paint(canvasInfo) {
        if (this.colorState === "SOLID") {
            if (this.animate === true) {
                this.animatePercentage = (this.animatePercentage + 1) % 360;
                let hsl = RGBtoHSL(HEXToRGB(this.startColor));
                let rgb = HSLtoRGBWithDeltaHue(hsl, this.animatePercentage);
                canvasInfo.myCtx.fillStyle = RGBToHEX(rgb);
            } else {
                canvasInfo.myCtx.fillStyle = this.startColor;
            }
            canvasInfo.myCtx.fillRect(0, 0, canvasInfo.w, canvasInfo.h);
        } else if (this.colorState === "LINEAL") {
            let min = Math.min(canvasInfo.h, canvasInfo.w);
            let grd =
                canvasInfo.myCtx.createLinearGradient(0, min, 0, 0);
            if (this.animate === true) {
                this.animatePercentage = (this.animatePercentage + 1) % 360;
                let hsl = RGBtoHSL(HEXToRGB(this.startColor));
                let rgb = HSLtoRGBWithDeltaHue(hsl, this.animatePercentage);
                grd.addColorStop(0, RGBToHEX(rgb));
                grd.addColorStop(1, this.endColor);
                canvasInfo.myCtx.fillStyle = grd;
            } else {
                grd.addColorStop(0, this.startColor);
                grd.addColorStop(1, this.endColor);
                canvasInfo.myCtx.fillStyle = grd;
            }
            canvasInfo.myCtx.fillRect(0, 0, canvasInfo.w, canvasInfo.h);
        } else if (this.colorState === "RADIAL") {
            let min = Math.min(canvasInfo.h, canvasInfo.w);
            let grd =
                canvasInfo.myCtx.createRadialGradient(canvasInfo.w / 2, canvasInfo.h / 2, min / 10, canvasInfo.w / 2, canvasInfo.h / 2, min);
            if (this.animate === true) {
                this.animatePercentage = (this.animatePercentage + 1) % 360;
                let hsl = RGBtoHSL(HEXToRGB(this.startColor));
                let rgb = HSLtoRGBWithDeltaHue(hsl, this.animatePercentage);
                grd.addColorStop(0, RGBToHEX(rgb));
                grd.addColorStop(1, this.endColor);
            } else {
                grd.addColorStop(0, this.startColor);
                grd.addColorStop(1, this.endColor);
            }
            canvasInfo.myCtx.fillStyle = grd;
            canvasInfo.myCtx.fillRect(0, 0, canvasInfo.w, canvasInfo.h);
        }
        if (this.videoScreen &&
            (this.type === "CAMERA" || (this.type === "SCREEN") && this.screenState === 'PLAY')) {
            let videoSettings = this.videoScreen.srcObject.getVideoTracks()[0].getSettings();
            let w = this.viewInfo.size / 100 * videoSettings.width;
            let h = this.viewInfo.size / 100 * videoSettings.height;
            let x = this.viewInfo.posX / 100 * videoSettings.width;
            x = Math.min(x, videoSettings.width - w);
            let y = this.viewInfo.posY / 100 * videoSettings.height;
            y = Math.min(y, videoSettings.height - h);
            canvasInfo.myCtx.clearRect(0, 0, canvasInfo.w, canvasInfo.h);
            if (videoSettings.width > videoSettings.height) {
                canvasInfo.myCtx.drawImage(this.videoScreen, x, y, w, h, 0, 0, canvasInfo.w, canvasInfo.h);
            } else {
                canvasInfo.myCtx.drawImage(this.videoScreen, x, y, w, h, canvasInfo.w / 4 - 1, 0, canvasInfo.w / 2, canvasInfo.h);
            }
        } else if (this.videoScreen && this.type === "LOCAL_VIDEO" && this.screenState === 'PLAY') {
            canvasInfo.myCtx.clearRect(0, 0, canvasInfo.w, canvasInfo.h);
            let ratio = this.viewInfo.size / 100;
            let ratioX = (this.viewInfo.posX + 100) / 100 - 1;
            let ratioY = (this.viewInfo.posY + 100) / 100 - 1;
            let sw = this.videoScreen.videoWidth;
            let sh = this.videoScreen.videoHeight;
            let dw = canvasInfo.w;
            let dh = canvasInfo.h;
            canvasInfo.myCtx.save();

            canvasInfo.myCtx.rotate(Math.PI * 3 / 2);
            canvasInfo.myCtx.drawImage(this.videoScreen, 0, 0, sw, sh, dh * ratioY, dw * ratioX, dw * ratio, (dw * ratio) / sw * sh);
            canvasInfo.myCtx.restore();
        } else if ((this.type === 'AUDIO' || this.type === 'MIC') && this.audioStream) {
            canvasInfo.myCtx.lineCap = 'round';
            if (this.audioState === 'FREQUENCY') {
                this.audioAnalyzer.getByteFrequencyData(this.audioBuffer);
                let x = 0;
                const barWidth = canvasInfo.w / this.validAudioBuffer;
                canvasInfo.myCtx.beginPath();
                for (let i = 0; i <= this.validAudioBuffer; i++) {
                    const ratio = i / this.validAudioBuffer;
                    const percent = this.audioBuffer[i] / 255 * (.88 + ratio * .82);
                    const [h, s, l] = [ratio, 0.8, 0.5];
                    const [r, g, b] = hslToRgb(h, s, l);
                    canvasInfo.myCtx.fillStyle = `rgb(${r},${g},${b})`;
                    let barHeight = (canvasInfo.h * percent);
                    canvasInfo.myCtx.fillRect(x, canvasInfo.h - barHeight, barWidth, barHeight);
                    x += barWidth;
                }
                canvasInfo.myCtx.stroke();
            } else {
                this.audioAnalyzer.getByteTimeDomainData(this.audioBuffer);
                const sliceWidth = canvasInfo.w / this.audioBuffer.length + 1;
                canvasInfo.myCtx.lineWidth = sliceWidth;
                canvasInfo.myCtx.beginPath();
                canvasInfo.myCtx.moveTo(0, canvasInfo.h / 2);
                let x = 0;
                for (let i = 0; i < this.audioBuffer.length; i++) {
                    let v = this.audioBuffer[i] / 128;
                    const diff = 3.4 * (v - 1);
                    v = 1 + diff;
                    const y = (v * canvasInfo.h) / 2;

                    canvasInfo.myCtx.moveTo(x, canvasInfo.h / 2);
                    canvasInfo.myCtx.lineTo(x, y);
                    x += sliceWidth + 1;
                }
                const grd = canvasInfo.myCtx.createLinearGradient(0, 0, canvasInfo.w, 0);
                grd.addColorStop(0, "red");
                grd.addColorStop(0.15, "yellow");
                grd.addColorStop(0.3, "green");
                grd.addColorStop(0.45, "aqua");
                grd.addColorStop(0.6, "blue");
                grd.addColorStop(0.7, "fuchsia");
                grd.addColorStop(1, "red");
                canvasInfo.myCtx.strokeStyle = grd;
                canvasInfo.myCtx.stroke();
            }
        }
    }
}

function RGBtoHSL(color) {
    let r = color[0] / 255;
    let g = color[1] / 255;
    let b = color[2] / 255;

    let cmin = Math.min(r, g, b),
        cmax = Math.max(r, g, b),
        delta = cmax - cmin,
        h = 0,
        s = 0,
        l = 0;
    if (delta == 0)
        h = 0;
    else if (cmax == r)
        h = ((g - b) / delta) % 6;
    else if (cmax == g)
        h = (b - r) / delta + 2;
    else
        h = (r - g) / delta + 4;
    h = Math.round(h * 60);
    if (h < 0)
        h += 360;
    l = (cmax + cmin) / 2;
    s = delta == 0 ? 0 : delta / (1 - Math.abs(2 * l - 1));
    s = +s * 100;
    l = +l * 100;
    return [h, s, l];
}

function hslToRgb(h, s, l) {
    let r;
    let g;
    let b;
    if (s == 0) {
        r = g = b = l; // achromatic
    } else {
        const hue2rgb = function hue2rgb(p, q, t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1 / 6) return p + (q - p) * 6 * t;
            if (t < 1 / 2) return q;
            if (t < 2 / 3) return p + (q - p) * (2 / 3 - t) * 6;
            return p;
        };
        const q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        const p = 2 * l - q;
        r = hue2rgb(p, q, h + 1 / 3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1 / 3);
    }
    return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}

function HSLtoRGBWithDeltaHue(color, hd) {
    let h = (color[0] + hd) % 360;
    let s = color[1] / 100;
    let l = color[2] / 100;
    let c = (1 - Math.abs(2 * l - 1)) * s;
    let x = c * (1 - Math.abs((h / 60) % 2 - 1));
    let m = l - c / 2;
    let r = 0;
    let g = 0;
    let b = 0;
    if (h < 0) { } else if (h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h < 300) {
        r = x;
        g = 0;
        b = c;
    } else if (h < 360) {
        r = c;
        g = 0;
        b = x;
    }
    r = Math.round((r + m) * 255);
    g = Math.round((g + m) * 255);
    b = Math.round((b + m) * 255);
    return [r, g, b];
}
function HSLtoRGB(color, hd, sd, ld) {
    let h = color[0] * hd;
    let s = color[1] / 100 * sd;
    let l = color[2] / 100 * ld;
    let c = (1 - Math.abs(2 * l - 1)) * s;
    let x = c * (1 - Math.abs((h / 60) % 2 - 1));
    let m = l - c / 2;
    let r = 0;
    let g = 0;
    let b = 0;
    if (h < 0) { } else if (h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h < 300) {
        r = x;
        g = 0;
        b = c;
    } else if (h < 360) {
        r = c;
        g = 0;
        b = x;
    }
    r = Math.round((r + m) * 255);
    g = Math.round((g + m) * 255);
    b = Math.round((b + m) * 255);
    return [r, g, b];
}
function HEXToRGB(h) {
    let r = 0, g = 0, b = 0;
    if (h.length == 4) {
        r = "0x" + h[1] + h[1];
        g = "0x" + h[2] + h[2];
        b = "0x" + h[3] + h[3];
    } else if (h.length == 7) {
        r = "0x" + h[1] + h[2];
        g = "0x" + h[3] + h[4];
        b = "0x" + h[5] + h[6];
    }
    return [r, g, b];
}
function RGBToHEX(rgb) {
    let r = rgb[0].toString(16);
    let g = rgb[1].toString(16);
    let b = rgb[2].toString(16);
    if (r.length == 1)
        r = "0" + r;
    if (g.length == 1)
        g = "0" + g;
    if (b.length == 1)
        b = "0" + b;
    return "#" + r + g + b;
}
class FilterPainter {
    constructor(canvasInfo) {
        this.type = "NONE";
        this.huePercentage = 50;
        this.saturationPercentage = 50;
        this.lightnessPercentage = 50;
        this.strategyFilter = null;
        this.canvasInfo = canvasInfo;
    }
    init(canvasInfo) {
        this.canvasInfo = canvasInfo;
    }
    setType(type) {
        this.type = type;
        if (this.type === "NONE") {
            this.strategyFilter = null;
        } else {
            if (this.type === "FILTER-0") {
                this.strategyFilter = new StarBkg();
            } else if (this.type === "FILTER-1") {
                this.strategyFilter = new HeartBkg();
            } else if (this.type === "FILTER-2") {
                this.strategyFilter = new RainBkg();
            }
            this.strategyFilter.init(this.canvasInfo);
        }
    }
    sendCommand(command, key, value) {
        if (command === "FilterPainter") {
            if (key === "setType") {
                this.setType(value);
            }
        }
    }
    paint(canvasInfo, time) {
        if (this.strategyFilter) {
            this.strategyFilter.paint(canvasInfo, time);
        }
    }
}

class RainBkg {
    constructor() {
        this.numRain = 200;
        this.speed = 15;
        this.y = [];
    }
    init(canvasInfo) {
        for (let i = 0; i < this.numRain; ++i) {
            this.y.push((Math.random() - .5) * canvasInfo.h);
        }
    }

    paint(canvasInfo, time) {
        this.randomSeed_ = 0;

        canvasInfo.myCtx.fillStyle = "#FFF";
        for (let i = 0; i < this.numRain; ++i) {
            const x = i * 10;
            this.y[i] = (this.y[i] + this.speed * canvasInfo.framesPassed) % canvasInfo.h;
            canvasInfo.myCtx.fillRect(x, this.y[i], 3, 8);
        }
    }
}

class Star {
    constructor(canvasInfo, r, c) {
        this.r = r;
        this.c = c;
        this.arrStars = [];
        this.rChange = 0.065;
        this.setRandomPosition(canvasInfo.w, canvasInfo.h);
    }
    setRandomPosition(w, h) {
        this.x = Math.floor((Math.random() * w) + 1);
        this.y = Math.floor((Math.random() * h) + 1);
    }
    update(canvasInfo) {
        if (this.r > 3.5 || this.r < .5) {
            this.rChange = -this.rChange;
            if (this.r < .8 && Math.random() < .5) {
                this.setRandomPosition(canvasInfo.w, canvasInfo.h);
            }
        }
        this.r += this.rChange * canvasInfo.framesPassed;
        if (this.r > 0) {
            canvasInfo.myCtx.beginPath();
            canvasInfo.myCtx.arc(this.x, this.y, this.r, 0, 2 * Math.PI, false);
            canvasInfo.myCtx.fillStyle = this.c;
            canvasInfo.myCtx.fill();
        }
    }
}

class StarBkg {
    constructor() {
        this.color = "#ffecd3";
        this.arrStars = [];
        this.numStars = 100;
    }
    init(canvasInfo) {
        for (let i = 0; i < this.numStars; i++) {
            let randR = Math.random() * 2 + .5;
            let star = new Star(canvasInfo, randR, this.color);
            this.arrStars.push(star);
        }
    }
    paint(canvasInfo, time) {
        for (let i = 0; i < this.arrStars.length; i++) {
            this.arrStars[i].update(canvasInfo);
        }
    }
}

class Particle {
    constructor(canvasInfo) {
        this.a = -90;
        this.array = ['#FFB8F7', '#D581CB', 'magenta'];
        this.reset(canvasInfo);
    }

    reset(canvasInfo) {
        this.color = this.array[Math.floor(Math.random() * this.array.length)];
        this.r = Math.floor(Math.random() * 70) + 10;
        this.vy = Math.floor(Math.random() * 4) + 4;
        this.life = Math.random() * 30;
        this.x = Math.random() * canvasInfo.w;
        this.y = Math.random() * canvasInfo.h + canvasInfo.h;
    }
}
class HeartBkg {
    constructor() {
        this.rad = (Math.PI / 180);
        this.nbOfParticles = 12;
    }
    init(canvasInfo) {
        this.particles = [];
        for (let i = 0; i < this.nbOfParticles; i++) {
            this.particles.push(new Particle(canvasInfo));
        }
    }
    paint(canvasInfo, time) {
        for (let t = 0; t < this.particles.length; t++) {
            let p = this.particles[t];
            canvasInfo.myCtx.beginPath();
            canvasInfo.myCtx.fillStyle = p.color;
            let x1 = p.x + p.r * Math.cos(p.a * this.rad);
            let y1 = p.y + p.r * Math.sin(p.a * this.rad);
            let cx1 = p.x + p.r * Math.cos((p.a + 22.5) * this.rad);
            let cy1 = p.y + p.r * Math.sin((p.a + 22.5) * this.rad);
            let cx2 = p.x + p.r * Math.cos((p.a - 22.5) * this.rad);
            let cy2 = p.y + p.r * Math.sin((p.a - 22.5) * this.rad);
            let chord = 2 * p.r * Math.sin(22.5 * this.rad / 2);

            canvasInfo.myCtx.beginPath();
            canvasInfo.myCtx.moveTo(x1, y1);
            canvasInfo.myCtx.arc(cx1, cy1, chord, (270 + p.a) * this.rad, (270 + p.a + 225) * this.rad);
            canvasInfo.myCtx.lineTo(p.x, p.y);
            canvasInfo.myCtx.moveTo(x1, y1);
            canvasInfo.myCtx.arc(cx2, cy2, chord, (90 + p.a) * this.rad, (90 + p.a + 135) * this.rad, true);
            canvasInfo.myCtx.lineTo(p.x, p.y);
            canvasInfo.myCtx.fill();
            p.y -= p.vy * canvasInfo.framesPassed;
            p.life *= 0.8;
            if (p.y < 0) {
                p.reset(canvasInfo);
            }
        }
    }
}

class TextPainter {
    constructor(canvasInfo) {
        this.text = "";
        this.fontType = 'Arial';
        this.startColor = 'red';
        this.midColor = 'green';
        this.endColor = 'blue';
        this.fontSizePercentage = 100;
        this.speedPercentage = 30;
        this.posPercentage = 100;
        this.initX = canvasInfo.w;
        this.x = this.initX;
        this.deltaX = 0;
        this.test = 0;
    }
    init() { }
    setText(text) {
        this.text = text;
    }
    setTextFontType(fontType) {
        this.fontType = fontType;
    }
    setStartColor(color) {
        this.startColor = color;
    }
    setMidColor(color) {
        this.midColor = color;
    }
    setEndColor(color) {
        this.endColor = color;
    }
    setTextSpeedPercentage(speedP) {
        this.speedPercentage = speedP;
    }
    setTextSizePercentage(sizePercentage) {
        this.fontSizePercentage = sizePercentage;
    }
    setTextPosPercentage(posPercentage) {
        this.posPercentage = posPercentage;
    }
    sendCommand(command, key, value) {
        if (command === "TextPainter") {
            if (key === "setText") {
                this.setText(value);
            } else if (key === "setTextFontType") {
                this.setTextFontType(value);
            } else if (key === "setStartColor") {
                this.setStartColor(value);
            } else if (key === "setMidColor") {
                this.setMidColor(value);
            } else if (key === "setEndColor") {
                this.setEndColor(value);
            } else if (key === "setTextSpeedPercentage") {
                this.setTextSpeedPercentage(value);
            } else if (key === "setTextSizePercentage") {
                this.setTextSizePercentage(value);
            } else if (key === "setTextPosPercentage") {
                this.setTextPosPercentage(value);
            }
        }
    }
    paint(canvasInfo) {
        if (this.text == "") {
            return;
        }
        let grd = canvasInfo.myCtx.createLinearGradient(0, 0, canvasInfo.w, 0);
        grd.addColorStop("0", this.startColor);
        grd.addColorStop("0.5", this.midColor);
        grd.addColorStop("1", this.endColor);
        canvasInfo.myCtx.fillStyle = grd;
        let fontSize = this.fontSizePercentage * canvasInfo.h / 72;
        canvasInfo.myCtx.font = `bold ${fontSize}px ${this.fontType}`;
        canvasInfo.myCtx.fillText(this.text, this.x, this.posPercentage * canvasInfo.h / 100);
        let txtSize = canvasInfo.myCtx.measureText(this.text).width;
        this.x =
            this.x < -txtSize ?
                canvasInfo.w :
                this.x - this.speedPercentage * .15 * canvasInfo.framesPassed;
    }
}

class BorderPainter {
    constructor(wsConnection) {
        this.wsC = wsConnection;
        this.enabled = false;
    }
    init() { }
    sendCommand(command, key, value) {
        if (command === "BorderPainter") {
            if (key === "setEnabled") {
                this.enabled = value;
            }
        }
    }
    paint(canvasInfo, time) {
        if (!this.enabled) {
            return;
        }
        this.wsC.getNodes().forEach((driverNode, nodeId) => {
            for (let k = 0; !driverNode.isLost && k < driverNode.metadata.channels; k++) {
                if (!driverNode.masterTemplate || !driverNode.masterTemplate.channels[k]) {
                    continue;
                }
                let channel = driverNode.masterTemplate.channels[k];
                for (let j = 0; j < channel.modules.length; j++) {
                    if (!channel.modules[j]) {
                        continue;
                    }
                    canvasInfo.myCtx.lineWidth = 1;
                    const module = channel.modules[j];
                    const pixModule = module.getBorderModule();
                    const scaleW = canvasInfo.computeScaleWidth();
                    const scaleH = canvasInfo.computeScaleHeight();
                    const sPixX = pixModule.x * scaleW;
                    const sPixY = pixModule.y * scaleH;
                    const sPixW = pixModule.w * scaleW;
                    const sPixH = pixModule.h * scaleH;
                    const sX = module.x * scaleW;
                    const sY = module.y * scaleH;
                    canvasInfo.myCtx.strokeStyle = j == 0 ? 'gold' : 'aqua';
                    this.moduleGrid(canvasInfo.myCtx, module, sPixX, sPixY, sPixW, sPixH, sX, sY, scaleH, scaleW);

                    if (module.w >= 3 && module.h >= 3) {
                        const offsetX = (module.d == ModuleDirection.ZIGZAGV_AD || module.d == ModuleDirection.ZIGZAGV_CB
                            || module.d == ModuleDirection.ZIGZAGH_AD || module.d == ModuleDirection.ZIGZAGH_CB)
                            ? 1 : -1;
                        const offsetY = (module.d == ModuleDirection.ZIGZAGV_AD || module.d == ModuleDirection.ZIGZAGV_BC
                            || module.d == ModuleDirection.ZIGZAGH_AD || module.d == ModuleDirection.ZIGZAGH_BC)
                            ? 1 : -1;
                        const cX = sPixX + offsetX * scaleW / 2;
                        const cY = sPixY + offsetY * scaleH / 2;
                        const cW = sPixW - offsetX * scaleW;
                        const cH = sPixH - offsetY * scaleH;
                        canvasInfo.myCtx.lineWidth = 3;
                        canvasInfo.myCtx.strokeStyle = 'green';
                        canvasInfo.myCtx.beginPath();
                        canvasInfo.myCtx.arc(cX, cY, Math.min(scaleW, scaleH) / 2, 0, 2 * Math.PI, false);
                        canvasInfo.myCtx.stroke()
                        canvasInfo.myCtx.strokeStyle = 'red';
                        canvasInfo.myCtx.beginPath();
                        canvasInfo.myCtx.moveTo(cX, cY);

                        if (module.isVerticalDirection()) {
                            const dirSign = (module.d == ModuleDirection.ZIGZAGV_AD || module.d == ModuleDirection.ZIGZAGV_CB)
                                ? 1 : -1;
                            canvasInfo.myCtx.lineTo(cX, cY + cH);
                            canvasInfo.myCtx.lineTo(cX + dirSign * scaleW, cY + cH);
                            canvasInfo.myCtx.lineTo(cX + dirSign * scaleW, cY);
                            canvasInfo.myCtx.lineTo(cX + dirSign * 2 * scaleW, cY);
                            canvasInfo.myCtx.lineTo(cX + dirSign * 2 * scaleW, cY + cH);
                            this.arrow(canvasInfo.myCtx, cX + dirSign * 2 * scaleW, cY, cX + dirSign * 2 * scaleW, cY + cH);
                        } else {
                            const dirSign = (module.d == ModuleDirection.ZIGZAGH_AD || module.d == ModuleDirection.ZIGZAGH_BC)
                                ? 1 : -1;
                            canvasInfo.myCtx.lineTo(cX + cW, cY);
                            canvasInfo.myCtx.lineTo(cX + cW, cY + dirSign * scaleH);
                            canvasInfo.myCtx.lineTo(cX, cY + dirSign * scaleH);
                            canvasInfo.myCtx.lineTo(cX, cY + dirSign * 2 * scaleH);
                            canvasInfo.myCtx.lineTo(cX + cW, cY + dirSign * 2 * scaleH);
                            this.arrow(canvasInfo.myCtx, cX, cY + dirSign * 2 * scaleH, cX + cW, cY + dirSign * 2 * scaleH);
                        }
                        canvasInfo.myCtx.stroke()
                    }
                }
            }
        });
    }
    moduleGrid(ctx, module, sPixX, sPixY, sPixW, sPixH, sX, sY, scaleH, scaleW) {
        ctx.beginPath();
        ctx.strokeRect(sPixX, sPixY, sPixW, sPixH);
        ctx.stroke()
        ctx.beginPath();
        ctx.strokeStyle = 'gray';
        for (let r = 1; r < module.h; r++) {
            const startX = sX;
            const startY = sY + r * scaleH;
            ctx.moveTo(startX, startY);
            const endX = sX + module.w * scaleW;
            const endY = startY;
            ctx.lineTo(endX, endY);
        }
        for (let c = 1; c < module.w; c++) {
            const startX = sX + c * scaleW;
            const startY = sY;
            ctx.moveTo(startX, startY);
            const endX = startX;
            const endY = sY + module.h * scaleH;
            ctx.lineTo(endX, endY);
        }
        ctx.stroke()
    }
    arrow(context, fromx, fromy, tox, toy) {
        var headlen = 10; // length of head in pixels
        var dx = tox - fromx;
        var dy = toy - fromy;
        var angle = Math.atan2(dy, dx);
        context.moveTo(fromx, fromy);
        context.lineTo(tox, toy);
        context.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));
        context.moveTo(tox, toy);
        context.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));
    }
}
class FPS {
    constructor() {
        this.frameRender = 0;
        this.msgSent = 0;
        this.msgReceived = 0;
        this.avgLatency = 0;
        this.avgBs = 0;
        this.tTime = 0;
        this.cTime = 0;
        this.deviceInfo = {
            fps: 0,
            fullLatency: 0,
            KBs: 0,
            lastDriverMsgTime: 0,
            driverMsgSent: 0,
            driverMsgReceived: 0,
            driverUploadKbs: 0,
            driverDownloadKbs: 0,
            driverStatus: 2,
            update: false,
        }
    }
    tick(deviceInfo) {
        this.deviceInfo.lastDriverMsgTime = Date.now();
        this.latency = this.cTime > 0 ? performance.now() - this.cTime : 0;
        this.avgLatency = this.avgLatency > 0 ? (this.avgLatency + this.latency) / 2 : this.latency;
        this.frameRender++;
        this.cTime = performance.now();
        if (performance.now() - this.tTime > 1000) {
            this.deviceInfo.driverStatus = deviceInfo.status;
            this.deviceInfo.fps = this.frameRender;
            this.deviceInfo.driverUploadKbs = (deviceInfo.uploadBytes / 1024).toFixed(3);
            this.deviceInfo.driverDownloadKbs = (deviceInfo.downloadBytes / 1024).toFixed(3);
            this.deviceInfo.driverMsgSent = deviceInfo.msgSent;
            this.deviceInfo.driverMsgReceived = deviceInfo.msgReceived;
            this.deviceInfo.fullLatency = Math.round(this.avgLatency * 100) / 100;
            this.deviceInfo.update = true;
            this.deviceInfo.ip = deviceInfo.ip;
            this.frameRender = 0;
            this.msgSent = 0;
            this.msgReceived = 0;
            this.avgLatency = 0;
            this.avgKBs = 0;
            this.tTime = performance.now();
            deviceInfo.downloadBytes = 0;
            deviceInfo.uploadBytes = 0;
            deviceInfo.msgSent = 0;
            deviceInfo.msgReceived = 0;
        }
        return this.deviceInfo;
    }
}
class CanvasDrawer {
    constructor(myCanvas, canvasWidth, canvasHeight, wsConnection, uiNotification) {
        this.canvasInfo = new CanvasInfo(myCanvas, canvasWidth, canvasHeight);
        this.wsConnection = wsConnection;
        this.uiNotification = uiNotification;
        this.painters = [];
        this.painters[0] = new BkgPainter(this.canvasInfo);
        this.painters[1] = new FilterPainter(this.canvasInfo);
        this.painters[2] = new TextPainter(this.canvasInfo);
        this.painters[3] = new BorderPainter(this.wsConnection);
        this.fpsStats = new FPS();
        this.fps = 20;
    }
    init() {
        for (let painter of this.painters) {
            painter.init(this.canvasInfo);
            break;
        }
    }
    setFps(fps) {
        this.fps = fps;
    }
    sendCommand(command, key, value, value2, value3) {
        for (let painter of this.painters) {
            painter.sendCommand(command, key, value, value2, value3);
        }
    }
    onMatrixResize(w, h) {
        if (!w || !h || w <= 0 || h <= 0) {
            return;
        }
        this.canvasInfo.setMatrixSize(w, h);
    }
    getMatrixWidth() {
        return this.canvasInfo.matrixW;
    }
    getMatrixHeight() {
        return this.canvasInfo.matrixH;
    }
    onResize(w, h) {
        this.canvasInfo.setSize(w, h);
        this.init();
    }
    paint() {
        const newTick = performance.now();
        const timeSpent = newTick - this.canvasInfo.lastTick;
        const framesPassed = timeSpent / (1000 / this.fps);
        if (framesPassed > .95) {
            let a = performance.now();
            this.canvasInfo.tick(newTick, framesPassed);
            for (let painter of this.painters) {
                painter.paint(this.canvasInfo, newTick);
            }
            this.canvasInfo.copyCanvas();
            const nodes = this.wsConnection.getNodes();
            [...nodes.keys()].forEach((node) => {
                let driver = nodes.get(node);
                let bin = pixelsToMsg(this.canvasInfo.showData, this.canvasInfo.matrixW, this.canvasInfo.matrixH, driver);
                if (this.wsConnection.getDeviceInfo().status == NetworkStatus.CONNECTED) {
                    this.wsConnection.sendWsMsg(bin);
                }
                this.uiNotification(this.fpsStats.tick(this.wsConnection.getDeviceInfo()), this.wsConnection.getNodes());
            });
            sss = performance.now();
            return true;
        }
        return false;
    }
}
var sss = 0;

function pixelsToMsg(pixelsD, matrixW, matrixH, driver) {
    if (pixelsD.length == 0) {
        return { command: 'PROCESS_NO_FRAME' };
    } else {
        let bin = new Uint8Array(driver.metadata.channels * driver.metadata.leds * 3 + driver.metadata.channels * 2);
        let binIdx = 0;
        let startChannel = 0;
        let ledCount = 0;
        for (let k = 0; !driver.isLost && k < driver.metadata.channels; k++) {
            binIdx += 2; // led size
            if (!driver.masterTemplate || !driver.masterTemplate.channels[k]) {
                continue;
            }
            let templateInfo = driver.masterTemplate.channels[k];
            let a = -1, b = -1
            for (let m = 0; m < templateInfo.modules.length; m++) {
                if (!templateInfo.modules[m]) {
                    continue;
                }
                let module = templateInfo.modules[m].getPixelModule();
                if (module.d == ModuleDirection.ZIGZAGV_AD || module.d == ModuleDirection.ZIGZAGV_BC
                    || module.d == ModuleDirection.ZIGZAGV_CB || module.d == ModuleDirection.ZIGZAGV_DA) {
                    for (let col = 0; module.w < 0 ? col > module.w : col < module.w; module.w < 0 ? col-- : col++) {
                        for (let row = 0; module.h < 0 ? row > module.h : row < module.h; module.h < 0 ? row-- : row++) {
                            let i = col % 2 == 0 ? module.y + row : module.y + module.h - row + (module.h < 0 ? 1 : -1);
                            let j = col + module.x;
                            if (a == i && b == j) {
                                console.log('repeated: ' + i + '---' + j);
                            }
                            a = i, b = j;
                            if (isInMatrix(i, j, matrixW, matrixH)) {
                                const matrixIdx = getMatrixIdx(i, j, matrixW);
                                const colorOrder = templateInfo.colorOrder;
                                binIdx = setMatrixColor(matrixIdx, pixelsD, bin, binIdx, colorOrder);
                            } else {
                                binIdx = setBlankColor(bin, binIdx);
                            }
                            ledCount++;
                        }
                    }
                } else if (module.d == ModuleDirection.ZIGZAGH_AD || module.d == ModuleDirection.ZIGZAGH_BC
                    || module.d == ModuleDirection.ZIGZAGH_CB || module.d == ModuleDirection.ZIGZAGH_DA) {
                    let dir = module.w > 0 ? true : false;
                    for (let row = 0; module.h < 0 ? row > module.h : row < module.h; module.h < 0 ? row-- : row++) {
                        for (let col = 0; module.w < 0 ? col > module.w : col < module.w; module.w < 0 ? col-- : col++) {
                            let j = dir ? col + module.x : module.x + module.w - col + (module.w < 0 ? 1 : -1);
                            let i = row + module.y;
                            if (isInMatrix(i, j, matrixW, matrixH)) {
                                const matrixIdx = getMatrixIdx(i, j, matrixW);
                                const colorOrder = templateInfo.colorOrder;
                                binIdx = setMatrixColor(matrixIdx, pixelsD, bin, binIdx, colorOrder);
                            } else {
                                binIdx = setBlankColor(bin, binIdx);
                            }
                            ledCount++;
                        }
                        dir = !dir;
                    }
                }
            }
            bin[startChannel] = parseInt(ledCount / 20);
            bin[startChannel + 1] = ledCount % 20;
            startChannel = binIdx;
            ledCount = 0;
        }
        return bin;
    }
}
function isInMatrix(i, j, matrixW, matrixH) {
    return !(i < 0 || i >= matrixH || j < 0 || j >= matrixW);
}
function getMatrixIdx(i, j, matrixW) {
    return i * matrixW * 4 + j * 4;
}
function setMatrixColor(matrixIdx, pixelsD, bin, binIdx, colorOrder) {
    r = pixelsD[matrixIdx];
    g = pixelsD[matrixIdx + 1];
    b = pixelsD[matrixIdx + 2];
    if (colorOrder == 'grb') {
        bin[binIdx++] = g;
        bin[binIdx++] = r;
        bin[binIdx++] = b;
    } else if (colorOrder == 'brg') {
        bin[binIdx++] = b;
        bin[binIdx++] = r;
        bin[binIdx++] = g;
    } else if (colorOrder == 'rbg') {
        bin[binIdx++] = r;
        bin[binIdx++] = b;
        bin[binIdx++] = g;
    } else {
        bin[binIdx++] = r;
        bin[binIdx++] = g;
        bin[binIdx++] = b;
    }
    return binIdx;
}
function setBlankColor(bin, binIdx) {
    bin[binIdx++] = 0;
    bin[binIdx++] = 0;
    bin[binIdx++] = 0;
    return binIdx;
}