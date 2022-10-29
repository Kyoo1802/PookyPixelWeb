const char global_js_h[] PROGMEM = R"(
// Common
const localStorage = window.localStorage;

function radios(name) {
    return document.getElementsByName(name);
}
function id(idName) {
    return document.getElementById(idName);
}
function ids(idPrefix, size) {
    let elements = [];
    for (let i = 0; i < size; i++) {
        elements[i] = document.getElementById(idPrefix + i);
    }
    return elements;
}
function idFrom(element, idName) {
    return element.querySelector('#' + idName);
}
function all(query) {
    return document.querySelectorAll(query);
}
function one(query) {
    return document.querySelector(query);
}
function tags(tag) {
    return document.getElementsByTagName(tag);
}
function tagsFrom(element, tag) {
    return element.getElementsByTagName(tag);
}
function click(element, fn) {
    element.addEventListener('click', e => fn(e));
}
function input(element, fn) {
    element.addEventListener('input', e => fn(e));
}
function change(element, fn) {
    element.addEventListener('change', e => fn(e));
}
function onLoad(fn) {
    window.addEventListener('load', e => fn(e));
}
function hideId(idC) {
    hide(id(idC));
}
function hide(component) {
    component.classList.add("hide");
}
function showId(idC) {
    show(id(idC));
}
function show(component) {
    component.classList.remove("hide");
}
function clearSelect(select) {
    while (select.firstChild) {
        select.removeChild(select.firstChild);
    }
}
function logI(msg) {
    console.log(msg);
}
function replacer(key, value) {
    if (value instanceof Map) {
        return {
            dataType: 'Map',
            value: Array.from(value.entries()), // or with spread: value: [...value]
        };
    } else {
        return value;
    }
}
function reviver(key, value) {
    if (typeof value === 'object' && value !== null) {
        if (value.dataType === 'Map') {
            return new Map(value.value);
        }
    }
    return value;
}
function readTextFile(file, callback) {
    var rawFile = new XMLHttpRequest();
    rawFile.overrideMimeType("application/json");
    rawFile.open("GET", file, false);
    rawFile.send(null);
    return rawFile.responseText;
}
function range(x, a, b) {
    return x >= a && x <= b;
}

const myRadios = all('input[type="radio"]');
const ledXCanvas = id('ledx-canvas');
const ledXVideo = id('ledx-video');
const bkgStartColor = id('bkg-start-color');
const bkgEndColorLabel = id('bkg-end-color-label');
const bkgEndColor = id('bkg-end-color');
const bkgSolid = id('bkg-solid');
const bkgRadial = id('bkg-radial');
const bkgLineal = id('bkg-lineal');
const bkgAnimate = id('bkg-animate');
const bkgCamStart = id('bkg-cam-start');
const bkgCamSelect = id('bkg-cam-select');
const bkgMicStart = id('bkg-mic-start');
const bkgMics = ids('mic-', 2);
const bkgScreenStart = id('bkg-screen-start');
const bkgAudioStart = id('bkg-audio-start');
const bkgAudios = ids('audio-', 2);
const canvasModule = id('canvas-module');
const canvasModuleX = id('canvas-module-x');
const canvasModuleY = id('canvas-module-y');
const filters = ids('filter-', 3);
const txtText = id('txt-text');
const txtFont = id('txt-font');
const txtStartColor = id('txt-start-color');
const txtMidColor = id('txt-mid-color');
const txtEndColor = id('txt-end-color');
const txtSpeed = id('txt-speed');
const txtSpeedValue = id('txt-speed-value');
const txtSize = id('txt-size');
const txtSizeValue = id('txt-size-value');
const txtPosition = id('txt-position');
const txtPositionValue = id('txt-position-value');
const driverFps = id("driver-fps");
const driverFpsValue = id("driver-fps-value");
const displaySize = id("display-size");
const displaySizeValue = id("display-size-value");
const displayPosX = id("display-pos-x");
const displayPosXValue = id("display-pos-x-value");
const displayPosY = id("display-pos-y");
const displayPosYValue = id("display-pos-y-value");
const menuMatrix = id("menu-matrix");
const ledMatrixWidth = id("led-matrix-width");
const ledMatrixHeight = id("led-matrix-height");
const deviceStatus = id("device-status");
const deviceStatusMsg = id("device-status-msg");
const driversUi = id("drivers");

