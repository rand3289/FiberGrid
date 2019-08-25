# FiberGrid

FiberGrid is an inexpensive 3D printed optical sensor framework for robotics.  If you don't want to spend years learning about electronics, how to interface sensors to microcontrollers and DSP but you are ready to jump into action building robots, this project is for you!  Forget Raspberry PI, arduino, PICs and other MCUs.  There is an easier and much cheaper way to add sensors to your projects.  Add hundreds of 3D printed sensors to your robots for the price of cheap plastic optic fiber.  The goal of this project is to take electronics out of the picture and allow ten year olds and adults around the world to build sophisticated devices without purchasing expensive components and spending time learning about electronics.   
More info is available here: https://hackaday.io/project/167317-fibergrid


### How it works
You design and 3d print sensors based on a principle of blocking light.  Fibergrid is able to sense how much light a sensor blocks and give you a value in software running on your PC, laptop, tablet, phone, Raspberry PI or anything that has or can interface with a camera.  Fibergrid hardware consists of two parts: a lightsource and a camera with an enclosure.  They are connected to sensors by inexpensive plastic fibers.  The light source shines light on a grid with exposed fiber ends.  Fibers transmit light to your light blocking sensors.  What light remains is picked up by another fiber that transmits it to the camera enclosure. Camera software converts each fiber's light to a value.  Alternatively LEDs can be placed within individual sensors.


### What you need
* Cheap plastic insulated end glow optical fiber.  I bought 100ft of 1 mm black jacket star ceiling optic lighting fiber for about $22USD from aliexpress.com  It consists of a very loose-fitting jacket over a plastic core.
* A camera.  Depending on your needs it can be a $5 usb cam or a Raspberry PI cam or in my case a $65 Kayeton Technology 330fps at 640x360 with a 4mm manual focus lens usb camera from aliexpress.  Camera framerate is important but resolution is not!  High FPS allows FiberGrid to read sensor values at a higher rate.  Camera needs to be able to focus on objects close to the lens (1 to 3 inches).  Manual focus lense is the best.
* USB LED lamp or a small flashlight.  Do not use lasers as they can burn out the CCD sensors in your camera!
* A 3D printer and BLACK PLA for printing all components.  Black PLA is used to block ambient light.


### Making the hardware
There are two versions of FiberGrid.  One for R&D and testing.  It features detachable fibers that can be disconnected from the grids.  Both fiber ends are glued into small 3D printed connectors.  The second is a "production version" where fibers are glued directly into the grids of the camera and light modules on one side and have a connector (plug) on the other.  This allows packing more fibers into the grid.  The diameter of the connector is greater than a plain fiber and takes up more grid space.  I am using a 1mm fiber.  If you need more than 200 - 300 sensors, try using 0.5mm fiber.  It should allow for a denser grid.  

* Choose between the two designs. Current R&D version models allow for 16 connections to each module.  The "production" version has 100 and 216 hole plates.

* Print the parts on a 3D printer. Cleanup the holes with a 1mm drill bit, a bit with diameter of your cladding and a ~4mm bit for large holes.

* Assemble the light and camera modules as shown in the pictures.

* Cut the fiber into pieces of appropriate length depending on your project.  If you have chosen the R&D version, glue the connectors onto both fiber ends with superglue.  Otherwise glue one connector to one fiber end and glue the other end directly into the grid of the camera module.  Do the same for the light module fibers.  Plug unused holes with pegs (R&D version) or fill them with black silicone.

* Print some sensors or design and print your own sensors with a 4mm diameter holes for fibers coming from the light module and going to the camera module.


### Alternative construction techniques
If you do not have a 3D printer, you can still play with this technology.  Bunch-O-Baloons water baloon straws are perfect for putting up to 37 fibers in a grid.  Fibers can be attached to individual straws with superglue.  Bunch-O-Baloons can screw onto a neck of a plastic water bottle making it easy to attach to any container with a light or a camera.  Glue the other end of the fibers directly into your sensors.  Paint everything black or wrap with black tape.
As an alternative to Bunch-O-Baloons, drill holes the thickness of your fiber in a sheet of black plastic.  Drill holes the thickness of your fiber jacket in another piece of plastic.  Align the holes and glue the two sheets together.  Secure fibers into the holes with glue.  When the fiber is inserted it goes through both sheets of plastic, however the jacket will go in half way.


