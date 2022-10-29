const char connection_js_h[] PROGMEM = R"(
const NetworkStatus = {
    CONNECTED: 0,
    CONNECTING: 1,
    DISCONNECTED: 2,
    ERROR: 3,
}
const LEDX_NAME = 'LedX';
const LEDX_IP = '192.168.1.100';
const EMPTY = new Uint8Array(3);
class WsConnection {
    constructor() {
        this.deviceInfo = {
            protocol: 'ws://',
            ip: LEDX_IP,
            port: ':7171/',
            status: NetworkStatus.DISCONNECTED,
            uploadBytes: 0,
            downloadBytes: 0,
            msgReceived: 0,
            msgSent: 0,
        };
        this.nodes = new Map();
    }
    // Requires a function which outputus a PixelInfo of object
    // which contains a string of pixels.
    init(initNodes) {
        this.initNodes = initNodes;
        setTimeout(() => {
            this.wsConnect();
        }, 3000);
        //        this.createFakeNode();
    }
    createFakeNode() {
        let newDriver = new Driver({ id: 'Ledx-1001', ip: '192.168.1.1', type: 'NODE', channels: 2, leds: 900 }, this);
        this.nodes.set(newDriver.id, newDriver);
    }
    wsConnect(retries = 2) {
        var self = this;
        if (this.deviceInfo.status == NetworkStatus.CONNECTING) {
            return
        }
        this.deviceInfo.status = NetworkStatus.CONNECTING;
        logI("Starting connection to: " + (this.deviceInfo.protocol + this.deviceInfo.ip + this.deviceInfo.port));
        try {
            this.ws = new WebSocket(this.deviceInfo.protocol + this.deviceInfo.ip + this.deviceInfo.port);
            this.ws.onopen = function (e) {
                logI("Network connected!");
                self.deviceInfo.status = NetworkStatus.CONNECTED;
            };
            this.ws.onclose = function (e) {
                logI("Network Closed!");
                self.deviceInfo.status = NetworkStatus.DISCONNECTED;
                [...self.nodes.keys()].forEach((node) => {
                    self.nodes.get(node).isLost = true;
                });
            };
            this.ws.onmessage = function (rawRcvdMsg) {
                self.deviceInfo.downloadBytes += rawRcvdMsg.data.length;
                self.deviceInfo.msgReceived++;
                self.handleRcvdMsg(JSON.parse(rawRcvdMsg.data));
            };
            this.ws.onerror = function (e) {
                logI("Websocket unexpected error: " + JSON.stringify(e));
                self.handleRcvdMsg({
                    id: LEDX_NAME,
                    command: 'LOST',
                });
                self.deviceInfo.status = NetworkStatus.DISCONNECTED;
                self.ws.close();
                if (retries > 0) {
                    self.deviceInfo.status = NetworkStatus.CONNECTING;
                    setTimeout(() => {
                        self.wsConnect(retries - 1);
                    }, 1000);
                }
            };
        } catch (e) { }
    }
    handleRcvdMsg(rcvdMsg) {
        if (!rcvdMsg.command) {
            logI('Invalid msg: ' + rcvdMsg);
            return;
        }
        let currentDriver = this.nodes.get(rcvdMsg.id);
        if (rcvdMsg.command === 'LOST') {
            logI('Node lost: ' + rcvdMsg.id);
            if (currentDriver) {
                currentDriver.isLost = true;
            }
        } else if (rcvdMsg.command === 'IDENTIFY') {
            if (!currentDriver) {
                logI('Adding node: ' + JSON.stringify(rcvdMsg));
                let newDriver = new Driver(rcvdMsg, this);
                if (this.initNodes && this.initNodes[rcvdMsg.id]) {
                    newDriver.masterTemplate = this.initNodes[rcvdMsg.id];
                }
                this.nodes.set(rcvdMsg.id, newDriver);
            } else if (currentDriver.isLost) {
                logI('Recovering Node: ' + JSON.stringify(rcvdMsg));
                currentDriver.isLost = false;
            } else {
                // logI('Updating Node: ' + JSON.stringify(rcvdMsg));
                currentDriver.metadata = rcvdMsg;
            }
        }
    }
    sendWsMsg(msg = EMPTY) {
        if (!this.ws || !this.ws.readyState || this.ws.readyState == WebSocket.CLOSED) {
            logI("Msg not sent: Not Connected");
            return 0;
        }
        if (this.ws.readyState == WebSocket.OPEN && msg.length > 0) {
            this.deviceInfo.uploadBytes += msg.length;
            this.deviceInfo.msgSent++;
            this.ws.send(msg);
            return msg.length;
        } else {
            logI("Msg not sent: " + JSON.stringify(this.ws));
        }
        return 0;
    }
    getNodes() {
        return this.nodes;
    }
    getDeviceInfo() {
        return this.deviceInfo;
    }
}

