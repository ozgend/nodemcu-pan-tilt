const ws = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
let axisX, axisY, valueX, valueY;

ws.onopen = function () {
    console.log('ws connect');
    ws.send('ws connect ' + new Date());
};

ws.onerror = function (error) {
    console.log('ws error ', error);
};

ws.onmessage = function (e) {
    console.log('ws message: ', e.data);
};

ws.onclose = function () {
    console.log('ws close');
};

const set = () => {
    const payload = `s${axisX.value},${axisY.value}`;
    console.log(payload);

    valueX.innerText = axisX.value;
    valueY.innerText = axisY.value;

    ws.send(payload);
};

const reset = () => {
    axisX.value = 90;
    axisY.value = 90;
    set();
};

const handleKey = (code) => {
    if (code === 'KeyR') {
        reset();
    }
    else if (code === 'ArrowUp') {
        ++axisY.value;
        set();
    }
    else if (code === 'ArrowDown') {
        --axisY.value;
        set();
    }
    else if (code === 'ArrowLeft') {
        --axisX.value;
        set();
    }
    else if (code === 'ArrowRight') {
        ++axisX.value;
        set();
    }
};

const init = () => {
    document.addEventListener('keydown', (e) => {
        handleKey(e.code);
    });

    axisX = document.getElementById('axis-x');
    axisY = document.getElementById('axis-y');
    valueX = document.getElementById('value-x');
    valueY = document.getElementById('value-y');

    reset();

    console.log('init done');
};

window.onload = () => init();