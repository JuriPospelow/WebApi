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
    };
    ws.onclose = function(ev) {
      showMessage("[connection closed]");
    };
    ws.onmessage = function(ev) {
      showMessage(ev.data);
      generateTable(ev.data);
      // generateTable('["servix", "xeonix", "gigantix", "duomensix3", "servix2", "commandix", "asterix", "tc2", "aoi2"]');
    };
    ws.onerror = function(ev) {
      showMessage("[error]");
      console.log(ev);
    };
    // ws.send("Hello");


    send.onclick = function(){
      ws.send("header");
    }
}


// let json_string = '["servix", "xeonix"]';

function generateTable(json_string) {
    // creates a <table> element and a <tbody> element
    const tbl = document.createElement("table");
    const tblBody = document.createElement("tbody");

    // creating all cells
    for (let i = 0; i < 2; i++) {
      // creates a table row
      const row = document.createElement("tr");

      for (let j = 0; j < 2; j++) {
        // Create a <td> element and a text node, make the text
        // node the contents of the <td>, and put the <td> at
        // the end of the table row
        const cell = document.createElement("td");
        const cellText = document.createTextNode(JSON.parse(json_string)[j]);
        // const cellText = document.createTextNode(`cell in row ${i}, column ${j}`);
        cell.appendChild(cellText);
        row.appendChild(cell);
      }

      // add the row to the end of the table body
      tblBody.appendChild(row);
    }

    // put the <tbody> in the <table>
    tbl.appendChild(tblBody);
    // appends <table> into <body>
    document.body.appendChild(tbl);
    // sets the border attribute of tbl to '2'
    tbl.setAttribute("border", "1");
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