class Driver {
    constructor(metadata, ws) {
        logI('Starting driver.');
        this.ws = ws;
        this.id = metadata.id;
        this.metadata = metadata;
        if (metadata.template) {
            logI('Using template from driver.');
            this.masterTemplate = MasterTemplate.fromJson(JSON.parse(window.atob(metadata.template)));
            globalMIdx += this.allModulesCount();
        } else if (this.existStoredTemplate(this.id)) {
            logI('Using template from local storage: ' + this.id);
            this.masterTemplate = this.loadTemplate(this.id);
            globalMIdx += this.allModulesCount();
        } else if (!this.masterTemplate || (this.masterTemplate.channels && metadata.channels != this.masterTemplate.channels.length)) {
            logI('Using template default.');
            this.masterTemplate = MasterTemplate.fromChannels(metadata.channels);
            globalMIdx += this.allModulesCount();
        } else {
            logI('Template not initialized.');
        }
        this.isLost = false;
        this.color = Math.floor(Math.random() * 16777215).toString(16);
    }
    toHtml(prevHtml = '') {
        const m = this.metadata;
        const newHtml = 'Ip: ' + m.ip + ', Type: ' + m.type + ', Channels: ' + m.channels
            + ', Leds: ' + m.leds;
        return newHtml === prevHtml ? '' : newHtml;
    }
    setTemplateName(cIdx, templateName) {
        const channels = this.masterTemplate.channels;
        if (!channels[cIdx]) {
            channels[cIdx] = new Channel(templateName);
        } else {
            channels[cIdx].templateName = templateName;
        }
        console.log('Update driver channel [template name] ' + cIdx + ' ' + templateName);
    }
    setTemplateColorOrder(cIdx, colorOrder) {
        const channels = this.masterTemplate.channels;
        channels[cIdx].colorOrder = colorOrder;
        console.log('Update driver channel [template colorOrder] ' + cIdx + ' ' + colorOrder);
    }
    getTemplateName(cIdx) {
        return this.masterTemplate.channels[cIdx].templateName;
    }
    addModule(cIdx, mIdx, module) {
        this.masterTemplate.channels[cIdx].modules[mIdx] = module;
        console.log('Update driver: addModule ' + cIdx + '-' + mIdx + ' ' + JSON.stringify(module));
    }
    removeModule(cIdx, mIdx) {
        this.masterTemplate.channels[cIdx].modules[mIdx] = undefined;
        console.log('Update driver: removeModule ' + cIdx + '-' + mIdx);
    }
    updateModule(cIdx, mIdx, x, y, w, h, d) {
        const module = this.masterTemplate.channels[cIdx].modules[mIdx];
        module.x = parseInt(x);
        module.y = parseInt(y);
        module.w = parseInt(w);
        module.h = parseInt(h);
        module.d = parseInt(d);
        console.log('Update driver: updateModule ' + cIdx + '-' + mIdx + ' ' + JSON.stringify(module));
    }

