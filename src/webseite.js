function info(msg, ...param)
{
  console.info(msg, ...param);
}

if (window.Worker) {
// --------------------------- ----------------------------------------------------------
// --------------------------- COMMUNICATION --------------------------------------------
// --------------------------- ----------------------------------------------------------
  const ws = new WebSocket("ws://localhost:8080");
  ws.onopen = function(ev) {
    info("[connection opened]");
    // ws.send("header");
    ws.send("01.24");
  };
  ws.onclose = function(ev) {
    info("[connection closed]");
  };
  ws.onmessage = function(ev) {
    info(ev.data);
    if(JSON.parse(ev.data)[0][0] == "actual"){
      tableActualState(ev.data);
    } else {
      generateTable(ev.data);
    }
  };
  ws.onerror = function(ev) {
    info("[error]");
    console.log(ev);
  };

// --------------------------- ----------------------------------------------------------
// --------------------------- BUTTONS --------------------------------------------
// --------------------------- ----------------------------------------------------------

  state.onclick = function(){
    ws.send("actual");
    document.getElementById("load").style.display = "block";
  }

  var i = 0;
  var monthKey = ["January 2024","February 2024", "March 2024", "April 2024"];
  var monthValue = ["01.24","02.24", "03.24", "04.24"];

  right.onclick = function(){
    if(i < monthKey.length - 1){
      textMidle.innerHTML = monthKey[++i];
      ws.send(monthValue[i]);
    }
  }

  left.onclick = function(){
    if(i > 0){
      textMidle.innerHTML = monthKey[--i];
      ws.send(monthValue[i]);
    }
  }

}

// --------------------------- ----------------------------------------------------------
// --------------------------- HELP FUNCTIONS --------------------------------------------
// --------------------------- ----------------------------------------------------------

function tableActualState(json_string) {
  console.log("tableActualState");
  let main = document.getElementsByTagName('footer_table')[0];
  document.getElementById("load").style.display = "none";

  if(document.getElementById("tableAS") != undefined){
    const tmp = document.getElementById("tableAS");
    main.removeChild(tmp);
  }

  // console.log("TA1: ", JSON.parse(json_string)[1]);
  // console.log("TA10: ", JSON.parse(json_string)[1][0]);
  // console.log("TA2: ", JSON.parse(json_string)[2]);
  // console.log("TA20: ", JSON.parse(json_string)[2][0]);

    // creates a <table> element and a <tbody> element
    const tbl = document.createElement("table");
    tbl.id = "tableAS";
    const tblBody = document.createElement("tbody");

    // creating all cells
    for (let cntArraysInPkg = 1; cntArraysInPkg < JSON.parse(json_string).length; cntArraysInPkg++) {
      // creates a table row
      const row = document.createElement("tr");

      for (let cntStringsInArray = 0; cntStringsInArray < JSON.parse(json_string)[1].length; cntStringsInArray++) {
        const cell = document.createElement("td");
        const cellText = document.createTextNode(JSON.parse(json_string)[cntArraysInPkg][cntStringsInArray]);
        if(cellText.data == "header") cellText.data = "Time";
        if(cellText.data == "Datum") cellText.data = "Date";
        cell.appendChild(cellText);
        row.appendChild(cell);
      }
      // add the row to the end of the table body
      tblBody.appendChild(row);
    }

  tbl.appendChild(tblBody);

  main.appendChild(tbl);

  tbl.setAttribute("border", "1");
  tbl.setAttribute("width", "80%");
  tbl.setAttribute("padding", "2%");
  tbl.setAttribute("align", "center");

}

function generateTable(json_string) {

  let main = document.getElementsByTagName('main')[0];
  if(document.getElementById("table") != undefined){
    const tmp = document.getElementById("table");
    main.removeChild(tmp);
  }

    // creates a <table> element and a <tbody> element
    const tbl = document.createElement("table");
    tbl.id = "table";
    const tblBody = document.createElement("tbody");

    // creating all cells
    for (let i = 0; i < JSON.parse(json_string).length; i++) {
      // creates a table row
      const row = document.createElement("tr");
      console.log("l: ", JSON.parse(json_string).length);

      for (let j = 0; j < JSON.parse(json_string)[0].length; j++) {
        // Create a <td> element and a text node, make the text
        // node the contents of the <td>, and put the <td> at
        // the end of the table row
        const cell = document.createElement("td");
        const cellText = document.createTextNode(JSON.parse(json_string)[i][j]);
        if(cellText.data == "header") cellText.data = "Weekday";
        if(cellText.data == "Datum") cellText.data = "Date";
        cell.appendChild(cellText);
        row.appendChild(cell);
      }
      // add the row to the end of the table body
      tblBody.appendChild(row);
    }

  tbl.appendChild(tblBody);

  main.appendChild(tbl);

  tbl.setAttribute("border", "1");
  tbl.setAttribute("width", "80%");
  tbl.setAttribute("padding", "2%");
  tbl.setAttribute("align", "center");

}
