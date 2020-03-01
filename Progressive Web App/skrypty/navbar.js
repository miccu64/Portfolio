function wyloguj() {
    localStorage.removeItem('token');
    alert('Wylogowano pomyślnie.');
}
if (localStorage.getItem('token') == null) {
    var link = document.getElementById('niezalogowany');
    link.style.display = 'none';
} else {
    var link2 = document.getElementById('zalogowany2');
    link2.style.display = 'none';
    var link3 = document.getElementById('zalogowany3');
    link3.style.display = 'none';
}