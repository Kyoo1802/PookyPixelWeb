const char ledx3_html_h[] PROGMEM = R"(<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0" />
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
    <title>Pixelandia App</title>
    <link rel="shortcut icon" href="#">
    <link rel="stylesheet" href="global.css">
</head>

<body class="ledx-splash">
    <div class="main">
        <!---Top Bar-->
        <div class="top-bar">
            <!---Initial Banner-->
            <div class="title">
                <h1 style="margin: 0;">LedX</h1>
                <p>By Pixelandia Mexico</p>
            </div>

            <!---Top Menu-->
            <div class="top-menu">
                <div>
                    <input type="radio" id="toggle-status" name="top-menu" />
                    <label for="toggle-status" class="toggle">
                        <p>Estatus</p>
                    </label>
                    <div class="menu-popup" style="width: 215px;transform: translateX(-95px);">
                        <h5 id="device-status" class="error">Desconectado</h5>
                        <p id="device-status-msg" style="text-align: left">No se ha encontrado ningun controlador
                            disponible.</p>

                        <div class="range">
                            <h5 id="driver-fps-value">FPS (20)</h5>
                            <input id="driver-fps" type="range" value="20" min="10" max="35">
                        </div>
                        <p id="render-msg" style="text-align: left">
                            <b>FPS:</b>0</br><b>Latencia del render:</b>0 ms
                        </p>
                    </div>
                </div>
                <div>
                    <input type="radio" id="toggle-controller" name="top-menu" />
                    <label for="toggle-controller" class="toggle">
                        <p>Controladores</p>
                    </label>
                    <div class="menu-popup" style="width: 350px;transform: translateX(-215px);">
                        <div id="menu-matrix" class="menu-matrix">
                            <h5>Pantalla de Captura</h5>
                            <p>Ancho</p>
                            <input id="led-matrix-width" type="number" min="1" max="400" value="50">
                            <p>Alto</p>
                            <input id="led-matrix-height" type="number" min="1" max="300" value="15">
                        </div>
                        <div class="menu-wiring">
                            <h5>Controladores </h5>
                            <div id="drivers" class="scroll" style="height:430px">
                            </div>
                        </div>
                    </div>
                </div>
                <div>
                    <input type="radio" id="toggle-display" name="top-menu" />
                    <label for="toggle-display" class="toggle">
                        <p>Fondo</p>
                    </label>
                    <div class="menu-popup" style="width: 242px;transform: translateX(-200px);">
                        <div class="range">
                            <h5 id="display-size-value">Tamaño (100%)</h5>
                            <input id="display-size" type="range" value="100" min="1" max="300">
                        </div>
                        <div class="range">
                            <h5 id="display-pos-x-value">Posicion horizontal (0%)</h5>
                            <input id="display-pos-x" type="range" value="0" min="-100" max="100">
                        </div>
                        <div class="range">
                            <h5 id="display-pos-y-value">Posicion vertical (0%)</h5>
                            <input id="display-pos-y" type="range" value="0" min="-100" max="100">
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!---Canvas video-->
        <div class="canvas-container">
            <span>
                <input type="checkbox" id="canvas-module" name="canvas-module" onChange="showModules();"
                    style="display: none" />
                <label for="canvas-module" class="toggle" style="margin-bottom: 25px;">
                    <p>Mostrar Layout</p>
                </label>
                <span class="canvas-colors">
                    <label id="canvas-module-x" class="hide" style="position: absolute;left: -17px;top: -25px;">
                        <p>0)&nbsp;&nbsp;X</p>
                    </label>
                    <label id="canvas-module-y" class="hide" style="position: absolute;left: -38px;top: -25px;">
                        <p>(0,</p>
                        <p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Y</p>
                    </label>
                    <canvas id="ledx-canvas"></canvas>
                    <video id="ledx-video" playsinline style="display: none;" controls loop></video>
                </span>
            </span>
        </div>
        <!---Bottom Bar-->
        <div class="bottom-bar">
            <input type="radio" id="toggle-bkg" name="bottom-menu" />
            <label for="toggle-bkg" class="toggle">
                <p>Fondo</p>
            </label>
            <input type="radio" id="toggle-filter" name="bottom-menu" />
            <label for="toggle-filter" class="toggle">
                <p>Filtros</p>
            </label>
            <input type="radio" id="toggle-text" name="bottom-menu" />
            <label for="toggle-text" class="toggle">
                <p>Texto</p>
            </label>
            <div class="menu-popup">
                <!---Background Menu-->
                <div id="toggle-bkg-container">
                    <div class="column-menu">
                        <input type="radio" id="toggle-bkg-color" name="bkg-menu" checked />
                        <label for="toggle-bkg-color" class="toggle">
                            <p>Color</p>
                        </label>
                        <input type="radio" id="toggle-bkg-cam" name="bkg-menu" />
                        <label for="toggle-bkg-cam" class="toggle">
                            <p>Camara</p>
                        </label>
                        <input type="radio" id="toggle-bkg-mic" name="bkg-menu" />
                        <label id="toggle-bkg-mic-lbl" for="toggle-bkg-mic" class="toggle">
                            <p>Microfono</p>
                        </label>
                        <input type="radio" id="toggle-bkg-screen" name="bkg-menu" />
                        <label id="toggle-bkg-screen-lbl" for="toggle-bkg-screen" class="toggle">
                            <p>Compartir Pantalla</p>
                        </label>
                        <input type="radio" id="toggle-bkg-audio" name="bkg-menu" />
                        <label id="toggle-bkg-audio-lbl" for="toggle-bkg-audio" class="toggle">
                            <p>Compartir Audio</p>
                        </label>
                    </div>

                    <div id="toggle-bkg-color-container" class="grow-container">
                        <div class="control-blocks">
                            <input type="radio" id="bkg-solid" name="bkg-color" checked />
                            <label for="bkg-solid" class="toggle">
                                <p>1 Color</p>
                            </label>
                            <input type="radio" id="bkg-radial" name="bkg-color" />
                            <label for="bkg-radial" class="toggle">
                                <p>2 colores Radial</p>
                            </label>
                            <input type="radio" id="bkg-lineal" name="bkg-color" />
                            <label for="bkg-lineal" class="toggle">
                                <p>2 Colores Lineal</p>
                            </label>
                        </div>
                        <div class="control-blocks">
                            <label>Color inicial:</label>
                            <input id="bkg-start-color" type="color" value="#000000">
                            <label id="bkg-end-color-label" class="hide">Color final:</label>
                            <input id="bkg-end-color" class="hide" type="color" value="#000000">
                            <br />
                            <input id="bkg-animate" type="checkbox" />
                            <label for="bkg-animate" style="font-size: 0.7em">Animacion</label>
                        </div>
                        <!-- <input type="file" name="file[]" id="local-video" accept="video/*"> -->
                    </div>
                    <div id="toggle-bkg-cam-container" class="grow-container hide">
                        <div class="control-blocks">
                            <input type="radio" id="bkg-cam-start" name="bkg-cam-start" />
                            <label for="bkg-cam-start" class="toggle">
                                <p>Iniciar Camara</p>
                            </label>
                        </div>
                        <div id="bkg-cam-start-container" class="control-blocks hide">
                            <label>Camara:</label>
                            <select name="cameras" id="bkg-cam-select" style="width: 165px;"></select>
                        </div>
                    </div>
                    <div id="toggle-bkg-screen-container" class="grow-container hide">
                        <div class="control-blocks">
                            <input type="radio" id="bkg-screen-start" name="bkg-screen-start" />
                            <label for="bkg-screen-start" class="toggle">
                                <p>Compartir pantalla</p>
                            </label>
                        </div>
                    </div>
                    <div id="toggle-bkg-audio-container" class="grow-container hide">
                        <div class="control-blocks">
                            <input type="radio" id="bkg-audio-start" name="bkg-audio-start" />
                            <label for="bkg-audio-start" class="toggle">
                                <p>Compartir Audio</p>
                            </label>
                        </div>
                        <div class="control-blocks scroll">
                            <input type="radio" id="audio-0" data-effect="FREQUENCY" name="toggle-bkg-audio" checked />
                            <label for="audio-0" class="toggle">
                                <p>Frequencia</p>
                            </label>
                            <input type="radio" id="audio-1" data-effect="TIME" name="toggle-bkg-audio" />
                            <label for="audio-1" class="toggle">
                                <p>Tiempo</p>
                            </label>
                        </div>
                    </div>
                    <div id="toggle-bkg-mic-container" class="grow-container hide">
                        <div class="control-blocks">
                            <input type="radio" id="bkg-mic-start" name="bkg-mic-start" />
                            <label for="bkg-mic-start" class="toggle">
                                <p>Compartir Microfono</p>
                            </label>
                        </div>
                        <div class="control-blocks scroll">
                            <input type="radio" id="mic-0" data-effect="FREQUENCY" name="toggle-bkg-mic" checked />
                            <label for="mic-0" class="toggle">
                                <p>Frequencia</p>
                            </label>
                            <input type="radio" id="mic-1" data-effect="TIME" name="toggle-bkg-mic" />
                            <label for="mic-1" class="toggle">
                                <p>Tiempo</p>
                            </label>
                        </div>
                    </div>
                </div>
                <!---Filter Menu-->
                <div id="toggle-filter-container" class="hide">
                    <div class="control-blocks scroll" style="min-width:306px; min-height: 116px;">
                        <input type="radio" id="filter-0" data-filter="FILTER-0" name="toggle-bkg-filter" />
                        <label for="filter-0" class="toggle">
                            <p>Estrellas</p>
                        </label>
                        <input type="radio" id="filter-1" data-filter="FILTER-1" name="toggle-bkg-filter" />
                        <label for="filter-1" class="toggle">
                            <p>Corazones</p>
                        </label>
                        <input type="radio" id="filter-2" data-filter="FILTER-2" name="toggle-bkg-filter" />
                        <label for="filter-2" class="toggle">
                            <p>Lluvia</p>
                        </label>
                    </div>
                </div>
                <!---Text Menu-->
                <div id="toggle-text-container" class="hide">
                    <div id="toggle-bkg-color-show" class="menu-bkg">
                        <div class="control-blocks" style="width: 193px;">
                            <label>Texto:</label>
                            <textarea id="txt-text" type="text" rows="7" style="height:130px;width:180px"
                                placeholder="LedX"></textarea>
                        </div>

                        <div class="control-blocks">
                            <label>Fuente</label>
                            <select id="txt-font">
                                <option> Arial </option>
                                <option> Courier </option>
                                <option> Calibri </option>
                                <option> Comic Sans MS </option>
                                <option> Lucida Console </option>
                                <option> Lucida Sans Unicode </option>
                                <option> Sans-Serif </option>
                                <option> Tahoma </option>
                                <option> Times New Roman </option>
                                <option> Trebuchet MS </option>
                                <option> Verdana </option>
                            </select>
                        </div>
                        <div class="control-blocks">
                            <label>Color inicial:</label>
                            <input id="txt-start-color" type="color" value="#ff0000">
                            <label>Color medio:</label>
                            <input id="txt-mid-color" type="color" value="#00ff00">
                            <label>Color final:</label>
                            <input id="txt-end-color" type="color" value="#0000ff">
                        </div>
                        <div class="control-blocks" style="width: 193px;">
                            <label id="txt-size-value">Tamaño (100%)</label>
                            <input id="txt-size" type="range" min="1" value="100">
                            <label id="txt-speed-value">Velocidad (30%)</label>
                            <input id="txt-speed" type="range" min="1" value="30">
                            <label id="txt-position-value">Posicion (100%)</label>
                            <input id="txt-position" type="range" min="1" value="100">
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script defer>
        LoadScripts();

        function LoadScripts() {
            var scripts = [];
            var _scripts = ['canvas.js', 'connection.js', 'global.js'];
            LoadScriptsSync(_scripts, scripts)
        }

        function LoadScriptsSync(_scripts, scripts) {
            var x = 0;
            var loopArray = function (_scripts, scripts) {
                loadScript(_scripts[x], scripts[x], function () {
                    x++;
                    if (x < _scripts.length) {
                        loopArray(_scripts, scripts);
                    }
                });
            }
            loopArray(_scripts, scripts);
        }
        function loadScript(src, script, callback) {

            script = document.createElement('script');
            script.onerror = function () {
                alert('Error to handle')
            }
            script.onload = function () {
                console.log(src + ' loaded ')
                callback();
            }
            script.src = src;
            document.getElementsByTagName('head')[0].appendChild(script);
        }
    </script>
</body>

</html>)";