let radioState = {};
let wsConnection = new WsConnection();
let canvasDrawer = new CanvasDrawer(ledXCanvas, ledXCanvas.getBoundingClientRect().width,
    ledXCanvas.getBoundingClientRect().height, wsConnection, uiNotification);
onLoad(() => {
    let initNodes = JSON.parse(readTextFile("./templates.json"));
    setupToggleBehavior(myRadios);
    setupSplash();

    // Background
    input(bkgStartColor, () => {
        canvasDrawer.sendCommand("BkgPainter", "setStartColor", bkgStartColor.value);
    });
    input(bkgEndColor, () => {
        canvasDrawer.sendCommand("BkgPainter", "setEndColor", bkgEndColor.value);
    });
    input(bkgAnimate, () => {
        canvasDrawer.sendCommand("BkgPainter", "setAnimate", bkgAnimate.checked);
    });
    input(bkgSolid, () => {
        canvasDrawer.sendCommand("BkgPainter", "setColorState", "SOLID");
        hide(bkgEndColor);
        hide(bkgEndColorLabel);
    });
    input(bkgRadial, () => {
        canvasDrawer.sendCommand("BkgPainter", "setColorState", "RADIAL");
        show(bkgEndColor);
        show(bkgEndColorLabel);
    });
    input(bkgLineal, () => {
        canvasDrawer.sendCommand("BkgPainter", "setColorState", "LINEAL");
        show(bkgEndColor);
        show(bkgEndColorLabel);
    });
    click(bkgCamStart, () => {
        if (radioState['bkg-cam-start'] == 0) {
            stopStream();
            loadCamDevices(bkgCamSelect);
        } else {
            stopStream();
        }
    });
    change(bkgCamSelect, () => {
        if (bkgCamSelect.selectedIndex != -1) {
            startCam(bkgCamSelect.options[bkgCamSelect.selectedIndex].value);
        }
    });
    click(bkgScreenStart, () => {
        if (radioState['bkg-screen-start'] == 0) {
            stopStream();
            startScreen();
        } else {
            stopStream();
        }
    });
    click(bkgAudioStart, () => {
        if (radioState['bkg-audio-start'] == 0) {
            stopStream();
            startAudioStream();
            checkAudioEffect(bkgAudios);
        } else {
            stopStream();
        }
    });
    toggleAudioEffect(bkgAudios);
    click(bkgMicStart, () => {
        if (radioState['bkg-mic-start'] == 0) {
            stopStream();
            startMicStream();
            checkAudioEffect(bkgMics);
        } else {
            stopStream();
        }
    });
    toggleAudioEffect(bkgMics);
    function toggleAudioEffect(audioRadios) {
        for (let i = 0; i < audioRadios.length; i++) {
            let radio = audioRadios[i];
            click(radio, () => {
                if (radio.checked) {
                    canvasDrawer.sendCommand("BkgPainter", "setAudioState", radio.dataset.effect);
                }
            });
        }
    }
    function checkAudioEffect(audioRadios) {
        for (let i = 0; i < audioRadios.length; i++) {
            let radio = audioRadios[i];
            if (radio.checked) {
                canvasDrawer.sendCommand("BkgPainter", "setAudioState", radio.dataset.effect);
            }
        }
    }

    // Filters
    // Filter Animations
    for (let i = 0; i < filters.length; i++) {
        let filter = filters[i];
        click(filter, () => {
            if (filter.checked) {
                canvasDrawer.sendCommand("FilterPainter", "setType", filter.dataset.filter);
            } else {
                canvasDrawer.sendCommand("FilterPainter", "setType", 'NONE');
            }
        });
    }

    // Texts
    input(txtText, () => {
        canvasDrawer.sendCommand("TextPainter", "setText", txtText.value);
    });
    change(txtFont, () => {
        canvasDrawer.sendCommand("TextPainter", "setTextFontType", txtFont.value);
    });
    input(txtStartColor, () => {
        canvasDrawer.sendCommand("TextPainter", "setStartColor", txtStartColor.value);
    });
    input(txtMidColor, () => {
        canvasDrawer.sendCommand("TextPainter", "setMidColor", txtMidColor.value);
    });
    input(txtEndColor, () => {
        canvasDrawer.sendCommand("TextPainter", "setEndColor", txtEndColor.value);
    });
    input(txtSpeed, () => {
        let v = parseInt(txtSpeed.value);
        txtSpeedValue.innerHTML = `Velocidad (${v}%)`;
        canvasDrawer.sendCommand("TextPainter", "setTextSpeedPercentage", v);
    });
    input(txtSize, () => {
        let v = parseInt(txtSize.value);
        txtSizeValue.innerHTML = `Tamaño (${v}%)`;
        if (v > 0) {
            canvasDrawer.sendCommand("TextPainter", "setTextSizePercentage", v);
        }
    });
    input(txtPosition, () => {
        let v = parseInt(txtPosition.value);
        txtPositionValue.innerHTML = `Posicion (${v}%)`;
        canvasDrawer.sendCommand("TextPainter", "setTextPosPercentage", v);
    });

    // Drivers
    input(driverFps, () => {
        let v = parseInt(driverFps.value);
        driverFpsValue.innerHTML = `FPS (${v})`;
        if (v > 0) {
            canvasDrawer.setFps(v);
        }
    });
    input(displaySize, () => {
        let v = parseInt(displaySize.value);
        displaySizeValue.innerHTML = `Tamaño (${v}%)`;
        if (v > 0) {
            canvasDrawer.sendCommand("BkgPainter", "setViewSize", v);
        }
    });
    input(displayPosX, () => {
        let v = parseInt(displayPosX.value);
        displayPosXValue.innerHTML = `Posicion horizontal (${v}%)`;
        canvasDrawer.sendCommand("BkgPainter", "setViewX", displayPosX.value);
    });
    input(displayPosY, () => {
        let v = parseInt(displayPosY.value);
        displayPosYValue.innerHTML = `Posicion vertical (${v}%)`;
        canvasDrawer.sendCommand("BkgPainter", "setViewY", displayPosY.value);
    });
    input(ledMatrixWidth, () => {
        let ww = parseInt(ledMatrixWidth.value);
        let hh = parseInt(ledMatrixHeight.value);
        if (!isNaN(ww) && !isNaN(hh) && range(ww, 1, 400) && range(hh, 1, 300)) {
            canvasDrawer.onMatrixResize(ww, hh);
            localStorage.setItem('matrix-w', ww);
        }
    });
    input(ledMatrixHeight, () => {
        let ww = parseInt(ledMatrixWidth.value);
        let hh = parseInt(ledMatrixHeight.value);
        if (!isNaN(ww) && !isNaN(hh) && range(ww, 1, 400) && range(hh, 1, 300)) {
            canvasDrawer.onMatrixResize(ww, hh);
            localStorage.setItem('matrix-h', hh);
        }
    });
    if (localStorage.getItem('matrix-w') !== null && localStorage.getItem('matrix-h') !== null) {
        ledMatrixWidth.value = parseInt(localStorage.getItem('matrix-w'));
        ledMatrixHeight.value = parseInt(localStorage.getItem('matrix-h'));
    }
    canvasDrawer.init();
    canvasDrawer.onMatrixResize(parseInt(ledMatrixWidth.value), parseInt(ledMatrixHeight.value));
    workerInterval(paint, 5);
    wsConnection.init(initNodes);
});
function setupToggleBehavior(myRadios) {
    for (let x = 0; x < myRadios.length; x++) {
        myRadios[x].onclick = function () {
            let myGroupRadios = all('input[name="' + this.name + '"]');
            let selectedIdx = -1;
            for (let j = 0; j < myGroupRadios.length; j++) {
                let myGroupRadio = myGroupRadios[j];
                let myRadioBlock = id(myGroupRadio.id + '-container');
                if (myGroupRadio == this) {
                    selectedIdx = j;
                }
                if (myGroupRadio == this && radioState[this.name] != j) {
                    myGroupRadio.checked = true;
                    if (myRadioBlock != null) {
                        show(myRadioBlock);
                    }
                } else {
                    myGroupRadio.checked = false;
                    if (myRadioBlock != null) {
                        hide(myRadioBlock);
                    }
                }
            }
            radioState[this.name] = radioState[this.name] != selectedIdx ? selectedIdx : -1;
        };
    }
}

