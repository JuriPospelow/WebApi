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
    ws.send("month_keys");
    ws.send("01.24");
  };
  ws.onclose = function(ev) {
    info("[connection closed]");
  };
  ws.onmessage = function(ev) {
    info(ev.data);
    if(JSON.parse(ev.data)[0][0] == "month_keys"){
      getMonthKey(ev.data);
    } else if(JSON.parse(ev.data)[0][0] == "actual"){
      generateTable(ev.data, 'footer_table', "tableAS");
    } else {
      generateTable(ev.data, 'main', "table");
    }
  };
  ws.onerror = function(ev) {
    info("[error]");
    console.log(ev);
  };

// --------------------------- ----------------------------------------------------------
// --------------------------- BUTTONS --------------------------------------------
// --------------------------- ----------------------------------------------------------
var monthKey = [];

 function getMonthKey(json_string){
   for (let cntStringsInArray = 0; cntStringsInArray < JSON.parse(json_string)[1].length; cntStringsInArray++) {
     monthKey[cntStringsInArray] = JSON.parse(json_string)[1][cntStringsInArray];
     console.log("monthKey:" , monthKey[cntStringsInArray]);

    }
 }

  state.onclick = function(){
    ws.send("actual");
    document.getElementById("load").style.display = "block";
  }

  var i = 0;

  right.onclick = function(){
    if(i < monthKey.length - 1){
      textMidle.innerHTML = monthKey[++i];
      ws.send(monthKey[i]);
    }
  }

  left.onclick = function(){
    if(i > 0){
      textMidle.innerHTML = monthKey[--i];
      ws.send(monthKey[i]);
    }
  }

}

// --------------------------- ----------------------------------------------------------
// --------------------------- HELP FUNCTIONS --------------------------------------------
// --------------------------- ----------------------------------------------------------

function generateTable(json_string, table_place, table_ud) {
  console.log("tableActualState");
  let main = document.getElementsByTagName(table_place)[0];
  if(table_ud == "tableAS") document.getElementById("load").style.display = "none";

  if(document.getElementById(table_ud) != undefined){
    const tmp = document.getElementById(table_ud);
    main.removeChild(tmp);
  }

  // console.log("TA1: ", JSON.parse(json_string)[1]);
  // console.log("TA10: ", JSON.parse(json_string)[1][0]);
  // console.log("TA2: ", JSON.parse(json_string)[2]);
  // console.log("TA20: ", JSON.parse(json_string)[2][0]);

    // creates a <table> element and a <tbody> element
    const tbl = document.createElement("table");
    tbl.id = table_ud;
    const tblBody = document.createElement("tbody");

    // creating all cells
    for (let cntArraysInPkg = 1; cntArraysInPkg < JSON.parse(json_string).length; cntArraysInPkg++) {
      // creates a table row
      const row = document.createElement("tr");

      for (let cntStringsInArray = 0; cntStringsInArray < JSON.parse(json_string)[1].length; cntStringsInArray++) {
        const cell = document.createElement("td");
        const cellText = document.createTextNode(JSON.parse(json_string)[cntArraysInPkg][cntStringsInArray]);
        if(cellText.data == "header") cellText.data = table_ud == "tableAS"? "Time" : "Weekday";
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
