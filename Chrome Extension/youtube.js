var inject = '(' + function () {

    window.pressAndListen = {
        player: "youtube"
    } ;

    function getVideoElement() {
        return document.getElementsByTagName('video')[0];
    }

    window.pressAndListen.playing = function () {
        return !getVideoElement().paused;
    } ;

    window.pressAndListen.play = function () {
        getVideoElement().play();
    };

    window.pressAndListen.pause = function () {
        getVideoElement().pause();
    };

    window.pressAndListen.prev = function () {
        var btn = document.getElementsByClassName('prev-playlist-list-item')[0];
        if (btn) {
            btn.click();
        }
    };

    window.pressAndListen.next = function () {
        var btn = document.getElementsByClassName('next-playlist-list-item')[0];
        if (btn) {
            btn.click();
        }
    };

    window.pressAndListen.song = function () {
        var songInfo = document.getElementById('watch7-content');
        return {
            id: songInfo.querySelectorAll('meta[itemprop="videoId"]')[0].content,
            title: songInfo.querySelectorAll('meta[itemprop="name"]')[0].content,
            artist: document.getElementById('watch7-user-header').querySelector('.yt-user-info a').text
        };
    };

} + ') ()';

var script = document.createElement('script');
script.textContent = inject;
(document.head || document.documentElement).appendChild(script);
script.parentNode.removeChild(script);