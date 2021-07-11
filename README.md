# ESP32 *F*ace *M*ask detection, *T*emperature measurement, automatic *S*anitizer *S*ystem (ESP32 FMTSS)
**(yes, i know, a very lengthy name)**

**Please note that this project is archived**, due to internal conflicts within the team over this software. There was never a project director, and me and my teammates were not open to the idea of modifying each others code, this led to us fighting over the project and it was a painful process from start to end. nonetheless this was a fun project, which i learned a lot from. This Project is now made Free software under the MIT license, and anyone can re-use this code for any purpose (WARRANTY IS NOT PROVIDED).

### The Project

This project, originally developed for the [Sahyadri Science Talent Hunt](https://sahyadri.edu.in/Home/ssth) (SSTH), was never showcased in the Sahyadri Science Talent Hunt "exhibition", since model making was cancelled halfway through the project.  

#### -> Our Goal

The Goal for this project was, in the view of the COVID-19 Pandemic, to curb chances of spreading the virus by implementing face mask detection, automatic sanitization and temperature measurement in one package, a package which could be easily implemented across public places and private businesses alike, and a package, which aimed at being affordable and free (as in freedom, not price).

The Face Mask Detection System had a camera, that would politely ask a potential customer/consumer to wear a face mask, if they werent already (if they were, the system would move on to the next "phase"), this was the first "phase"/step to the system. 

the system then checked the customers body temperature, and determined if it was above or below critical levels, if they were above critical levels, the system politely asked the customer to contact the appropriate staff members of that area (if they were below critical levels, the system moved on to the next "phase", again), This was the second "phase"/step. 

The system then finally activated its automatic sanitizer subsystem, which, obviously dispensed a fixed amount of sanitizer, This was the third and final "phase", the customer/consumer could now access the area, preferably while continuing to practice COVID protocols.

**(please note that, these were just our *goals* and not necessarily what was/is implemented in the actual project. what was implemented is further elaborated in the below sections)**

#### -> Our implementation

**(As you know, this project was not fully completed, hence, what *was* and what *wasn't* completed is clearly documented in this section)**

##### The components used

1. An [ESP32-CAM Development Board](https://www.amazon.in/BAIAI-Development-Bluetooth-OV2640-Camera/dp/B093GYNLFT/ref=sr_1_4?dchild=1&keywords=esp32+cam&qid=1625918101&sr=8-4). We chose this [Development board](https://en.wikipedia.org/wiki/Microprocessor_development_board) due to the inbuilt OV2640 2.0 MP camera, and its easy availability on the market, along with its extremely affordable price.
2. A [MLX90614 Non-Contact IR thermometer module](https://www.amazon.in/xcluma-MLX90614-Contactless-Temperature-Sensor/dp/B073DDV1GB/ref=sr_1_1?dchild=1&keywords=Mlx90614&qid=1625918143&sr=8-1). We chose this module to measure body temperature due to the fact that it was the only non-contact infrared thermometer we could find in the market. it had a accuracy of ± 0.5°C which meant it wasnt mediacal grade, however, to get a medical grade sensor, one had to contact melexis (the company who made the MLX90614), which wasnt possible at the time.
3. A []


### The Team

Sanath Shet:- The author of this README, primarily wrote code for the ESP32 and helped out heavily with hardware and circuitry, was involved in the making of the projects powerpoint presentation, made graphics for the projects built in OLED display and generated the text to speech files for the projects MP3 Player system, managed clickup (todo-list software) workspace, bought the components for the project.

Rishon Fernandes:- Primarily wrote python code for the face mask detection algorithm (please keepin mind that this algorithm was forked from [Chandrika Debs Repository](https://github.com/chandrikadeb7/Face-Mask-Detection) and was heavily involved in communication between the ESP32 and the computer, handled communications, maintained this github repository, heavily involved in making the projects powerpoint presentation.  

Nandan Bhandary:- Primarily worked on hardware.

we all played an equal role in our team!



 
