<?php

$periodo = "";
if ( !empty($_POST) && isset($_POST['periodo']) ) {
	$periodo = $_POST['periodo'];
}

$con = mysqli_connect ("localhost","root","", "datos_bruto");
if (!$con) {
	die ('Could not connect: ' . mysqli_error ());
}

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
	$datosDia = mysqli_query($con, "SELECT fecha, temperatura, humedadRel, presion, iluminacion FROM `datos_clasificados` WHERE id_estacion=1 AND fecha >= now() - INTERVAL 1 DAY ") or die ("Connection error");
while($row = mysqli_fetch_array($datosDia)) 
{ 
	echo $row['fecha'] . "/" . $row['temperatura']. "/" . $row['humedadRel']. "/" . $row['presion']. "/" . $row['iluminacion']. "/" ;
}



mysqli_close ($con);

?>
