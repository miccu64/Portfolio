var mymap = L.map('mapid').setView([50.0614, 19.9365], 13);

L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
}).addTo(mymap);

var marker = null;
mymap.on('click', function(e) {
    var coord = e.latlng;
    var lat = coord.lat;
    var lng = coord.lng;
    if (marker !== null) {
        mymap.removeLayer(marker);
    }
    marker = L.marker([lat, lng]).addTo(mymap);
    document.getElementById("latitude").value = Math.round((lat + Number.EPSILON) * 10000) / 10000;
    document.getElementById("longitude").value = Math.round((lng + Number.EPSILON) * 10000) / 10000;

    function simpleReverseGeocoding(lon, lat) {
        fetch('http://nominatim.openstreetmap.org/reverse?format=json&lon=' + lng + '&lat=' + lat).then(function(response) {
            return response.json();
        }).then(function(json) {
            //console.log(json.display_name);
            if ((json.address.road) != undefined) {
                document.getElementById("street").value = json.address.road;
            } else document.getElementById("street").value = null;
            if ((json.address.house_number) != undefined) {
                document.getElementById("building").value = json.address.house_number;
            } else document.getElementById("building").value = null;
        })
    }
    simpleReverseGeocoding(lng, lat);
});