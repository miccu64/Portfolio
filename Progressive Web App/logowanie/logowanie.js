$('#add-form').submit(function() {
    klik();
    return false;
});

function klik() {
    var poczta = document.getElementById("poczta").value;
    var haslo = document.getElementById("haslo").value;
    var url2 = 'https://polar-chamber-44010.herokuapp.com/api/v1/user/login';

    var http = new XMLHttpRequest();
    http.open('POST', url2, true);
    //Send the proper header information along with the request
    http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    http.onreadystatechange = function() { //Call a function when the state changes.
        if (http.readyState == 4 && http.status == 200) {
            localStorage.setItem('token', http.responseText);
            alert('Zalogowano pomyślnie!');
            window.location.replace("/");
        } else if (http.readyState == 4) {
            alert('Złe dane logowania!');
        }
    }
    http.send('email=' + poczta + '&password=' + haslo);
}