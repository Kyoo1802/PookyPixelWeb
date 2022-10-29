const char global_css_h[] PROGMEM = R"(
:root {
  --white: #ffffff;
  --white-hover: #383838;
  --light: #f0eff3;
  --light-black: #373737;
  --black: #181818;
  --bar-black: #202020ee;
  --dark-blue: #1f2029;
  --dark-light: #353746;
  --red: #ff0000;
  --thumb: #1e90ff;
  --yellow: #f8ab37;
  --orange: #ffa500;
  --green: #259925;
  --grey: #ecedf3;
  --blue: #2d88ff;
  --blue-hover: #2d88ff;

  --gradient-shadow: linear-gradient(45deg,
      #fb0094,
      #0000ff,
      #00ff00,
      #ffff00,
      #ff0000,
      #fb0094,
      #0000ff,
      #00ff00,
      #ffff00,
      #ff0000);
}

@keyframes internal-glow {

  from,
  to {
    box-shadow: inset 0 0 15px 0 red;
    border: solid 2px red;
  }

  16% {
    box-shadow: inset 0 0 15px 0 yellow;
    border: solid 2px yellow;
  }

  32% {
    box-shadow: inset 0 0 15px 0 green;
    border: solid 2px green;
  }

  48% {
    box-shadow: inset 0 0 15px 0 aqua;
    border: solid 2px aqua;
  }

  64% {
    box-shadow: inset 0 0 15px 0 blue;
    border: solid 2px blue;
  }

  80% {
    box-shadow: inset 0 0 15px 0 fuchsia;
    border: solid 2px fuchsia;
  }
}

@keyframes box-glow {
  0% {
    box-shadow: inset 0 0 4px var(--blue), inset 0 0 8px var(--blue);
    border: solid 2px var(--blue);
  }

  20% {
    box-shadow: inset 0 0 5px var(--blue), inset 0 0 10px var(--blue),
      inset 0 0 27px var(--blue);
    border: solid 2px var(--blue);
  }

  39% {
    box-shadow: inset 0 0 4px var(--blue), inset 0 0 8px var(--blue);
    border: solid 2px var(--blue);
  }

  40% {
    box-shadow: inset 0 0 4px var(--red), inset 0 0 8px var(--red);
    border: solid 2px var(--red);
  }

  60% {
    box-shadow: inset 0 0 5px var(--red), inset 0 0 10px var(--red),
      inset 0 0 27px var(--red);
    border: solid 2px var(--red);
  }

  79% {
    box-shadow: inset 0 0 4px var(--red), inset 0 0 8px var(--red);
    border: solid 2px var(--red);
  }

  80% {
    box-shadow: inset 0 0 5px var(--green), inset 0 0 10px var(--green),
      inset 0 0 27px var(--green);
    border: solid 2px var(--green);
  }

  100% {
    box-shadow: inset 0 0 4px var(--green), inset 0 0 8px var(--green);
    border: solid 2px var(--green);
  }
}

html,
body {
  height: 100%;
  width: 100%;
  margin: 0;
  font-family: "system-ui";
  background: radial-gradient(circle, var(--black) 0%, var(--black) 100%);
  background-repeat: no-repeat;
  background-attachment: fixed;
  color: var(--white);
}

::-webkit-scrollbar {
  width: 10px;
}

::-webkit-scrollbar-track {
  background: var(--bar-black);
}

::-webkit-scrollbar-thumb {
  background: var(--blue);
}

::-webkit-scrollbar-thumb:hover {
  background: var(--blue);
}

.success-bkg {
  color: var(--black);
  background-color: var(--white);
}

.error-bkg {
  color: var(--white);
  background-color: var(--red);
}

.success {
  color: green;
}

.error {
  color: var(--red);
}

.warning {
  color: var(--orange);
}

select {
  display: block;
  padding: 5px;
  background-color: var(--bar-black);
  color: var(--white);
}

select:hover {
  background-color: var(--light-black);
  border: solid 1px var(--white);
}

select,
input {
  transition: opacity 0.2s;
  margin: 5px auto;
  max-width: 100%;
  border-width: 1px;
}

.btn-success {
  border: solid 2px var(--light-black);
  background: var(--green);
  padding: 5px 10px;
  display: block;
}

.btn-error {
  border: solid 2px var(--light-black);
  background: var(--red);
  padding: 5px 10px;
  margin: 0 auto;
  display: block;
}

.scroll {
  height: 75px;
  overflow: auto;
  margin-left: 3px;
  text-align: center;
}

p {
  font-style: normal;
  margin: 5px;
  font-size: 0.7em;
  text-align: center;
}

h5 {
  text-align: center;
  margin: 10px 0;
}

h6 {
  font-size: 0.7em;
  text-align: center;
  margin: 4px 0;
}

input[type="range"] {
  height: 20px;
  appearance: none;
  background: none;
  margin: 0;
  width: 100%;
  outline: none;
  opacity: 0.7;
  transition: opacity 0.2s;
  cursor: unset;
}

input[type="range"]:hover {
  cursor: pointer;
}

input[type="range"]:focus {
  outline: none;
}

input[type="range"]::-webkit-slider-runnable-track {
  width: 100%;
  height: 14px;
  cursor: pointer;
  transition: opacity 0.2s;
  box-shadow: 1px 1px 1px #50555c;
  background: #50555c;
  border-radius: 14px;
  border: 0px solid #000000;
}

input[type="range"]::-webkit-slider-thumb {
  box-shadow: 0px 0px 0px #000000;
  border: 0px solid #000000;
  height: 20px;
  width: 20px;
  border-radius: 12px;
  background: var(--thumb);
  cursor: pointer;
  appearance: none;
  margin-top: -2px;
  padding-left: 2px;
}

input[type="range"]:focus::-webkit-slider-runnable-track {
  background: #50555c;
}

input[type="range"]::-moz-range-track {
  width: 100%;
  height: 14px;
  cursor: pointer;
  transition: opacity 0.2s;
  box-shadow: 1px 1px 1px #50555c;
  background: #50555c;
  border-radius: 14px;
  border: 0px solid #000000;
}

input[type="range"]::-moz-range-thumb {
  box-shadow: 0px 0px 0px #000000;
  border: 0px solid #000000;
  height: 20px;
  width: 40px;
  border-radius: 12px;
  background: var(--thumb);
  cursor: pointer;
}

input[type="range"]::-ms-track {
  width: 100%;
  height: 14px;
  cursor: pointer;
  transition: opacity 0.2s;
  background: transparent;
  border-color: transparent;
  color: transparent;
}

input[type="range"]::-ms-fill-lower {
  background: #50555c;
  border: 0px solid #000000;
  border-radius: 28px;
  box-shadow: 1px 1px 1px #50555c;
}

input[type="range"]::-ms-fill-upper {
  background: #50555c;
  border: 0px solid #000000;
  border-radius: 28px;
  box-shadow: 1px 1px 1px #50555c;
}

input[type="range"]::-ms-thumb {
  margin-top: 1px;
  box-shadow: 0px 0px 0px #000000;
  border: 0px solid #000000;
  height: 20px;
  width: 40px;
  border-radius: 12px;
  background: var(--thumb);
  cursor: pointer;
}

input[type="range"]:focus::-ms-fill-lower {
  background: #50555c;
}

input[type="range"]:focus::-ms-fill-upper {
  background: #50555c;
}

textarea {
  margin: 5px;
  max-width: 100%;
}

input[type="radio"] {
  display: none;
}

.hide {
  display: none !important;
}

.border-connected {
  border: solid 2px var(--green);
}

.border-disconnected {
  border: solid 2px var(--red) !important;
  opacity: 0.3;
  pointer-events: none;
}

.disabled:after textarea {
  width: 100%;
  height: 100%;
  position: absolute;
}

input[type="radio"]:hover+.toggle,
input[type="radio"]:hover+.toggle-tab,
input[type="checkbox"]:hover+.toggle {
  background-color: var(--white-hover);
}

input[type="radio"]:checked+.toggle-tab {
  background-color: var(--thumb);
  color: white;
}

input[type="radio"]:checked+.toggle,
input[type="checkbox"]:checked+.toggle {
  background-color: var(--grey);
  color: black;
}

.toggle, .toggle-tab {
  position: relative;
  display: inline-flex;
  width: auto;
  height: 30px;
  border-radius: 10px;
  letter-spacing: 1px;
  background-color: var(--bar-black);
  color: var(--white);
  border: solid 2px var(--light-black);
  cursor: pointer;
  vertical-align: middle;
  padding: 0 5px;
  margin: 2px 0;
}

.toggle p, .toggle-tab p {
  margin: auto;
}

button {
  padding: 15px;
  border: solid 2px rgba(230, 230, 230, 1);
  background-color: white;
  color: black;
  cursor: pointer;
  letter-spacing: 1px;
  border-radius: 10px;
  margin: 2px;
  position: relative;
  opacity: 1;
  margin: 10px auto;
}

button:active {
  opacity: 0.7;
}

.button-configure {
  background-color: royalblue;
  color: white;
}

.modal {
  display: none;
  position: fixed;
  z-index: 1;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  color: black;
  overflow: auto;
  background-color: rgba(0, 0, 0, 0.4);
}

.modal-content {
  background-color: #fefefe;
  margin: 15% auto;
  padding: 20px;
  border: 1px solid #888;
  width: 80%;
  max-width: 300px;
}

.modal-content>div {
  margin-bottom: 15px;
}

div.modal-matrix {
  width: 60px;
  height: 60px;
  display: inline-block;
  cursor: pointer;
  padding: 10px;
}

div.modal-matrix.selected,
div.modal-matrix:hover {
  background-color: rgba(0, 0, 0, 0.15);
}

.close {
  color: #aaa;
  float: right;
  font-weight: bold;
}

.close:hover,
.close:focus {
  color: black;
  text-decoration: none;
  cursor: pointer;
}

@keyframes glow {
  0% {
    text-shadow: 0 0 0px blue;
  }

  10% {
    text-shadow: 0 0 5px blue, 0 0 10px blue, 0 0 20px blue, 0 0 30px blue, 0 0 40px blue, 0 0 50px blue, 0 0 60px blue;
  }

  19% {
    text-shadow: 0 0 0px blue;
  }

  20% {
    text-shadow: 0 0 0px red;
  }

  30% {
    text-shadow: 0 0 5px red, 0 0 10px red, 0 0 20px red, 0 0 30px red, 0 0 40px red, 0 0 50px red, 0 0 60px red;
  }

  39% {
    text-shadow: 0 0 0px red;
  }

  40% {
    text-shadow: 0 0 0px orange;
  }

  50% {
    text-shadow: 0 0 5px orange, 0 0 10px orange, 0 0 20px orange, 0 0 30px orange, 0 0 40px orange, 0 0 50px orange, 0 0 60px orange;
  }

  59% {
    text-shadow: 0 0 0px orange;
  }

  60% {
    text-shadow: 0 0 0px green;
  }

  70% {
    text-shadow: 0 0 5px green, 0 0 10px green, 0 0 20px green, 0 0 30px green, 0 0 40px green, 0 0 50px green, 0 0 60px green;
  }

  79% {
    text-shadow: 0 0 0px green;
  }

  80% {
    text-shadow: 0 0 0px magenta;
  }

  91% {
    text-shadow: 0 0 5px magenta, 0 0 10px magenta, 0 0 20px magenta, 0 0 30px magenta, 0 0 40px magenta, 0 0 50px magenta, 0 0 60px magenta;
  }

  100% {
    text-shadow: 0 0 0px magenta;
  }
}

html,
body {
  height: 100%;
  display: flex;
  flex-direction: column;
}

.flex {
  display: flex;
}

.ledx-start span {
  display: flex;
  flex-flow: column;
  align-self: center;
  width: 100%;
  max-width: 640px;
  max-height: 480px;
}

.ledx-start .canvas-colors,
.ledx-start canvas {
  width: 100%;
  /* max-width: 640px;
  max-height: 480px;
  align-self: center; */
  position: relative;
  background: linear-gradient(0deg, #000, #262626);
}

.ledx-start .canvas-colors:before {
  content: "";
  position: absolute;
  top: -2px;
  left: -2px;
  width: calc(100% + 4px);
  height: calc(100% + 4px);
  background-image: linear-gradient(45deg,
      #fb0094,
      #0000ff,
      #00ff00,
      #ffff00,
      #ff0000,
      #fb0094,
      #0000ff,
      #00ff00,
      #ffff00,
      #ff0000);
  background-size: 400%;
  background-position: 0 100%;
  filter: blur(20px);
}

button {
  background: none;
  color: white;
  padding: 5px;
}

.glow {
  animation: glow 10s linear infinite;
}

.main {
  position: relative;
  transition: all 1s;
  top: 0;
  max-height: 100%;
  height: 100%;
  min-width: 375px;
  display: flex;
  flex-direction: column;
}

.ledx-start .top-bar {
  display: flex;
  justify-content: flex-end;
}

.top-bar {
  height: 55px;
  align-items: center;
  flex-grow: 0;
  z-index: 1;
}

.title {
  font-size: 1.15em;
  color: white;
  margin: 0;
  text-align: center;
  transition: all 1s;
  left: 0;
  position: absolute;
  width: 100%;
}

.title p {
  margin-top: -5px;
  transition: margin 1s;
  font-size: .6em;
}

.top-menu {
  flex-grow: 0;
  width: auto;
}

.top-bar .top-menu {
  padding-right: 9px;
}

.top-menu>div {
  display: inline-block;
}

.bottom-bar {
  visibility: visible;
  flex-direction: column;
  flex-grow: 0;
  overflow: hidden;
  text-align: center;
}

.bottom-bar,
.top-menu {
  transition: all 1s;
  opacity: 0%;
}

.menu-popup {
  opacity: 0%;
  position: absolute;
  background-color: var(--bar-black);
  color: #f5f5f5;
  border: solid 2px var(--light-black);
  padding: 5px;
  transition: all .5s;
  border-radius: 10px;
  visibility: hidden;
}

.bottom-bar .menu-popup {
  overflow: auto;
  height: 0;
  position: relative;
  border-radius: 0;
}

.bottom-bar input[type="radio"]:checked~.menu-popup {
  opacity: 100%;
  visibility: visible;
  height: 185px;
}

.menu-matrix p {
  display: inline-block;
}

input[type="radio"]:checked~.menu-popup {
  opacity: 100%;
  visibility: visible;
}

.ledx-start .canvas-container {
  flex-grow: 1;
  display: flex;
  justify-content: center;
  opacity: 100%;
}

#toggle-bkg-container {
  display: flex;
}

input[type="password"] {
  width: 115px;
}

.canvas-container {
  transition: opacity 1s;
  opacity: 0%;
}

.ledx-splash .main {
  top: 35%;
  height: 20%;
}

.ledx-splash .title {
  font-size: 4em;
}

.ledx-splash .title p {
  margin-top: -30px;
}

.ledx-start .app,
.ledx-start .top-bar,
.ledx-start .bottom-bar,
.ledx-start .top-menu {
  opacity: 100%;
}

.ledx-start .title {
  width: 120px;
}

.menu-matrix {
  text-align: center;
}

.menu-matrix input {
  width: 75px;
  text-align: right;
  display: inline-block;
}

.bottom-bar .menu-popup .column-menu {
  display: flex;
  flex-direction: column;
}

.bkg-show {
  flex-grow: 1;
  padding: 0 5px;
  text-align: center;
  display: none;
}

.grow-container {
  flex-grow: 1;
}

.control-blocks {
  vertical-align: top;
  position: relative;
  width: 92px;
  border: 1px solid var(--light-black);
  border-radius: 5px;
  display: inline-block;
  min-height: 150px;
  margin: 5px;
  padding: 5px;
}

.toggle-tab {
  margin-bottom: 0;
  border-bottom-color: white;
  border-bottom-right-radius: 0;
  border-bottom-left-radius: 0;
}

.control-blocks .toggle {
  height: 50px;
  min-width: 65px;
}

@media only screen and (max-width: 500px) {
  #toggle-bkg-cam-container .control-blocks {
    width: 80px;
  }

  .ledx-start span,
  .ledx-start canvas {
    max-width: 320px;
    max-height: 240px;
  }
}

@media only screen and (max-height: 500px) {

  .ledx-start span,
  .ledx-start canvas {
    max-width: 210px;
    max-height: 140px;
  }
}

@media only screen and (min-height: 755px) {
  .bottom-bar input[type="radio"]:checked~.menu-popup {
    height: 300px;
  }
}

@media (pointer:none),
(pointer:coarse) {

  #toggle-bkg-screen,
  #toggle-bkg-screen-lbl {
    display: none;
  }

  #toggle-bkg-audio,
  #toggle-bkg-audio-lbl {
    display: none;
  }
}