function setupSplash() {
    const box = one("div.top-bar");
    box.addEventListener("transitionend", () => {
        document.body.classList.add("ledx-start")
        canvasDrawer.onResize(ledXCanvas.getBoundingClientRect().width, ledXCanvas
            .getBoundingClientRect().height);
        canvasDrawer.onMatrixResize(parseInt(ledMatrixWidth.value), parseInt(ledMatrixHeight.value));
    });
    window.setTimeout(function () {
        document.body.classList.remove("ledx-splash");
    }, 1000);
}

function showModules() {
    canvasDrawer.sendCommand("BorderPainter", "setEnabled", canvasModule.checked);
    if (canvasModule.checked) {
        show(canvasModuleX);
        show(canvasModuleY);
    } else {
        hide(canvasModuleX);
        hide(canvasModuleY);
    }
}


// Worker
var timeoutId = 0;
var timeouts = {};
var worker = new Worker("timeout-worker.js");
worker.addEventListener("message", function (evt) {
    var data = evt.data,
        id = data.id,
        fn = timeouts[id].fn,
        args = timeouts[id].args;

    fn.apply(null, args);
});
workerInterval = function (fn, delay) {
    var args = Array.prototype.slice.call(arguments, 2);
    timeoutId += 1;
    delay = delay || 0;
    var id = timeoutId;
    timeouts[id] = {
        fn: fn,
        args: args
    };
    worker.postMessage({
        command: "setInterval",
        id: id,
        timeout: delay
    });
    return id;
};

