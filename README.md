# Makma3D
A Vulkan-based game engine, created as a hobby project. Aside from just programming, also tries to be a lesson in design by really focussing on the usable Engine aspect (instead of being one game specific) and by trying to adopt a little Silicon Valley-style design process.

## Features
As time progresses, the Makma3D-engine will grow in features and in capabilities. For a complete overview of our roadmap, check the [issues](https://github.com/Lut99/Makma3D/issues), but a short overview can be seen below as well.

A short overview of the (planned) features:
- [ ] Basic model loading
- [ ] Moveable camera
- [ ] Basic texture loading
- [ ] Material support:
  - [ ] Just taking vertex colours
  - [ ] Giving it a static colour
  - [ ] Loading textures (without lighting)
  - [ ] Basic diffuse lighting on vertex colours
  - [ ] Basic diffuse lighting on a static colour
  - [ ] Basic diffuse lighting on textures
- [ ] Asynchronous model/texture loading
- [ ] Development console for interactivity
- [ ] Physics for objects

## Compilation
\<TBD\>

## Dependencies
Apart from standard dependencies such as CMake and a compiler, compiling the Makma3D-engine only depends externally on a [Vulkan](https://www.vulkan.org/) installation and the [GLFW3](https://www.glfw.org/) library. CMake should find your Vulkan installation automatically, and the same holds for GLFW if you installed it via your distribution's package manager (```libglfw3-dev``` on Ubuntu, and ```glfw``` in the Arch repositories). If you manually install GLFW on Windows, however, the CMake file has to manually contain the path where it should look for GLFW's CMake files; by default, this is set to ```C:/Program Files (x86)/GLFW/lib/cmake/glfw3```, but you should change this according to your setup.

For running the executable, only Vulkan is required as the GLFW3 library is compiled statically. The only thing it requires other than itself are the compiled ```.spv``` shader files and the models/textures. These can be found under any pre-compiled zip for Windows on [GitHub](https://github.com/Lut99/Makma3D/releases), or are compiled with the rest of the source code if you build the executable yourself.

## Contributing
Because this is a hobby project, we do not accept contributions to this fork of the source code. However, feel free to fork the code yourself and go nuts - it is a hobby project, after all. Just make sure to keep your project open-source, and it would be appreciated if you referenced this repository somewhere too.

## Contact
Do you have any questions about or issues with the project? Feel free to raise [an issue](https://github.com/Lut99/Makma3D/issues)! Bug reports are always appreciated if you encounter them, and if you do, mark your issue with the ```bug```-tag.
