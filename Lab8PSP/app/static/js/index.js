document.addEventListener('DOMContentLoaded', () => {
    get_ships();
  });

function get_ships(){
    // Инициализировать новый запрос
          var request = new XMLHttpRequest();
          request.open('GET', '/get_ships');

          // Функция обратного вызова, когда запрос завершен
          request.onload = () => {
              // Извлечение данных JSON из запроса
              const data = JSON.parse(request.responseText);
              fillShipsTable(data);
          }

          // Послать запрос
          request.send(null);
}

function fillShipsTable(data){
    var table = document.getElementById("table");
    var text = "<tr><td>Id</td><td>Name</td><td>Type</td><td>Displacement</td><td>Country</td><td>Update</td><td>Delete</td></tr>";
    for(let i = 0; i < data.length; i++){
        let id = data[i].ship.id;
        let name = data[i].ship.name;
        let type = data[i].ship.type;
        let displacement = data[i].ship.displacement;
        let country = data[i].ship.country;
        text += "<tr>" +
                    `<td>${id}</td>` +
                    "<td>" +
                        `<input type=\"text\" class=\"\input_text\" id=\"name${id}_input\" value=\"${name}\">` +
                    "</td>" +
                    "<td>" +
                        `<input type=\"text\" class=\"\input_text\" id=\"type${id}_input\" value=\"${type}\">` +
                    "</td>" +
                    "<td>" +
                        `<input type=\"text\" class=\"\input_text\" id=\"displacement${id}_input\" value=\"${displacement}\">` +
                    "</td>" +
                    "<td>" +
                        `<input type=\"text\" class=\"\input_text\" id=\"country${id}_input\" value=\"${country}\">` +
                    "</td>" +
                    "<td>" +
                        `<a class="green" onclick=\"update_ship(${id})\">Update</a>` +
                    "</td><td>" + 
                        `<a class="green" onclick=\"remove_ship(${id})\">Remove</a>` +
                    "</td></tr>";
    }
    text += "<tr><td></td><td><input type=\"text\" class=\"\input_text\" id=\"name_input\">" +
            "</td><td><input type=\"text\" class=\"\input_text\" id=\"type_input\">" +
            "</td><td><input type=\"text\" class=\"\input_text\" id=\"displacement_input\">" +
            "</td><td><input type=\"text\" class=\"\input_text\" id=\"country_input\">" +
            "</td><td><a class=\"green\" onclick=\"create_ship()\">Add</a>" +
            "</td><td></td></tr>"

    table.innerHTML = text;
}

function remove_ship(id){
    var request = new XMLHttpRequest();
    request.open("POST", '/delete_ship?id=' + id);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
          response = JSON.parse(request.responseText);
          if(request.status == '200'){
            get_books();
            WriteMessage(response.success, true);
          }
          else{
            WriteMessage(response.error, false);
          }
      }

    request.send();
}

function create_ship(){
    var request = new XMLHttpRequest();
    var name = document.getElementById("name_input").value;
    var type = document.getElementById("type_input").value;
    var displacement = document.getElementById("displacement_input").value;
    var country = document.getElementById("country_input").value;
    request.open("POST", `/create_ship?name=${name}&type=${type}&displacement=${displacement}&country=${country}`);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
          response = JSON.parse(request.responseText);
          if(request.status == '200'){
            get_ships();
            WriteMessage(response.success, true);
          }
          else{
            WriteMessage(response.error, false);
          }
      }

    request.send();
}

function update_ship(id){
    var request = new XMLHttpRequest();
    var name = document.getElementById(`name${id}_input`).value;
    var type = document.getElementById(`type${id}_input`).value;
    var displacement = document.getElementById(`displacement${id}_input`).value;
    var country = document.getElementById(`country${id}_input`).value;
    request.open("POST", `/update_ship?id=${id}&name=${name}&type=${type}&displacement=${displacement}&country=${country}`);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
          response = JSON.parse(request.responseText);
          if(request.status == '200'){
            get_ships();
            WriteMessage(response.success, true);
          }
          else{
            WriteMessage(response.error, false);
          }
      }

    request.send();
}

function WriteMessage(message, success){
    var now = new Date();
    status_text = document.getElementById("answertext");
    status_text.style.color = success ? "green" : "red";
    Year = now.getFullYear();
    Month = now.getMonth();
    Day = now.getDate();
    Hour = now.getHours();
    Minutes = now.getMinutes();
    Seconds = now.getSeconds();
    status_text.innerHTML = `${Year}.${Month}.${Day} ${Hour}:${Minutes}:${Seconds} - ${message}`;
}