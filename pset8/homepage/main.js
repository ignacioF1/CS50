var d = new Date();
var months = ["Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Augosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"];
document.getElementById("data").innerHTML = d.getDate() + ' de ' + months[d.getMonth()] + ' de ' + d.getFullYear();
