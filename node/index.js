var player = require('play-sound')(opts = {})
var tonal = require('tonal')
var Scale = tonal.Scale

//create a synth and connect it to the master output (your speakers)

var recordsGen = [1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2]
var recordsTimerGen = [1524048573000,1524048573500,1524048574000,1524048575000,1524048575500,1524048576000,1524048577000,1524048577500,1524048578000,1524048579000,1524048579500,152404858000]
//var recordsGen = [1,1,1]
//var recordsTimerGen = [1524048573000,1524048574000,1524048575000]
//
var loopTimer = recordsTimerGen[1]-recordsTimerGen[0]


var possibleImproTimers = []
var scale = []

var notes = ["A","Bb","B","Cb","C","D","Eb","E","Fb","F","Gb","G"]

var timers = [];
var date

var noteProbability = 35


function play(){

  for (let i in timers){
    setTimeout(function () {
      callBack(i)
      if(timers[i].type === "drum"){
        let instrument = (timers[i].value == 1)?'sound/wethat2.wav':'sound/rapsnare.wav'
        player.play(instrument, function(err){
          if (err) console.log("err drum",err);
        })
      }

      let proba = Math.floor(Math.random() * 100)
          let random = Math.floor(Math.random() * scale.length)
          console.log(scale[random]);
          let note = ("sound/" + scale[random] + ".wav").toLocaleLowerCase()
          player.play(note, function(err){
            if (err) console.log("err sound",note,err);
          })
          noteProbability += 10
    },timers[i].timer);
  }

}

function callBack(i){
  console.log("cb: ",i);
  if (i == timers.length - 1){
    setTimeout(function () {
      play()
    }, loopTimer)
  }
}

function improvise(){
  let scales = Scale.names();
  console.log(scales);
  let randomScale = Math.floor(Math.random() * scales.length)
  let randomNote = Math.floor(Math.random() * notes.length)
  //scale = Scale.notes(notes[randomNote] + " " + scales[randomScale])
  // scale = Scale.notes("C major blues")
  scale = Scale.notes("D whole tone pentatonic")

  console.log(notes[randomNote] + " " + scales[randomScale]);
  console.log(scale);

  for (i = 0;i<recordsTimerGen.length - 1;i++){
    let interval = 0
    let distance = recordsTimerGen[1]-recordsTimerGen[0]
    interval = distance/300
    possibleImproTimers.push(recordsTimerGen[i])

    for (j = 1; j<interval ; j++){
      possibleImproTimers.push((distance/(interval+1)*j) + recordsTimerGen[i])
      console.log("interval : ",interval);
      console.log("recordsTimerGen[i] : ",recordsTimerGen[i]);
    }
  }

  possibleImproTimers.push(recordsTimerGen[recordsTimerGen.length-1])


  for (i=0;i<possibleImproTimers.length;i++){
    if(possibleImproTimers[i] == recordsTimerGen[0]){
      timers.push({
        "type":"drum",
        "value":recordsGen[0],
        "timer":possibleImproTimers[i] - possibleImproTimers[0]
      })
      recordsGen.shift()
      recordsTimerGen.shift()
    }else{
      timers.push({
        "type":"note",
        "timer":possibleImproTimers[i] - possibleImproTimers[0]
      })
    }
  }

  console.log(possibleImproTimers)


  timers.sort(compare)
  console.log(timers)

  play()
}

function compare(a,b) {
  if (a.timer < b.timer)
    return -1;
  if (a.timer > b.timer)
    return 1;
  return 0;
}

n = new Date().getTime()
improvise()