function paint() {
    if (canvasDrawer.paint()) {
        updateUi();
    }
}
function updateUi() {
    wsConnection.getNodes().forEach((driverNode, nodeId) => {
        const driverInfoUi = idFrom(driversUi, nodeId + '-info');
        if (driverNode) {
            const newHtml = driverNode.toHtml(driverInfoUi.innerHTML);
            if (!!newHtml) {
                driverInfoUi.innerHTML = newHtml;
            }
        }
    });
}

function loadCamDevices(camSelect) {
    if (!navigator.mediaDevices || !navigator.mediaDevices.enumerateDevices) {
        console.log("enumerateDevices() not supported.");
        return;
    }
    navigator.mediaDevices.enumerateDevices()
        .then(deviceInfos => {
            clearSelect(camSelect);
            if (deviceInfos.length > 0) {
                const option = document.createElement('option');
                option.value = 'NONE'
                option.text = 'Ninguna';
                camSelect.appendChild(option);
            }
            for (let i = 0, idx = 1; i < deviceInfos.length; i++) {
                const deviceInfo = deviceInfos[i];
                const option = document.createElement('option');
                if (deviceInfo.kind === 'videoinput') {
                    option.value = deviceInfo.deviceId;
                    option.text = deviceInfo.label || `camera ${idx}`;
                    camSelect.appendChild(option);
                    idx++;
                }
            }
        })
        .catch(error => {
            logI(`getUserMedia error: ${error}`);
        });
}

async function startCam(camSelect) {
    if (camSelect === 'NONE') {
        stopStream();
        return;
    }
    const constraints = window.constraints = {
        audio: false,
        video: {
            deviceId: camSelect ? {
                exact: camSelect
            } : undefined,
            width: {
                "max": 360
            },
            height: {
                "max": 360
            }
        }
    };
    try {
        ledXVideo.srcObject = await navigator.mediaDevices.getUserMedia(constraints);
        ledXVideo.onloadedmetadata = (e) => {
            ledXVideo.play();
            canvasDrawer.sendCommand('BkgPainter', 'setCameraScreen', ledXVideo);
            canvasDrawer.sendCommand('BkgPainter', 'setCameraState', 'PLAY');
            bkgScreenStart.checked = false
            bkgAudioStart.checked = false
            bkgMicStart.checked = false
        };
        ledXVideo.srcObject.oninactive = () => {
            bkgCamStart.checked = false
            canvasDrawer.sendCommand('BkgPainter', 'setCameraState', 'STOP');
        };
    } catch (e) {
        console.log(e);
        bkgCamStart.checked = false
    }
}

async function startScreen() {
    const constraints = {
        audio: false,
        video: {
            // cursor: "always",
            width: {
                "max": 360
            },
            height: {
                "max": 360
            }
        }
    };
    try {
        ledXVideo.srcObject = await navigator.mediaDevices.getDisplayMedia(constraints);
        ledXVideo.onloadedmetadata = (e) => {
            ledXVideo.play();
            canvasDrawer.sendCommand('BkgPainter', 'setVideoScreen', ledXVideo);
            canvasDrawer.sendCommand('BkgPainter', 'setScreenState', 'PLAY');
            bkgCamStart.checked = false
            bkgAudioStart.checked = false
            bkgMicStart.checked = false
        };
        ledXVideo.srcObject.oninactive = () => {
            bkgScreenStart.checked = false
            canvasDrawer.sendCommand('BkgPainter', 'setScreenState', 'STOP');
        };
    } catch (e) {
        console.log(e);
        bkgScreenStart.checked = false
    }
}

