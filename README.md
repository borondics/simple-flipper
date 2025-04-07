# simple-flipper
Arduino driven flipper devices


Here, we provide software and hardware design to build a thermoelectric cooler/heater (TEC) sample stage with precision temperature control.
The design can be modified to fit any instrument. Our implementation is compatible with [neaSNOM/neaSCOPE](https://www.neaspec.com/) microscopes.

## Software installation

This software was designed to run on Windows 10. However, we used Python modules that should allow cross-platform 
operation.

⚠️ COM port listing: when using other operation systems, please modify the COM port addressing part of the code accordingly

We suggest to install the software in a new Python environment as follows:

`(base) > conda create --name <environment_name> --file requirements.txt`

The [pyMeCom](https://github.com/spomjaksilp/pyMeCom) Python module for the communication protocol of the Controller can be installed with pip

```
(base) > conda activate <environment_name>
(<environment_name>) > conda install pip
(<environment_name>) > pip install git+https://github.com/spomjaksilp/pyMeCom.git
```

Launch the main application file:

` (<environment_name>) > python heaterApp.py`


# Components

## Main parts

- https://www.amazon.fr/dp/B07236KYVC/ref=pe_27091421_487030221_TE_SCE_dp_1

## Electronics

### Thermoelectric controller

The Peltier elements are controlled with a Meerstetter TEC-1091 precision Peltier temperature controller. Product page: https://www.meerstetter.ch/products/tec-controllers/tec-1091


Our implementation looks like this:
![cad_design](/Images/cad_design.png)

### Connectors

We used a D-Sub 9 pins connector for both terminals, thus all GPIO and sensor pins of the controller are available and accessible through the two D-Sub 9 pins ports on the box. The input and the output ports are DC power plugs.

### Wiring

The wiring between the controller pins and the D-Sub 9 pins pins is arbitrary. Here is an example of the sensor pins/D-Sub layout.
![wiring](/Images/TECcontroller_wiring.png)

Our version looks like this (colors and pins do not correspond to the design figure above):
![final_box](/Images/final_box.png)

### Peltier elements

In v1.0 we can accommodate two Peltier elements that we purchased from Thorlabs.

- TECD2S: https://www.thorlabs.com/thorproduct.cfm?partnumber=TECD2S
- TECF2S: https://www.thorlabs.com/thorproduct.cfm?partnumber=TECF2S

⚠️ To properly drive the Peltier elements, you have to give their characteristics and electronic properties (such as maximum current, resistance, voltage) to the TEC controller.

Here we provide the configuration files for our stage/peltier combinations. The configuration files can be uploaded to the controller via the [TEC Service Software](https://www.meerstetter.ch/products/tec-controllers/tec-1091) from the related downloads/software panel.

We provide here the two config files in this repository /Software/tecd2s_parameters.ini and /Software/tecf2s_parameters.ini

⚠️ It is likely that you have to retune the PID parameters of the controller. You can do this by using the autotune option of the TEC Service Software. For more information see the controller manual.

The CAD files to build the sample stage with two, replaceable Peltier elements are in the CAD directory. 
In v1.0 we modified a [commercial heatsink](https://hu.rs-online.com/web/p/hutobordak/5040772?gb=b) to dissipate the heat when cooling, which is integrated with the sample stage as the image shows below.

![stage](/Images/heaterStage_hardware.png)

## Performance

Temperature response and stability of the heating stage.

![controllapp_new](/Images/controllapp_new.png)

## License

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg
