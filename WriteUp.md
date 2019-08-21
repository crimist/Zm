# Zm - My first game cheat

This is a write up for my teacher as I chose this as a project in one of my classes. I had to fit specific criteria about what to cover which is why certain sections are there.

## The beginning

### Notes

I should start with the fact that this isn't my first cheat (I'm a liar). I've written external cheats in C# w/ Winforms that modify memory for unlimited health, ammo, ect. I've also written some ASM hacks using the lua scripting engine in CE ([Cheat Engine](https://github.com/cheat-engine/cheat-engine)) for FTL, Risk of Rain, and some other single player 16 bit games.

When I say first game cheat I mean first 3D multilayer cheat; the kind people pay money for (if you're far better than I am). My cheat includes wallhack, aimbot, triggerbot, and a couple other features.

### Very start

My journey started on [Unknowncheats](https://unknowncheats.me), a game cheating forum that's full of helpful people and source code. Conveniently enough, it also has a "Black Ops 2" section where you can specifically talk about reversing BO2 so that's where I began to look.

The first thing I stumbled upon was some open source BO2 zombies cheats to read through to get an idea of what was going on. Now, while different cheats use different DX versions, GUI library, ect. it was a good place to start in terms of how to model the cheat and things of that nature.

This turned out to be very helpful because while many of these cheats were on different versions I was able to find some very helpful offsets and structs that I made use of later down the line.

After familiarizing myself with some of the other cheats for the game I ran into my first major decision, I had a choice between a internal or external cheat. And let me tell you, the differences between the two are pretty big.

Internal cheats are DLLs that are injected into the victim process and acquire values internal by assigning variables to specific memory addresses like `int *value = reinterpret_cast<int *>(0xDEADBEEF)`. These kinds of cheats have to find a way to hide themselves in memory or disable / trick the anticheat as the processs could simply read their own memory and discover the rogue DLL.

On the other hand external cheats are PEs that acquire a winAPI handle on the process from which they can use functions like `ReadProcessMemory()` and `WriteProcessMemory` to acquire and modify memory. These kinds of cheats are stopped by blocking or stripping the permission sof the handle that it gets of it's victims process. This can be very easily done by registering a simple Windows Kernel driver.

From what I had heard internal cheats were harder to debug as they would crash the host processes if any exceptions occurred but had far better performance than external cheats.

Knowing that I wanted to write an aimbot, wallhack, and other things of the sort I knew that I would have to go internal so that I could draw on the screen and wouldn't have to deal with performance issues of calling `RPM` 144 times a second just so I could update my aimbot position. From here I started going a bit of research into windows DLL, how they worked, how you could inject them into processes, ect. Looking back now, had I gone internal this project would have just been a frustrating mess. I'm very glad I went internal.

## Pain Points

### ViewMatrix

The view matrix is a 4x4 2D array of floats that correspond to the view model of your character. Or, in other words a `float [4][4]`

I needed this because I had found the Vectors (`float x,y,z`) that contained the location of the enemy but I need a way to covert that to screen coordinates.

This proved to be really hard as the game had many places where 2D arrays were updating every frame or every movement which led to many false positives. Though after many hours of search through memory and reversing I did manage to find it with a consistent offset.

### Drawing on screen

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

The part you've (Jeff) been waiting for; what I learned. Without further ado, I learned:

* A lot about more advanced C++
* A lot about using VS and windows C++
  * I much prefer the build process, & types on *nix
* Lots about the Windows API (`Windows.h`)
  * I also enjoyed writing with *nix APIs.
    * For example listening on a socket in windows takes far more code and is far uglier than on *nix
* A bit about how visual programs work. I learned how they draw to the screen and what APIs (DirectX) they use.
* I gained some experience in game engines and 3D/2D math. It took a while but I learned how games check what's on the screen and convert a 3D environment into the 2D environment you see on screen. This was definitely the hardest thing to wrap my head around as I didn't have an previous experience in where as for the rest I already had some foundation.
* Finally I learned about hooking. A process where you overwrite functions and have them point into your own before executing. This involves allocating memory and then writing an x86_64 `JMP` instruction at the start of the command into your own. It's definitely interesting and I understand a bunch of thing that use that far better now.

## Conclusion

### The end

I had a lot of fun with my IDS. So much to the point where I was up at 3 AM trying to find the memory addresses of offsets so I could reach a breakthrough in the development process. I ended up having so much fun I worked on it for the entire week and a half until I finished it and then I went back to playing games. I'm definitely glad I chose this as my IDS. It involves fields I'm interested and gave me insight into other things I didn't have so much experience in. To truly conclude this IDS taught me a lot and I'm very glad that I chose it. 👍

## Credits

I didn't post on unknowncheats but I'd like to credit a couple people for their contributions to the forum.

* Unknowncheats: As a whole it was a great resource to find information. I got offsets, functions, some addresses, and some general source code that was invaluable for someone just beginning in the 3D world.
* SMBB: I used they're R6 cheat as an example of how to structure the project and make it somewhat readable. I also used a bunch of you helper functions and such.
* CypherPresents: Great offsets and a couple addresses / AOB for function addys.
* stevemk14ebr: Enums for the weapon list. Glad I didn't have to manually find them all.
