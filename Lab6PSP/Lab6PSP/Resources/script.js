function SendPass(){
	let pass = document.getElementById('passtext').value;
	let correct = document.getElementById('correct');
	var request = new XMLHttpRequest();
    request.open("POST", '/' + pass);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
		  correct.innerHTML = request.responseText;
      }

    request.send();
}