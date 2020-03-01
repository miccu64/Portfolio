$('#add-form').submit(function() {
    edycja();
    return false;
});

czyZalogowany();

function czyZalogowany() {
    var token = localStorage.getItem('token');
    if (token == null) {
        alert("Musisz się zalogować, aby dodać zabytek!");
        window.location.replace("/logowanie");
    }
}

function wstecz() {
    window.history.back();
}

function edycja() {
    var obj = $('#add-form').serializeJSON();
    var url2 = 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/';
    $.ajax({
        url: url2,
        dataType: 'json',
        type: "POST",
        headers: {
            'content-Type': 'application/json',
            'Authorization': localStorage.getItem('token')
        },
        data: JSON.stringify(obj),
        contentType: 'application/json',
        success: function(response) {
            localStorage.setItem('idDodane', response.id);

            let czyIstnieje = document.getElementById('archivalPhoto').files.length;
            if (czyIstnieje == 0) {
                return;
            } else {
                let formData = new FormData();
                formData.append('file', document.getElementById('archivalPhoto').files[0]);
                var id = localStorage.getItem('idDodane');
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

            let czyIstnieje2 = document.getElementById('recentPhotos').files.length;
            if (czyIstnieje2 == 0) {
                return;
            } else {
                let formData2 = new FormData();
                formData2.append('file', document.getElementById('recentPhotos').files[0]);
                var id = localStorage.getItem('idDodane');
                $.ajax({
                    type: 'POST',
                    url: 'https://polar-chamber-44010.herokuapp.com/api/v1/artifacts/by_id/' + id + '/photo' + '?archival=false',
                    data: formData2,
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

            alert('Pomyślnie dodano zabytek.');
            window.location.replace("/ostatnie");
        },
        error: function(error) {
            alert('Wystąpił błąd!')
        },
    });
}