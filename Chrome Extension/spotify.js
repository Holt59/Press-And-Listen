var inject = '(' + function () {

    window.pressAndListen = {
        player: "spotify"
    };

    window.pressAndListen._iframe = document.getElementById('app-player');
    window.pressAndListen._document = window.pressAndListen._iframe.contentDocument;

    window.pressAndListen.playing = function () {
        return window.pressAndListen._iframe.contentDocument.getElementById('play-pause').getAttribute('class').indexOf('playing') >= 0;
    };

    window.pressAndListen.play = function () {
        if (!window.pressAndListen.playing()) {
            window.pressAndListen._iframe.contentDocument.getElementById('play-pause').click()
        }
    };

    window.pressAndListen.pause = function () {
        if (window.pressAndListen.playing()) {
            window.pressAndListen._iframe.contentDocument.getElementById('play-pause').click()
        }
    };

    window.pressAndListen.prev = function () {
        window.pressAndListen._iframe.contentDocument.getElementById('previous').click()
    };

    window.pressAndListen.next = function () {
        window.pressAndListen._iframe.contentDocument.getElementById('next').click()
    };

    window.pressAndListen.song = function () {
        var songInfo = window.pressAndListen._iframe.contentDocument.getElementById('track-name');
        var artistInfo = window.pressAndListen._iframe.contentDocument.getElementById('track-artist');
        if (!songInfo) {
            return false;
        }
        return {
            id: songInfo.getElementsByTagName('a')[0].href,
            title: songInfo.getElementsByTagName('a')[0].text,
            artist: artistInfo.getElementsByTagName('a')[0].text
        };
    };

} + ') ()';

var script = document.createElement('script');
script.textContent = inject;
(document.head || document.documentElement).appendChild(script);
script.parentNode.removeChild(script);