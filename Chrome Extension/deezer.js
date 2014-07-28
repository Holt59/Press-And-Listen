var inject = '(' + function () {

    window.pressAndListen = {
        player: "deezer"
    } ;

    window.pressAndListen.playing = function () {
        return dzPlayer.isPlaying();
    };

    window.pressAndListen.play = function () {
        document.getElementById('player_control_play').click();
    };

    window.pressAndListen.pause = function () {
        document.getElementById('player_control_pause').click();
    };

    window.pressAndListen.prev = function () {
        document.getElementById('player_control_prev').click();
    };

    window.pressAndListen.next = function () {
        document.getElementById('player_control_next').click();
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