var led_one = document.getElementById('onBtn');
var led_off = document.getElementById('offBtn');
var firebaseRef = firebase.database().ref().child("LedStatus");
 function onFunction() {
    firebaseRef.set('ON')
    console.log("0n")
    OUT_TEXT.innerText = 'Chế độ tự động';
}

  
function offFunction() {
    firebaseRef.set('OFF')
    OUT_TEXT.innerText = 'Chế độ bằng tay';
    console.log("0ff")
}
