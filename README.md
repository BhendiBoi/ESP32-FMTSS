# ESP32 *F*ace *M*ask detection, *T*emperature measurement, automatic *S*anitizer *S*ystem (ESP32 FMTSS)
**(yes, i know, a very lengthy name)**

**Please note that this project is archived**, due to internal conflicts within the team over this software. There was never a project director, and me and my teammates (see the "The Team" section) were not open to the idea of modifying each others code, this led to us fighting over the project and it was a painful process from start to end. nonetheless this was a fun project, which i learned a lot from. This Project is now made [Free software](https://www.fsf.org/about/what-is-free-software) under [the MIT license](https://en.wikipedia.org/wiki/MIT_License), and anyone can re-use this code for any purpose (WARRANTY IS NOT PROVIDED).

### The Project

This project, originally developed for the [Sahyadri Science Talent Hunt](https://sahyadri.edu.in/Home/ssth) (SSTH), was never showcased in the Sahyadri Science Talent Hunt "exhibition", since the "exhibition" was cancelled halfway through the project.  

#### -> How we started this project

**One fine Day, one of my friends told me that there was going to be a Science and Technology competition being held locally, this was, of course the [Sahyadri Science Talent Hunt](https://sahyadri.edu.in/Home/ssth), being held in the [Sahyadri College of Engineering & Management](https://sahyadri.edu.in/), i wanted to participate, so i gathered my close, *geeky* friends, and told them about it. We decided to participate, and thus, registered ourselves into the competition.** 

first, we established our goals as stated in the "Our Goal" section, we then proceeded to discuss what components we would be using, and how the system was going to work. Then, we started Building/programming, in the midst of this however, things went downhill and the project was abandoned.

(This is a **very** brief overview of what happend, it didnt go as smoothly as it sounds, obviously)


#### -> Our Goal

The Goal for this project was, in the view of the [COVID-19](https://www.cdc.gov/coronavirus/2019-ncov/index.html) Pandemic, to curb chances of spreading the virus by implementing face mask detection, automatic sanitization and temperature measurement in one package, a package which could be easily implemented across public places and private businesses alike, and a package, which aimed at being affordable and [free (as in freedom, not price)](https://www.fsf.org/about/what-is-free-software).

The Face Mask Detection System had a camera, that would politely ask a potential customer/consumer to wear a face mask, if they werent already (if they were, the system would move on to the next "phase"), this was the first "phase"/step to the system. 

the system then checked the customers body temperature, and determined if it was above or below critical levels, if they were above critical levels, the system politely asked the customer to contact the appropriate staff members of that area (if they were below critical levels, the system moved on to the next "phase", again), This was the second "phase"/step. 

The system then finally activated its automatic sanitizer subsystem, which, obviously dispensed a fixed amount of sanitizer, This was the third and final "phase", the customer/consumer could now access the area, preferably while continuing to practice COVID protocols.

**(please note that, these were just our *goals* and not necessarily what was/is implemented in the actual project. what was implemented is further elaborated in the below sections)**

#### -> Our implementation

**(As you know, this project was not fully completed, hence, what *was* and what *wasn't* completed is clearly documented in this section)**

##### The components used

1. An [ESP32-CAM Development Board](https://www.amazon.in/BAIAI-Development-Bluetooth-OV2640-Camera/dp/B093GYNLFT/ref=sr_1_4?dchild=1&keywords=esp32+cam&qid=1625918101&sr=8-4). We chose this [Development board](https://en.wikipedia.org/wiki/Microprocessor_development_board) due to the inbuilt OV2640 2.0 MP camera, and its easy availability on the market, along with its extremely affordable price.
2. A [MLX90614 Non-Contact IR thermometer module](https://www.amazon.in/xcluma-MLX90614-Contactless-Temperature-Sensor/dp/B073DDV1GB/ref=sr_1_1?dchild=1&keywords=Mlx90614&qid=1625918143&sr=8-1). We chose this module to measure body temperature due to the fact that it was the only non-contact infrared thermometer we could find in the market. it had a accuracy of ± 0.5°C which meant it wasnt mediacal grade, however, to get a medical grade sensor, one had to contact melexis (the company who made the MLX90614), which wasnt possible at the time.
3. A [MP3-TF-16P MP3 Player](https://www.google.com/aclk?sa=L&ai=DChcSEwiXodqjtd3xAhURGysKHYt0ACMYABAHGgJzZg&ae=2&sig=AOD64_3-sReI5FKVoZCxiuN1FwobZM2YTw&ctype=5&q=&ved=2ahUKEwiVqtKjtd3xAhWrzDgGHQryDzUQ9aACegQIARBz&adurl=). We used this module because of its good sound quality and affordability.
4. A [128x64 I2C OLED](https://www.google.com/aclk?sa=L&ai=DChcSEwjEnZ7vtd3xAhWGQ2AKHcZqAGkYABAHGgJ0bQ&ae=2&sig=AOD64_20XQ_Bg6KLsPxUyGMuK3OnLNP2IQ&ctype=5&q=&ved=2ahUKEwie_5Lvtd3xAhUBwTgGHcgiCckQ9aACegQIARBi&adurl=) we used this since it is relatively low power, and can be used without a backlight, viewed from almost 180 degrees sideways.
5. Some Other small/[Passive Components](https://en.wikipedia.org/wiki/Passivity_(engineering)).

### The Team

Sanath Shet:- The author of this README, primarily wrote code for the ESP32 and helped out heavily with hardware and circuitry, was involved in the making of the projects powerpoint presentation, made graphics for the projects built in OLED display and generated the text to speech files for the projects MP3 Player system, managed clickup (todo-list software) workspace, bought the components for the project.

Rishon Fernandes:- Primarily wrote python code for the face mask detection algorithm (please keepin mind that this algorithm was forked from [Chandrika Debs Repository](https://github.com/chandrikadeb7/Face-Mask-Detection) and was heavily involved in communication between the ESP32 and the computer, handled communications, maintained this github repository, heavily involved in making the projects powerpoint presentation.  

Nandan Bhandary:- Primarily worked on hardware.

we all played an equal and important role in our team!



 
