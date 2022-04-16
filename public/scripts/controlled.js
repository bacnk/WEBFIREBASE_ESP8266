var led_one = document.getElementById('onBtn');
var led_off = document.getElementById('offBtn');

 function onFunction() {
    State.set('ON')
    console.log("0n")
    OUT_TEXT.innerText = 'Chế độ tự động';
}

  
function offFunction() {
    State.set('OFF')
    OUT_TEXT.innerText = 'Chế độ bằng tay';
    console.log("0ff")
}