    saveDriver() {
        const eTemplate = window.btoa(JSON.stringify(this.masterTemplate));
        if (eTemplate !== this.currentStoredTemplate(this.id)) {
            this.setETemplate(this.id, eTemplate)
            this.ws.sendWsMsg(eTemplate);
            console.log('New template saved, sent: ' + eTemplate);
        } else {
            console.log('No changes in the new template');
        }
    }
    loadTemplate(id) {
        const template = this.getETemplate(id);
        return MasterTemplate.fromJson(JSON.parse(window.atob(template)));
    }
    currentStoredTemplate(id) {
        return this.existStoredTemplate(id) ? this.getETemplate(id) : '';
    }
    existStoredTemplate(id) {
        return this.getETemplate(id) !== null;
    }
    setETemplate(id, eTemplate) {
        localStorage.setItem(id + '-template', eTemplate);
    }
    getETemplate(id) {
        return localStorage.getItem(id + '-template');
    }
    allModulesCount() {
        const channels = this.masterTemplate.channels;
        let count = 0;
        for (let i = 0; i < channels.length; i++) {
            if (channels[i]) {
                count += this.moduleCount(i);
            }
        }
        return count;
    }
    moduleCount(cIdx) {
        const modules = this.masterTemplate.channels[cIdx].modules;
        let count = 0;
        for (let i = 0; i < modules.length; i++) {
            if (modules[i]) {
                count++;
            }
        }
        return count;
    }
}
class MasterTemplate {
    constructor() {
        this.channels = [];
    }
    static fromChannels(numChannels) {
        let template = new MasterTemplate();
        for (let i = 0; i < numChannels; i++) {
            template.channels[i] = new Channel();
        }
        return template;

    }
    static fromJson(json) {
        let template = new MasterTemplate();
        if (json && json.channels && json.channels.length) {
            for (let i = 0, ii = 0; i < json.channels.length; i++) {
                if (json.channels[i]) {
                    template.channels[ii++] = Channel.fromJson(json.channels[i]);
                }
            }
        }
        return template;

    }
}
class Channel {
    constructor() {
        this.templateName = 'custom';
        this.colorOrder = 'rgb'
        this.modules = [];
    }
    static fromJson(json) {
        let channel = new Channel();
        if (json.templateName) {
            channel.templateName = json.templateName;
        }
        if (json.colorOrder) {
            channel.colorOrder = json.colorOrder;
        }
        if (json.modules && json.modules.length) {
            for (let i = 0, ii = 0; i < json.modules.length; i++) {
                if (json.modules[i]) {
                    channel.modules[ii++] = Module.fromJson(json.modules[i]);
                }
            }
        }
        return channel;
    }
}

const ModuleDirection = {
    ZIGZAGV_CB: 0,
    ZIGZAGV_AD: 1,
    ZIGZAGV_BC: 2,
    ZIGZAGV_DA: 3,
    ZIGZAGH_CB: 4,
    ZIGZAGH_AD: 5,
    ZIGZAGH_BC: 6,
    ZIGZAGH_DA: 7,
}

class Module {
    constructor(x, y, w, h, d) {
        this.x = parseInt(x);
        this.y = parseInt(y);
        this.w = parseInt(w);
        this.h = parseInt(h);
        this.d = parseInt(d);
    }
    getPixelModule() {
        if (this.d == ModuleDirection.ZIGZAGV_AD || this.d == ModuleDirection.ZIGZAGH_AD) {
            return new Module(this.x, this.y, this.w, this.h, this.d);
        } else if (this.d == ModuleDirection.ZIGZAGV_BC || this.d == ModuleDirection.ZIGZAGH_BC) {
            return new Module(this.x + this.w - 1, this.y, -this.w, this.h, this.d);
        } else if (this.d == ModuleDirection.ZIGZAGV_CB || this.d == ModuleDirection.ZIGZAGH_CB) {
            return new Module(this.x, this.y + this.h - 1, this.w, -this.h, this.d);
        } else {
            return new Module(this.x + this.w - 1, this.y + this.h - 1, -this.w, -this.h, this.d);
        }
    }
    getBorderModule() {
        if (this.d == ModuleDirection.ZIGZAGV_AD || this.d == ModuleDirection.ZIGZAGH_AD) {
            return new Module(this.x, this.y, this.w, this.h, this.d);
        } else if (this.d == ModuleDirection.ZIGZAGV_BC || this.d == ModuleDirection.ZIGZAGH_BC) {
            return new Module(this.x + this.w, this.y, -this.w, this.h, this.d);
        } else if (this.d == ModuleDirection.ZIGZAGV_CB || this.d == ModuleDirection.ZIGZAGH_CB) {
            return new Module(this.x, this.y + this.h, this.w, -this.h, this.d);
        } else {
            return new Module(this.x + this.w, this.y + this.h, -this.w, -this.h, this.d);
        }
    }
    isVerticalDirection() {
        return this.d == ModuleDirection.ZIGZAGV_AD || this.d == ModuleDirection.ZIGZAGV_CB
            || this.d == ModuleDirection.ZIGZAGV_BC || this.d == ModuleDirection.ZIGZAGV_DA;
    }
    static fromJson(json) {
        return new Module(json.x, json.y, json.w, json.h, json.d);
    }
}
)";
