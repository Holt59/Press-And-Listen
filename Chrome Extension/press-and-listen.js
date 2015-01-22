var inject = '(' + function () {

    var websock ;

    function send (info) {
        websock.send(JSON.stringify(info));
    }

    function sendSongInfo() {
        document.dispatchEvent(new CustomEvent('PaL_SongInfoEvent', {
            detail: {
                playing: window.pressAndListen.playing(),
                info: window.pressAndListen.song()
            }
        }));
    }
    
    document.addEventListener('PaL_CommandEvent', function (e) {
        switch (e.detail) {
            case "toggle":
                if (window.pressAndListen.playing()) {
                    window.pressAndListen.pause();
                }
                else {
                    window.pressAndListen.play();
                }
                break;
            case "play":
                window.pressAndListen.play();
                break;
            case "pause":
                window.pressAndListen.pause();
                break;
            case "prev":
                window.pressAndListen.prev();
                break;
            case "next":
                window.pressAndListen.next();
                break;
            default:
                console.log("Unknow command: " + cmd);
        }
        sendSongInfo();
    });

    setInterval(function () {
        sendSongInfo();
    }, 500);

    document.dispatchEvent(new CustomEvent('Pal_OpenEvent', { detail: window.pressAndListen.player }));

} + ') ()';

document.addEventListener('Pal_OpenEvent', function (e) {
    var port = chrome.runtime.connect({ name: e.detail });
    document.addEventListener('PaL_SongInfoEvent', function (e) {
        port.postMessage({
            type: 'songinfo',
            info: e.detail
        });
    });
    port.onMessage.addListener(function (msg) {
        if (msg.type == 'command') {
            document.dispatchEvent(new CustomEvent('PaL_CommandEvent', { detail: msg.command }));
        }
    });
});

var script = document.createElement('script');
script.textContent = inject;
(document.head || document.documentElement).appendChild(script);
script.parentNode.removeChild(script);