function isMobile() {
    let check = false;
    (function (a) { if (/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|mobile.+firefox|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows ce|xda|xiino|android|ipad|playbook|silk/i.test(a) || /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(a.substr(0, 4))) check = true; })(navigator.userAgent || navigator.vendor || window.opera);
    return check;
}
async function startMicStream() {
    try {
        let stream = await navigator.mediaDevices.getUserMedia({ audio: true });
        const audioCtx = new (window.AudioContext || window.webkitAudioContext)();

        const source = audioCtx.createMediaStreamSource(stream);
        let analyzer = audioCtx.createAnalyser();
        source.connect(analyzer);
        analyzer.fftSize = 2 ** 8;
        canvasDrawer.sendCommand('BkgPainter', 'setAudioStream', 'MIC', stream, analyzer);
        canvasDrawer.sendCommand('BkgPainter', 'setAudioState', 'FREQUENCY');
        bkgScreenStart.checked = false
        bkgAudioStart.checked = false
        bkgCamStart.checked = false
    } catch (e) {
        console.log(e);
        bkgMicStart.checked = false;
        canvasDrawer.sendCommand('BkgPainter', 'setAudioState', 'STOP');
    }
}

let audioStream;
async function startAudioStream() {
    const constraints = {
        audio: true,
        video: {
            width: {
                "max": 360
            },
            height: {
                "max": 360
            }
        }
    };
    try {
        audioStream = await navigator.mediaDevices.getDisplayMedia(constraints);
        const audioCtx = new AudioContext();
        const analyzer = audioCtx.createAnalyser();
        const mediaStream = new MediaStream(audioStream.getTracks());
        const source = audioCtx.createMediaStreamSource(mediaStream);
        source.connect(analyzer);
        // How much data should we collect
        analyzer.fftSize = 2 ** 6;
        canvasDrawer.sendCommand('BkgPainter', 'setAudioStream', 'AUDIO', mediaStream, analyzer);
        canvasDrawer.sendCommand('BkgPainter', 'setAudioState', 'FREQUENCY');
        bkgScreenStart.checked = false
        bkgCamStart.checked = false
        bkgMicStart.checked = false

        audioStream.oninactive = () => {
            bkgAudioStart.checked = false
            canvasDrawer.sendCommand('BkgPainter', 'setScreenState', 'STOP');
        };
    } catch (e) {
        console.log(e);
        bkgAudioStart.checked = false;
        canvasDrawer.sendCommand('BkgPainter', 'setAudioState', 'STOP');
    }
}

