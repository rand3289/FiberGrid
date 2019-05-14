# FiberGrid

FiberGrid is an optical sensor framework for robotics.  If you don't want to spend years learning about electronics, how to interface sensors to microcontrollers and DSP but you own a glue gun and you are ready to jump into action, this project is for you!  Forget arduino, Raspberry PI and MCUs.  There is an easier and much cheaper way to add sensors to your projects.  Add hundreds or thousands of sensors to your robots for a price of cheap plastic optic fiber.  Glue-gun robotics is the future!

### What you need
* Cheap plastic insulated end glow optical fiber.  I bought 100ft of 1mm black jacket star ceiling optic lighting fiber for about $15USD from aliexpress.com
* LEDs or a tiny flashlight.  Do not use lasers as they can burn out the CCD sensors in your camera.
* A camera.  Depending on your needs it can be a $5 usb cam or a Raspberry PI cam or in my case a $65 Kayeton Technology 330fps at 640x360 with a 4mm manual focus lens usb camera from aliexpress.  Camera framerate is important but resolution is not!  It should also be able to focus on objects close to the lens.  Manual focus is the best.
* Two small plastic bottles or film canisters that blocks light.  I painted a playdoh container black.
* A piece of flat polarized plastic (film) from an old monitor or disposable 3D movie theater glasses if you want to make rotation sensors.

### How to make it
Cut the fiber into pieces of appropriate length depending on your project.

Take half of the fibers and form a bunch with fiber ends aligned on one side (the other side of each fiber hangs free).  Put this end of the fibers bunch into a hole in your canister.  Put a camera into a hole on the other side of your canister.  If you are planning to make polarized light sensors, insert a piece of polarized plastic between the fibers and the camera inside the container to filter light.  Glue gun time! Plugin the camera into your computer and point the other side of the fibers bunch towards the light.  You should see bright dots on your computer screen.  It is at most importance that the fibers are fixed securely in place and do not move on the screen when you slightly twist or pull on them.  Use lots of glue!  For the best results drill holes the thickness of your fiber in a piece of black plastic and secure fibers in the holes one by one.

Take the other half of the fibers, make a bunch out of them and insert it into a hole in the second canister.  Insert a flashlight in a hole on the other side or wire up some LEDs and put them inside of the canister.  Place a piece of polarized plastic in between the LEDs and fiber edges if you plan to make rotation sensors.  Put a reflector (aluminum foil?) behind LEDs for better efficiency.

### How the sensors work
You can make a myriad of sensors for use with fibergrid.  Fibers can be used as compound eyes as-is.  Sensors can be 3D printed.  Made from cardboard or drinking straws and chewing gum :)  For example if you put polarized plastic in both containers, try placing one fiber from the emitter (the canister with LEDs) and another fiber from the camera into each side of a hollow cofee stirrer (straw).  While watching the cam on the screen, twist the fibers or push them in and out.  Notice how the brightness of one dot on the screen changes.  You've just made your first sensor.

You can use as many fibers as you want.  From one to thousands!  You could use a few sensors directly connected to pinholes on a canister without any fibers or mount a perfboard in front of your camera or a peg board.  Imagination is the limit.

### How the software works
There are two parts to FiberGrid software.  FiberCal is a calibration program that finds centers and bounds of the fibers in the camera's visual field and saves them to a C++ header file (fiberinit.h).  Second is the FiberGrid "driver".  It includes the header file during compilation and provides a simple API for reading your sensors.  Both programs rely on OpenCV library to read sensor values from the bundle of optic fibers.  Driver and calibration software were separated for two reasons.  Detecting fibers can be done more accurately with human interaction. You have an ability to change threshold, fiber size and add or remove fibers manually by clicking.  The location of the fibers should never change if you have made the device properly.  The second reason is portability.  The driver could probably be written in 50 lines of python code or re-written in C++ without using OpenCV or written in Basic or in javascript pulling an image from a web cam and the calibration utility does not need to change.

Git clone. Change to the directory and type make.  Build the hardware and connect it to your computer.  Run fc (fiberCal) and generate fiberinit.h by pressing "s".  Play with fc sliders to get the best fiber detection (green squares around fibers).  Add fiberinit.h, fibergrid.h and fibergrid.cpp to your project or run ft (fibertest.cpp).  Use FiberGrid::read() or FiberGrid::readNormalized() to get sensor values!  Run your project to see the sensor values on a debug screen.  Comment out FIBERGRID_DEBUG to disable driver debug screen.  

### Notes
FPS of your camera is important.  High FPS allows FiberGrid to read sensor values faster.  Current usb mice have optical sensors in them that have hundreds of pixels and internal framerates of thousands of frames per second.  Many of them allow raw image readout.  Reading them at high FPS is another story.  I heard about read speeds of 10fps over serial.  Mouse sensors could be used to add FiberGrid support to low end MCUs without USB.  As of right now you will need at least a Pi zero.  If you succeed at reading any mouse image sensors at 30 fps or higher using any bus or find inexpensive high fps cameras or CCD/CMOS image sensors, please let me know!

### Have fun!
Send me a link to your creations or pictures and suggestions to  toandrey(at)yahoo(dot)com
