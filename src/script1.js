let header;
if (window.Worker) {
    var ws = null;
    function showMessage(msg) {
    //   messages.innerText += msg + "\n";
    //   messages.scrollTop = messages.scrollHeight - messages.clientHeight;
    header = msg;

        console.info(msg);
    };

    ws = new WebSocket("ws://localhost:8080");
    ws.onopen = function(ev) {
      showMessage("[connection opened]");
      // ws.send("header");
      ws.send("January");
    };
    ws.onclose = function(ev) {
      showMessage("[connection closed]");
    };
    ws.onmessage = function(ev) {
      showMessage(ev.data);
      generateTable(ev.data);
    };
    ws.onerror = function(ev) {
      showMessage("[error]");
      console.log(ev);
    };


    // send.onclick = function(){
    //   ws.send("header");
    // }
}

var i = 0;
var month = ["January 2024","February 2024", "March 2024", "April 2024"];
right.onclick = function(){
  if(i < month.length - 1)textMidle.innerHTML = month[++i];
}

left.onclick = function(){
  if(i > 0)textMidle.innerHTML = month[--i];
}

function generateTable(json_string) {
    // creates a <table> element and a <tbody> element
    const tbl = document.createElement("table");
    const tblBody = document.createElement("tbody");

    console.log(JSON.parse(json_string)[0]);

  //   if(JSON.parse(json_string)[0] == "header"){
  //     // creates a table row
  //     const row = document.createElement("tr");
  //     console.log("l: ", JSON.parse(json_string).length);

  //     for (let j = 1; j < JSON.parse(json_string).length; j++) {
  //       const cell = document.createElement("td");
  //       const cellText = document.createTextNode(JSON.parse(json_string)[j]);
  //       cell.appendChild(cellText);
  //       row.appendChild(cell);
  //     }

  //     // add the row to the end of the table body
  //     tblBody.appendChild(row);

  // } else {
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
  // }
  // put the <tbody> in the <table>
  tbl.appendChild(tblBody);


  const main = document.getElementsByTagName('main')[0];
  main.appendChild(tbl);

  // tbl.classList.add("centered");

  tbl.setAttribute("border", "1");
  tbl.setAttribute("width", "80%");
  tbl.setAttribute("padding", "2%");
  // tbl.setAttribute("margin-inline", "auto");
  tbl.setAttribute("align", "center");

}
  // generateTable();
//  class ParamTable
// {

//   constructor(parent){
//     //- let data = JSON.parse(info).data;
//     this.tbl = document.createElement("table");
//     this.tblHead = document.createElement("thead");
//     this.tblBody = document.createElement("tbody");
//     // put the <tbody> in the <table>
//     this.tbl.appendChild(this.tblHead);
//     this.tbl.appendChild(this.tblBody);
//     parent.appendChild(this.tbl);
//   }

//   tableHead(){  // TABLE HEAD
//     // creates a table row

//     const row = document.createElement("tr");
//     row.id = "headParam";

//     for (let i_laser = 0; i_laser < 1; i_laser++) {

//       const cell = document.createElement("td");
//       let cellText;
//       if(i_laser==0) {
//         cellText = document.createTextNode("PARAMETERS");
//       } else {
//         // cellText = document.createTextNode(this.data_laser[i_laser-1].name.toUpperCase());
//       }
//       cell.appendChild(cellText);
//       row.appendChild(cell);
//     }

//     // add the row to the end of the table body
//     this.tblHead.appendChild(row);
//   }

// }

// const main = document.getElementsByTagName('main')[0];
// this.paramConfigT = document.createElement("div");
// this.paramConfigT.id = "ParamConfig";
// this.paramConfigT.classList.add("tab-wrap");
// main.appendChild(this.paramConfigT);

// tabelle = new ParamTable(main);