function stopStream() {
    canvasDrawer.sendCommand('BkgPainter', 'stopStream');
    if (ledXVideo.srcObject) {
        const tracks = ledXVideo.srcObject.getTracks();
        tracks.forEach(function (track) {
            track.stop();
        });
        ledXVideo.srcObject = null;
    }
    if (audioStream) {
        audioStream.getTracks().forEach(function (track) {
            track.stop();
        });
        audioStream = null;

    }
}
var globalMIdx = 0;
function createModuleHtml(dId, cIdx, mIdx) {
    const driver = wsConnection.getNodes().get(dId);
    const template = driver.masterTemplate;
    const channel = template.channels[cIdx];
    const module = channel.modules[mIdx];
    const dIdcIdmId = dId + '-c' + cIdx + '-m' + mIdx;
    return '<div id="' + dIdcIdmId + '" style="border-bottom: solid 2px gray;">'
        + '  <div style="display: flex;flex-flow: row-reverse; justify-content: space-between;">'
        + '   <button style=\'margin:5px; background:darkred;border: 0;\' onclick=onRemoveModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') >Quitar</button>'
        + '  </div>'
        + '  <div>'
        + '   <div>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d0\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGV_CB) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d0\' class=\'toggle arrow\' style=\'transform: scaleX(-1)\'> </label>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d1\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGV_AD) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d1\' class=\'toggle arrow\' style=\'transform: rotate(180deg) \'> </label>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d2\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGV_BC) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d2\' class=\'toggle arrow\' style=\'transform: rotate(180deg) scaleX(-1)\'> </label>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d3\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGV_DA) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d3\' class=\'toggle arrow\'> </label>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d4\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGH_CB) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d4\' class=\'toggle arrow\' style=\'transform: rotate(90deg) \'> </label>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d5\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGH_AD) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d5\' class=\'toggle arrow\' style=\'transform: rotate(90deg) scaleX(-1)\'> </label>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d6\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGH_BC) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d6\' class=\'toggle arrow\' style=\'transform: rotate(270deg) \'> </label>'
        + '    <input type=\'radio\' id=\'' + dIdcIdmId + '-d7\' name=\'' + dIdcIdmId + '-d\' ' + checked(module.d, ModuleDirection.ZIGZAGH_DA) + ' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') />'
        + '    <label for=\'' + dIdcIdmId + '-d7\' class=\'toggle arrow\' style=\'transform: rotate(270deg) scaleX(-1)\'> </label>'
        + '   </div>'
        + '   <div>'
        + '    <label for=\'' + dIdcIdmId + '-x\'>X:</label>'
        + '    <input id=\'' + dIdcIdmId + '-x\' type=\'number\' min=\'-40\' max=\'80\' value=\'' + module.x + '\' style=\'width: 35px\' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') >'
        + '    <label for=\'' + dIdcIdmId + '-y\'>Y:</label>'
        + '    <input id=\'' + dIdcIdmId + '-y\' type=\'number\' min=\'-30\' max=\'60\' value=\'' + module.y + '\' style=\'width: 35px\' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') >'
        + '    <label for=\'' + dIdcIdmId + '-w\'>Ancho:</label>'
        + '    <input id=\'' + dIdcIdmId + '-w\' type=\'number\' min=\'0\' max=\'128\' value=\'' + module.w + '\' style=\'width: 35px\' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') >'
        + '    <label for=\'' + dIdcIdmId + '-h\'>Alto:</label>'
        + '    <input id=\'' + dIdcIdmId + '-h\' type=\'number\' min=\'0\' max=\'64\' value=\'' + module.h + '\' style=\'width: 35px\' onchange=onUpdateModule(\'' + dId + '\',' + cIdx + ',' + mIdx + ') >'
        + '   </div>'
        + '  </div>'
        + ' </div>';
}
function checked(t1, t2) {
    return t1 === t2 ? 'checked' : '';
}

function onUpdateModule(dId, cIdx, mIdx) {
    const dIdcIdmId = dId + '-c' + cIdx + '-m' + mIdx;

    const radioD = radios(dIdcIdmId + '-d');
    let d = 0;
    for (i = 0; i < radioD.length; i++) {
        if (radioD[i].checked) {
            d = i;
            break;
        }
    }
    const inX = id(dIdcIdmId + '-x');
    const inY = id(dIdcIdmId + '-y');
    const inW = id(dIdcIdmId + '-w');
    const inH = id(dIdcIdmId + '-h');

    const driver = wsConnection.getNodes().get(dId);
    driver.updateModule(cIdx, mIdx, inX.value, inY.value, inW.value, inH.value, d);
    showId(dId + '-save');
}

function onRemoveModule(dId, cIdx, mIdx) {
    const dIdcIdmId = dId + '-c' + cIdx + '-m' + mIdx;
    id(dIdcIdmId).remove();
    const driver = wsConnection.getNodes().get(dId);
    driver.removeModule(cIdx, mIdx);
    showId(dId + '-save');
}

