#!/bin/sh
# Con openwrt debemos cambiar lo anterior por #!/bin/sh
# Configuración del broker y topic a escuchar.
# Cambiar por valores propios las siguientes variables:
broker="localhost"
port="8883"
topic="miraisra/estacion/datos"
basedatos="datos_bruto"
#### Defino los parametros de conexión a la BD mysql
sql_host="localhost"
slq_usuario="root"
sql_password="" #*881FE54C7FBCF27D3481542F7023C216004E03DD
sql_database="datos_bruto"
sql_table="valores"
### Se monta los parámetros de conexión
sql_args="-h $sql_host -u $slq_usuario -p$sql_password -D $sql_database -s -e" 
#------------------------------------
#Ponemos el cliente de mosquitto escuchando
mosquitto_sub -t $topic -h $broker -p $port | while read value; do
   # Guardamos la fecha y hora actual en una variable.
   hora=$( date "+%Y-%m-%d %H:%M:%S" );
   # Insertamos el valor leido en la base de datos
   mysql $sql_args "INSERT INTO valores values ( '', '$hora', '$value' );"
   ### Mi sentencia Sql para que la muestre
####   mysql $sql_args "SELECT * from datos_bruto.valores;"

   ### Ejecutamos el procedimiento para clasificar los datos.
   mysql $sql_args "CALL ClasificaDatos;"

done
