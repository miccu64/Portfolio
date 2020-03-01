function wstecz() {
    window.history.back();
}

function edycja() {
    location.href = "/edytuj/";
}
wyslij();

function wyslij() {
    var url2 = 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/in_area?lat1=50&lat2=50.15&lon1=19.75&lon2=20.15';
    var http = new XMLHttpRequest();
    http.open('GET', url2, true);
    //Send the proper header information along with the request
    http.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    http.onreadystatechange = function() { //Call a function when the state changes.
        if (http.readyState == 4 && http.status == 200) {
            var myArr = this.responseText;
            myArr = JSON.parse(this.responseText);
            localStorage.setItem('tablica', JSON.stringify(myArr.content));
            localStorage.setItem('iloscKart', myArr.totalElements);
            localStorage.setItem('iloscStron', myArr.totalPages);

            wypisz(myArr);

        } else if (http.readyState == 4) {
            alert('Błąd!');
        }
    }
    http.send();
}

function wypisz(myArr) {
    var mymap = L.map('mapid').setView([50.0614, 19.9365], 14);
    for (var a = 0; a < (myArr.length); a++) {

        var lat = myArr[a].latitude;
        var lng = myArr[a].longitude;

        L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
            attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
        }).addTo(mymap);

        var marker = L.marker([lat, lng]).addTo(mymap);
        marker.bindPopup("<p class='myCss'><b>Typ:</b> " + myArr[a].type + "<b><br>ID:</b> " + myArr[a].id);
    }
}