function createChannelHtml(dId, cIdx) {
    const driver = wsConnection.getNodes().get(dId);
    const template = driver.masterTemplate;
    const channel = template.channels[cIdx];
    const dIdcId = dId + '-c' + cIdx;
    const channelHide = cIdx > 0 ? 'class=\'hide\'' : '';
    const moduleHide = !selected(channel.templateName, 'custom') ? 'class=\'hide\'' : '';
    let channelHtml = '<div id=\'' + dIdcId + '-container\' ' + channelHide + '>'
        + '  <div>'
        + '   <select id=\'' + dIdcId + '-t\' name=\'' + dIdcId + '-t\' onchange=onSelectTemplate(\'' + dId + '\',' + cIdx + ')'
        + '         style=\'display:inline-flex;margin:10px\'>'
        + '    <option value=\'pix600\' ' + selected(channel.templateName, 'pix600') + '>Pix 600</option>'
        + '    <option value=\'pix900\' ' + selected(channel.templateName, 'pix900') + '>Pix 900</option>'
        + '    <option value=\'custom\' ' + selected(channel.templateName, 'custom') + '>Personalizado</option>'
        + '   </select>'
        + '   <select id=\'' + dIdcId + '-order\' name=\'' + dIdcId + '-order\' onchange=onSelectOrder(\'' + dId + '\',' + cIdx + ') '
        + '         style=\'display:inline-flex;margin:10px\'>'
        + '    <option value=\'rgb\' ' + selected(channel.colorOrder, 'rgb') + '>RGB</option>'
        + '    <option value=\'rbg\' ' + selected(channel.colorOrder, 'rbg') + '>RBG</option>'
        + '    <option value=\'grb\' ' + selected(channel.colorOrder, 'grb') + '>GRB</option>'
        + '    <option value=\'brg\' ' + selected(channel.colorOrder, 'brg') + '>BRG</option>'
        + '   </select>'
        + '  </div>'
        + '  <div id=\'' + dIdcId + '-m\' ' + moduleHide + ' style=\'display:flex;flex-wrap:wrap;flex-direction:column\'>';

    for (let mIdx = 0; mIdx < channel.modules.length; mIdx++) {
        if (channel.modules[mIdx]) {
            channelHtml += createModuleHtml(dId, cIdx, mIdx);
        }
    }
    channelHtml += '  </div>'
        + '  <button ' + moduleHide + ' id=\'' + dIdcId + '-m-add\' onclick=onAddModule(\'' + dId + '\',' + cIdx + ') >'
        + '   Agregar modulo</button>'
        + ' </div>';
    return channelHtml;
}

function selected(t1, t2) {
    return t1 === t2 ? 'selected' : '';
}
function newDefaultModule() {
    return new Module(0, 0,
        canvasDrawer.getMatrixWidth(), canvasDrawer.getMatrixHeight(), ModuleDirection.ZIGZAGV_CB);
}
function onSelectOrder(dId, cIdx) {
    const dIdcId = dId + '-c' + cIdx;
    const colorOrder = id(dIdcId + '-order').value;
    const driver = wsConnection.getNodes().get(dId);
    driver.setTemplateColorOrder(cIdx, colorOrder);
    showId(dId + '-save');
}

function onSelectTemplate(dId, cIdx) {
    const dIdcId = dId + '-c' + cIdx;
    const templateName = id(dIdcId + '-t').value;
    let mContainer = id(dIdcId + '-m');
    let mAddButton = id(dIdcId + '-m-add');
    if (templateName === 'custom') {
        show(mContainer);
        show(mAddButton);
    } else {
        hide(mContainer);
        hide(mAddButton);
    }
    const driver = wsConnection.getNodes().get(dId);
    driver.setTemplateName(cIdx, templateName);
    showId(dId + '-save');
}

function onAddModule(dId, cIdx) {
    const driver = wsConnection.getNodes().get(dId);
    driver.addModule(cIdx, globalMIdx, newDefaultModule());
    const dIdcId = dId + '-c' + cIdx;
    let mContainer = htmlToElement(createModuleHtml(dId, cIdx, globalMIdx));
    id(dIdcId + '-m').appendChild(mContainer);
    showId(dId + '-save');
    globalMIdx++;
}

function htmlToElement(html) {
    var template = document.createElement('template');
    html = html.trim(); // Never return a text node of whitespace as the result
    template.innerHTML = html;
    return template.content.firstChild;
}

function createChannelsHtml(dId, dNode) {
    let chansHtml = '<div style=\'border-bottom: solid 2px white;text-align: left;\'>';
    const template = dNode.masterTemplate;
    for (let cIdx = 0; cIdx < template.channels.length; cIdx++) {
        const dIdcId = dId + '-c' + cIdx;
        const checked = cIdx == 0 ? 'checked' : '';
        chansHtml += '<input type=\'radio\' id=\'' + dIdcId + '\' name=\'' + dId + '-c\' ' + checked + ' />'
            + '<label for=\'' + dIdcId + '\' class=\'toggle-tab\'> <p>Canal ' + (cIdx + 1) + '</p> </label>';
    }
    chansHtml += '</div>'
    for (let cIdx = 0; cIdx < template.channels.length; cIdx++) {
        if (template.channels[cIdx]) {
            chansHtml += createChannelHtml(dId, cIdx);
        }
    }
    return chansHtml;
}

