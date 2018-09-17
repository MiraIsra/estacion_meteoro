<?php
$csv_end = "  
";  
$csv_sep = ",";  
$csv_file = "data.csv";  
$csv="";  
$con = mysqli_connect ("localhost","root","", "datos_bruto");
$periodo = "";
if (!$con) {
	die('Could not connect: ' . mysqli_error ());
}

if ( !empty($_POST) && isset($_POST['periodo']) ) {
	$periodo = $_POST['periodo'];
}

//$csv_array[] = array();
$csv_array="Hora, Temperatura, Humedad relativa, Presion, Iluminacion".$csv_end;

if ($periodo=="Dia")
	$datosDia = mysqli_query($con, "SELECT fecha, temperatura, humedadRel, presion, iluminacion FROM `datos_clasificados` WHERE id_estacion=1 AND fecha >= now() - INTERVAL 1 DAY ") or die ("Connection error"); 
else if ($periodo=="Hora")
	$datosDia = mysqli_query($con, "SELECT fecha, temperatura, humedadRel, presion, iluminacion FROM `datos_clasificados` WHERE id_estacion=1 AND fecha >= now() - INTERVAL 1 HOUR ") or die ("Connection error");
else if ($periodo=="Semana")
	$datosDia = mysqli_query($con, "SELECT fecha, temperatura, humedadRel, presion, iluminacion FROM `datos_clasificados` WHERE id_estacion=1 AND fecha >= now() - INTERVAL 7 DAY ") or die ("Connection error");
else if ($periodo=="Mes")
	$datosDia = mysqli_query($con, "SELECT fecha, temperatura, humedadRel, presion, iluminacion FROM `datos_clasificados` WHERE id_estacion=1 AND fecha >= now() - INTERVAL 1 MONTH ") or die ("Connection error");
else if ($periodo=="Ano")
	$datosDia = mysqli_query($con, "SELECT fecha, temperatura, humedadRel, presion, iluminacion FROM `datos_clasificados` WHERE id_estacion=1 AND fecha >= now() - INTERVAL 1 YEAR ") or die ("Connection error");
else
	$datosDia = mysqli_query ($con, "SELECT fecha, temperatura, humedadRel, presion, iluminacion FROM `datos_clasificados` WHERE id_estacion=1 AND fecha >= now() - INTERVAL 7 DAY ") or die ("Connection error");

while($row = mysqli_fetch_array ($datosDia))
{  
    $csv_array.=$row["fecha"].$csv_sep.$row["temperatura"].$csv_sep.$row["humedadRel"].$csv_sep.$row["presion"].$csv_sep.$row["iluminacion"].$csv_end;  
}
//echo $csv_array;  
//Generamos el csv de todos los datos  
if (!$handle = fopen ($csv_file, "w")) {  
    echo "Cannot open file";  
    exit;  
}  
if (fwrite ($handle, utf8_decode($csv_array)) === FALSE) {  
    echo "Cannot write to file";  
    exit;  
}  
fclose($handle); 


mysqli_close ($con);

header ("Content-Type: application/force-download");
header ("Content-Disposition: attachment; filename=".$csv_file);
header ("Content-Transfer-Encoding: binary");
header ("Content-Length: ".filesize ($csv_file));

readfile ($csv_file);

?>
