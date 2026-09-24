# 🚀 2D Arcade Space Shooter

Welcome to the **2D Arcade Space Shooter**! This is a classic, retro-style arcade game built from scratch using **C++** and the **SFML** graphics library. Dodge incoming targets, fire lasers, and test your reaction speed to keep your ship flying!

---

## 🎮 How to Play

The game runs directly on your computer window at a smooth **60 Frames Per Second**. 

1. **Move Your Ship:** Use your controls to slide left and right across the bottom of the screen.
2. **Blast Targets:** Tap your fire button to shoot lasers upward.
3. **Survive:** Incoming targets automatically spawn and drop from the top of the screen. Clear them out before they crash into you!

---

## 🛠️ How it Works under the Hood

While it looks like a classic arcade game on the outside, the code is optimized to run smoothly on any laptop (like a Dell Inspiron 15) without lagging or crashing:
* **Smart Memory Cleanup:** The game automatically deletes lasers and targets after they leave the screen so your computer never slows down.
* **Instant Collision Checking:** The game uses quick math to check the exact borders of your ship and the targets to register hits instantly.
* **Buttery Smooth Graphics:** It talks directly to your graphics card to ensure zero stuttering while you play.

---

## 💻 How to Build and Run the Game

If you want to run this game on your own machine using **MSYS2 (UCRT64)**, follow these quick steps:

### 1. open your terminal and find the game folder:
```bash
cd /c/Users/name/OneDrive/Documents/ArcadeGame 
```

### 2. Compile the game (this builds your code into a playable app):
```bash
g++ main.cpp -o arcade_game -lsfml-graphics -lsfml-window -lsfml-system
```

### 3. Launch and play!
```bash
./arcade_game
```
