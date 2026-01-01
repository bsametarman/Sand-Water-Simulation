# Falling Sand Simulation With C++

This project is a pixel based physics simulation (falling sand game) developed by using **C++** and **SDL3** graphics library.

## 🚀 Features

#### Elements:
* **🟨Sand:** Affected by gravity, piles up on top of each other, slides towards the gaps and sinks in the water.
* **🟦Water:** Takes the shape of its container, and spreads horizontally.
* **⬛Rock:** Static blocking block.

## 🎮 Controls
* **Left Click:** Draws selected element on the screen.
* **Mouse Middle Button:** Changes element.

## Used Technologies
* **Language:** C++
* **Graphics Engine:** SDL3

## 📦 Prerequisites

Before you begin, ensure you have the following:

* A C++ Compiler (GCC/G++ recommended).
* **SDL3** and **SDL3_ttf** libraries.
* **arial.ttf** font file (must be in the same directory as **.exe** file).

## 🛠️ Compilation

You can compile the project using `g++`.

Open your terminal in the project directory and run:

```bash
g++ main.cpp -o Sandsim.exe -lSDL3 -lSDL3_ttf
```

## Live Demo

![Live Demo](/.github/assets/simulation.gif)

## 🤝 Contributing
Feel free to open issues or submit pull requests if you find any bugs or have suggestions for improvements.

## 📄 License
This project is licensed under the MIT License - see the LICENSE file for details.