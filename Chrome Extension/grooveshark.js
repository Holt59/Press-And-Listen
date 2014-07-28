var inject = '(' + function () {

    window.pressAndListen = {
        player: "grooveshark"
    };


    window.pressAndListen.playing = function () {
        document.getElementById('play-pause').getAttribute('class').indexOf('playing') >= 0;
    };

    window.pressAndListen.play = function () {
        if (!window.pressAndListen.playing()) {
            document.getElementById('play-pause').click();
        }
    };

    window.pressAndListen.pause = function () {
        if (window.pressAndListen.playing()) {
            document.getElementById('play-pause').click();
        }
    };

    window.pressAndListen.prev = function () {
        document.getElementById('play-prev').click()
    };

    window.pressAndListen.next = function () {
        document.getElementById('play-next').click()
    };

    window.pressAndListen.song = function () {
        return {
            id: document.getElementById('now-playing-metadata').getElementsByClassName('song')[0].getAttribute('data-song-id'),
            title: document.getElementById('now-playing-metadata').getElementsByClassName('song')[0].title,
            artist: document.getElementById('now-playing-metadata').getElementsByClassName('artist')[0].title
        };
    };

} + ') ()';

var script = document.createElement('script');
script.textContent = inject;
(document.head || document.documentElement).appendChild(script);
script.parentNode.removeChild(script);