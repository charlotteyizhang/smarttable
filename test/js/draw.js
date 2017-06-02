$(function() {
    console.log( "ready!" );
    // var filename = ["data/03061128-testwithtoto.json","data/03071232-testwithmom.json", "data/03131701-testwithtcoursemate.json"];

    getJson(0);
    //add listener
$('input[type=radio][name=file]').change(function() {
  d3.select("svg").remove();
    getJson(this.value);
  });

function getJson(indexNum){

  var filename = ["data/03061128-testwithtoto.json","data/03071232-testwithmom.json", "data/03131701-testwithtcoursemate.json", "data/03271530-testTable.json",
  "data/03301655-testwithP.json","data/03301924-testwithA.json"];

//get json data
  $.getJSON( filename[indexNum], function( data ) {

    //create two arrays to get each person's voice data
    var items1 = [];
    var items2 = [];
    //push the voice data to arrays
    $.each( data, function( key, val ) {
      // if(val.data1 > 40){
      //   val.data1 = 40;
      // }
      // (val.data1 > 40) ? (val.data1 = 40) : (val.data1 < 2.5 ? (val.data1 = 0.5) : null);
    (val.data1 > 40) ? (val.data1 = 40) : null;

    (val.data2 > 40) ? (val.data2 = 40) : null;

      // (val.data2 > 40) ? (val.data2=40) : (val.data2 < 2.5 ? (val.data2 = 0.5) : null);
     items1.push(val.data1);
     items2.push(val.data2);
    });

    var obj = {};
    obj.items1 = items1;
    obj.items2 = items2;


    draw(data,obj);

  });
}

function draw(data,obj) {
  //initiate the canvas size

var margin = {top: 10, right: 20, bottom: 80, left: 60},
    width = 960 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

//initiate the canvas svg
var svg = d3.select("#canvas").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");


//x-axis is from 0->max
var x = d3.scale.linear()
    .range([0, width]);
//y-axis is from max->0
var y = d3.scale.linear()
    .range([height, 0]);

//initiate Json data
// var data = getJson(0);

var yScale = Math.max(d3.max(obj.items1),d3.max(obj.items2)) ;
var xScale = obj.items1.length;
  // Set the scale domain.
 x.domain([0, xScale]);
 y.domain([0, yScale]);


  svg.selectAll(".bin1")
  .data(data)
  .attr("class", "bin1")
  .enter()
      .append("rect")
      .attr("transform", "translate(0, "+height+") rotate(-90)")
      .attr("width", function(d){return (d.data1*height/yScale );})
      .attr("height", 1)
      .attr("y", function (d, i) {return Math.round(i/xScale*width) ;})
      .attr("fill", "blue");

  svg.selectAll(".bin2")
  .data(data)
  .attr("class", "bin2")
  .enter()
      .append("rect")
      .attr("transform", "translate(0, "+height+") rotate(-90)")
      .attr("width", function(d){return (d.data2*height/yScale );})
      .attr("height", 1)
      .attr("y", function (d, i) {return Math.round(i/xScale*width) ;})
      .attr("fill", "red");

svg.append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 0 - margin.left)
        .attr("x",0 - (height / 2))
        .attr("dy", "1em")
        .style("text-anchor", "middle")
        .text("Amplitude");
//add
  svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(d3.svg.axis()
      .scale(x)
      .orient("bottom"));


svg.append("text")
        .attr("x", width / 2)
        .attr("y", height + margin.bottom/2)
        .attr("dy", "1em")
        .style("text-anchor", "middle")
        .text("Time (s)");
//add y
  svg.append("g")
      .attr("class", "y axis")
      .call(d3.svg.axis()
      .scale(y)
      .orient("left"));

}

});

/**
function processScale(data){
    
    d3.json(fileToOpen, function(error, bins) {
      if (error) throw error;

      var yScale = d3.max(bins.map(function(d) { return Math.max(d.data1,d.data2); })) ;
      var xScale = d3.max(bins.map(function(d,i){return i;}));
      // Set the scale domain.
      x.domain([0, d3.max(bins.map(function(d,i){return i;}))]);
      y.domain([0, d3.max(bins.map(function(d) { return Math.max(d.data1,d.data2); }))]);

  });
}


function draw(data){
  svg.selectAll(".bin1")
      .data(bins)
      .attr("class", "bin1")
      .enter()
          .append("rect")
          .attr("transform", "translate(0, "+height+") rotate(-90)")
          .attr("width", function (d) {return (d.data1*height/yScale - 107);})
          .attr("height", 2)
          .attr("y", function (d, i) {return i*2 ;})
          .attr("fill", "blue");

  svg.selectAll(".bin2")
        .data(bins)
        .attr("class", "bin2")
        .enter()
            .append("rect")
            .attr("transform", "translate(0, "+height+") rotate(-90)")
            .attr("width", function (d) {return (d.data2*height/yScale - 107);})
            .attr("height", 2)
            .attr("y", function (d, i) {return i*2 ;})
            .attr("fill", "red");
//add
  svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(d3.svg.axis()
      .scale(x)
      .orient("bottom"));
//add y
  svg.append("g")
      .attr("class", "y axis")
      .call(d3.svg.axis()
      .scale(y)
      .orient("left"));
}

*/


/////////////
// });
