var inject = '(' + function () {

    window.pressAndListen = {
        player: "deezer"
    } ;

    window.pressAndListen.playing = function () {
        return dzPlayer.isPlaying();
    };

    window.pressAndListen.play = function () {
        document.querySelector('.control.control-play').click();
    };

    window.pressAndListen.pause = function () {
        document.querySelector('.control.control-pause').click();
    };

    window.pressAndListen.prev = function () {
        document.querySelector('.control.control-prev').click();
    };

    window.pressAndListen.next = function () {
        document.querySelector('.control.control-next').click();
    };

    window.pressAndListen.song = function () {
        return {
            id: dzPlayer.getSongId(),
            title: dzPlayer.getSongTitle(),
            artist: dzPlayer.getArtistName()
        };
    };

} + ') ()';

var script = document.createElement('script');
script.textContent = inject;
(document.head || document.documentElement).appendChild(script);
script.parentNode.removeChild(script);