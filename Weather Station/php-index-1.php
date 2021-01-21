<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "id15536666_webserver0800";
// REPLACE with Database user
$username = "id15536666_pk0080server_";
// REPLACE with Database user password
$password = "(P@!K0080ronil??}}+";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, value1, value2, value3, value4, value5, value6, reading_time FROM Sensor order by reading_time desc limit 40";

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()){
    $sensor_data[] = $data;
}

$readings_time = array_column($sensor_data, 'reading_time');

// ******* Uncomment to convert readings time array to your timezone ********
$i = 0;
foreach ($readings_time as $reading){
    // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
    //$readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading - 1 hours"));
    // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
    $readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading + 5 hours 30 minute"));
    $i += 1;
}

$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$value2 = json_encode(array_reverse(array_column($sensor_data, 'value2')), JSON_NUMERIC_CHECK);
$value3 = json_encode(array_reverse(array_column($sensor_data, 'value3')), JSON_NUMERIC_CHECK);
$value4 = json_encode(array_reverse(array_column($sensor_data, 'value4')), JSON_NUMERIC_CHECK);
$value5 = json_encode(array_reverse(array_column($sensor_data, 'value5')), JSON_NUMERIC_CHECK);
$value6 = json_encode(array_reverse(array_column($sensor_data, 'value6')), JSON_NUMERIC_CHECK);
$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

$result->free();
$conn->close();
?>

<!DOCTYPE html>
<html>
<meta name="viewport" content="width=device-width, initial-scale=1">
<!--Visit highcharts for more features applicable on graphs-->
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <style>
    body {
      min-width: 310px;
    	max-width: 1280px;
    	height: 500px;
      margin: 0 auto;
    }
    h2 {
      font-family: Arial;
      font-size: 2.5rem;
      text-align: center;
    }
  </style>
  <body>
    <h2>Weather Station</h2>
    <div id="chart-temperature" class="container"></div>
	<div id="chart-pressure" class="container"></div>
    <div id="chart-altitude" class="container"></div>
    <div id="chart-humidity" class="container"></div>
	<div id="chart-luminosity" class="container"></div>
	<div id="chart-airquality" class="container"></div>
<script>

var value1 = <?php echo $value1; ?>;
var value2 = <?php echo $value2; ?>;
var value3 = <?php echo $value3; ?>;
var value4 = <?php echo $value4; ?>;
var value5 = <?php echo $value5; ?>;
var value6 = <?php echo $value6; ?>;
var reading_time = <?php echo $reading_time; ?>;

var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature' },
  tooltip: {							//show axis parallel lines...
        crosshairs: [true,true]
  },
  title: { text: 'Temperature' },
  series: [{
    showInLegend: false,
	data: value1,
    name: 'Temp.'
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#059e8a', lineWidth: 1.7 }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Temperature (Celsius)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});

var chartP = new Highcharts.Chart({
  chart:{ renderTo:'chart-pressure' },
  tooltip: {							//show axis parallel lines...
        crosshairs: [true,true]
  },
  title: { text: 'Pressure' },
  series: [{
    showInLegend: false,
    data: value2,
	name: 'Pre.'
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#18009c', lineWidth: 1.7 }
  },
  xAxis: {
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Pressure (hPa)' }
  },
  credits: { enabled: false }
});

var chartA = new Highcharts.Chart({
  chart:{ renderTo : 'chart-altitude' },
  tooltip: {							//show axis parallel lines...
        crosshairs: [true,true]
  },
  title: { text: 'Altitude' },
  series: [{
    showInLegend: false,
	data: value3,
    name: 'Alt.'
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#74caff',lineWidth: 1.7 }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Altitude (m)' }
  },
  credits: { enabled: false }
});

var chartA = new Highcharts.Chart({
  chart:{ renderTo : 'chart-humidity' },
  tooltip: {							//show axis parallel lines...
        crosshairs: [true,true]
  },
  title: { text: 'Humidity' },
  series: [{
    showInLegend: false,
	data: value4,
    name: 'Humi.'
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#fe8f24',lineWidth: 1.7 }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Humidity (%)' }
  },
  credits: { enabled: false }
});

var chartQ = new Highcharts.Chart({
  chart:{ renderTo : 'chart-airquality' },
  tooltip: {							//show axis parallel lines...
        crosshairs: [true,true]
  },
  title: { text: 'Air Quality' },
  series: [{
    showInLegend: false,
	data: value6,
    name: 'AQI'
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#000000', lineWidth: 1.7 }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Air Quality (PPM)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});

var chartA = new Highcharts.Chart({
  chart:{ renderTo : 'chart-luminosity' },
  tooltip: {							//show axis parallel lines...
        crosshairs: [true,true]
  },
  title: { text: 'Luminosity' },
  series: [{
    showInLegend: false,
	data: value5,
    name: 'Lumi.'
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#33FFFF', lineWidth: 1.7 }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Luminosity (%)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});

function refresh(refreshPeriod) 
{
	setTimeout('location.reload(true)', refreshPeriod);
} 
window.onload = refresh(7000);

</script>
</body>
</html>