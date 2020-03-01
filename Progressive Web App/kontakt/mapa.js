var mymap = L.map('mapid').setView([50.0557, 19.9371], 18);

L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
}).addTo(mymap);
var marker = L.marker([50.0557, 19.9371]).addTo(mymap);
marker.bindPopup("<b>WUOZ Kraków</b>").openPopup();