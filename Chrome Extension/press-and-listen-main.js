var players = {};

function createSocket(port) {
    return (function (port) {
        var websock = new WebSocket('ws://localhost:52132');
        websock.onopen = function () {
            websock.send(JSON.stringify({
                command: 'initialized',
                browser: 'chrome',
                player: port.name
            }));
        };
        websock.onmessage = function (e) {
            var cmd = e.data;
            if (cmd == "closeTab") {
                websock.close();
                chrome.tabs.remove(port.sender.tab.id);
            }
            else if (cmd == "focusTab") {
                chrome.tabs.update(port.sender.tab.id, { selected: true });
            }
            else {
                port.postMessage({
                    type: 'command',
                    command: cmd
                });
            }
        }
        return websock;
    }) (port);
}

chrome.runtime.onConnect.addListener(function (port) {
    players[port.sender.tab.id] = createSocket(port);
    port.onMessage.addListener((function (port) {
        return function (msg) {
            var websock = players[port.sender.tab.id];
            if (websock.readyState == WebSocket.OPEN) {
                if (msg.type == 'songinfo') {
                    websock.send(JSON.stringify({
                        command: 'songinfo',
                        playing: msg.info.playing,
                        info: msg.info.info
                    }));
                }
            }
            else if (websock.readyState != WebSocket.CONNECTING) {
                players[port.sender.tab.id] = createSocket(port);
            }
        };
    }(port)));
});

chrome.tabs.onRemoved.addListener(function (tabId, removeInfo) {
    if (tabId in players) {
        players[tabId].close();
        delete players[tabId];
    }
});