.arrow {
  padding: 0;
  width: 32px;
  height: 32px;
  border-radius: 5px;
  background: url('data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0nMS4wJyBlbmNvZGluZz0ndXRmLTgnPz48c3ZnIHZlcnNpb249JzEuMScgaWQ9J0xheWVyXzEnIHhtbG5zPSdodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZycgeG1sbnM6eGxpbms9J2h0dHA6Ly93d3cudzMub3JnLzE5OTkveGxpbmsnIHg9JzBweCcgeT0nMHB4JyB3aWR0aD0nMzJweCcgaGVpZ2h0PSczMnB4JyB2aWV3Qm94PScwIDAgMTIyLjg3OSAxMTcuMDQxJyBlbmFibGUtYmFja2dyb3VuZD0nbmV3IDAgMCAzMiAzMicgeG1sOnNwYWNlPSdwcmVzZXJ2ZSc+PGc+PHBhdGggZD0nTTEzLjQxNywyNi44NjR2NTYuNzE4YzAsOS4xNzYsMy42MzgsMTcuNTMxLDkuNDk3LDIzLjU5OGM1Ljg3Niw2LjA4NCwxMy45NjksOS44NjEsMjIuODYsOS44NjFzMTYuOTgzLTMuNzc3LDIyLjg2LTkuODYxIGM1Ljg1OS02LjA2Niw5LjQ5Ni0xNC40MjIsOS40OTYtMjMuNTk4VjMzLjcyNWwwLDB2LTAuMjY3YzAtMy41NjYsMS4zOTYtNi43OTUsMy42NDgtOS4xMjZjMi4yMzQtMi4zMTMsNS4zMjgtMy43NSw4Ljc0NC0zLjc1IGMzLjQxNCwwLDYuNTEsMS40MzcsOC43NDQsMy43NWMyLjI1LDIuMzMxLDMuNjQ4LDUuNTYsMy42NDgsOS4xMjZ2NzYuNzgxaDE5Ljk2M1YzMy40NThjMC05LjE3Ni0zLjYzNy0xNy41My05LjQ5Ni0yMy41OTcgQzEwNy41MDgsMy43NzcsOTkuNDE0LDAsOTAuNTIzLDBjLTguODkzLDAtMTYuOTg0LDMuNzc3LTIyLjg1OSw5Ljg2Yy01Ljg2MSw2LjA2Ny05LjQ5OCwxNC40MjEtOS40OTgsMjMuNTk3djQ4LjUxM2wwLDB2MS42MTEgYzAsMy41NjYtMS4zOTcsNi43OTUtMy42NDcsOS4xMjdjLTIuMjM1LDIuMzEzLTUuMzMsMy43NS04Ljc0NSwzLjc1cy02LjUxLTEuNDM4LTguNzQ1LTMuNzVjLTIuMjUtMi4zMzItMy42NDgtNS41NjEtMy42NDgtOS4xMjcgVjI2Ljg2NGgxMy45NjZMMjQuMTc4LDBMMCwyNi44NjRIMTMuNDE3TDEzLjQxNywyNi44NjR6Jy8+PC9nPjwvc3ZnPg==')
}
  )";
