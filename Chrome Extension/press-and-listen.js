var inject = '(' + function () {

    var websock ;

    function send (info) {
        websock.send(JSON.stringify(info));
    }
    
    function createSocket() {

        websock = new WebSocket('ws://localhost:52132');

        websock.onopen = function () {
            send({
                command: 'initialized',
                browser: 'chrome',
                player: window.pressAndListen.player
            });
        };

        websock.onmessage = function (e) {
            var cmd = e.data;

            if (cmd == "closeTab") {
                websock.close();
                document.dispatchEvent(new CustomEvent('PaL_CloseTabEvent'));
                return;
            }

            if (cmd == "focusTab") {
                document.dispatchEvent(new CustomEvent('PaL_FocusTabEvent'));
                return;
            }

            if (cmd == "toggle") {
                if (window.pressAndListen.playing()) {
                    window.pressAndListen.pause();
                }
                else {
                    window.pressAndListen.play();
                }
            }
            else if (cmd == "play") {
                window.pressAndListen.play();
            }
            else if (cmd == "pause") {
                window.pressAndListen.pause();
            }
            else if (cmd == "prev") {
                window.pressAndListen.prev();
            }
            else if (cmd == "next") {
                window.pressAndListen.next();
            }
            else {
                console.log("Unknow command: " + cmd);
            }
            send({
                command: 'songinfo',
                playing: window.pressAndListen.playing(),
                info: window.pressAndListen.song()
            });
        }

    }

    createSocket();

    setInterval(function () {
        if (websock.readyState == WebSocket.OPEN) {
            send({
                command: 'songinfo',
                playing: window.pressAndListen.playing(),
                info: window.pressAndListen.song()
            });
        }
        else if (websock.readyState != WebSocket.CONNECTING) {
            createSocket();
        }
    }, 500);

} + ') ()';

document.addEventListener('PaL_CloseTabEvent', function (e) {
    chrome.extension.sendMessage("closeTab");
});

document.addEventListener('PaL_FocusTabEvent', function (e) {
    chrome.extension.sendMessage("focusTab");
});

var script = document.createElement('script');
script.textContent = inject;
(document.head || document.documentElement).appendChild(script);
script.parentNode.removeChild(script);