### Troubleshooting
Plugin the camera into your computer and point the fibers towards the light.  Start your favorite camera software.  You should see bright dots on your computer screen.  Nothing except fibers should allow light to enter the module.  It is important that the fibers are fixed securely in place and do not move on the screen when you slightly twist or pull on them.  If they shift or rotate, sensor readings will not be accurate.


### How the software works
There are two parts to FiberGrid software.  FiberCal is a calibration program that finds centers and bounds of the fibers in the camera's visual field and saves them to a C++ header file (fiberinit.h).  Second is the FiberGrid "driver".  It includes the header file during compilation and provides a simple API for reading your sensors.  Both programs rely on OpenCV library to read sensor values from the bundle of optic fibers.  Driver and calibration software were separated for two reasons.  Detecting fibers can be done more accurately with human interaction. You have an ability to change threshold, fiber visual size and add or remove fibers manually by clicking.  The location of the fibers should never change if you have made the hardware device properly.  The second reason is portability.  The driver could probably be written in 50 lines of python code or re-written in C++ without using OpenCV or written in Basic or javascript meanwhile the calibration utility does not need to change.

Git clone. Change to the directory and type make.  Build the hardware and connect it to your computer.  Run fc (fiberCal) and generate fiberinit.h by pressing "s".  Play with fc sliders to get the best fiber detection (green squares around fibers).  Add fiberinit.h, fibergrid.h and fibergrid.cpp to your project or run ft (fibertest.cpp).  Use FiberGrid::read() or FiberGrid::readNormalized() to get sensor values!  Run your project to see the sensor values on a debug screen.  Comment out FIBERGRID_DEBUG to disable driver debug screen.


### How the sensors work
You can make a myriad of sensors for use with fibergrid.  Fibers can be used as compound eyes as-is.  Sensors can be 3D printed, made from cardboard or drinking straws and chewing gum :)  Try placing one fiber from the light module and another fiber from the camera into each side of a hollow cofee stirrer (straw) or fiber cladding.  While watching the cam on the screen, push the fibers in and out or bend the straw.  Notice how the brightness of one dot on the screen changes.  You've just made your first sensor.

It is possible to use a piece of flat polarized plastic (film) from disposable 3D movie theater glasses or an old monitor to make rotation sensors. Make a sensor with two small pieces that spin relative to each other.  When polarization of the pieces is at 90 degrees they will block a lot of light coming from the emitter fiber.

Depending on your fiber, bending it might scatter light and change the sensor values.  To compensate for this, bundle two fibers going to a sensor together.  One fiber will be connected to a sensor and another directly to a light emitting fiber via a junction that dimms the light.  This emitter fiber has to be bundled with an emitter fiber going to the sensor.  Alternatively an LED can be placed directly inside the sensor and both fibers lit up by one LED.  Decrease in light intensity  due to physical deformation of the fiber can be compensated in software by adjusting the sensor value with respect to the second fiber value change.

It is easy to make position sensors.  Measuring force is harder.  Here is what comes to mind:
* Find a stiff yet compliant semi-transparent matherial / 3D printer filament.
* Stiff reflective matherial that changes the amount of light reflected at a certain angle when bent.
* Hydraulic / pneumatic / mechanical load cells that indirectly change position of the fiber.
The goal is to make it somewhat linear, require as little displacement as possible and stable over a good temperature range.


### Request
Current usb mice have optical sensors in them that have hundreds of pixels and internal framerates of thousands of frames per second.  Many of them allow raw image readout.  Reading them at high FPS is another story.  I've heard about read speeds of only 10fps over serial.  At that speed mouse sensors could only be used to add FiberGrid support to low end MCUs without USB.  If you succeed at reading any mouse image sensors at 30 fps or higher using any bus or find inexpensive high fps cameras or CCD/CMOS image sensors, please let me know!

### Have fun!
Please share your sensor designs!  Send me a link to your creations or pictures and suggestions to toandrey(at)yahoo(dot)com
