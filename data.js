function EnviarPeriodo(date_range){
 
    var y_values_temp = [];
    var y_values_hum = [];
    var y_values_pres = [];
    var y_values_ilu = [];
    var x_values = [];
    var switch1 = true;
    var switchTemp = true;
    var switchHum = false;
    var switchPres = false;
    var switchIlum = false;



// recogemos todos los datos.
    $.post('values.php', {'periodo': date_range}, function (data) {

        data = data.split('/');
        for (var i in data)
        {
            if (switch1 == true)
            {
                var ts = timeConverter(data[i]);
                x_values.push(ts);
                switch1 = false;
            }
            else
            {
				if (switchTemp == true)
				{
					y_values_temp.push(parseFloat(data[i]));
					switchTemp = false;
					switchHum = true;
				}
				else if (switchHum == true)
				{
					y_values_hum.push(parseFloat(data[i]));
					switchHum = false;
					switchPres = true;
				}
				else if (switchPres == true)
				{
					y_values_pres.push(parseFloat(data[i]));
					switchPres = false;
					switchIlum = true;
				}
				else if (switchIlum == true)
				{
					y_values_ilu.push(parseFloat(data[i]));
					switchIlum = false;
					switchTemp = true;
					switch1 = true;
				}

            }
 
        }
        x_values.pop();

       // Pintamos los datos de temperatura.
        $('#chartTemp').highcharts({
            chart : {
	        animation: Highcharts.svg,
                type : 'line'
            },
            title : {
                text : 'Datos temperaturas'
            },
            subtitle : {
                text : 'Source: Estacion meteorologica privada'
            },
            xAxis : {
                title : {
		    text : 'Tiempo'
                },
                categories : x_values
            },
            yAxis : {
                title : {
                    text : 'Temperatura'
                },
                labels : {
                    formatter : function() {
                        return this.value + ' ºC'
                    }
                }
            },
            tooltip : {
                crosshairs : true,
                shared : true,
                valueSuffix : ''
            },
            plotOptions : {
                line : {
                    marker : {
                        lineColor : '#666666',
                        lineWidth : 1
                    }
                }
            },
            series : [{
 
                name : 'Temperatura',
                data : y_values_temp
            }]
        });



	// pintamos los datos humedad relativa
        $('#chartHum').highcharts({ 
            chart : {
                type : 'line' 
            }, 
            title : {
                text : 'Datos humedad relativa' 
            },
            subtitle : { 
                text : 'Source: Estacion meteorologica privada' 
            },
             xAxis : {
                title : {
                    text : 'Tiempo'
                },
                categories : x_values
            },
            yAxis : {
                title : {
                    text : 'Humedad relativa'
                },
                labels : {
                    formatter : function() {
                        return this.value + ' %'
                    }
                }
            },
            tooltip : {
                crosshairs : true,
                shared : true,
                valueSuffix : ''
            },
            plotOptions : {
                line : {
                    marker : {
                        lineColor : '#00FF00',
                        lineWidth : 1
                    }
                }
            },
            series : [{
                name : 'Humedad relativa',
                data : y_values_hum
            }]
		});


	// pintamos los datos presion
        $('#chartPres').highcharts({ 
            chart : {
                type : 'line' 
            }, 
            title : {
                text : 'Datos presion barometrica' 
            },
            subtitle : { 
                text : 'Source: Estacion meteorologica privada' 
            },
             xAxis : {
                title : {
                    text : 'Tiempo'
                },
                categories : x_values
            },
            yAxis : {
                title : {
                    text : 'Presion barometrica'
                },
                labels : {
                    formatter : function() {
                        return this.value + ' hBar'
                    }
                }
            },
            tooltip : {
                crosshairs : true,
                shared : true,
                valueSuffix : ''
            },
            plotOptions : {
                line : {
                    marker : {
                        lineColor : '#FE2E64',
                        lineWidth : 1
                    }
                }
            },
            series : [{
                name : 'Presion barometrica',
                data : y_values_pres
            }]
		});
		
			// pintamos los datos iluminacion
        $('#chartIlu').highcharts({ 
            chart : {
                type : 'line' 
            }, 
            title : {
                text : 'Datos iluminacion' 
            },
            subtitle : { 
                text : 'Source: Estacion meteorologica privada' 
            },
             xAxis : {
                title : {
                    text : 'Tiempo'
                },
                categories : x_values
            },
            yAxis : {
                title : {
                    text : 'Iluminacion'
                },
                labels : {
                    formatter : function() {
                        return this.value + ' %'
                    }
                }
            },
            tooltip : {
                crosshairs : true,
                shared : true,
                valueSuffix : ''
            },
            plotOptions : {
                line : {
                    marker : {
                        lineColor : '##FFFF00',
                        lineWidth : 1
                    }
                }
            },
            series : [{
                name : 'Iluminacion',
                data : y_values_ilu
            }]
		});
	});
}

function timeConverter(fecha){
  var a = new Date(fecha);
  var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
  var year = a.getFullYear();
  var month = months[a.getMonth()];
  var date = a.getDate();
  var hour = a.getHours();
  var min = a.getMinutes() < 10 ? '0' + a.getMinutes() : a.getMinutes();
  var sec = a.getSeconds() < 10 ? '0' + a.getSeconds() : a.getSeconds();
  var time = date + ' ' + month + ' ' + year + ' ' + hour + ':' + min ;
  return time;
}


