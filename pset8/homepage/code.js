function hide()
{
    let dt = document.querySelector('#dateTime');
    dt.innerHTML = Date();
    if (dt.style.visibility === "hidden") {
        dt.style.visibility = "visible";
        dt.innerHTML = Date();
    }
    else {
        dt.style.visibility = "hidden";
    }
}

function error()
{
    let name = document.querySelector("#name").value;

    if (name === '')
    {
        alert("Well, it would be better if I know your name.\nDon't worry, your name is not stored in any database!");
    }
    else
    {
         document.querySelector("#text").innerHTML = "Hello, " + name + "! Nice to meet you!";
    }

}

document.querySelectorAll(".flip-container").forEach(item => {
    item.addEventListener("click", function() {
        item.classList.toggle("flipped");
    });
});

let body = document.querySelector("body");
document.querySelector("#red").onclick = function() {
    body.style.backgroundColor = "rgba(255,0,0,0.5)";
    //body.style.color = "white";
};

document.querySelector("#green").onclick = function() {
    body.style.backgroundColor = "rgba(0,255,0,0.5)";
    //body.style.color = "black";
};

document.querySelector("#blue").onclick = function() {
    body.style.backgroundColor = "rgba(0,0,255,0.5)";
    //body.style.color = "white";
};

document.querySelector("#default").onclick = function() {
    body.style.backgroundColor = "LightCyan";
    //body.style.color = "black";
};

// document.querySelector("select").onchange = function() {
//     document.querySelector(".p2").style.fontSize = this.value;
// };