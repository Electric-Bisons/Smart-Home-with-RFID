let uid = document.getElementById("numar");
let valuesGlobal = loadFile("/test.txt").split("\n");


function loadFile(filePath) {
    var result = null;
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", filePath, false);
    xmlhttp.send();
    if (xmlhttp.status == 200) {
        result = xmlhttp.responseText;
    }
    return result;
}

function updateData() {
    let valuesLocal;
    valuesLocal = loadFile("/test.txt").split("\n");
    if (valuesGlobal != valuesLocal) {


        uid.value = valuesLocal[valuesLocal.length - 2]
        let history = document.getElementById("history");
        history.innerHTML = '';
        for (let i = valuesLocal.length - 2; i >= 0; i--) {
            if (valuesLocal[i] != "\r") {
                let entry = document.createElement('li');
                entry.innerHTML = valuesLocal[i];
                history.appendChild(entry);
            }
        }
    }
    valuesGlobal = valuesLocal;
}

setInterval(updateData, 0);