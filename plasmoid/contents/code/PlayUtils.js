/**
 * Play Modes
 * 0 - sequential 
 * 1 - random
 * 2 - loop
 **/

var toPlay = [];
var played = [];


function getNextStationIndex() {
    var next = toPlay.shift()
    if (next != undefined)
        played.push(next);
    // print ("Next Station: " + next)
    return next;
}

function getPreviousStationIndex() {
    var prev = played.pop()
    if (prev  != undefined)
        toPlay.unshift(prev)
    // print ("Previous Station: " + played[played.length - 1])
    return played[played.length - 1];
}

function genRandonPlayList(listSise) {
    // print ("genRandomPlayList")
    for (var i = 0; i < listSise; i ++) {
        toPlay.push( Math.floor((Math.random() * listSise) )  )
    }
}

function genSecuentialPlayList(listSise, current) {
    
    for (var i = current; i < listSise; i ++) {
        toPlay.push(i)
    }
    // print ("genSecuentialPlayList" + toPlay)
}

function genLoopPlayList(current) {
    // print ("genLoopPlayList")
    for (var i = 0; i < 1000; i ++) {
        toPlay.push(current)
    }
}

function genPlayList(playMode, listSize, current) {
    toPlay = [];
    played = [];

    if (current < 0)
        current = 0;
    
    if (playMode == 0)
        genSecuentialPlayList(listSize, current)
    if (playMode == 1)
        genRandonPlayList(listSize)
    if (playMode == 2)
        genLoopPlayList(current)
    
    // print ("Play Program: ")
    print (toPlay);
}