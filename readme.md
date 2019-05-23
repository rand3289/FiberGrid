# FiberGrid

FiberGrid is an optical sensor framework for robotics.  If you don't want to spend years learning about electronics, how to interface sensors to microcontrollers and DSP but you are ready to jump into action building robots, this project is for you!  Forget arduino, Raspberry PI and MCUs.  There is an easier and much cheaper way to add sensors to your projects.  Add hundreds of sensors to your robots for a price of cheap plastic optic fiber.  The goal of this project is to take electronics out of the picture and allow ten year olds around the world to build sophisticated robots.

### What you need
* Cheap plastic insulated end glow optical fiber.  I bought 100ft of 0.75mm black jacket star ceiling optic lighting fiber for about $15USD from aliexpress.com  It consists of a very loose-fitting jacket over a plastic core.
* A camera.  Depending on your needs it can be a $5 usb cam or a Raspberry PI cam or in my case a $65 Kayeton Technology 330fps at 640x360 with a 4mm manual focus lens usb camera from aliexpress.  Camera framerate is important but resolution is not!  It should also be able to focus on objects close to the lens (1 to 3 inches).  Manual focus is the best.
* LEDs or a tiny flashlight.  Do not use lasers as they can burn out the CCD sensors in your camera.  Most commercial fiber lamps might be too bright.
* Two small plastic bottles or containers or film canisters that block light.

### How to make it
Cut the fiber into pieces of appropriate length depending on your project.  Use as many as you want as long as they all fit in a visual field of the camera.

Take half of the fibers and form a bunch with fiber ends aligned on one side (the other side of each fiber hangs free).  Put this end of the fiber bunch into a hole in your canister.  Bunch-O-Baloons water baloon straws are perfect for putting up to 37 fibers in a grid.  Fibers can be attached to individual straws with superglue.  Straws can screw onto some bottles making it easy to attach.  

Put a camera into a hole on the other side of your canister.  Glue gun time! Plugin the camera into your computer and point the other side of the fiber bunch towards the light.  You should see bright dots on your computer screen.  It is at most importance that the fibers are fixed securely in place and do not move on the screen when you slightly twist or pull on them.  As an alternative, drill holes the thickness of your fiber in a piece of black plastic and secure fibers in the holes one by one. Nothing except fibers should allow light to enter the canister. Paint it or use black electrical tape.

Take the other half of the fibers, make a bunch out of them and insert it into a hole in the second canister.  Insert a flashlight in a hole on the other side or wire up some LEDs and put them inside of the canister.  Put a reflector (aluminum foil?) behind LEDs for better efficiency.  Put something to disperse light in front of LEDs.  For example a slice of plastic from a milk jug.  Alternatively LEDs can be placed within individual sensors.

### How the sensors work
You can make a myriad of sensors for use with fibergrid.  Fibers can be used as compound eyes as-is.  Sensors can be 3D printed.  Made from cardboard or drinking straws and chewing gum :)  Try placing one fiber from the emitter (the canister with LEDs) and another fiber from the camera into each side of a hollow cofee stirrer (straw) or a cut-off bunch-o-baloons straw or fiber cladding.  While watching the cam on the screen, push the fibers in and out or bend the junction.  Notice how the brightness of one dot on the screen changes.  You've just made your first sensor.

It is possible to use a piece of flat polarized plastic (film) from an old monitor or disposable 3D movie theater glasses to make rotation sensors. Make a sensor with two small pieces that spin relative to each other.  When polarization of the pieces is at 90 degrees they will block a lot of light coming from the emitter fiber.

You could use a few sensors directly connected to pinholes on a canister without any fibers or mount a perfboard in front of your camera or a peg board.

Depending on your fiber, bending it might scatter light and change the sensor values.  To compensate for this, bundle two fibers going to a sensor together.  One fiber will be connected to a sensor and another directly to an emitter fiber via an inflexible junction.  This emitter fiber has to be bundled with an emitter fiber going to the sensor.  Alternatively an LED can be placed directly inside the sensor and both fibers lit up by one LED.  Decrease in light intensity  due to physical deformation of the fiber can be compensated in software by adjusting the sensor value with respect to the second fiber value change.

### How the software works
There are two parts to FiberGrid software.  FiberCal is a calibration program that finds centers and bounds of the fibers in the camera's visual field and saves them to a C++ header file (fiberinit.h).  Second is the FiberGrid "driver".  It includes the header file during compilation and provides a simple API for reading your sensors.  Both programs rely on OpenCV library to read sensor values from the bundle of optic fibers.  Driver and calibration software were separated for two reasons.  Detecting fibers can be done more accurately with human interaction. You have an ability to change threshold, fiber size and add or remove fibers manually by clicking.  The location of the fibers should never change if you have made the device properly.  The second reason is portability.  The driver could probably be written in 50 lines of python code or re-written in C++ without using OpenCV or written in Basic or in javascript pulling an image from a web cam and the calibration utility does not need to change.

Git clone. Change to the directory and type make.  Build the hardware and connect it to your computer.  Run fc (fiberCal) and generate fiberinit.h by pressing "s".  Play with fc sliders to get the best fiber detection (green squares around fibers).  Add fiberinit.h, fibergrid.h and fibergrid.cpp to your project or run ft (fibertest.cpp).  Use FiberGrid::read() or FiberGrid::readNormalized() to get sensor values!  Run your project to see the sensor values on a debug screen.  Comment out FIBERGRID_DEBUG to disable driver debug screen.  

### Notes
FPS of your camera is important.  High FPS allows FiberGrid to read sensor values faster.  Current usb mice have optical sensors in them that have hundreds of pixels and internal framerates of thousands of frames per second.  Many of them allow raw image readout.  Reading them at high FPS is another story.  I heard about read speeds of 10fps over serial.  Mouse sensors could be used to add FiberGrid support to low end MCUs without USB.  As of right now you will need at least a Pi zero.  If you succeed at reading any mouse image sensors at 30 fps or higher using any bus or find inexpensive high fps cameras or CCD/CMOS image sensors, please let me know!

It is easy to make position sensors.  Measuring force is harder.  Here is what comes to mind:
* Find a stiff yet compliant semi-transparent matherial.  Possibly a semi-transparent liquid.
* Stiff reflective matherial that changes the amount of light reflected at a certain angle when bent.
* Hydraulic / pneumatic load cells that indirectly change position of the fiber.
The goal is to make it somewhat linear and require as little displacement as possible.  Please share your load cell  designs with me.

### Have fun!
Send me a link to your creations or pictures and suggestions to  toandrey(at)yahoo(dot)com
