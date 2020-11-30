# C++ implementation of Lexi

**Lexi** - WYSIWYG text editor described in second chapter of [Design Patterns: Elements of Reusable Object-Oriented Software](https://en.wikipedia.org/wiki/Design_Patterns)
**Motivation** - I've been reading the book(as most of you) and faced the questions like "How this pattern is implemented?" or "Is there an open-source implementation of Lexi?". After searching internet for the answers I decided to implement it all by myself.
**Note**: Even though I've tried to implement stuff as described in the book, there are differences that I'll try to describe in the future.

## Pattern referencing
- [IGlyph](https://github.com/romaonishuk/LexI/blob/master/inc/glyphs/i_glyph.hpp) Abstract, base for all graphical units class. Described in Section 2.2 of GoF.
- [Composite](https://github.com/romaonishuk/LexI/blob/master/inc/glyphs/i_composite_glyph.hpp) Described in Section 2.2 of GoF.
- Strategy: basically any child glyph.
- [Decorator](https://github.com/romaonishuk/LexI/blob/master/inc/decorator.hpp) Note: currently, the scroller isn't a decorator as described in GoF due to implementation specific. Described in Section 2.4.
- [Abstract Factory](https://github.com/romaonishuk/LexI/blob/master/inc/window_system_factory.hpp) Described in Section 2.5.
- Bridge: [Base Window](https://github.com/romaonishuk/LexI/blob/master/inc/window.hpp), interface [WindowImpl](https://github.com/romaonishuk/LexI/blob/master/inc/window_impl.hpp) and its child [XWindowImpl](https://github.com/romaonishuk/LexI/blob/master/src/lexi_linux/inc/x_window_impl.hpp). Described in Section 2.6.
- [Command](https://github.com/romaonishuk/LexI/blob/master/inc/i_command.hpp) Described in Section 2.7.

*Note*: some of the patterns are WIP state and may distinguish from the implementation provided in GoF 

## [Functionality demo](https://github.com/romaonishuk/LexI/blob/master/docs/functionality_demo/demo.md)
Projects functionality(in Gif format) across the versions

## In Progress
- ~~Finishing text editor~~ (Finished basic functionality)
- Migration to Windows

## TODO List
- use only smart pointers
- migrate to a Windows(which includes expansion of WindowSystemFactory)
- add important glyphs as text editor
- appropriate processing of all fonts(now only mono is supported)

## HowTo
Build Lexi:
```shell
cmake %path_to_lexi
make
```

**Linux build troubleshooting**

I'm using X11 library for basic graphic operation. If it is not installed in Your system, You can install it:
```shell
sudo apt install libx11-dev
```


## Discussion
- Pls let me know if I'm violating some property rights (romaonishuk@gmail.com)
