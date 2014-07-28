var inject = '(' + function () {

    window.pressAndListen = {
        player: "youtube"
    } ;

    window.pressAndListen.playing = function () {
        return !yt.player.utils.VideoTagPool.instance_.g[0].paused;
    } ;

    window.pressAndListen.play = function () {
        yt.player.utils.VideoTagPool.instance_.g[0].playVideo();
    };

    window.pressAndListen.pause = function () {
        yt.player.utils.VideoTagPool.instance_.g[0].pauseVideo();
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
            artist: document.getElementById('watch7-user-header').getElementsByClassName('yt-user-name')[0].text
        };
    };

} + ') ()';

var script = document.createElement('script');
script.textContent = inject;
(document.head || document.documentElement).appendChild(script);
script.parentNode.removeChild(script);