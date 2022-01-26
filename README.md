# Proyecto MPC con bajo uso de hw
Este proyecto es una modificación directa de [este proyecto](https://gitlab.com/oscar-rc/IPD432). 
Se implementa la versión de MPC para un motor DC, utilizando un horizonte de predicción de 2 muestras, adaptado para la tarjeta Zybo.

Los proyectos han sido creados utilizando Windows y Vitis 2021.1

## Proyecto HLS
Para crear el proyecto HLS, desde *Vitis HLS 2021.1 Command Prompt*
```
vitis_hls -f create_project.tcl
```
El proyecto creado se puede abrir desde la GUI o usando
```
vitis_hls -p mpc_motor_dc_2
```

### Proyecto Vivado
Para crear el proyecto en Vivado, se debe tener generada la IP desde HLS previamente. El .zip generado debe extraerse en la carpeta *vivado/axi_mpc* 

El proyecto en Windows, se genera ejecutando desde consola:
```
./create_project.bat
```
o bien, desde *TCL console*
```
source create_project.tcl
```

### Proyecto Vitis
El proyecto en Vitis a su vez, requiere del archivo .xsa generado por Vivado al exportar el hardware.

El proyecto completo se encuentra comprimido en .zip, el cual debe importarse desde Vitis para ser reconocido como workspace.

Dentro, se encuentran dos apps, que incluyen la aplicación de MPC, resuelta completamente por software *mpc_dc_motor_2_sw_noip* y la versión que utiliza soporte de hw, *mpc_dc_moto_2_sw*.


### Utils
La carpeta *utils* contiene una goldenReference de 10.000 muestras, que puede ser utilizada a través del script *Serialcmd.py* que envía y recibe los datos a la tarjeta a través de stdio.