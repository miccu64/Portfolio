$('#add-form').submit(function() {
    edycja();
    return false;
});

czyZalogowany();

function czyZalogowany() {
    var token = localStorage.getItem('token');
    if (token == null) {
        alert("Musisz się zalogować, aby edytować zabytek!");
        window.location.replace("/logowanie");
    }
}

function wstecz() {
    window.history.back();
}

function wyslijZdj() {
    let czyIstnieje = document.getElementById('recentPhotos').files.length;
    if (czyIstnieje == 0) {
        return;
    } else {
        let formData = new FormData();
        formData.append('file', document.getElementById('recentPhotos').files[0]);
        var id = localStorage.getItem('id');
        $.ajax({
            type: 'POST',
            url: 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/by_id/' + id + '/photo' + '?archival=false',
            data: formData,
            contentType: false,
            processData: false,
            headers: {
                'Authorization': localStorage.getItem('token')
            },
            success: function(data) {},
            error: function(data) {
                alert('Błąd - nie dodano zdjęcia.');
            }
        });
    }
}

function wyslijZdjArchiw() {
    let czyIstnieje = document.getElementById('archivalPhoto').files.length;
    if (czyIstnieje == 0) {
        return;
    } else {
        let formData = new FormData();
        formData.append('file', document.getElementById('archivalPhoto').files[0]);
        var id = localStorage.getItem('id');
        $.ajax({
            type: 'POST',
            url: 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/by_id/' + id + '/photo' + '?archival=true',
            data: formData,
            contentType: false,
            processData: false,
            headers: {
                'Authorization': localStorage.getItem('token')
            },
            success: function(data) {},
            error: function(data) {
                alert('Błąd - nie dodano zdjęcia archiwalnego.');
            }
        });
    }
}

function edycja() {
    wyslijZdj();
    wyslijZdjArchiw();
    var obj = $('#add-form').serializeJSON();
    console.log(JSON.stringify(obj));
    var id = localStorage.getItem("id");
    var url2 = 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/by_id/' + id;
    $.ajax({
        url: url2,
        dataType: 'json',
        type: "PUT",
        headers: {
            'content-Type': 'application/json',
            'Authorization': localStorage.getItem('token')
        },
        data: JSON.stringify(obj),
        contentType: 'application/json',
        success: function(response) {
            alert('Pomyślnie edytowano zabytek.')
            window.location.replace("/ostatnie");
        },
        error: function(error) {
            alert('Wystąpił błąd w edycji!')
        }
    });
}

pobierz();

function pobierz() {
    var id = localStorage.getItem("id");
    var url2 = 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/by_id/' + id;
    $.ajax({
        url: url2,
        type: "GET",
        success: function(response) {
            wypisz(response);
        },
        error: function(error) {
            console.log("Błąd połączenia!", error);

        }
    });
}

function wypisz(myArr) {
    document.getElementById('street').value = myArr.street;
    document.getElementById('latitude').value = myArr.latitude;
    document.getElementById('longitude').value = myArr.longitude;
    document.getElementById('building').value = myArr.building;
    document.getElementById('name').value = myArr.name;
    document.getElementById('type').value = myArr.type;
    document.getElementById('description').value = myArr.description;

    var lat = myArr.latitude;
    var lng = myArr.longitude;
    if (marker !== null) {
        mymap.removeLayer(marker);
    }
    marker = L.marker([lat, lng]).addTo(mymap);
}