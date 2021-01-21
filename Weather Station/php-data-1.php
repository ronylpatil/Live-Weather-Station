<?php
$servername = "localhost";

// REPLACE with your Database name
$dbname = "id15536666_webserver0800";
// REPLACE with Database user
$username = "id15536666_pk0080server_";
// REPLACE with Database user password
$password = "(P@!K0080ronil??}}+";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. If you change this value, the ESP32 sketch needs to match
$api_key_value = "azxcvbnml";

$api_key = $value1 = $value2 = $value3 = $value4 = $value5 = $value6 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $value1 = test_input($_POST["value1"]);
        $value2 = test_input($_POST["value2"]);
        $value3 = test_input($_POST["value3"]);
        $value4 = test_input($_POST["value4"]);
		$value5 = test_input($_POST["value5"]);
		$value6 = test_input($_POST["value6"]);

        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO Sensor (value1, value2, value3, value4, value5, value6)
        VALUES ('" . $value1 . "', '" . $value2 . "', '" . $value3 . "', '" . $value4 . "', '" . $value5 . "', '" . $value6 . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}