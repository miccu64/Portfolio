function wstecz() {
    window.history.back();
}

function edycja() {
    location.href = "/edytuj/";
}
wyslij();

function wyslij() {
    var id = localStorage.getItem("id");
    var url2 = 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/by_id/' + id;
    $.ajax({
        url: url2,
        type: "GET",
        success: function(response) {
            wypisz(response);
        },
        error: function(error) {
            alert("Błąd sieci!", error);
        }
    });
}

function wypisz(myArr) {

    document.getElementById('street').value = myArr.street;
    document.getElementById('wspx').value = myArr.latitude;
    document.getElementById('wspy').value = myArr.longitude;
    document.getElementById('numer').value = myArr.building;
    document.getElementById('nazwa').value = myArr.name;
    document.getElementById('typ').value = myArr.type;
    document.getElementById('poleTekstowe').value = myArr.description;

    var lat = document.getElementById("wspx").value;
    var lng = document.getElementById("wspy").value;

    var mymap = L.map('mapid').setView([lat, lng], 18);

    L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
    }).addTo(mymap);

    var marker = L.marker([lat, lng]).addTo(mymap);
    marker.bindPopup(document.getElementById("nazwa").value).openPopup();

    //ukrywanie zdjec
    var doZdj = 'https://polar-chamber-44010.herokuapp.com/assets/';
    var zdjArch = myArr.archivalPhoto;
    if (zdjArch == null) {
        document.getElementById('archNapis').style.display = 'none';
    } else {
        zdjArch = myArr.archivalPhoto.imageName;
        zdjArch = doZdj + zdjArch;
        document.getElementById('arch11').src = zdjArch;
        document.getElementById('arch1').href = zdjArch;
    }
    var zdj0 = myArr.recentPhotos[0];
    var zdj1 = myArr.recentPhotos[1];
    var zdj2 = myArr.recentPhotos[2];

    if (zdj0 == undefined) {
        document.getElementById('recent00').style.display = 'none';
        document.getElementById('recent0').style.display = 'none';
    } else {
        zdj0 = myArr.recentPhotos[0].imageName;
        zdj0 = doZdj + zdj0;
        document.getElementById('recent00').src = zdj0;
        document.getElementById('recent0').href = zdj0;
    }

    if (zdj1 == undefined) {
        document.getElementById('recent1').style.display = 'none';
        document.getElementById('recent11').style.display = 'none';
    } else {
        zdj1 = myArr.recentPhotos[1].imageName;
        zdj1 = doZdj + zdj1;
        document.getElementById('recent11').src = zdj1;
        document.getElementById('recent1').href = zdj1;
    }

    if (zdj2 == undefined) {
        document.getElementById('recent2').style.display = 'none';
        document.getElementById('recent22').style.display = 'none';
    } else {
        zdj2 = myArr.recentPhotos[2].imageName;
        zdj2 = doZdj + zdj2;
        document.getElementById('recent22').src = zdj2;
        document.getElementById('recent2').href = zdj2;
    }

    if ((zdj0 == undefined) & (zdj1 == undefined) & (zdj2 == undefined)) {
        document.getElementById('recentNapis').style.display = 'none';
        if (myArr.archivalPhoto == null) {
            document.getElementById('opis').style.display = 'none';
        }
    }

    document.getElementById('dataDod').innerHTML = myArr.createdAt.slice(0, 10);
    document.getElementById('dodal').innerHTML = myArr.createdBy.name;
    var edytowane = myArr.editedAt;
    if (edytowane != null) {
        document.getElementById('dataEd').innerHTML = edytowane.slice(0, 10);
        document.getElementById('edytowal').innerHTML = myArr.editedBy.name;
    }
}