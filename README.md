<h1>How to run:</h1>

Clone this git repository, then (<strong>in CLion</strong>) select CMakeLists.txt as a current cmake file and run project. 

<h1>Run issues:</h1>

<h3>MacOS:</h3>
<p>
If compiler does not know what is 'glew' --> run in console: <em>brew install glew</em><br>
Now this step is not really needed, because in CMake file this command will run automatically.<br>
Also you need to install brew for this --> instruction: <em>https://brew.sh/</em>
</p>

<h3>Windows:</h3>
<p>
If you have 64-bit system - you probably do not have any problems; 32-bits systems are not supported.
</p>

<h3>Unix (not apple):</h3>
<p>
Run in console:

`sudo apt update`

`sudo apt install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev`

`sudo apt install libglew-dev libglfw3 libglfw3-dev`

Maybe there are some extra libs, that are not necessary. Should check it later
</p>
