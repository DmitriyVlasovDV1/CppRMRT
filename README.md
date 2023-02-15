<h1>Чтобы запустить проект (на CLion):</h1>
<h3>1.</h3> Установка всех библиотек и тп:
<p>
<em>
--> OpenGL/glsl (glew 2.2.0 latest) (для реализации рендера),<br>
--> GLFW 3.3.8 (latest) (для создания окна),<br>
--> conan (пакетный менеджер для сборки проекта на MacOS/Linux/Windows)<br>
</em>
</p>
<p>

<h4>Для MacOS:</h4>
<p>
brew install glew<br>
brew install glfw<br>
</p>

<h4>Для Windows:</h4>
<p>
гайд: https://medium.com/@bhargav.chippada/how-to-setup-opengl-on-mingw-w64-in-windows-10-64-bits-b77f350cea7e
(удачи + gl установлен по дефолту примерно всегда)
</p>

<h4>Для Linux:</h4>
<p>
sudo apt install glew<br>
sudo apt install glfw<br>
Не проверяли работоспособность этих команд - у вас есть возможность разобраться самим, если не сработает)
</p>

<h4>Открываем консоль:</h4>

(1) cd {директория в котором будет находиться проект}<br>
(2) git clone {url}<br> 
(3) cd {путь до директории build-debug}<br>
<p>
Если нет это деректории: в CLion-e зайти в CMakeLists.txt, нажать reload, создасться эта директория и сделать шаг (3)<br>
</p>
(4) conan install<br>
(5) conan install —build=missing ..<br>

<h3>2.</h3>
Запускать сам проект из CLion-a<br><br><br>
<strong>На данный момент у тебя все должно работать</strong> - если нет --> гг