function createDriverHtml(dNode) {
    const dId = dNode.id;
    const dInfo = dNode.toHtml();
    const dColor = dNode.color
    return '<div id=\'' + dId + '\' style=\'display: flex;flex-direction: column;padding:5px;border-radius:10px;border-color:' + dColor + '\' class=\'border-connected\'>'
        + '  <div style=\'display:flex;justify-content:space-between;\'>'
        + '   <div style=\'color:deepskyblue\'>' + dId + '</div>'
        + '   <button style=\'margin-right:10px;\' class=\'hide\' id=\'' + dId + '-save\' onclick=onSaveDriver(\'' + dId + '\') >Guardar</button>'
        + '  </div>'
        + '  <div id=\'' + dId + '-info\' style=\'text-align:left;font-size:small\'>' + dInfo + '</div>'
        + createChannelsHtml(dId, dNode)
        + '</div>';
}

function onSaveDriver(dId) {
    const driver = wsConnection.getNodes().get(dId);
    hideId(dId + '-save');
    driver.saveDriver();
}

function uiNotification(deviceInfo, nodes) {
    [...nodes.keys()].forEach((node) => {
        const driverNode = nodes.get(node);
        const driverUi = idFrom(driversUi, driverNode.id);
        if (!driverUi) {
            driversUi.innerHTML += createDriverHtml(driverNode);
            setupToggleBehavior(all('input[name="' + driverNode.id + '-c"]'));
        } else {
            if (driverNode.isLost) {
                driverUi.classList.remove('border-connected');
                driverUi.classList.add('border-disconnected');
            } else {
                driverUi.classList.remove('border-disconnected');
                driverUi.classList.add('border-connected');
            }
        }
    });
    if (!deviceInfo.update) {
        return;
    }
    let driverMsg = ""
    if (deviceInfo.driverStatus === NetworkStatus.CONNECTED) {
        deviceStatus.classList.remove("error");
        deviceStatus.classList.remove("warning");
        deviceStatus.classList.add("success");
        deviceStatus.innerHTML = 'Conectado';
        const t = new Date(deviceInfo.lastDriverMsgTime);
        var dateString = deviceInfo.lastDriverMsgTime == 0 ?
            "Ninguna" :
            t.getUTCFullYear() + "/" + (t.getMonth() + 1) + "/" + t.getDate() + " " + t.getHours() + ":" + t.getMinutes() + ":" + t.getSeconds();
        driverMsg =
            '<b>Server Ip:</b> ' + deviceInfo.ip + ' </br></br > ' +
            '<b>Ultima actualizacion:</b> ' + dateString + '</br>' +
            '<b>Velocidad de subida:</b> ' + deviceInfo.driverUploadKbs + ' KB/s</br>' +
            '<b>Velocidad de descarga:</b> ' + deviceInfo.driverDownloadKbs + ' KB/s</br>' +
            '<b>Paquetes enviados:</b> ' + deviceInfo.driverMsgSent + '</br>' +
            '<b>Paquetes recibidos:</b> ' + deviceInfo.driverMsgReceived + '</br></br>';
    } else if (deviceInfo.driverStatus === NetworkStatus.CONNECTING) {
        deviceStatus.classList.add("warning");
        deviceStatus.classList.remove("error");
        deviceStatus.classList.remove("success");
        deviceStatus.innerHTML = 'Conectando';
        driverMsg =
            "Intentando conectar al controlador...</br></br>";
    } else {
        deviceStatus.classList.add("error");
        deviceStatus.classList.remove("warning");
        deviceStatus.classList.remove("success");
        deviceStatus.innerHTML = 'Desconectado';
        driverMsg =
            "No se ha encontrado ningun controlador disponible.<button class='btn-success' onclick='connectDriver()'>Reconectar</button>";
    }

    deviceStatusMsg.innerHTML = driverMsg + '<b>FPS:</b> ' + deviceInfo.fps + '</br>' +
        '<b>Latencia del render:</b> ' + deviceInfo.fullLatency + ' ms';
    deviceInfo.update = false;
}

function connectDriver() {
    wsConnection.wsConnect(0);
}
)";
