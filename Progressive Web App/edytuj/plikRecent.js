function checkextension() {
    var file = document.querySelector("#recentPhotos");
    //console.log(file.files[0]);
    if (/\.(jpe?g|png|gif)$/i.test(file.files[0].name) === false) {

        alert("To nie jest zdjęcie lub niewłaściwy format!");
    } else if (file.files[0].size > 5000000) {
        alert("Zdjęcie musi mieć mniejszy rozmiar niż 5MB.");
        return false;
    } else return true;
}

$('.custom-file-input').on('change', function() {
    if (checkextension() == true) {
        let fileName = $(this).val().split('\\').pop();
        $(this).next('.custom-file-label').addClass("selected").html(fileName);
    } else {
        $(this).next('.custom-file-label').addClass("selected").html('Wybierz plik...');
        document.getElementById("recentPhotos").value = null;
    }
});