const redLight = document.querySelector('.light.red.right');
const yellowLight = document.querySelector('.light.yellow.right');
    const greenLight = document.querySelector('.light.green.right');
    const car7 = document.getElementById("car7");
const car6 = document.getElementById("car6");
const car3 = document.getElementById("car3");
const car1 = document.getElementById("car1");

let sequenceTimeouts = [];
let blinkingInterval = null;
let greenCarInterval = null;
let isGreenLoopRunning = false;

function runCarCycle(carID) {
  const car = document.getElementById(carID);
  car.classList.add('drive1');
  setTimeout(() => {
    car.classList.remove('drive1');
    car.classList.add('drive2');
  }, 500);
  setTimeout(() => {
    car.classList.remove('drive2');
    car.classList.add('drive3');
  }, 1500); 
   setTimeout(() => {
    car.classList.remove('drive3');
    car.classList.add('drive4');
  }, 3500); 
   setTimeout(() => {
    car.classList.remove('drive4');
  }, 4000); 
}


function setLight({ red, yellow, green }) {
    redLight.style.opacity = red ? 1 : 0.3;
    yellowLight.style.opacity = yellow ? 1 : 0.3;
    greenLight.style.opacity = green ? 1 : 0.3;

    if (blinkingInterval !== null) {
        currentLight = "blinking";
    } else if (green) {
        currentLight = "green";
    } else if (yellow) {
        currentLight = "yellow";
    } else if (red) {
        currentLight = "red";
    }
}

function clearAllTimers() {
    sequenceTimeouts.forEach(t => clearTimeout(t));
    sequenceTimeouts = [];
    if (blinkingInterval !== null) {
        clearInterval(blinkingInterval);
        blinkingInterval = null;
    }
}

function goToGreen() {

    if (currentLight === "green") return;

    clearAllTimers();

    if (currentLight === "red") {
        setLight({ red: true, yellow: false, green: false });
        sequenceTimeouts.push(setTimeout(() => {
            setLight({ red: true, yellow: true, green: false });
            sequenceTimeouts.push(setTimeout(() => {
                setLight({ red: false, yellow: false, green: true });
    

                runCarCycle('car1');
                runCarCycle('car3');
                runCarCycle('car6');
                runCarCycle('car7');

                isGreenLoopRunning = true;

                greenCarInterval = setInterval(() => {
                    if (!isGreenLoopRunning) return;
                        runCarCycle('car1');
                        runCarCycle('car3');
                        runCarCycle('car6');
                        runCarCycle('car7');
                }, 4500); 
            }, 1000));
        }, 800));
    }
   else if (currentLight === "yellow") {
        setLight({ red: false, yellow: false, green: true});

        runCarCycle('car1');
                runCarCycle('car3');
                runCarCycle('car6');
                runCarCycle('car7');

                isGreenLoopRunning = true;

                greenCarInterval = setInterval(() => {
                    if (!isGreenLoopRunning) return;
                        runCarCycle('car1');
                        runCarCycle('car3');
                        runCarCycle('car6');
                        runCarCycle('car7');
                }, 4500); 
    }
}

function goToRed() {
    if (currentLight === "red") return;

    clearAllTimers();

    if (currentLight === "green") {
        setLight({ red: false, yellow: false, green: true });
        sequenceTimeouts.push(setTimeout(() => {
            setLight({ red: false, yellow: true, green: false });
            sequenceTimeouts.push(setTimeout(() => {
                setLight({ red: true, yellow: false, green: false });
                isGreenLoopRunning = false;
                if (greenCarInterval !== null) {
                    clearInterval(greenCarInterval);
                    greenCarInterval = null;
                }
            }, 1000));
        }, 800));
    } else if (currentLight === "yellow") {
        setLight({ red: true, yellow: false, green: false });

    }
}

function blinkYellow() {
    if (currentLight === "blinking") return;

    clearAllTimers();
    currentLight = "blinking";

    let on = true;
    setLight({ red: false, yellow: true, green: false });

    blinkingInterval = setInterval(() => {
        yellowLight.style.opacity = on ? 1 : 0.3;
        on = !on;
    }, 500);
}

greenLight.addEventListener('click', goToGreen);
redLight.addEventListener('click', goToRed);
yellowLight.addEventListener('click', blinkYellow);

setLight({ red: true, yellow: false, green: false });
