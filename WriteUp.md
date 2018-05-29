# Zm - My first game cheat

## The beginning

### Notes

I should start with the fact that this isn't my first cheat (I'm a liar). I wrote one in C# with a Winforms GUI that just had unlimited health ammo ect. I've also written some ASM hacks using the lua scripting engine in CE ([Cheat Engine](https://github.com/cheat-engine/cheat-engine)) for a game called FTL but these were both single player 16 bit games and I was just changeing simple memory address / the assembly.

When I say first game cheat I mean first 3D multilayer cheat; the kind people pay money for. My cheat includes wallhack, aimbot, triggerbot, and a couple other features.

### Very start

My journey started on ~~A torrent site where I downloaded the game. Don't worry I already own it so it's not illegal but I didn't want to get banned by the anti cheat when I was testing things at the start.~~ Whoops. I mean I started on [unknowncheats](unknowncheats.me), a game cheating forum that's full of helpful people and source code.

The first thing I did was find some open source cheats to read through to get an idea of what was going on.

Cheats use different Direct X version but they're all quite similar so it was a good place to start.

I started to find other Black Ops 2 cheats and started looking at their code. I was on a different version so the offsets and ASM are different I was able to find some structs because of it so it ended up being extremely valueable.

The first major decision I had was the choice between a internal or external cheat. The differences between the two are pretty big.

Internal cheats are DLLs that are injected into the victim process and acquire values internal by assigning variables to specific memory addresses `int *value = reinterpret_cast<int *>(0xDEADBEEF)`

On the other hand external cheats run and acquire a windows API handle on the process where they use functions like `ReadProcessMemory()` and `WriteProcessMemory` to change and acquire in game values.

From what I had heard internal cheats were harder to debug and crash the game if they do anything wrong but have far better performance than external cheats.

Because I was using an aimbot and I had made an external cheat before I decided to make it internal to learn how DLLs work. This proved to be a really good choice.

## Pain Points

### ViewMatrix

The view matrix is a 4x4 2D array of floats that correspond to the view model of your character.

In other words a `float [4][4]`

I needed this because I had found the Vectors (`float x,y,z`) that contained the location of the enemy but I need a way to covert that to screen coordinates.

This proved to be really hard and took 3 hours of reverse engineering and searching memory.

### Building a GUI

Having no experience with 3D graphics I had no idea where to start. I ended up following the example of [this](https://github.com/SMBB/R6-Internal-cheat) project and using [ImGUI](https://github.com/ocornut/imgui).

I had to learn how to use the functions to draw on the 2D screen but after a bit of messing around it started to make sense. The harder part was making the boxes and such smaller when the zombies were further away. It proved to be quite the challenge with a lot of annoying math to find how big the sides needed to be.

### On screen menu

Starting the on screen menu proved to be a pretty big pain in the ass. I didn't know where to start so I reviewed other projects that used it, examples in the GitHub repo, and what functions the library offered. I eventually figured out that ImGUI had a bunch of functions to build menus and it had some fantastic examples. Using those I was able to get a basic menu up that I then expanded until all the features in the cheat were toggleable. I did run into the issue of the menu sometimes wasn't at the top of the screen and as such you could see it but couldn't interact with it. I found some info online of how to hook a function that would let me toggle interacting with DirectX.

## Clean up

### Bugs

The menu ended up creating a lot of bugs. The main one being the menu would be up but the user would be interacting with the game or the reverse where the menu is gone but the user is trying to interact with it.

There were a couple of very minor bugs I couldn't fix because they were more rooted in the game engine than my code.

## What I learned

### 🎉

The part you've (Jeff) all been waiting for; what I learned.

Without further ado, I learned:

* Quite a few C++ tricks.
  * The different types of type casting
  * Threading

* A lot about using VS and windows C++. I got to say its trash compared to *nix but it's Microsoft so I didn't expect much.

* Lots about the Windows API (`Windows.h`) and how bad it is.

* A bit about how visual programs work. I learned how they draw to the screen and what APIs (DirectX) they use.

* I gained some experience in game engines and 3D/2D math. It took a while but I learned how games check what's on the screen and convert a 3D environment into the 2D environment you see on screen. This was definitely the hardest thing to wrap my head around as I didn't have an previous experience in where as for the rest I already had some foundation.

* Finally I learned about hooking. A process where you overwrite functions and have them point into your own before executing. This involves allocating memory and then writing an x86_64 `JMP` instruction at the start of the command into your own. It's definitely interesting and I understand a bunch of thing that use that far better now.

## Conclusion

### The end

I had a lot of fun with my IDS. So much to the point where I was up at 3 AM trying to find the memory addresses of offsets so I could reach a breakthrough in the development process. I ended up having so much fun I worked on it for the entire week and a half until I finished it and then I went back to playing games. I'm definitely glad I chose this as my IDS. It involves fields I'm interested and gave me insight into other things I didn't have so much experience in. To truly conclude this IDS taught me a lot and I'm very glad that I chose it. 👍

## Credits

I didn't personally contact anyone via PM when making this but I'd like to reconize some people for their source.

* Unknowncheats: As a whole it was a great resource to find people. I got offsets, functions, some addresses, and some general source code that was invaluable for someone just begining in the 3D world.

* SMBB: I used your R6 internal cheat as an example of how to lay the cheat out and not have it look really bloated. I also used a bunch of you helper functions and such.

* CypherPresents: Great offsets and a couple addresses / AOB for function addys.

* stevemk14ebr: Enums for the weapon list.